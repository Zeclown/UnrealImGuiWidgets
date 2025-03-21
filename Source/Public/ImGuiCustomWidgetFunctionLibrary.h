﻿#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ImGuiCustomWidgetFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS(meta=(ScriptName="ImGui"))
class UNREALIMGUIWIDGETS_API UImGuiCustomWidgetFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="ImGui")
	static void BeginGroupPanel(FString Label, FVector2D Size);
	UFUNCTION(BlueprintCallable, Category="ImGui")
	static void EndGroupPanel();

	
	
};
