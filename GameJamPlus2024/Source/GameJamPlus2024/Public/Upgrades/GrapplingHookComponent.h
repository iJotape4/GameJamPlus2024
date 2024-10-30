#pragma once

#include "CoreMinimal.h"
#include "CharacterUpgrade.h"
#include "InputAction.h"
#include "CableComponent.h"
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

	void HookLineTrace(FHitResult& OutHit);
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void LaunchHook(const FInputActionValue& Value);

	UFUNCTION()
	void SetCableComponentVisibility(bool bVisible);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
