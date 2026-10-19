#include "RDGameMode.h"
#include "RDCharacter.h"
#include "RDPlayerController.h"
#include "RDHUD.h"

ARDGameMode::ARDGameMode()
{
	DefaultPawnClass = ARDCharacter::StaticClass();
	PlayerControllerClass = ARDPlayerController::StaticClass();
	HUDClass = ARDHUD::StaticClass();
}
