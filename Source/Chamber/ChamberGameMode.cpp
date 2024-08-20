// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChamberGameMode.h"
#include "ChamberCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/ConstructorHelpers.h"


namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

AChamberGameMode::AChamberGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AChamberGameMode::BeginPlay()

{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void AChamberGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch();
		}
	}
	else if (MatchState == MatchState::InProgress)
	{
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}
	else if (MatchState == MatchState::Cooldown)
	{
		CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			RestartGame();
		}
	}*/
}
//
//void AChamberGameMode::OnMatchStateSet()
//{
//	Super::OnMatchStateSet();
//
//	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
//	{
//		ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It);
//		if (BlasterPlayer)
//		{
//			ChamberPlayer->OnMatchStateSet(MatchState, bTeamsMatch);
//		}
//	}
//}

float AChamberGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	return BaseDamage;
}

//void AChamberGameMode::PlayerEliminated(class AChamberCharacter* ElimmedCharacter, class AChamberPlayerController* VictimController, AChamberPlayerController* AttackerController)
//{
//	if (AttackerController == nullptr || AttackerController->PlayerState == nullptr) return;
//	if (VictimController == nullptr || VictimController->PlayerState == nullptr) return;
//	AChamberPlayerState* AttackerPlayerState = AttackerController ? Cast<AChamberPlayerState>(AttackerController->PlayerState) : nullptr;
//	AChamberPlayerState* VictimPlayerState = VictimController ? Cast<AChamberPlayerState>(VictimController->PlayerState) : nullptr;
//
//	AChamberGameMode* BlasterGameState = GetGameState<ABlasterGameState>();
//
//	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState && BlasterGameState)
//	{
//		TArray<ABlasterPlayerState*> PlayersCurrentlyInTheLead;
//		for (auto LeadPlayer : BlasterGameState->TopScoringPlayers)
//		{
//			PlayersCurrentlyInTheLead.Add(LeadPlayer);
//		}
//
//		AttackerPlayerState->AddToScore(1.f);
//		ChamberGameState->UpdateTopScore(AttackerPlayerState);
//		if (ChamberGameState->TopScoringPlayers.Contains(AttackerPlayerState))
//		{
//			AChamberCharacter* Leader = Cast<AChamberCharacter>(AttackerPlayerState->GetPawn());
//			if (Leader)
//			{
//				Leader->MulticastGainedTheLead();
//			}
//		}
//
//		for (int32 i = 0; i < PlayersCurrentlyInTheLead.Num(); i++)
//		{
//			if (!ChamberGameState->TopScoringPlayers.Contains(PlayersCurrentlyInTheLead[i]))
//			{
//				AChamberCharacter* Loser = Cast<AChamberCharacter>(PlayersCurrentlyInTheLead[i]->GetPawn());
//				if (Loser)
//				{
//					Loser->MulticastLostTheLead();
//				}
//			}
//		}
//	}
//	if (VictimPlayerState)
//	{
//		VictimPlayerState->AddToDefeats(1);
//	}
//
//	if (ElimmedCharacter)
//	{
//		ElimmedCharacter->Elim(false);
//	}
//
//	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
//	{
//		ABlasterPlayerController* ChamberPlayer = Cast<AChamberPlayerController>(*It);
//		if (ChamberPlayer && AttackerPlayerState && VictimPlayerState)
//		{
//			ChamberPlayer->BroadcastElim(AttackerPlayerState, VictimPlayerState);
//		}
//	}
//}

void AChamberGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}
