#include "ImGuiWindow.h"

#include "ImGuiWorldSubsystem.h"
#include "Engine/Engine.h"

void UImGuiWindow::PostInitProperties()
{
	Super::PostInitProperties();
	OnPostInitProperties();
}

void UImGuiWindow::Init(const FString& InWindowName, const FString& InCategory, bool InbShouldBePinned)
{
	if (HasAnyFlags(RF_ClassDefaultObject) && this->GetClass() != UImGuiWindow::StaticClass())
	{
		UImGuiWorldSubsystem::RegisterWindowClass(this->GetClass());
	}

	WindowName = InWindowName;
	Category = InCategory;
	bShouldBePinned = InbShouldBePinned;
}
