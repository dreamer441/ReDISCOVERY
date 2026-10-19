// Root Gameplay Tags shared by all REDISCOVERY modules.
// Modules hang their own tags under these roots (e.g. RD.State.FirstPerson lives in RDPlayerStates).
// RDCore must NOT define feature-specific tags here.
#pragma once

#include "NativeGameplayTags.h"

namespace RDCoreTags
{
	RDCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State);     // RD.State    - player states
	RDCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Element);   // RD.Element  - magic elements
	RDCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Function);  // RD.Function - magic functions

	// Generic reasons a request can be refused (used in FRDRequestResult::RefusalReason).
	RDCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Refusal_NotAuthority);  // caller may not decide this
	RDCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Refusal_Unknown);       // the requested thing does not exist
	RDCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Refusal_AlreadyActive); // nothing would change
	RDCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Refusal_Blocked);       // rules say "not now"
}
