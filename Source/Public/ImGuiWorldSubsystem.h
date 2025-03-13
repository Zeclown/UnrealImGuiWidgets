#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "Subsystems/EngineSubsystem.h"
#include "Templates/SubclassOf.h"
#include "Engine/DeveloperSettings.h"
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

UCLASS(config = Game, defaultconfig, meta=(DisplayName="Angelscript ImGui"))
class UImGuiDebugSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category="Toggle Menu")
	FKey ToggleMenuKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category="Toggle Menu")
	FKey CloseAllWindowsKey;
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
UCLASS(config = ImGui)
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

	void TryToBindInput();
	
	void ShowCategory(UImGuiCategory* Category);
	void CheckRegisteredClasses();
	
	UFUNCTION(BlueprintCallable)
	void Exec_ToggleImGui();
	
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
	TWeakObjectPtr<APlayerController> BoundPlayerController;
	
	UPROPERTY()
	UImGuiCategory* RootCategory;

	bool bStarted = false;
	
	static FAutoConsoleCommandWithWorldAndArgs ToggleImGuiCommand;
	UImGuiDebugSettings const* Settings;
};
