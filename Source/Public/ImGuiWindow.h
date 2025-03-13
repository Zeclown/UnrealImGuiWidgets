#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ImGuiCommon.h"
#include "ImGuiWindow.generated.h"

/**
 * 
 */
UCLASS()
class UNREALIMGUIWIDGETS_API UImGuiWindow : public UObject
{
	GENERATED_BODY()

public:
	// This should be overriden to call Init() with the correct parameters
	virtual void PostInitProperties() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPostInitProperties();
	
	UFUNCTION(BlueprintCallable)
	void Init(const FString& InWindowName, const FString& InCategory, bool InbShouldBePinned);

	UFUNCTION(BlueprintImplementableEvent)
	void Show();
	
	bool bIsShown = false;

	// Name of the window shown in the menu
	FString WindowName;

	// Category Path in the menu. ex.: "Tools/Cheats"
	FString Category;
	
	// If the window is pinned it will be shown on top of the game screen even when imgui is hidden
	bool bShouldBePinned = false;
};
