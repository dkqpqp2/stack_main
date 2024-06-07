// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BasePlayerState.h"
#include "GamePlayerState.generated.h"

UENUM(BlueprintType)
enum class EItem : uint8 {
	E_NONE = 0 UMETA(DisplayName = "NONE"),
	E_BOOSTER = 1 UMETA(DisplayName = "BOOSTER"),
	E_BARRIER = 2 UMETA(DisplayName = "BARRIER"),
};
/**
 * 
 */
UCLASS()
class STACKOBOT_API AGamePlayerState : public ABasePlayerState
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	virtual void SetPlayerEnterID(int32 NewEnterID);
	virtual void SetIsRedTeamTo(bool IsChecked);
	virtual void SetSelectedCharacter(FString NewCharacter);

public:
	void TrySetScore(float NewScore);
protected:
	UFUNCTION(Server, Reliable)
	void SV_TrySetScore(float NewScore);
	virtual void OnSetScore();
public:
	virtual void OnRep_Score() override;

public:
	// Call when OnPawnSet Done. Since TeamColor is not replicated, Do Initial Paint.
	UFUNCTION()
	void SetPlayerPawn(APlayerState* Player, APawn* NewPawn, APawn* OldPawn);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

// --------- ITEM ----------
protected:

	UPROPERTY(ReplicatedUsing = OnRep_CurrentItem)
	EItem CurrentItem;

	UFUNCTION()
	void OnRep_CurrentItem();

public:
	void SetCurrentItem(EItem NewItem);

	UFUNCTION(BlueprintCallable)
	void UseItem();

protected:
	UFUNCTION(Server, Reliable)
	void SV_UseItem();


protected:
	void UseBarrier();
};
