// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleJumpComponent.h"

#include "GameFramework/Character.h"

// Sets default values for this component's properties
UDoubleJumpComponent::UDoubleJumpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UDoubleJumpComponent::SetEnabled(bool bNewEnabled)
{
	Super::SetEnabled(bNewEnabled);
	AActor* Owner = GetOwner();
	if(Owner == nullptr)
	{
		UE_LOG(LogActor, Error, TEXT("Owner is null"));
		return;
	}
	
	ACharacter* Character = Cast<ACharacter>(Owner);
	
	if (bNewEnabled)
	{
		Owner->Tags.Add("Upgrade-DoubleJump");
		Character->JumpMaxCount = 2;
	}
	else
	{
		Owner->Tags.Remove("Upgrade-DoubleJump");
		Character->JumpMaxCount = 1;
	}
}


// Called when the game starts
void UDoubleJumpComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDoubleJumpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

