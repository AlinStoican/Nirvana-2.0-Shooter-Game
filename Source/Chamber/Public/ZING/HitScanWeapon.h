#pragma once
// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreMinimal.h"
#include "Rifle.h"
#include "HitScanWeapon.generated.h"

/**
 *
 */

UCLASS()
class CHAMBER_API AHitScanWeapon : public ARIFLE
{
	GENERATED_BODY()
public:
	virtual void Fire(const FVector& HitTarget) override;
protected:

	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	USoundCue* HitSound;
private:



	UPROPERTY(EditAnywhere)
	UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundCue* FireSound;

};
