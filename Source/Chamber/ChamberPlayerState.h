
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ChamberPlayerState.generated.h"

UENUM(BlueprintType)
enum class ETeam : uint8
{
	// TO DO //
	ET_RedTeam UMETA(DisplayName = "RedTeam"),
	ET_BlueTeam UMETA(DisplayName = "BlueTeam"),
	ET_NoTeam UMETA(DisplayName = "NoTeam"),
	ET_MAX UMETA(DisplayName = "DefaultMAX")
};
 

UCLASS()
class CHAMBER_API AChamberPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	/**
	* Replication notifies
	*/
	virtual void OnRep_Score() override;

	UFUNCTION()
	virtual void OnRep_Defeats();

	void AddToScore(float ScoreAmount);
	void AddToDefeats(int32 DefeatsAmount);
private:

	UPROPERTY()
	class AChamberCharacter* Character;

	UPROPERTY()
	class AChamberPlayerController* Controller;

	int32 Defeats;

	ETeam Team = ETeam::ET_NoTeam;

	UFUNCTION()
	void OnRep_Team();

public:
	FORCEINLINE ETeam GetTeam() const { return Team; }
	void SetTeam(ETeam TeamToSet);
};
