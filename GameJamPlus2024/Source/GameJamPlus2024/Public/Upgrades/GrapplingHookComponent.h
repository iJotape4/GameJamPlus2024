#pragma once

#include "CoreMinimal.h"
#include "CharacterUpgrade.h"
#include "InputAction.h"
#include "CableComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GrapplingHookComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAMPLUS2024_API UGrapplingHookComponent : public UCharacterUpgrade
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grappling Hook", meta = (AllowPrivateAccess = "true"))
	UCableComponent* CableComponent; 

	bool isGrappling;
	FVector Grabpoint;
	UCharacterMovementComponent* CharacterMovement;

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

public:
};
