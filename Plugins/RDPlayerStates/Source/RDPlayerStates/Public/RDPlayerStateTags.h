// Tags that identify the player states. A state is found by its tag, never by a switch over an enum.
#pragma once

#include "NativeGameplayTags.h"

namespace RDPlayerStateTags
{
	RDPLAYERSTATES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(FirstPerson); // RD.State.FirstPerson
	RDPLAYERSTATES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ThirdPerson); // RD.State.ThirdPerson
	RDPLAYERSTATES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TopDown);     // RD.State.TopDown
	RDPLAYERSTATES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(FreeRoam);    // RD.State.FreeRoam
}
