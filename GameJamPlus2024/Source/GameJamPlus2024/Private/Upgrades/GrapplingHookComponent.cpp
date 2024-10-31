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
	if(!isGrappling)
		return;

	CableComponent->EndLocation = UKismetMathLibrary::InverseTransformLocation(GetOwner()->GetTransform(), Grabpoint);
	FVector ForceToApply;
	ForceToApply =  UKismetMathLibrary::GetDirectionUnitVector(GetOwner()->GetActorLocation(), Grabpoint) + GetOwner()->GetActorRightVector()*0.7f;
	UKismetMathLibrary::Vector_Normalize ( ForceToApply, 1.0f);
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

void UGrapplingHookComponent::PerformLineTrace(const FVector& IntersectionPoint, FHitResult& OutHit)
{
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	const bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Character->GetActorLocation(), IntersectionPoint, ECC_Visibility, CollisionParams);

	SetCableComponentVisibility(true);
	if (bHit)
	{
		HandleHit(OutHit);
	}
	else
	{
		HandleMiss(IntersectionPoint);
	}
	//Draw debug line for visualization
	//DrawDebugLine(GetWorld(), Character->GetActorLocation(), IntersectionPoint, FColor::Red, false, 4.0f, 0, 1.0f);}
}

void UGrapplingHookComponent::HandleHit(const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());
	Grabpoint = HitResult.ImpactPoint;
	isGrappling = true;
	MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
}

void UGrapplingHookComponent::HandleMiss(FVector IntersectionPoint)
{
	UE_LOG(LogTemp, Warning, TEXT("No Hit"));
	CableComponent->EndLocation = (IntersectionPoint - GetOwner()->GetActorLocation());
	isGrappling = false;
	UKismetSystemLibrary::Delay(GetWorld(), 0.2f, FLatentActionInfo());
}

