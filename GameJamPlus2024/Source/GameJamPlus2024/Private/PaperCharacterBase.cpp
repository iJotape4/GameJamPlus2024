// Fill out your copyright notice in the Description page of Project Settings.


#include "GameJamPlus2024/Public/PaperCharacterBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PaperFlipbookComponent.h"
#include "GameJamPlus2024/GameJamPlus2024Character.h"

APaperCharacterBase::APaperCharacterBase()
{
}


void APaperCharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APaperCharacterBase::Move);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APaperCharacterBase::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		SwitchAnimation(MovementVector);
	}
}

void APaperCharacterBase::SwitchAnimation(const FVector2d& value)
{
	if(GetSprite() == nullptr)
	{
		UE_LOG(LogAnimation, Error, TEXT("SpriteComponent is null"));
		return;
	}
	
	if(value.X > 0)
	{
		GetSprite()->SetFlipbook(Animations["LEFT"]);
	}
	else if(value.X < 0)
	{
		GetSprite()->SetFlipbook(Animations["RIGHT"]);
	}
	if(value.Y > 0)
	{
		GetSprite()->SetFlipbook(Animations["UP"]);
	}
	else if(value.Y < 0)
	{
		GetSprite()->SetFlipbook(Animations["DOWN"]);
	}
}

void APaperCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}
