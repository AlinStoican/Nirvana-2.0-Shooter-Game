// Fill out your copyright notice in the Description page of Project Settings.

#include "ChamberCHAnimInstance.h"
#include "D:\Unreal\Chamber\Source\Chamber\ChamberCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ZING/Rifle.h"




void UChamberCHAnimInstance::NativeInitializeAnimation()
{

Super::NativeInitializeAnimation();

ChamberCharacter = Cast<AChamberCharacter>(TryGetPawnOwner());

}

void UChamberCHAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (ChamberCharacter == nullptr)
	{
		ChamberCharacter = Cast<AChamberCharacter>(TryGetPawnOwner());
	}
	if (ChamberCharacter == nullptr) return;

	FVector Velocity = ChamberCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = ChamberCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = ChamberCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bWeaponEquipped = ChamberCharacter->IsWeaponEquipped();
	bAiming = ChamberCharacter->IsAiming();
	TurningInPlace = ChamberCharacter->GetTurningInPlace();
	bRotateRootBone = ChamberCharacter->ShouldRotateRootBone();
	bElimmed = ChamberCharacter->IsElimmed();

	FRotator AimRotation = ChamberCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ChamberCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
	YawOffset = DeltaRotation.Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = ChamberCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);
	AO_Yaw = ChamberCharacter->GetAO_Yaw();
	AO_Pitch = ChamberCharacter->GetAO_Pitch();
	
	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && ChamberCharacter->GetMesh())
	{
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		ChamberCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));
		if (ChamberCharacter->IsLocallyControlled())
		{
			bLocallyControlled = true;
			FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("hand_r"), ERelativeTransformSpace::RTS_World);
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - ChamberCharacter->GetHitTarget()));
			RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaTime, 30.f);
		}
	}

	bUseFABRIK = ChamberCharacter->GetCombatState() == ECombatState::ECS_Unoccupied;
	bool bFABRIKOverride = ChamberCharacter->IsLocallyControlled() &&
		ChamberCharacter->bFinishedSwapping;
	if (bFABRIKOverride)
	{
		bUseFABRIK = !ChamberCharacter->IsLocallyReloading();
	}
	bUseAimOffsets = ChamberCharacter->GetCombatState() == ECombatState::ECS_Unoccupied && !ChamberCharacter->GetDisableGameplay();
	bTransformRightHand = ChamberCharacter->GetCombatState() == ECombatState::ECS_Unoccupied && !ChamberCharacter->GetDisableGameplay();

}