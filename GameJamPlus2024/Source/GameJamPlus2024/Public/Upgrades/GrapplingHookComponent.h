#pragma once

#include "CoreMinimal.h"
#include "CharacterUpgrade.h"
#include "CableComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GrapplingHookComponent.generated.h"

class UCameraComponent;
class APaperCharacterBase;
/**
 * 
 */
UCLASS()
class GAMEJAMPLUS2024_API UGrapplingHookComponent : public UCharacterUpgrade
{
	GENERATED_BODY()

private:
	UCameraComponent* Camera;
	APaperCharacterBase* Character;
	APlayerController* PlayerController;
	UCharacterMovementComponent* MovementComponent;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grappling Hook", meta = (AllowPrivateAccess = "true"))
	UCableComponent* CableComponent; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grappling Hook", meta = (AllowPrivateAccess = "true"))
	bool isGrappling;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grappling Hook", meta = (AllowPrivateAccess = "true"))
	FVector Grabpoint;

	void HookLineTrace(FHitResult& OutHit);
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void LaunchHook(const FInputActionValue& Value);

	UFUNCTION()
	void RetractHook(const FInputActionValue& Value);

	UFUNCTION()
	void HookGrappling(const FInputActionValue& Value) ;
	
	UFUNCTION()
	void SetCableComponentVisibility(bool bVisible);

private:
	UFUNCTION()
	bool InitializeComponents();

	UFUNCTION()
	bool GetMousePosition(FVector2D& MousePosition) const;

	UFUNCTION()
	bool DeprojectMousePosition(const FVector2D& MousePosition, FVector& WorldLocation, FVector& WorldDirection) const;

	UFUNCTION()
	bool CalculateLinePlaneIntersection(const FVector& WorldLocation, const FVector& WorldDirection, FVector& IntersectionPoint) const;

	UFUNCTION()
	void PerformLineTrace(const FVector& IntersectionPoint, FHitResult& OutHit);

	UFUNCTION()
	void HandleHit(const FHitResult& HitResult);

	UFUNCTION()
	void HandleMiss(FVector IntersectionPoint);
};
