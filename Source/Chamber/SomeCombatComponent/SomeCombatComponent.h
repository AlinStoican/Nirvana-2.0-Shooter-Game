// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "D:\Unreal\Chamber\Source\Chamber\Chamber\ChamberTypes\WeaponTypes.h"
#include "D:\Unreal\Chamber\Source\Chamber\HUD\ChamberHUD.h"
#include "SomeCombatComponent.generated.h"

#define TRACE_LENGTH 80000.f

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class  CHAMBER_API USomeCombatComponent : public UActorComponent
{
	GENERATED_BODY()


public:	

	USomeCombatComponent();

	friend class AChamberCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void EquipWeapon(class ARIFLE* WeaponToEquip);
protected:


	virtual void BeginPlay() override;

	void SetAiming(bool bIsAiming);

	void FireButtonPressed(bool bPressed);

private:
	ARIFLE* EquippedWeapon;

	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	void SwapWeapons();
	void Reload();
	UFUNCTION(BlueprintCallable)
	void FinishReloading();
	void HandleReload();
	UFUNCTION(BlueprintCallable)
	void FinishSwap();

	UFUNCTION(BlueprintCallable)
	void FinishSwapAttachWeapons();
	//void SetHUDAmmo();

	void PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount);
	bool bLocallyReloading = false;
protected:
	
	void Fire();
	void FireProjectileWeapon();
	void FireHitScanWeapon();
	void LocalFire(const FVector_NetQuantize& TraceHitTarget);
	

	int32 AmountToReload();


	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);



	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> GrenadeClass;

	//void DropEquippedWeapon();
	void DropEquippedWeapon();
	void AttachActorToRightHand(AActor* ActorToAttach);
	void AttachActorToLeftHand(AActor* ActorToAttach);
	void AttachFlagToLeftHand(ARIFLE* Flag);
	void AttachActorToBackpack(AActor* ActorToAttach);
	void UpdateCarriedAmmo();
	void PlayEquipWeaponSound(ARIFLE* WeaponToEquip);
	void ReloadEmptyWeapon();
	void EquipPrimaryWeapon(ARIFLE* WeaponToEquip);
	void EquipSecondaryWeapon(ARIFLE* WeaponToEquip);
private:
	UPROPERTY()
	class AChamberCharacter* Character;
	UPROPERTY()
	class AChamberPlayerController* Controller;
	UPROPERTY()
	class AChamberHUD* HUD;


	ARIFLE* SecondaryWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_Aiming)
	bool bAiming = false;
	bool bAimButtonPressed = false;

	UFUNCTION()
	void OnRep_Aiming();

	UFUNCTION()
	void Fire_Implementation();

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;


	bool bFireButtonPressed;

	/**
	* HUD and crosshairs
	*/

	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;

	FVector HitTarget;

	FHUDPackage HUDPackage;


	// Field of view when not aiming; set to the camera's base FOV in BeginPlay
	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomedFOV = 30.f;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);

	FTimerHandle FireTimer;
	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();

	bool CanFire();

	int32 CarriedAmmo;

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
	int32 MaxCarriedAmmo = 500;

	UPROPERTY(EditAnywhere)
	int32 StartingARAmmo = 30;


	UPROPERTY(EditAnywhere)
	int32 StartingPistolAmmo = 0;


	UPROPERTY(EditAnywhere)
	int32 StartingSniperAmmo = 0;

	void InitializeCarriedAmmo();

	ECombatState CombatState = ECombatState::ECS_Unoccupied;
	void UpdateAmmoValues();

public:
	
	bool ShouldSwapWeapons();
};

