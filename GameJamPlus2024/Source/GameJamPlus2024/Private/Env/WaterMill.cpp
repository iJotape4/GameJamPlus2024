// Fill out your copyright notice in the Description page of Project Settings.


#include "Env/WaterMill.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"

// Sets default values
AWaterMill::AWaterMill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaterMill::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWaterMill::Turn(float DeltaTime)
{
	// Define the rotation speed (degrees per second)
	float RotationSpeed = 45.0f;  // 45 degrees per second
	// Calculate the rotation for this frame
	FRotator RotationDelta(0.0f,  0.0f, -RotationSpeed * DeltaTime);  // Y-axis rotation
	// Apply the rotation
	AddActorLocalRotation(RotationDelta, false, nullptr, ETeleportType::None);
}

// Called every frame
void AWaterMill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Turn(DeltaTime);
}

