// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)
	bool bIsHost;
public:
	void SetIsHost(bool IsHost);
	bool GetIsHost() const;

// --------- Player Enter ID ---------
protected:
	// 먼저들어온 사람부터 0, 1, 2, 3...
	UPROPERTY(Replicated)
	int32 PlayerEnterID = 0;
public:
	virtual void SetPlayerEnterID(int32 NewEnterID);
	int32 GetPlayerEnterID() const;

// ----------- IsRedTeam's ----------------------------
protected:
	UPROPERTY(ReplicatedUsing = OnRep_IsRedTeam)
	bool IsRedTeam;

public:
	virtual void SetIsRedTeamTo(bool IsChecked);
	bool GetIsRedTeam() const;

protected:
	//클라이언트에서 값 변경 notify받는 함수, 안에서 OnIsRedTeamChanged호출.
	UFUNCTION()
	virtual void OnRep_IsRedTeam();

	virtual void OnIsRedTeamChanged();

// ------------ Character Color -------------------

protected:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerColor, VisibleAnywhere, BlueprintReadOnly)
	FVector4 PlayerColor;

public:
	UFUNCTION(BlueprintCallable)
	virtual void SetPlayerColor(FVector4 NewColor);
	FVector4 GetPlayerColor() const;

	UFUNCTION()
	virtual void OnRep_PlayerColor();

	virtual void OnPlayerColorChanged();

	

// ------------ Selected Character's ----------------------
protected:
	UPROPERTY(ReplicatedUsing = OnRep_SelectedCharacter)
	FString SelectedCharacter = "Bot";

public:
	virtual void SetSelectedCharacter(FString NewCharacter);
	FString GetSelectedCharacter() const;

protected:

	virtual void OnChangeCharacter();

	UFUNCTION()
	virtual void OnRep_SelectedCharacter();


protected:
	virtual void CopyProperties(APlayerState* PlayerState) override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// 제발!!!
};
