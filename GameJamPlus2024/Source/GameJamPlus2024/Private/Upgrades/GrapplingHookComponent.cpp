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
	CharacterMovement->AddForce(ForceToApply);
}

void UGrapplingHookComponent::SetCableComponentVisibility(bool bVisible)
{
	CableComponent->SetVisibility(bVisible);
}

void UGrapplingHookComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterMovement = Cast<APaperCharacterBase>(GetOwner())->FindComponentByClass<UCharacterMovementComponent>();
	CableComponent = GetOwner()->FindComponentByClass<UCableComponent>();
}

void UGrapplingHookComponent::HookLineTrace(FHitResult& OutHit)
{
	UCameraComponent* Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
	if (!Camera) return;
	
	APaperCharacterBase* Character = Cast<APaperCharacterBase>(GetOwner());
	if (!Character) return;
	
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (!PlayerController) return;
	
	FVector2D MousePosition(0.0f, 0.0f);
	Cast<APlayerController>(PlayerController)->GetMousePosition(MousePosition.X, MousePosition.Y);

	// Deproject the mouse position to get the world direction
	if (FVector WorldLocation, WorldDirection; PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		float Distance = 2000.0f;
		///////// LINE PLANE INTERSECTION
		FVector Start = Camera->GetComponentLocation(); // The character's location
		FVector End = WorldLocation + (WorldDirection * Distance) ; // Adjust the distance as needed (10,000 units)
		FVector PlaneOrigin = Character->GetActorLocation();  // A point on the plane (e.g., origin)
		FVector PlaneNormal(1.0f, 0.0f, 0.0f);  // The plane's normal (e.g., x-axis)
		FVector IntersectionPoint;
		float T;
		UKismetMathLibrary::LinePlaneIntersection_OriginNormal(Start, End, PlaneOrigin, PlaneNormal, T, IntersectionPoint);
		
		/////////// Perform the line trace
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner()); // Ignore the character itself
		
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, PlaneOrigin, IntersectionPoint, ECC_Visibility, CollisionParams);
        
		SetCableComponentVisibility(true);
		
		if (bHit)
		{
			// Handle hit
			UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());

			//CableComponent->EndLocation = HitResult.ImpactPoint;
			Grabpoint = HitResult.ImpactPoint;
			isGrappling = true;

			GetOwner()->FindComponentByClass<UCharacterMovementComponent>()->SetMovementMode(EMovementMode::MOVE_Flying);
		}
		else
		{
			// Handle no hit 
			UE_LOG(LogTemp, Warning, TEXT("No Hit"));
			CableComponent->EndLocation =  (HitResult.TraceEnd - GetOwner()->GetActorLocation());
			isGrappling = false;
			UKismetSystemLibrary::Delay(GetWorld(), 0.2f, FLatentActionInfo());
			//SetCableComponentVisibility(false);
		}

		//Draw debug line for visualization
		DrawDebugLine(GetWorld(), PlaneOrigin, IntersectionPoint, FColor::Red, false, 4.0f, 0, 1.0f);
	}
}
