// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Minigames/GameMap/GamePlayerState.h"
#include "MG_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AMG_PlayerState : public AGamePlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ---------- 순위 ------------
protected:
	UPROPERTY(ReplicatedUsing = OnRep_Rank, VisibleAnywhere)
	int32 Rank;

	UFUNCTION()
	void OnRep_Rank();

	void UpdateRankUI();
public:
	void SetRank(int32 NewRank);
	int32 GetRank();
	int32 GetFinalRank();
	UFUNCTION(BlueprintCallable)
	void SetFinalRank(int32 NewRank);
protected:
	UPROPERTY(ReplicatedUsing = OnRep_FinalRank, BlueprintReadWrite)
	int32 FinalRank = 0;
	UFUNCTION()
	void OnRep_FinalRank();

	// --------- ITEM ----------
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> ItemDataTable;
protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentItem)
	FName CurrentItemName;


	FItemStruct* CurrentItemStruct = nullptr;


	UFUNCTION()
	void OnRep_CurrentItem();
protected:
	void SetCurrentItem(FName NewItemName);
	void UpdateItemSlotUI();

public:
	void SetCurrentItemToRandomItem();

	UFUNCTION(BlueprintCallable)
	void UseItem();

protected:
	UFUNCTION(Server, Reliable)
	void SV_UseItem();


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ASlowBarrier> SlowBarrierBPClass;
protected:
	void UseBarrier();
	void UseShield();
	void UseMissile();
};
