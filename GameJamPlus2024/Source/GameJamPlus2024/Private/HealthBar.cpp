// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"
#include "EventsManager.h"

void UHealthBar::OnDamageTaken(E_EventType Arg, int Damage)
{
	SetHealthBarPercent(-Damage);
	UE_LOG(LogActor, Warning, TEXT("Damage Taken: %d"), Damage);
}

void UHealthBar::NativeConstruct()
{
	EventsManager::AddListener(E_EventType::DamageTaken, this, &UHealthBar::OnDamageTaken);
	EventsManager::AddListener(E_EventType::MaxHealthUpgrade, this, &UHealthBar::SetMaxHealth);
}

void UHealthBar::SetHealthBarPercent(float Percent)
{
	float NewPercent = HealthBar->GetPercent() + Percent / MaxHealth;
	HealthBar->SetPercent(NewPercent);	
}

void UHealthBar::SetMaxHealth(E_EventType Arg, int NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
	HealthBar->SetPercent(1.0f);
}