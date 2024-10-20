// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterUpgrade.h"
#include "InputAction.h"
#include "GrapplingHookComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAMPLUS2024_API UGrapplingHookComponent : public UCharacterUpgrade
{
	GENERATED_BODY()
	
protected:
	void HookLineTrace(FHitResult& OutHit);
public:
	void LaunchHook(const FInputActionValue& Value);

};
