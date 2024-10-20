// Fill out your copyright notice in the Description page of Project Settings.


#include "Upgrades/GrapplingHookComponent.h"

#include <optional>

#include "DrawDebugHelpers.h"  // For visualizing the trace in the editor (optional)
#include "PaperCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"       // For accessing the world
#include "GameFramework/Actor.h" // For working with actors
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"  // Optional for advanced tracing


void UGrapplingHookComponent::LaunchHook(const FInputActionValue& Value)
{
	APaperCharacterBase* Character = Cast<APaperCharacterBase>(GetOwner());

	FHitResult HitResult;  // The hit result of the line trace
	HookLineTrace(HitResult);  // Perform the line trace
}

void UGrapplingHookComponent::HookLineTrace(FHitResult& OutHit)
{
	// UCameraComponent* Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
	// if (!Camera) return;
	//
	// APaperCharacterBase* Character = Cast<APaperCharacterBase>(GetOwner());
	// if (!Character) return;
	//
	// APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	// if (!PlayerController) return;
	//
	// FVector2D MousePosition(0.0f, 0.0f);
	// Cast<APlayerController>(PlayerController)->GetMousePosition(MousePosition.X, MousePosition.Y);
	//
	// FVector WorldLocation;
	// FVector WorldDirection;
	// float Distance = 2000.0f;
	//
	// // Deproject the mouse position to get the world direction
	// if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	// {
	// 	// Set the trace start and end points
	// 	FVector Start = Camera->GetComponentTransform().GetLocation(); // The character's location
	// 	FVector End = WorldLocation + (WorldDirection * Distance) ; // Adjust the distance as needed (10,000 units)
	// 	
	// 	///////// LINE PLANE INTERSECTION
	// 	FVector PlaneOrigin = Character->GetActorLocation();  // A point on the plane (e.g., origin)
	// 	FVector PlaneNormal(1.0f, 0.0f, 0.0f);  // The plane's normal (e.g., x-axis)
	// 	/// // Compute the dot product between the line direction and the plane normal
	// 	float Denominator = FVector::DotProduct(PlaneNormal, End);
	//
	// 	// Check if the line is parallel to the plane
	// 	if (FMath::IsNearlyZero(Denominator)) {
	// 		return; // No intersection (or line is parallel to the plane)
	// 	}
	//
	// 	// Compute the parameter 't' for the intersection point
	// 	float t = FVector::DotProduct(PlaneNormal, PlaneOrigin - Start) / Denominator;
	//
	// 	// Compute the intersection point using the parametric line equation
	// 	FVector IntersectionPoint = Start + t * End;
	//
	// 	
	// 	/////////// Perform the line trace
	// 	FHitResult HitResult;
	// 	FCollisionQueryParams CollisionParams;
	// 	CollisionParams.AddIgnoredActor(GetOwner()); // Ignore the character itself
	// 	
	// 	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, PlaneOrigin, IntersectionPoint, ECC_Visibility, CollisionParams);
 //        
	// 	if (bHit)
	// 	{
	// 		// Handle hit (e.g., log hit actor's name)
	// 		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());
	// 	}
	//
	// 	// Optional: Draw debug line for visualization
	// 	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 4.0f, 0, 1.0f);
	// }
}