// Module entry point for RDCore. Nothing to start up yet; the module only provides shared contracts.
#include "RDCoreLog.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogRDCore);

IMPLEMENT_MODULE(FDefaultModuleImpl, RDCore);
