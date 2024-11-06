// Fill out your copyright notice in the Description page of Project Settings.


#include "Upgrades/GrapplingHookComponent.h"

#include "DrawDebugHelpers.h"  // For visualizing the trace in the editor (optional)
#include "PaperCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"       // For accessing the world
#include "GameFramework/Actor.h" // For working with actors
#include "Kismet/KismetSystemLibrary.h"  // Optional for advanced tracing
#include "Kismet/KismetMathLibrary.h"
#include "CableComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"


void UGrapplingHookComponent::LaunchHook(const FInputActionValue& Value)
{

	FHitResult HitResult;  // The hit result of the line trace
	HookLineTrace(HitResult);  // Perform the line trace
}

void UGrapplingHookComponent::RetractHook(const FInputActionValue& Value)
{
	isGrappling = false;
	SetCableComponentVisibility(false);
	GetOwner()->FindComponentByClass<UCharacterMovementComponent>()->SetMovementMode(EMovementMode::MOVE_Falling);
}

void UGrapplingHookComponent::HookGrappling(const FInputActionValue& Value)
{
	if(!isGrappling || CableTimeline->IsPlaying())
		return;

	CableComponent->EndLocation = UKismetMathLibrary::InverseTransformLocation(Character->GetTransform(), Grabpoint);
	FVector ForceToApply;
	ForceToApply =  UKismetMathLibrary::GetDirectionUnitVector(Character->GetActorLocation(), Grabpoint) + (Character->GetActorRightVector() * 0.7f);
	UKismetMathLibrary::Vector_Normalize ( ForceToApply, 0.0001f);
	ForceToApply *= 250000.0f;
	MovementComponent->AddForce(ForceToApply);
}

void UGrapplingHookComponent::SetCableComponentVisibility(bool bVisible)
{
	CableComponent->SetVisibility(bVisible);
}

void UGrapplingHookComponent::BeginPlay()
{
	Super::BeginPlay();

	if (CableCurve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("UpdateCableLocation"));
		CableTimeline = NewObject<UTimelineComponent>(this, FName("CableTimeline"));
		CableTimeline->AddInterpFloat(CableCurve, TimelineProgress);
		FOnTimelineEvent TimelineFinished;
		TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));
		CableTimeline->SetTimelineFinishedFunc(TimelineFinished);
		CableTimeline->RegisterComponent();
	}
}

bool UGrapplingHookComponent::InitializeComponents()
{
	Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
	Character = Cast<APaperCharacterBase>(GetOwner());
	PlayerController = Character ? Cast<APlayerController>(Character->GetController()) : nullptr;
	MovementComponent = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
	CableComponent = GetOwner()->FindComponentByClass<UCableComponent>();
	
	return Camera && Character && PlayerController && MovementComponent  &&  CableComponent;
}

void UGrapplingHookComponent::UpdateCableLocation(float Value)
{
	if(isGrappling)
	{
		TargetLocation = UKismetMathLibrary::InverseTransformLocation(Character->GetTransform(), Grabpoint);
	}
	FVector NewLocation = FMath::Lerp(InitialLocation, TargetLocation, Value);
	CableComponent->EndLocation = NewLocation;
	SetCableComponentVisibility(true);
	//UE_LOG(LogTemp, Warning, TEXT("New Location: %s"), *NewLocation.ToString());
}

void UGrapplingHookComponent::OnTimelineFinished()
{
	if(isGrappling)
	{
		CableTimeline->Stop();
	}
    else
    {
        CableTimeline->Reverse();
        FLatentActionInfo LatentInfo;
        LatentInfo.CallbackTarget = this;
        LatentInfo.ExecutionFunction = FName("OnReverseCableAfterFailFinished");
        LatentInfo.Linkage = 0;
        LatentInfo.UUID = __LINE__;
        UKismetSystemLibrary::Delay(GetWorld(), 0.3f, LatentInfo);
    }
}

void UGrapplingHookComponent::OnReverseCableAfterFailFinished()
{
	if(!isGrappling)
		SetCableComponentVisibility(false);
	
	if(CableTimeline)
		CableTimeline->Stop();
}

void UGrapplingHookComponent::HookLineTrace(FHitResult& OutHit)
{
	if (!InitializeComponents()) return;

	FVector2D MousePosition;
	if (!GetMousePosition(MousePosition)) return;

	FVector WorldLocation, WorldDirection;
	if (!DeprojectMousePosition(MousePosition, WorldLocation, WorldDirection)) return;

	FVector IntersectionPoint;
	if (!CalculateLinePlaneIntersection(WorldLocation, WorldDirection, IntersectionPoint)) return;

	PerformLineTrace(IntersectionPoint, OutHit);
}


bool UGrapplingHookComponent::GetMousePosition(FVector2D& MousePosition) const
{
	return PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
}

bool UGrapplingHookComponent::DeprojectMousePosition(const FVector2D& MousePosition, FVector& WorldLocation, FVector& WorldDirection) const
{
	return PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
}

bool UGrapplingHookComponent::CalculateLinePlaneIntersection(const FVector& WorldLocation, const FVector& WorldDirection, FVector& IntersectionPoint) const
{
	const float Distance = 2000.0f;
	const FVector Start = Camera->GetComponentLocation();
	const FVector End = WorldLocation + (WorldDirection * Distance);
	const FVector PlaneOrigin = Character->GetActorLocation();
	const FVector PlaneNormal(1.0f, 0.0f, 0.0f);
	float T;

	return UKismetMathLibrary::LinePlaneIntersection_OriginNormal(Start, End, PlaneOrigin, PlaneNormal, T, IntersectionPoint);
}

void UGrapplingHookComponent::GetPointBetween2PointsByDistance(const FVector& IntersectionPoint, FVector StartPoint, FVector& Endpoint, float& Distance)
{
	FVector DirectionVector = StartPoint - IntersectionPoint;
	DirectionVector.Normalize();
	Endpoint = StartPoint - (DirectionVector*Distance);
}

void UGrapplingHookComponent::PerformLineTrace(const FVector& IntersectionPoint, FHitResult& OutHit)
{
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	FVector StartPoint = Character->GetActorLocation();
	FVector Endpoint;
	
	GetPointBetween2PointsByDistance(IntersectionPoint, StartPoint, Endpoint, GrapplingHookDistance);
	
	const bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, StartPoint, Endpoint, ECC_Visibility, CollisionParams);

	if (bHit)
	{
		HandleHit(OutHit);
	}
	else
	{
		HandleMiss(Endpoint);
	}

	if (CableTimeline)
	{
		CableTimeline->PlayFromStart();
	}
	//Draw debug line for visualization
	//DrawDebugLine(GetWorld(), Character->GetActorLocation(), Endpoint, FColor::Red, false, 4.0f, 0, 1.0f);
}

void UGrapplingHookComponent::HandleHit(const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());
	isGrappling = true;
	Grabpoint = HitResult.ImpactPoint;
	MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
}

void UGrapplingHookComponent::HandleMiss(FVector EndPoint)
{
	TargetLocation = EndPoint - GetOwner()->GetActorLocation();
	isGrappling = false;
}

