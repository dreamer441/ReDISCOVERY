// Module entry point for RDCasting. Also defines the module's log category and tags.
#include "RDCastingLog.h"
#include "RDCastTypes.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogRDCasting);

namespace RDCastingTags
{
	UE_DEFINE_GAMEPLAY_TAG(BasicProjectile, "RD.Function.BasicProjectile");
}

IMPLEMENT_MODULE(FDefaultModuleImpl, RDCasting);
