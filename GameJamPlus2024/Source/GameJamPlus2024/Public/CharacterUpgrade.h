// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterUpgrade.generated.h"


UCLASS(Abstract )
class GAMEJAMPLUS2024_API UCharacterUpgrade : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bEnabled = false;
	
public:	
	// Sets default values for this component's properties
	UCharacterUpgrade();

	//Getters
	UFUNCTION(BlueprintCallable, Category = "Upgrades")
	bool IsEnabled() const { return bEnabled; }

	//Setters
	UFUNCTION(BlueprintCallable, Category = "Upgrades")
	virtual void SetEnabled(bool bNewEnabled) { bEnabled = bNewEnabled; };
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
