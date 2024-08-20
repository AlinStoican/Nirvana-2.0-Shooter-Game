


#include "ZING/Rifle.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Chamber/ChamberCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimationAsset.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Chamber\SomeCombatComponent\SomeCombatComponent.h"
#include "Chamber\ChamberPlayerController\ChamberPlayerController.h"


DEFINE_LOG_CATEGORY(LogRifle);

ARIFLE::ARIFLE()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;
	
	UE_LOG(LogRifle, Log, TEXT("ARIFLE::ARIFLE constructor called"));

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	/*PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);*/


	/*static ConstructorHelpers::FClassFinder<AProjectile> ProjectileBP(TEXT("/Game/PathToYourProjectileBP"));
	if (ProjectileBP.Succeeded())
	{
		ProjectileClass = ProjectileBP.Class;
	}*/

}

void ARIFLE::EnableCustomDepth(bool bEnable)
{
	if (WeaponMesh)
	{
		WeaponMesh->SetRenderCustomDepth(bEnable);
	}
}

void ARIFLE::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ARIFLE::OnSphereOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &ARIFLE::OnSphereEndOverlap);
	}
	/*if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}*/
}

void ARIFLE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARIFLE::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ARIFLE::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogRifle, Log, TEXT("ARIFLE::OnSphereOverlap called with OtherActor: %s"), *OtherActor->GetName());

	AChamberCharacter* ChamberCharacter = Cast<AChamberCharacter>(OtherActor); 
	if (ChamberCharacter)
	{
		ChamberCharacter->SetOverlappingWeapon(this);
	}
}

void ARIFLE::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogRifle, Log, TEXT("ARIFLE::OnSphereEndOverlap called with OtherActor: %s"), *OtherActor->GetName());

	AChamberCharacter* ChamberCharacter = Cast<AChamberCharacter>(OtherActor);
	if (ChamberCharacter)
	{
		ChamberCharacter->SetOverlappingWeapon(nullptr);
	}
} 

void ARIFLE::ShowPickupWidget()
{
	/*if (PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}*/
}

void ARIFLE::Fire(const FVector& HitTarget)
{
	UE_LOG(LogRifle, Log, TEXT("ARIFLE::Fire called with HitTarget: %s"), *HitTarget.ToString());

	if (FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
		UE_LOG(LogRifle, Log, TEXT("Fire animation played."));
	}
	else
	{
		UE_LOG(LogRifle, Warning, TEXT("Fire animation is null."));
	}
}

void ARIFLE::AddAmmo(int32 AmmoToAdd)
{
	Ammo = FMath::Clamp(Ammo + AmmoToAdd, 0, MagCapacity);
	//SetHUDAmmo();
}

// RE-CHECK
void ARIFLE::SetWeaponState(EWeaponState State)
{
	WeaponState = State;

	UE_LOG(LogRifle, Log, TEXT("ARIFLE::SetWeaponState called with State: %d"), (int32)State);

	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		//ShowPickupWidget(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}


void ARIFLE::OnWeaponStateSet()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		OnEquipped();
		break;
	case EWeaponState::EWS_EquippedSecondary:
		OnEquippedSecondary();
		break;
	case EWeaponState::EWS_Dropped:
		OnDropped();
		break;
	}
}

void ARIFLE::OnEquipped()
{
	//ShowPickupWidget(false);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (WeaponType == EWeaponType::EWT_AssaultRifle)
	{
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	EnableCustomDepth(false);

	ChamberOwnerCharacter = ChamberOwnerCharacter == nullptr ? Cast<AChamberCharacter>(GetOwner()) : ChamberOwnerCharacter;
	if (ChamberOwnerCharacter )
	{
		ChamberOwnerController = ChamberOwnerController == nullptr ? Cast<AChamberPlayerController>(ChamberOwnerCharacter->Controller) : ChamberOwnerController;
	
	}
}

void ARIFLE::OnDropped()
{
	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetEnableGravity(true);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	WeaponMesh->MarkRenderStateDirty();
	EnableCustomDepth(true);

	ChamberOwnerCharacter = ChamberOwnerCharacter == nullptr ? Cast<AChamberCharacter>(GetOwner()) : ChamberOwnerCharacter;
	if (ChamberOwnerCharacter)
	{
		ChamberOwnerController = ChamberOwnerController == nullptr ? Cast<AChamberPlayerController>(ChamberOwnerCharacter->Controller) : ChamberOwnerController;
		
	}
}

void ARIFLE::OnEquippedSecondary()
{
	//ShowPickupWidget(false);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if (WeaponMesh)
	
	{	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_TAN);
		WeaponMesh->MarkRenderStateDirty();
	}
	ChamberOwnerCharacter = ChamberOwnerCharacter == nullptr ? Cast<AChamberCharacter>(GetOwner()) : ChamberOwnerCharacter;

	if (ChamberOwnerCharacter)
	{
		ChamberOwnerController = ChamberOwnerController == nullptr ? Cast<AChamberPlayerController>(ChamberOwnerCharacter->Controller) : ChamberOwnerController;
		
	}
}

void ARIFLE::SpendRound()
	{
		Ammo = FMath::Clamp(Ammo - 1, 0, MagCapacity);
		/*if (HasAuthority())
		{ 
			++Sequence;
		}*/
	}

void ARIFLE::Dropped()
{
	SetWeaponState(EWeaponState::EWS_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	WeaponMesh->DetachFromComponent(DetachRules);
	SetOwner(nullptr);
	ChamberOwnerCharacter = nullptr;
	ChamberOwnerController = nullptr;
}

bool ARIFLE::IsEmpty()
{
	return Ammo <= 0;
}

bool ARIFLE::IsFull()
{
	return Ammo == MagCapacity;
}


FVector ARIFLE::TraceEndWithScatter(const FVector& HitTarget)
{
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket == nullptr) return FVector();

	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
	const FVector TraceStart = SocketTransform.GetLocation();

	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	const FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;
	const FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);
	const FVector EndLoc = SphereCenter + RandVec;
	const FVector ToEndLoc = EndLoc - TraceStart;

	
	DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 12, FColor::Red, true);
	DrawDebugSphere(GetWorld(), EndLoc, 4.f, 12, FColor::Orange, true);
	DrawDebugLine(
		GetWorld(),
		TraceStart,
		FVector(TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size()),
		FColor::Cyan,
		true);

	return FVector(TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size());
}
