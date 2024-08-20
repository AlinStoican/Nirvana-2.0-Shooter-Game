// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZING/Rifle.h"
#include "ProjectileRIFLE.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogProjectileRifle, Log, All);

UCLASS()
class CHAMBER_API AProjectileRIFLE : public ARIFLE
{
	GENERATED_BODY()
public:
	virtual void Fire(const FVector& HitTarget) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile>ProjectileClass;

};
