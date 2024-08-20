// Fill out your copyright notice in the Description page of Project Settings.


#include "ChamberGameState.h"
#include "Net/UnrealNetwork.h"
#include "D:\Unreal\Chamber\Source\Chamber\ChamberPlayerState.h"
#include "Chamber/ChamberPlayerController/ChamberPlayerController.h"

//void AChamberGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	//DOREPLIFETIME(AChamberGameState, TopScoringPlayers);
//	//DOREPLIFETIME(AChamberGameState, RedTeamScore);
//	//DOREPLIFETIME(AChamberGameState, BlueTeamScore);
//}

//void AChamberGameState::UpdateTopScore(class ABlasterPlayerState* ScoringPlayer)
//{
//	if (TopScoringPlayers.Num() == 0)
//	{
//		TopScoringPlayers.Add(ScoringPlayer);
//		TopScore = ScoringPlayer->GetScore();
//	}
//	else if (ScoringPlayer->GetScore() == TopScore)
//	{
//		TopScoringPlayers.AddUnique(ScoringPlayer);
//	}
//	else if (ScoringPlayer->GetScore() > TopScore)
//	{
//		TopScoringPlayers.Empty();
//		TopScoringPlayers.AddUnique(ScoringPlayer);
//		TopScore = ScoringPlayer->GetScore();
//	}
//}
//
//void AChamberGameState::RedTeamScores()
//{
//	++RedTeamScore;
//	AChamberPlayerController* BPlayer = Cast<AChamberPlayerController>(GetWorld()->GetFirstPlayerController());
//	if (BPlayer)
//	{
//		BPlayer->SetHUDRedTeamScore(RedTeamScore);
//	}
//}
//
//void AChamberGameState::BlueTeamScores()
//{
//	++BlueTeamScore;
//	AChamberPlayerController* BPlayer = Cast<AChamberPlayerController>(GetWorld()->GetFirstPlayerController());
//	if (BPlayer)
//	{
//		BPlayer->SetHUDBlueTeamScore(BlueTeamScore);
//	}
//}
//
//void AChamberGameState::OnRep_RedTeamScore()
//{
//	AChamberPlayerController* BPlayer = Cast<AChamberPlayerController>(GetWorld()->GetFirstPlayerController());
//	if (BPlayer)
//	{
//		BPlayer->SetHUDRedTeamScore(RedTeamScore);
//	}
//}
//
//void AChamberGameState::OnRep_BlueTeamScore()
//{
//	AChamberPlayerController* BPlayer = Cast<AChamberPlayerController>(GetWorld()->GetFirstPlayerController());
//	if (BPlayer)
//	{
//		BPlayer->SetHUDBlueTeamScore(BlueTeamScore);
//	}
//}
