// On-screen overlay. For now: the centre dot, drawn whenever the active player state asks for it
// (always in First Person, while aiming in Third Person).
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RDHUD.generated.h"

UCLASS()
class REDISCOVERY_API ARDHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	UPROPERTY(EditDefaultsOnly, Category = "Crosshair")
	float DotRadius = 3.f;
};
