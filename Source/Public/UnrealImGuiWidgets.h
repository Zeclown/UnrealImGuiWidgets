#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class UNREALIMGUIWIDGETS_API FUnrealImGuiWidgetsModule : public FDefaultModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};