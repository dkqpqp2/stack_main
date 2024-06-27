// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../TeamChangeInterface.h"
#include "MG_CharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class STACKOBOT_API AMG_CharacterBase : public ACharacter, public ITeamChangeInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMG_CharacterBase();

protected:
	virtual void SetCharacterControlData(const class UMG_ControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UMG_ControlData*> CharacterControlManager;

	// 팀선택시 색깔 바뀔 때 쓸 다이나믹 머티리얼
	UMaterialInstanceDynamic* MaterialDynamicInstance;
	// DynamicMaterial Set.
	virtual void BeginPlay();

public:
	virtual void PossessedBy(AController* NewController) override;
	// 머티리얼 색깔을 바꾸는 함수 (인터페이스로 상속받았다)
	virtual void SetMaterialToTeamColor(FVector4 NewColor) override;

};
