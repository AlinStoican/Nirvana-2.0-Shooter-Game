 

#include "ChamberCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "D:\Unreal\Chamber\Source\Chamber\Chamber.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "SomeCombatComponent/SomeCombatComponent.h"
#include "D:\Unreal\Chamber\Source\Chamber\CombatComponent\CombatComponent.h"
#include "ZING/Rifle.h"
#include "Sound/SoundCue.h"
#include "D:\Unreal\Chamber\Source\Chamber\EnemyController.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ChamberCHAnimInstance/ChamberCHAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "D:\Unreal\Chamber\Source\Chamber\ChamberPlayerState.h"
#include "D:\Unreal\Chamber\Source\Chamber\Attributes\AttributeComponent.h"


DEFINE_LOG_CATEGORY(LogChamberCharacter);


AChamberCharacter::AChamberCharacter()
{
	

	PrimaryActorTick.bCanEverTick = true;


	//Attributes = CreateDefaultSubobject<UChamberAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	Combat = CreateDefaultSubobject<USomeCombatComponent>(TEXT("SomeCombatComponent"));
	Combat->SetIsReplicated(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 850.f);
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;

}

void AChamberCharacter::BeginPlay()
{
	
	Super::BeginPlay();

	EquippedWeapon = nullptr; // Initialize the pointer
}

void AChamberCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void AChamberCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AimOffset(DeltaTime);
	if (GetLocalRole() > ENetRole::ROLE_SimulatedProxy && IsLocallyControlled())
	{
		AimOffset(DeltaTime);
	}
	else
	{
		TimeSinceLastMovementReplication += DeltaTime;
		if (TimeSinceLastMovementReplication > 0.25f)
		{
			OnRep_ReplicatedMovement();
		}
		CalculateAO_Pitch();
	}

	HideCameraIfCharacterClose();
}

void AChamberCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{


	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAxis("MoveForward", this, &AChamberCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AChamberCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AChamberCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AChamberCharacter::LookUp);

	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &AChamberCharacter::EquipButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AChamberCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AChamberCharacter::AimButtonReleased);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AChamberCharacter::FireButtonReleased);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AChamberCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AChamberCharacter::StartFire);

}

void AChamberCharacter::StartFire()
{
	if (EquippedWeapon)
	{
		FVector HitTarget = GetHitTarget(); // Assuming GetHitTarget() is defined and returns a FVector
			UE_LOG(LogChamberCharacter, Log, TEXT("AChamberCharacter::StartFire calling Fire with HitTarget: %s"), *HitTarget.ToString());
		EquippedWeapon->Fire(HitTarget);

		UE_LOG(LogChamberCharacter, Log, TEXT("Weapon equipped: %s"), *EquippedWeapon->GetName());

	}
	else
	{
			UE_LOG(LogChamberCharacter, Warning, TEXT("No weapon equipped"));
	}

}

void AChamberCharacter::EquipWeapon(ARIFLE* WeaponToEquip)
{
	if (WeaponToEquip)
	{
		EquippedWeapon = WeaponToEquip;
		UE_LOG(LogChamberCharacter, Log, TEXT("Equipped weapon: %s"), *WeaponToEquip->GetName());
		// Additional logic for equipping the weapon...
	}
	else
	{
		UE_LOG(LogChamberCharacter, Warning, TEXT("Tried to equip a null weapon"));
	}
}

bool AChamberCharacter::IsAiming()
{
	return (Combat && Combat->bAiming);
}

void AChamberCharacter::PlaySwapMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SwapMontage)
	{
		AnimInstance->Montage_Play(SwapMontage);
	}
}

void AChamberCharacter::PlayReloadMontage()
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ReloadMontage)
	{
		AnimInstance->Montage_Play(ReloadMontage);
		FName SectionName;

		switch (Combat->EquippedWeapon->GetWeaponType())
		{
		case EWeaponType::EWT_AssaultRifle:
			SectionName = FName("Rifle");
			break;
		
		case EWeaponType::EWT_Pistol:
			SectionName = FName("Pistol");
			break;
	
		case EWeaponType::EWT_SniperRifle:
			SectionName = FName("SniperRifle");
			break;
	
		}

		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void  AChamberCharacter::HideCameraIfCharacterClose()
{
//	if (!IsLocallyControlled()) return;
//	if ((FollowCamera->GetComponentLocation() - GetActorLocation()).Size() < CameraThreshold)
//	{
//		GetMesh()->SetVisibility(false);
//		if (Combat && Combat->EquippedWeapon && Combat->EquippedWeapon->GetWeaponMesh())
//		{
//			Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = true;
//		}
//		if (Combat && Combat->SecondaryWeapon && Combat->SecondaryWeapon->GetWeaponMesh())
//		{
//			Combat->SecondaryWeapon->GetWeaponMesh()->bOwnerNoSee = true;
//		}
//	}
//	else
//	{
//		GetMesh()->SetVisibility(true);
//		if (Combat && Combat->EquippedWeapon && Combat->EquippedWeapon->GetWeaponMesh())
//		{
//			Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = false;
//		}
//		if (Combat && Combat->SecondaryWeapon && Combat->SecondaryWeapon->GetWeaponMesh())
//		{
//			Combat->SecondaryWeapon->GetWeaponMesh()->bOwnerNoSee = false;
//		}
//	}

	//if (!IsLocallyControlled()) return;
	//if ((FollowCamera->GetComponentLocation() - GetActorLocation()).Size() < CameraThreshold)
	//{
	//	GetMesh()->SetVisibility(false);
	//	if (Combat && Combat->EquippedWeapon && Combat->EquippedWeapon->GetWeaponMesh())
	//	{
	//		Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = true;
	//	}
	//}
	//else
	//{
	//	GetMesh()->SetVisibility(true);
	//	if (Combat && Combat->EquippedWeapon && Combat->EquippedWeapon->GetWeaponMesh())
	//	{
	//		Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = false;
	//	}
	//}
}

void AChamberCharacter::AimOffset(float DeltaTime)
{
	if (Combat && Combat->EquippedWeapon == nullptr) return;
	float Speed = CalculateSpeed();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && !bIsInAir) // standing still, not jumping
	{
		bRotateRootBone = true;
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		if (TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = true;
		TurnInPlace(DeltaTime);
	}
	if (Speed > 0.f || bIsInAir) // running, or jumping
	{
		bRotateRootBone = false;
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}

	CalculateAO_Pitch();
}

void AChamberCharacter::PlayFireMontage(bool bAiming)
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		FName SectionName;
		SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void AChamberCharacter::PlayHitReactMontage()
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		FName SectionName("FromFront");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}



void AChamberCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat)
	{
		Combat->Character = this;

	}
}

void AChamberCharacter::EquipButtonPressed()
{
	if (Combat && HasAuthority())
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

void AChamberCharacter::AimButtonPressed()
{
	
	if(Combat)
	{
		Combat->bAiming = true;
	}
}

void AChamberCharacter::AimButtonReleased()
{

	if (Combat)
	{
		Combat->bAiming = false;
	}
}

void AChamberCharacter::FireButtonPressed()
{
	if (Combat)
	{
		Combat->FireButtonPressed(true);
	}

}

void AChamberCharacter::FireButtonReleased()
{
	if (Combat)
	{
		Combat->FireButtonPressed(false);
	}

}

void AChamberCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void AChamberCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void AChamberCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AChamberCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AChamberCharacter::SetOverlappingWeapon(ARIFLE* Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget();
	}
	OverlappingWeapon = Weapon;
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget();
		}
	}
}


bool AChamberCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}


void AChamberCharacter::CalculateAO_Pitch()
{
	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f && !IsLocallyControlled())
	{
		// map pitch from [270, 360) to [-90, 0)
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

void AChamberCharacter::SimProxiesTurn()
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;
	bRotateRootBone = false;
	float Speed = CalculateSpeed();
	if (Speed > 0.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}

	ProxyRotationLastFrame = ProxyRotation;
	ProxyRotation = GetActorRotation();
	ProxyYaw = UKismetMathLibrary::NormalizedDeltaRotator(ProxyRotation, ProxyRotationLastFrame).Yaw;

	UE_LOG(LogTemp, Warning, TEXT("ProxyYaw: %f"), ProxyYaw);

	if (FMath::Abs(ProxyYaw) > TurnThreshold)
	{
		if (ProxyYaw > TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_Right;
		}
		else if (ProxyYaw < -TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_Left;
		}
		else
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		}
		return;
	}
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;

}


ARIFLE* AChamberCharacter::GetEquippedWeapon()
{
	if (Combat == nullptr) return nullptr;
	return Combat->EquippedWeapon;
}

FVector AChamberCharacter::GetHitTarget() const
{
	if (Combat == nullptr) return FVector();
	return Combat->HitTarget;
}

bool AChamberCharacter::IsLocallyReloading()
{
	if (Combat == nullptr) return false;
	return Combat->bLocallyReloading;
}

ECombatState AChamberCharacter::GetCombatState() const
{
	if (Combat == nullptr) return ECombatState::ECS_MAX;
	return Combat->CombatState;
}

float AChamberCharacter::CalculateSpeed()
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	return Velocity.Size();
}

void AChamberCharacter::RotateInPlace(float DeltaTime)
{
	if (Combat)
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}
	if (bDisableGameplay)
	{
		bUseControllerRotationYaw = false;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}
	if (GetLocalRole() > ENetRole::ROLE_SimulatedProxy && IsLocallyControlled())
	{
		AimOffset(DeltaTime);
	}
	else
	{
		TimeSinceLastMovementReplication += DeltaTime;
		if (TimeSinceLastMovementReplication > 0.25f)
		{
			OnRep_ReplicatedMovement();
		}
		CalculateAO_Pitch();
	}
}


void AChamberCharacter::TurnInPlace(float DeltaTime)
{
	if (AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}
	if (TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 4.f);
		AO_Yaw = InterpAO_Yaw;
		if (FMath::Abs(AO_Yaw) < 15.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

void AChamberCharacter::Die()
{
	bDead = true;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
		AnimInstance->Montage_JumpToSection(FName("DeathA"), DeathMontage);
	}
}

float AChamberCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - DamageAmount <= 0.f)
	{
		Health = 0.f;
		Die();

		auto EnemyController = Cast<AEnemyController>(EventInstigator);
		//TO DO
		/*if (EnemyController)
		{
			EnemyController->GetBlackboardComponent()->SetValueAsBool(
				FName(TEXT("CharacterDead")),
				true
			);
		}*/
	}
	else
	{
		Health -= DamageAmount;
	}
	return DamageAmount;
}


void AChamberCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	//ChamberGameMode = ChamberGameMode == nullptr ? GetWorld()->GetAuthGameMode<AChamberGameMode>() : ChamberGameMode;
	//if (bElimmed || ChamberGameMode == nullptr) return;
	//Damage = ChamberGameMode->CalculateDamage(InstigatorController, Controller, Damage);

	//float DamageToHealth = Damage;
	//if (Shield > 0.f)
	//{
	//	if (Shield >= Damage)
	//	{
	//		Shield = FMath::Clamp(Shield - Damage, 0.f, MaxShield);
	//		DamageToHealth = 0.f;
	//	}
	//	else
	//	{
	//		DamageToHealth = FMath::Clamp(DamageToHealth - Shield, 0.f, Damage);
	//		Shield = 0.f;
	//	}
	//}

	//Health = FMath::Clamp(Health - DamageToHealth, 0.f, MaxHealth);

	////UpdateHUDHealth();
	////UpdateHUDShield();
	//PlayHitReactMontage();

	//if (Health == 0.f)
	//{
	//	if (ChamberGameMode)
	//	{
	//		ChamberPlayerController = ChamberPlayerController == nullptr ? Cast<AChamberPlayerController>(Controller) : ChamberPlayerController;
	//		AChamberPlayerController* AttackerController = Cast<AChamberPlayerController>(InstigatorController);
	//		ChamberGameMode->PlayerEliminated(this, ChamberPlayerController, AttackerController);
	//	}
	//}
}