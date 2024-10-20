// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleJumpComponent.h"
#include "InputActionValue.h"
#include "PaperCharacter.h"
#include "PaperFlipbook.h"
#include "GameJamPlus2024/GameJamPlus2024Character.h"
#include "Upgrades/GrapplingHookComponent.h"
#include "PaperCharacterBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class GAMEJAMPLUS2024_API APaperCharacterBase : public APaperCharacter
{
	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	//** Rotate Camera Input ACtion */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RotateCameraAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TMap<FString, UPaperFlipbook*> Animations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* GrapplingHookAction;

public:
	APaperCharacterBase();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Upgrades", meta = (AllowPrivate))
	UDoubleJumpComponent* DoubleJumpComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Upgrades", meta = (AllowPrivate))
	UGrapplingHookComponent* GrapplingHookComponent;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivate = "true"))
	float CameraRotationAngle = 90.0f;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void SwitchAnimation(const FVector2d& Value);
	void RotateCamera(const FInputActionValue& Value);
	void TakeDamage(int Damage);
	void EnableMouseEvents();


	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;
public:	
	FVector2D GetMousePosition();
	FVector ConvertScreenToWorldPoint(const FVector2D& ScreenPosition);

private:
	
	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
