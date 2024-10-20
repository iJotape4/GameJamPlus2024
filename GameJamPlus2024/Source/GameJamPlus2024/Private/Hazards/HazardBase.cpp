// Fill out your copyright notice in the Description page of Project Settings.


#include "Hazards/HazardBase.h"

// Sets default values
AHazardBase::AHazardBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
}

// Called when the game starts or when spawned
void AHazardBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHazardBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

