#include "ImGuiWorldSubsystem.h"

#include "ImGuiModule.h"

#include <imgui.h>
#include "ImGuiWindow.h"
#include "Engine/World.h"
#include "HAL/IConsoleManager.h"

#define MISSING_FLAG 0

TArray<TSubclassOf<UImGuiWindow>> UImGuiWorldSubsystem::RegisteredClasses = TArray<TSubclassOf<UImGuiWindow>>();

FAutoConsoleCommandWithWorldAndArgs UImGuiWorldSubsystem::ToggleImGuiCommand(TEXT("game.toggleImGui"),
	TEXT("Toggles the Imgui Debug Windows."),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& InParams, UWorld* InWorld)
	{
		UImGuiWorldSubsystem* WorldSubsystem = InWorld->GetSubsystem<UImGuiWorldSubsystem>();
		if(WorldSubsystem)
		{
			WorldSubsystem->Exec_ToggleImGui();
		}
	}));

void UImGuiWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UImGuiWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();

	UImGuiWorldSubsystem* CDO = GetMutableDefault<UImGuiWorldSubsystem>();
	CDO->SavedWindowInfos.Empty();
	for(UImGuiWindow* EachWindow : WindowInstances)
	{
		if(!EachWindow->bIsShown)
		{
			continue;
		}
		FSavedImGuiWindowInfo NewSavedWindowInfo;
		NewSavedWindowInfo.WindowClass = EachWindow->GetClass();
		CDO->SavedWindowInfos.Add(NewSavedWindowInfo);
	}
	CDO->SaveConfig(CPF_Config, *CDO->GetDefaultConfigFilename());

	if(GWorld)
	{
		FImGuiDelegates::OnWorldDebug().RemoveAll(this);
	}

	if(bIsToggledOn)
	{
		bIsToggledOn = false;
		FImGuiModule::Get().SetInputMode(false);
	}
}

void UImGuiWorldSubsystem::Start()
{
	bStarted = true;
	Settings = GetDefault<UImGuiDebugSettings>();
	FImGuiDelegates::OnWorldDebug(GetWorld()).AddUObject(this, &UImGuiWorldSubsystem::Display);
	
	CheckRegisteredClasses();
	//Restore the window positions and sizes
	for(FSavedImGuiWindowInfo const & EachSavedWindowInfo : SavedWindowInfos)
	{
		if(EachSavedWindowInfo.WindowClass == nullptr)
		{
			continue;
		}
		FString EachWindowName = EachSavedWindowInfo.WindowClass->GetDefaultObject<UImGuiWindow>()->WindowName;
		ShowImGuiWindowByName(EachWindowName, true);
	}
	SavedWindowInfos.Empty();
}

bool UImGuiWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
#if UE_BUILD_SHIPPING
	return false;
#else
	return Super::ShouldCreateSubsystem(Outer);
#endif
}

void UImGuiWorldSubsystem::Tick(float DeltaTime)
{
	bIsToggledOn = FImGuiModule::Get().IsInputMode();
	// TODO: This is a hack to make sure the input is bound to the correct player controller
	if(GetWorld() && BoundPlayerController != GetWorld()->GetFirstPlayerController())
	{
		TryToBindInput();
	}
	
	CheckRegisteredClasses();
}

bool UImGuiWorldSubsystem::IsTickable() const
{
	return bStarted;
}

void UImGuiWorldSubsystem::Exec_CloseAllImGuiWindows()
{
	for(UImGuiWindow* EachWindow : WindowInstances)
	{
		EachWindow->bIsShown = false;
	}
}

void UImGuiWorldSubsystem::TryToBindInput()
{
	if(APlayerController * PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if(PlayerController->InputComponent)
		{
			PlayerController->InputComponent->BindKey(Settings->ToggleMenuKey, IE_Pressed, this, &UImGuiWorldSubsystem::Exec_ToggleImGui);
			PlayerController->InputComponent->BindKey(Settings->CloseAllWindowsKey, IE_Released, this, &UImGuiWorldSubsystem::Exec_CloseAllImGuiWindows);
			BoundPlayerController = PlayerController;
		}
	}
}

UImGuiWindow* UImGuiWorldSubsystem::FindWindow(TSubclassOf<UImGuiWindow> WindowClass)
{
	for (UImGuiWindow* EachWindow : WindowInstances)
	{
		if (EachWindow->IsA(WindowClass))
		{
			return EachWindow;
		}
	}
	return nullptr;
}

UImGuiWindow* UImGuiWorldSubsystem::FindWindow(FString WindowName)
{
	for (UImGuiWindow* EachWindow : WindowInstances)
	{
		if (EachWindow->WindowName == WindowName)
		{
			return EachWindow;
		}
	}
	return nullptr;
}

TArray<FString> UImGuiWorldSubsystem::GetAllWindowNames()
{
	TArray<FString> WindowNames;
	for (UImGuiWindow* EachWindow : WindowInstances)
	{
		WindowNames.Add(EachWindow->WindowName);
	}
	return WindowNames;
}

bool UImGuiWorldSubsystem::ToggleImGuiWindowByName(FString WindowName)
{
	UImGuiWindow* Window = FindWindow(WindowName);
	if (Window)
	{
		return ShowImGuiWindowByName(WindowName, !Window->bIsShown);
	}
	return false;
}

bool UImGuiWorldSubsystem::ShowImGuiWindowByName(FString WindowName, const bool bIsShown)
{
	UImGuiWindow* Window = FindWindow(WindowName);
	if (Window)
	{
		Window->bIsShown = bIsShown;
		return true;
	}
	return false;
}

bool UImGuiWorldSubsystem::RegisterWindowClass(TSubclassOf<UImGuiWindow> WindowClass)
{
	RegisteredClasses.Add(WindowClass);
	return true;
}

void UImGuiWorldSubsystem::Display()
{
	auto &colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.1f, 0.13f, 1.0f};
	colors[ImGuiCol_MenuBarBg] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

	// Border
	colors[ImGuiCol_Border] = ImVec4{0.44f, 0.37f, 0.61f, 0.29f};
	colors[ImGuiCol_BorderShadow] = ImVec4{0.0f, 0.0f, 0.0f, 0.24f};

	// Text
	colors[ImGuiCol_Text] = ImVec4{1.0f, 1.0f, 1.0f, 1.0f};
	colors[ImGuiCol_TextDisabled] = ImVec4{0.5f, 0.5f, 0.5f, 1.0f};

	// Headers
	colors[ImGuiCol_Header] = ImVec4{0.13f, 0.13f, 0.17, 1.0f};
	colors[ImGuiCol_HeaderHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
	colors[ImGuiCol_HeaderActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{0.13f, 0.13f, 0.17, 1.0f};
	colors[ImGuiCol_ButtonHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
	colors[ImGuiCol_ButtonActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
	colors[ImGuiCol_CheckMark] = ImVec4{0.74f, 0.58f, 0.98f, 1.0f};

	// Popups
	colors[ImGuiCol_PopupBg] = ImVec4{0.1f, 0.1f, 0.13f, 0.92f};

	// Slider
	colors[ImGuiCol_SliderGrab] = ImVec4{0.44f, 0.37f, 0.61f, 0.54f};
	colors[ImGuiCol_SliderGrabActive] = ImVec4{0.74f, 0.58f, 0.98f, 0.54f};

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{0.13f, 0.13, 0.17, 1.0f};
	colors[ImGuiCol_FrameBgHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
	colors[ImGuiCol_FrameBgActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
	colors[ImGuiCol_TabHovered] = ImVec4{0.24, 0.24f, 0.32f, 1.0f};
	colors[ImGuiCol_TabActive] = ImVec4{0.2f, 0.22f, 0.27f, 1.0f};
	colors[ImGuiCol_TabUnfocused] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
	colors[ImGuiCol_TitleBgActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

	// Scrollbar
	colors[ImGuiCol_ScrollbarBg] = ImVec4{0.1f, 0.1f, 0.13f, 1.0f};
	colors[ImGuiCol_ScrollbarGrab] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{0.24f, 0.24f, 0.32f, 1.0f};

	// Seperator
	colors[ImGuiCol_Separator] = ImVec4{0.44f, 0.37f, 0.61f, 1.0f};
	colors[ImGuiCol_SeparatorHovered] = ImVec4{0.74f, 0.58f, 0.98f, 1.0f};
	colors[ImGuiCol_SeparatorActive] = ImVec4{0.84f, 0.58f, 1.0f, 1.0f};

	// Resize Grip
	colors[ImGuiCol_ResizeGrip] = ImVec4{0.44f, 0.37f, 0.61f, 0.29f};
	colors[ImGuiCol_ResizeGripHovered] = ImVec4{0.74f, 0.58f, 0.98f, 0.29f};
	colors[ImGuiCol_ResizeGripActive] = ImVec4{0.84f, 0.58f, 1.0f, 0.29f};
		

	auto &style = ImGui::GetStyle();
	style.TabRounding = 4;
	style.ScrollbarRounding = 9;
	style.WindowRounding = 7;
	style.GrabRounding = 3;
	style.FrameRounding = 3;
	style.PopupRounding = 4;
	style.ChildRounding = 4;
	
	for(UImGuiWindow * Window : WindowInstances)
	{
		if(Window->bIsShown)
		{
			if(!bIsToggledOn)
			{
				ImGui::SetNextWindowBgAlpha(0.3f);
			}
			ImGui::Begin(IMGUI_STR(Window->WindowName), &Window->bIsShown, ImGuiWindowFlags_AlwaysVerticalScrollbar);
			{
				Window->Show();
			}
			ImGui::End();
		}
	}
	
	if(bIsToggledOn)
	{
		if(ImGui::BeginMainMenuBar())
		{
			for(auto EachCategory : RootCategory->SubCategories)
			{
				ShowCategory(EachCategory.Value);
			}
			ImGui::EndMainMenuBar();
		}
	}
}

void UImGuiWorldSubsystem::Exec_ToggleImGui()
{
	bIsToggledOn = !bIsToggledOn;
	FImGuiModule::Get().SetInputMode(bIsToggledOn);
}

void UImGuiWorldSubsystem::ShowCategory(UImGuiCategory* InCategory)
{
	if (ImGui::BeginMenu(IMGUI_STR(InCategory->Name)))
	{
		for(auto EachSubCategories : InCategory->SubCategories)
		{
			ShowCategory(EachSubCategories.Value);
		}
		
		for(int i = 0; i < InCategory->WindowNames.Num(); ++i)
		{
			UImGuiWindow* Window = FindWindow(InCategory->WindowNames[i]);
			if(Window)
			{
				ImGui::Checkbox(IMGUI_STR(InCategory->WindowNames[i]), &Window->bIsShown);
			}
		}
		ImGui::EndMenu();
	}
}

void UImGuiWorldSubsystem::CheckRegisteredClasses()
{
	RegisteredClasses.RemoveAll([](TSubclassOf<UImGuiWindow> WindowClass) { return WindowClass == nullptr; });
	for(auto EachRegisteredClass : RegisteredClasses)
	{
		if(!EachRegisteredClass)
		{
			continue;
		}

		if(EachRegisteredClass->GetName().Contains("REPLACED"))
		{
			continue;
		}
		
		UImGuiWindow* FoundWindow = FindWindow(EachRegisteredClass);
		if(FoundWindow)
		{
			continue;
		}
		
		UImGuiWindow* Window = NewObject<UImGuiWindow>(this, EachRegisteredClass);
		WindowInstances.Add(Window);

			
		// Build a path for the category.
		FString Category = Window->Category;
		TArray<FString> CategoryPath;
		while (Category.Len() > 0)
		{
			// Separate Category using / as a delimiter.
			FString SubCategory = Category;
			int32 Index = Category.Find(TEXT("/"));
			if (Index != INDEX_NONE)
			{
				SubCategory = Category.Left(Index);
				Category = Category.RightChop(Index + 1);
			}
			else
			{
				Category.Empty();
			}
			if (!SubCategory.IsEmpty())
			{
				CategoryPath.Add(SubCategory);
			}
		}
			
		UImGuiCategory* CurrentCategory = nullptr;
		// Add the category by going through the path.
		for(int i = 0; i < CategoryPath.Num(); ++i)
		{
			if(i == 0)
			{
				if(RootCategory == nullptr)
				{
					RootCategory = NewObject<UImGuiCategory>(this);
				}
				if(!RootCategory->SubCategories.Contains(CategoryPath[i]))
				{
					CurrentCategory = NewObject<UImGuiCategory>(this);
					RootCategory->SubCategories.Add(CategoryPath[i], CurrentCategory);
				}
				else
				{
					CurrentCategory = *RootCategory->SubCategories.Find(CategoryPath[i]);
				}

			}
			else
			{
				if(CurrentCategory->SubCategories.Contains(CategoryPath[i]))
				{
					CurrentCategory = *CurrentCategory->SubCategories.Find(CategoryPath[i]);
				}
				else
				{
					UImGuiCategory* NewCategory = NewObject<UImGuiCategory>(this);
					CurrentCategory->SubCategories.Add(CategoryPath[i], NewCategory);
					CurrentCategory = NewCategory;
				}
			}
			CurrentCategory->Name = CategoryPath[i];
		}
		CurrentCategory->WindowNames.Add(Window->WindowName);
	}
}
