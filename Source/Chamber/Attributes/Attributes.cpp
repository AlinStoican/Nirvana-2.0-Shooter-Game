// Fill out your copyright notice in the Description page of Project Settings.


#include "Attributes.h"

 UAttribute::UAttribute()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAttribute::BeginPlay()
{
	Super::BeginPlay();

}

void UAttribute::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}


float UAttribute::GetHealthPercent()
{
	return Health / MaxHealth;
}


bool UAttribute::IsAlive()
{
	return Health > 0.f;
}


void UAttribute::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


