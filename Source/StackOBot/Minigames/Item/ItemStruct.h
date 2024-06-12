#pragma once

#include "CoreMinimal.h"
#include "ItemBoxItemEnum.h"
#include "Engine/DataTable.h"
#include "ItemStruct.generated.h"

USTRUCT(BlueprintType)
struct FItemStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItem ItemEnum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemString;
};