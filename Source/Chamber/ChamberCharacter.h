// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Chamber/ChamberTypes/TurnInPlace.h"
#include "Interfaces/HitInterface.h"
#include "Character/CharacterTypes.h"
#include "Character/BaseCharacter.h"
#include "D:\Unreal\Chamber\Source\Chamber\Interfaces\CrosshairsInterface.h"
#include "D:\Unreal\Chamber\Source\Chamber\Chamber\ChamberTypes\WeaponTypes.h"
#include "ChamberCharacter.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogChamberCharacter, Log, All);

UCLASS()
class  CHAMBER_API  AChamberCharacter : public ACharacter, public ICrosshairsInterface
{

	GENERATED_BODY()


public:
	AChamberCharacter();


	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	void PlayFireMontage(bool bAiming);
	void PlayReloadMontage();
	void PlayElimMontage();
	void PlaySwapMontage();

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	void Die();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	ARIFLE* EquippedWeapon;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void CalculateAO_Pitch();
	void SimProxiesTurn();
	void EquipButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void AimOffset(float DeltaTime);
	void FireButtonPressed();
	void FireButtonReleased();
	void PlayHitReactMontage();
	void StartFire();
	void EquipWeapon(ARIFLE* WeaponToEquip);
private:


	UPROPERTY(VisibleAnywhere)
	class USomeCombatComponent* Combat;


	bool bElimmed = false;
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;
	void HideCameraIfCharacterClose();

	UPROPERTY(EditAnywhere, Category = Camera)
	float CameraThreshold = 300.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bDead;

	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BloodParticles;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ElimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* SwapMontage;

	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	float CalculateSpeed();

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;
public:
	bool bDisableGameplay = false;
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE bool IsElimmed() const { return bElimmed; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	ECombatState GetCombatState() const;
	void SetOverlappingWeapon(ARIFLE* Rifle);
	bool IsWeaponEquipped();
	bool IsAiming();
	bool bFinishedSwapping = false;
	//FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE UParticleSystem* GetBloodParticles() const { return BloodParticles; }

	


	ARIFLE* GetEquippedWeapon();
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FVector GetHitTarget() const;

	bool IsLocallyReloading();


	UPROPERTY(EditAnywhere)
	TSubclassOf<ARIFLE> DefaultWeaponClass;

	class ARIFLE* OverlappingWeapon;
	void RotateInPlace(float DeltaTime);

};
		