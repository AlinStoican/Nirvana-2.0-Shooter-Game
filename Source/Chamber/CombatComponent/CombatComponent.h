// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "D:\Unreal\Chamber\Source\Chamber\Chamber\ChamberTypes\WeaponTypes.h"
#include "D:\Unreal\Chamber\Source\Chamber\HUD\ChamberHUD.h"
#include "CombatComponent.generated.h"


#define TRACE_LENGTH 80000.f


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class CHAMBER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UCombatComponent();
 };
