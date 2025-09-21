#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "Subsystems/EngineSubsystem.h"
#include "Templates/SubclassOf.h"
#include "Engine/DeveloperSettings.h"
#include "GameFramework/PlayerInput.h"
#include "Subsystems/WorldSubsystem.h"

#include "ImGuiWorldSubsystem.generated.h"

struct IConsoleCommand;
class UImGuiWindow;

UCLASS()
class UImGuiCategory : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Name;

	UPROPERTY()
	TMap<FString, UImGuiCategory*> SubCategories;

	UPROPERTY()
	TArray<FString> WindowNames;
};

UCLASS(config = Plugin, defaultconfig, meta=(DisplayName="Angelscript ImGui"))
class UImGuiDebugSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category="ImGui Controls")
	FKey ToggleMenuKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category="ImGui Controls")
	FKey CloseAllWindowsKey;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

USTRUCT()
struct FSavedImGuiWindowInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<UImGuiWindow> WindowClass;
};

/**
 * 
 */
UCLASS(config = ImGuiWidgets)
class UNREALIMGUIWIDGETS_API UImGuiWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Needs to be called by gameplay code when gameplay is ready to start
	UFUNCTION(BlueprintCallable)
	virtual void Start();
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void UpdateInputs();
	
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual bool IsTickableWhenPaused() const override { return true; }


	UImGuiWindow *FindWindow(TSubclassOf<UImGuiWindow> WindowClass);
	UImGuiWindow *FindWindow(FString WindowName);
	TArray<FString> GetAllWindowNames();

	template <class T>
	T *FindWindow(UWorld *World) { return static_cast<T *>(FindWindow(T::StaticClass())); }

	bool ToggleImGuiWindowByName(FString WindowName);
	bool ShowImGuiWindowByName(FString WindowName, const bool bIsShown);

	static bool RegisterWindowClass(TSubclassOf<UImGuiWindow> WindowClass);
	
	void Display();
	
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UImGuiWorldSubsystem, STATGROUP_Tickables);
	}
	
protected:
	
	void ShowCategory(UImGuiCategory* Category);
	void CheckRegisteredClasses();
	
	UFUNCTION(BlueprintCallable)
	void Exec_ToggleImGui();

	UFUNCTION(BlueprintCallable)
	void Exec_CloseAllImGuiWindows();
private:
	
	bool bIsToggledOn = false;
	
	static TArray<TSubclassOf<UImGuiWindow>> RegisteredClasses;
	
	//Used to restore window positions and sizes in the same session
	UPROPERTY(config)
	TArray<FSavedImGuiWindowInfo> SavedWindowInfos;
	
	UPROPERTY()
	TArray<UImGuiWindow *> WindowInstances;
	
	UPROPERTY()
	UImGuiCategory* RootCategory;

	bool bStarted = false;
	
	static FAutoConsoleCommandWithWorldAndArgs Exec_ToggleImGuiCommand;
	static FAutoConsoleCommandWithWorldAndArgs Exec_CloseAllImGuiWindowsCommand;
	UImGuiDebugSettings const* Settings;
};
