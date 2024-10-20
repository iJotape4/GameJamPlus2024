// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventsManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthBar.generated.h"

UCLASS()
class GAMEJAMPLUS2024_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

	void OnDamageTaken(E_EventType Arg, int Damage);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HealthBar;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	int MaxHealth = 100;

protected:	
	// This function is called when the widget is constructed
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void SetHealthBarPercent(float Percent);

	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(E_EventType Arg, int NewMaxHealth);
};
