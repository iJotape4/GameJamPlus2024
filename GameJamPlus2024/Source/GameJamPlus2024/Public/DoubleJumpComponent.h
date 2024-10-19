// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterUpgrade.h"
#include "Components/ActorComponent.h"
#include "DoubleJumpComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEJAMPLUS2024_API UDoubleJumpComponent : public UCharacterUpgrade
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoubleJumpComponent();

	void SetEnabled(bool bNewEnabled) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
