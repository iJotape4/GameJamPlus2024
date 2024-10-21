// Fill out your copyright notice in the Description page of Project Settings.


#include "GameJamPlus2024/Public/PaperCharacterBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EventsManager.h"
#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "GameJamPlus2024/GameJamPlus2024Character.h"
#include "Hazards/HazardBase.h"
#include "Kismet/GameplayStatics.h"

APaperCharacterBase::APaperCharacterBase()
{
	DoubleJumpComponent = CreateDefaultSubobject<UDoubleJumpComponent>(TEXT("DoubleJumpComponent"));
	GrapplingHookComponent = CreateDefaultSubobject<UGrapplingHookComponent>(TEXT("GrapplingHookComponent"));
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

		//Camera Rotation
		EnhancedInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Triggered, this, &APaperCharacterBase::RotateCamera);

		//Jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Grappling Hook

		//Bind Action, and call the hook launch method in the GrapplingHookComponent passing the mouse position
		EnhancedInputComponent->BindAction(GrapplingHookAction, ETriggerEvent::Triggered, GrapplingHookComponent, &UGrapplingHookComponent::LaunchHook); 

		//Cast<APlayerController>(GetController())->GetMousePosition(MousePosition.X, MousePosition.Y);
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
	else if(value.Y > 0)
	{
		GetSprite()->SetFlipbook(Animations["UP"]);
	}
	else if(value.Y < 0)
	{
		GetSprite()->SetFlipbook(Animations["DOWN"]);
	}
}

//ROTATECAMERA METHOD DEPRECATED TEMPORARILY
void APaperCharacterBase::RotateCamera(const FInputActionValue& Value)
{

	// Obtiene el nombre del nivel actual
	FName NombreNivelActual = *GetWorld()->GetMapName();
	// Vuelve a cargar el nivel actual
	UGameplayStatics::OpenLevel(this, NombreNivelActual);
	// FVector CameraRotation = Value.Get<FVector>();
	//
	// //Method to rotate the camera smoothly by a certain angle when the input is triggered
	// FRotator CurrentRotation = Controller->GetControlRotation();
	// FRotator TargetRotation = CurrentRotation;
	//
	// if (CameraRotation.X < 0)
	// {
	// 	TargetRotation.Yaw -= CameraRotationAngle;  // Rotate left
	// }
	// else if (CameraRotation.X > 0)
	// {
	// 	TargetRotation.Yaw += CameraRotationAngle;  // Rotate right
	// }
	// GetController()->SetControlRotation(TargetRotation);
}

void APaperCharacterBase::TakeDamage(int Damage)
{
	EventsManager::SendEvent(E_EventType::DamageTaken, Damage);
}


void APaperCharacterBase::EnableMouseEvents()
{
	APlayerController* PC = Cast<APlayerController>(GetController());

	if (PC)
	{
		PC->bShowMouseCursor = true; 
		PC->bEnableClickEvents = true; 
		PC->bEnableMouseOverEvents = true;
	}
}

FVector2D APaperCharacterBase::GetMousePosition()
{
	FVector2D MousePosition(0.0f, 0.0f);
	Cast<APlayerController>(GetController())->GetMousePosition(MousePosition.X, MousePosition.Y);
	return FVector2D(MousePosition);
}

FVector APaperCharacterBase::ConvertScreenToWorldPoint(const FVector2D& ScreenPosition)
{
	FVector WorldLocation;
	FVector WorldDirection = GetActorRightVector();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	FVector PlayerLocation = PlayerController->GetPawn()->GetActorLocation();
	//UE_LOG(LogTemp, Log, TEXT("Player Location: X = %f, Y = %f, Z = %f"), PlayerLocation.X, PlayerLocation.Y, PlayerLocation.Z);
	PlayerController->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldLocation,WorldDirection);
	
	//return FVector(PlayerLocation.X, WorldLocation.X, WorldLocation.Y);
	return WorldLocation;
}

void APaperCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &APaperCharacterBase::OnBeginOverlap);

	EnableMouseEvents();
}

void APaperCharacterBase::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != this) // Ensure it's a valid actor and not self
	{
		AHazardBase* Hazard = Cast<AHazardBase>(OtherActor);
		if(Hazard != nullptr)
		{
			TakeDamage(Hazard->Damage);
		}
	}
}
