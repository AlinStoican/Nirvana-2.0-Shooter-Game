
#include "ChamberPlayerController.h"
#include "D:\Unreal\Chamber\Source\Chamber\HUD\ChamberOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "D:\Unreal\Chamber\Source\Chamber\ChamberCharacter.h"
#include "Net/UnrealNetwork.h"
#include "D:\Unreal\Chamber\Source\Chamber\HUD\ChamberHUD.h"
#include "D:\Unreal\Chamber\Source\Chamber\ChamberGameMode.h"
#include "D:\Unreal\Chamber\Source\Chamber\ChamberPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "D:\Unreal\Chamber\Source\Chamber\Attributes\Announcement.h"
#include "D:\Unreal\Chamber\Source\Chamber\SomeCombatComponent\SomeCombatComponent.h"
#include "Components/Image.h"
#include "D:\Unreal\Chamber\Source\Chamber\ReturnToMainMenu\ReturnToMainMenu.h"


void AChamberPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AChamberPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*SetHUDTime();

	PollInit();
	CheckPing(DeltaTime);*/
}

void AChamberPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AChamberPlayerController, MatchState);
	//DOREPLIFETIME(AChamberPlayerController, bShowTeamScores);
}

void AChamberPlayerController::ShowReturnToMainMenu()
{

	if (ReturnToMainMenu == nullptr)
	{
		ReturnToMainMenu = CreateWidget<UReturnToMainMenu>(this, ReturnToMainMenuWidget);
	}
	if (ReturnToMainMenu)
	{
		bReturnToMainMenuOpen = !bReturnToMainMenuOpen;
		if (bReturnToMainMenuOpen)
		{
			ReturnToMainMenu->MenuSetup();
		}
		else
		{
			ReturnToMainMenu->MenuTearDown();
		}
	}
}

void AChamberPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AChamberPlayerState* ChamberCharacter = Cast<AChamberPlayerState>(InPawn);
	
	/*if (ChamberCharacter)
	{
		SetHUDHealth(ChamberCharacter->GetHealth(), ChamberCharacter->GetMaxHealth());
	}*/
}
 
void AChamberPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	//ChamberHUD = ChamberHUD == nullptr ? Cast<AChamberHUD>(GetHUD()) : ChamberHUD;
	//bool bHUDValid = ChamberHUD &&
	//	ChamberHUD->CharacterOverlay &&
	//	ChamberHUD->CharacterOverlay->HealthBar &&
	//	ChamberHUD->CharacterOverlay->HealthText;
	//if (bHUDValid)
	//{
	//	const float HealthPercent = Health / MaxHealth;
	//	ChamberHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
	//	FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
	//	ChamberHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	//}
	//else
	//{
	//	bInitializeHealth = true;
	//	HUDHealth = Health;
	//	HUDMaxHealth = MaxHealth;
	//}
}

void AChamberPlayerController::SetHUDScore(float Score)
{
	//ChamberHUD = ChamberHUD == nullptr ? Cast<AChamberHUD>(GetHUD()) : ChamberHUD;
	//bool bHUDValid = ChamberHUD &&
	//	ChamberHUD->CharacterOverlay &&
	//	ChamberHUD->CharacterOverlay->ScoreAmount;

	//if (bHUDValid)
	//{
	//	FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
	//	ChamberHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
	//}
	//else
	//{
	//	bInitializeScore = true;
	//	HUDScore = Score;
	//}
}

void AChamberPlayerController::SetHUDDefeats(int32 Defeats)
{
	//ChamberHUD = ChamberHUD == nullptr ? Cast<AChamberHUD>(GetHUD()) : ChamberHUD;
	//bool bHUDValid = ChamberHUD &&
	//	ChamberHUD->CharacterOverlay &&
	//	ChamberHUD->CharacterOverlay->DefeatsAmount;
	//if (bHUDValid)
	//{
	//	FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
	//	ChamberHUD->CharacterOverlay->DefeatsAmount->SetText(FText::FromString(DefeatsText));
	//}
	//else
	//{
	//	bInitializeDefeats = true;
	//	HUDDefeats = Defeats;
	//}
}

void AChamberPlayerController::SetHUDWeaponAmmo(int32 Ammo)
{
	//ChamberHUD = ChamberHUD == nullptr ? Cast<AChamberHUD>(GetHUD()) : ChamberHUD;
	//bool bHUDValid = ChamberHUD &&
	//	ChamberHUD->CharacterOverlay &&
	//	ChamberHUD->CharacterOverlay->WeaponAmmoAmount;
	//if (bHUDValid)
	//{
	//	FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
	//	ChamberHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(AmmoText));
	//}
	//else
	//{
	//	bInitializeWeaponAmmo = true;
	//	HUDWeaponAmmo = Ammo;
	//}
}

void AChamberPlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
//	ChamberHUD = ChamberHUD == nullptr ? Cast<AChamberHUD>(GetHUD()) : ChamberHUD;
//	bool bHUDValid = ChamberHUD &&
//		ChamberHUD->CharacterOverlay &&
//		ChamberHUD->CharacterOverlay->CarriedAmmoAmount;
//	if (bHUDValid)
//	{
//		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
//		ChamberHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
//	}
//	else
//	{
//		bInitializeCarriedAmmo = true;
//		HUDCarriedAmmo = Ammo;
//	}
}

void AChamberPlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	//ChamberHUD = ChamberHUD == nullptr ? Cast<AChamberHUD>(GetHUD()) : ChamberHUD;
	//bool bHUDValid = ChamberHUD &&
	//	ChamberHUD->CharacterOverlay &&
	//	ChamberHUD->CharacterOverlay->MatchCountdownText;
	//if (bHUDValid)
	//{
	//	if (CountdownTime < 0.f)
	//	{
	//		ChamberHUD->CharacterOverlay->MatchCountdownText->SetText(FText());
	//		return;
	//	}

	//	int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
	//	int32 Seconds = CountdownTime - Minutes * 60;

	//	FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	//	ChamberHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(CountdownText));
	//}
}

void AChamberPlayerController::SetHUDAnnouncementCountdown(float CountdownTime)
{
	ChamberHUD = ChamberHUD == nullptr ? Cast<AChamberHUD>(GetHUD()) : ChamberHUD;
	bool bHUDValid = ChamberHUD &&
		ChamberHUD->Announcement &&
		ChamberHUD->Announcement->WarmupTime;
	if (bHUDValid)
	{
		if (CountdownTime < 0.f)
		{
			ChamberHUD->Announcement->WarmupTime->SetText(FText());
			return;
		}

		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		ChamberHUD->Announcement->WarmupTime->SetText(FText::FromString(CountdownText));
	}
}

void AChamberPlayerController::SetHUDTime()
{
	float TimeLeft = 0.f;
	/*if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::InProgress) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::Cooldown) TimeLeft = CooldownTime + WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);

	if (HasAuthority())
	{
		
		if (ChamberGameMode == nullptr)
		{
			ChamberGameMode = Cast<AChamberGameMode>(UGameplayStatics::GetGameMode(this));
			LevelStartingTime = ChamberGameMode->LevelStartingTime;
		}
		ChamberGameMode = ChamberGameMode == nullptr ? Cast<AChamberGameMode>(UGameplayStatics::GetGameMode(this)) : ChamberGameMode;
		if (ChamberGameMode)
		{
			SecondsLeft = FMath::CeilToInt(ChamberGameMode->GetCountdownTime() + LevelStartingTime);
		}
	}

	if (CountdownInt != SecondsLeft)
	{
		if (MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown)
		{
			SetHUDAnnouncementCountdown(TimeLeft);
		}
		if (MatchState == MatchState::InProgress)
		{
			SetHUDMatchCountdown(TimeLeft);
		}
	}

	CountdownInt = SecondsLeft;*/
}

void AChamberPlayerController::PollInit()
{
	if (CharacterOverlay == nullptr)
	{
		if (ChamberHUD && ChamberHUD->CharacterOverlay)
		{
			CharacterOverlay = ChamberHUD->CharacterOverlay;
			if (CharacterOverlay)
			{
				if (bInitializeHealth) SetHUDHealth(HUDHealth, HUDMaxHealth);
				if (bInitializeScore) SetHUDScore(HUDScore);
				if (bInitializeDefeats) SetHUDDefeats(HUDDefeats);
				if (bInitializeCarriedAmmo) SetHUDCarriedAmmo(HUDCarriedAmmo);
				if (bInitializeWeaponAmmo) SetHUDWeaponAmmo(HUDWeaponAmmo);

				AChamberPlayerState* ChamberCharacter = Cast<AChamberPlayerState>(GetPawn());
				
			}
		}
	}
}

void AChamberPlayerController::SetupInputComponent()
{
	//Super::SetupInputComponent();
	if (InputComponent == nullptr) return;

	InputComponent->BindAction("Quit", IE_Pressed, this, &AChamberPlayerController::ShowReturnToMainMenu);

}

void AChamberPlayerController::HandleMatchHasStarted(bool bTeamsMatch)
{
	//if (HasAuthority()) bShowTeamScores = bTeamsMatch;
	//ChamberHUD = ChamberHUD == nullptr ? Cast<AChamberHUD>(GetHUD()) : ChamberHUD;
	//if (ChamberHUD)
	//{
	//	if (ChamberHUD->CharacterOverlay == nullptr) ChamberHUD->AddCharacterOverlay();
	//	if (ChamberHUD->Announcement)
	//	{
	//		ChamberHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
	//	}
	//	if (!HasAuthority()) return;
	//	if (bTeamsMatch)
	//	{
	//		InitTeamScores();
	//	}
	//	else
	//	{
	//		HideTeamScores();
	//	}
	//}
}

void AChamberPlayerController::HandleCooldown()
{
	ChamberHUD = ChamberHUD == nullptr ? Cast<AChamberHUD>(GetHUD()) : ChamberHUD;
	if (ChamberHUD)
	{
		ChamberHUD->CharacterOverlay->RemoveFromParent();
		bool bHUDValid = ChamberHUD->Announcement &&
			ChamberHUD->Announcement->AnnouncementText &&
			ChamberHUD->Announcement->InfoText;

	//	if (bHUDValid)
	//	{
	//		ChamberHUD->Announcement->SetVisibility(ESlateVisibility::Visible);
	//		FString AnnouncementText = Announcement::NewMatchStartsIn;
	//		ChamberHUD->Announcement->AnnouncementText->SetText(FText::FromString(AnnouncementText));
	//		AChamberGameState* ChamberGameState = Cast<AChamberGameState>(UGameplayStatics::GetGameState(this));
	//		AChamberPlayerState* ChamberPlayerState = GetPlayerState<AChamberPlayerState>();
	//		if (ChamberGameState && ChamberGameState)
	//		{
	//			TArray<AChamberPlayerState*> TopPlayers = ChamberGameState->TopScoringPlayers;
	//			FString InfoTextString = bShowTeamScores ? GetTeamsInfoText(BlasterGameState) : GetInfoText(TopPlayers);

	//			ChamberHUD->Announcement->InfoText->SetText(FText::FromString(InfoTextString));
	//		}
	//	}
	//}
	//AChamberPlayerState* ChamberCharacter = Cast<AChamberPlayerState>(GetPawn());
	//if (ChamberCharacter && ChamberCharacter->GetCombat())
	//{
	//	ChamberCharacter->bDisableGameplay = true;
	//	ChamberCharacter->GetCombat()->FireButtonPressed(false);
	}
}

void AChamberPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

}