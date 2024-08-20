// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ChamberGameMode.generated.h"

namespace MatchState
{
	extern CHAMBER_API const FName Cooldown; // Match duration has been reached. Display winner and begin cooldown timer.
}

UCLASS(minimalapi)
class AChamberGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AChamberGameMode();

	virtual void Tick(float DeltaTime) override;
	//virtual void PlayerEliminated(class AChamberCharacter* ElimmedCharacter, class AChamberPlayerController* VictimController, AChamberPlayerController* AttackerController);
	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);

	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage);
	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;

	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 10.f;

	float LevelStartingTime = 0.f;

	bool bTeamsMatch = false;
protected:
	virtual void BeginPlay() override;


private:
	float CountdownTime = 0.f;
	
};



