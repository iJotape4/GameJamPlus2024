// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "PaperCharacter.h"
#include "PaperFlipbook.h"
#include "GameJamPlus2024/GameJamPlus2024Character.h"
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TMap<FString, UPaperFlipbook*> Animations;

	
public:
	APaperCharacterBase();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void SwitchAnimation(const FVector2d& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;
};
