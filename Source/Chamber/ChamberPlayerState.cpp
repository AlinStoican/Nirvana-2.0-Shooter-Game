
#include "ChamberPlayerState.h"
#include "D:\Unreal\Chamber\Source\Chamber\ChamberCharacter.h"
#include "Chamber/ChamberPlayerController/ChamberPlayerController.h"


void AChamberPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AChamberPlayerState, Defeats);
	//DOREPLIFETIME(AChamberPlayerState, Team);
}

void AChamberPlayerState::AddToScore(float ScoreAmount)
{
	//SetScore(GetScore() + ScoreAmount);
	//Character = Character == nullptr ? Cast<AChamberCharacter>(GetPawn()) : Character;
	//if (Character)
	//{
	//	Controller = Controller == nullptr ? Cast<AChamberPlayerController>(Character->Controller) : Controller;
	//
	 /*	if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}*/
}

void AChamberPlayerState::OnRep_Score()
{/*
	Super::OnRep_Score();
	
	Character = Character == nullptr ? Cast<AChamberCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AChamberPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}*/
}

void AChamberPlayerState::AddToDefeats(int32 DefeatsAmount)
{
	Defeats += DefeatsAmount;
	Character = Character == nullptr ? Cast<AChamberCharacter>(GetPawn()) : Character;
	/*
	if (Character)
		
	{
		Controller = Controller == nullptr ? Cast<AChamberPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
	*/
}

void AChamberPlayerState::OnRep_Defeats()
{
	Character = Character == nullptr ? Cast<AChamberCharacter>(GetPawn()) : Character;

	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AChamberPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
}

void AChamberPlayerState::SetTeam(ETeam TeamToSet)
{
	Team = TeamToSet;

	/*AChamberCharacter* BCharacter = Cast <AChamberCharacter>(GetPawn());
	if (BCharacter)
	{
		BCharacter->SetTeamColor(Team);
	}*/
}

void AChamberPlayerState::OnRep_Team()
{
	AChamberCharacter* BCharacter = Cast <AChamberCharacter>(GetPawn());
	/*if (BCharacter)
	{
		BCharacter->SetTeamColor(Team);
	}*/
}
