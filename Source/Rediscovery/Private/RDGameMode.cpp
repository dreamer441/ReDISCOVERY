#include "RDGameMode.h"
#include "RDCharacter.h"
#include "RDPlayerController.h"

ARDGameMode::ARDGameMode()
{
	DefaultPawnClass = ARDCharacter::StaticClass();
	PlayerControllerClass = ARDPlayerController::StaticClass();
}
