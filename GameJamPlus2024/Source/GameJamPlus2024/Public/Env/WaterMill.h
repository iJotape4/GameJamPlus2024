// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaterMill.generated.h"

UCLASS()
class GAMEJAMPLUS2024_API AWaterMill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaterMill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Turn(float DeltaTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
