// The game mode only assembles: which pawn and which controller a joining player gets.
// It contains no gameplay rules.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RDGameMode.generated.h"

UCLASS()
class REDISCOVERY_API ARDGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARDGameMode();
};
