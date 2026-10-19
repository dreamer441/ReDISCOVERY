#include "RDHUD.h"
#include "RDPlayerStateComponent.h"
#include "RDPlayerStateDefinition.h"
#include "CanvasItem.h"
#include "Engine/Canvas.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

void ARDHUD::DrawHUD()
{
	Super::DrawHUD();

	const APawn* Pawn = PlayerOwner ? PlayerOwner->GetPawn() : nullptr;
	const URDPlayerStateComponent* States = Pawn ? Pawn->FindComponentByClass<URDPlayerStateComponent>() : nullptr;
	const URDPlayerStateDefinition* State = States ? States->GetCurrentDefinition() : nullptr;
	if (!Canvas || !State || !State->WantsCrosshair())
	{
		return;
	}

	// A white dot with a thin dark ring so it reads on bright and dark backgrounds.
	const FVector2D Centre(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	FCanvasNGonItem Ring(Centre, FVector2D(DotRadius + 1.5f), 16, FLinearColor(0.f, 0.f, 0.f, 0.6f));
	Canvas->DrawItem(Ring);
	FCanvasNGonItem Dot(Centre, FVector2D(DotRadius), 16, FLinearColor::White);
	Canvas->DrawItem(Dot);
}
