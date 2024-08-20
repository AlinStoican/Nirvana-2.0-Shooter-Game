// Fill out your copyright notice in the Description page of Project Settings.


#include "ZING/ProjectileRIFLE.h"
#include "Engine/SkeletalMeshSocket.h"
#include "ZING/Projectile.h"

    DEFINE_LOG_CATEGORY(LogProjectileRifle);

void AProjectileRIFLE::Fire(const FVector& HitTarget)
{

    Super::Fire(HitTarget);
    APawn* InstigatorPawn = Cast<APawn>(GetOwner());
    const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
    if (MuzzleFlashSocket)
    {
        FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
        FVector ToTarget = HitTarget - SocketTransform.GetLocation();
        FRotator TargetRotation = ToTarget.Rotation();
        if (ProjectileClass && InstigatorPawn)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = GetOwner();
            SpawnParams.Instigator = InstigatorPawn;
            UWorld* World = GetWorld();
            if (World)
            {
                AProjectile* SpawnedProjectile = World->SpawnActor<AProjectile>(
                    ProjectileClass,
                    SocketTransform.GetLocation(),
                    TargetRotation,
                    SpawnParams
                );

                if (SpawnedProjectile)
                {
                    SpawnedProjectile->Damage = Damage;
                    SpawnedProjectile->HeadShotDamage = HeadShotDamage;
                    UE_LOG(LogProjectileRifle, Log, TEXT("Projectile spawned successfully at %s with damage %f and headshot damage %f"),
                        *SocketTransform.GetLocation().ToString(), Damage, HeadShotDamage);
                }
                else
                {
                    UE_LOG(LogProjectileRifle, Log, TEXT("Failed to spawn projectile!"));
                }
            }
            else
            {
                UE_LOG(LogProjectileRifle, Log, TEXT("World is nullptr!"));
            }
        }
        else
        {
            UE_LOG(LogProjectileRifle, Log , TEXT("ProjectileClass or InstigatorPawn is not valid!"));
        }
    }
    else
    {
        UE_LOG(LogProjectileRifle, Log,  TEXT("MuzzleFlashSocket not found!"));
    }
}