#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EItem : uint8 {
	E_NONE = 0 UMETA(DisplayName = "NONE"),
	E_BOOSTER = 1 UMETA(DisplayName = "BOOSTER"),
	E_BARRIER = 2 UMETA(DisplayName = "BARRIER"),
	E_SHIELD = 3 UMETA(DisplayName = "SHIELD"),
	E_MISSILE = 4 UMETA(DisplayName = "MISSILE"),
};