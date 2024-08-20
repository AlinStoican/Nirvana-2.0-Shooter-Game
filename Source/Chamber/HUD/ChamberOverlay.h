// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChamberOverlay.generated.h"


UCLASS()
class CHAMBER_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetHealthBarPercent(float Percent);

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthProgressBar;


};
