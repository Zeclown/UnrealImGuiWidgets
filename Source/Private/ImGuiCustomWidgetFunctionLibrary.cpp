#include "ImGuiCustomWidgetFunctionLibrary.h"

#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

#include "ImDrawList.h"
#include "ImGuiCommon.h"

static ImVector<ImRect> s_GroupPanelLabelStack;
static ImVector<bool> s_GroupPanelShouldMaskStack;

void UImGuiCustomWidgetFunctionLibrary::BeginGroupPanel(FString Label, FVector2D Size, bool bHideLabel)
{
	ImGui::BeginGroup();

	ImVec2 size = IMGUI_FVECTOR2D_TO_IMVEC2(Size);
	const char* name = TCHAR_TO_ANSI(*Label);
	auto cursorPos = ImGui::GetCursorScreenPos();
	auto itemSpacing = ImGui::GetStyle().ItemSpacing;
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

	auto frameHeight = ImGui::GetFrameHeight();
	ImGui::BeginGroup();

	ImVec2 effectiveSize = size;
	if (size.x < 0.0f)
		effectiveSize.x = 0;
	else
		effectiveSize.x = size.x;
	ImGui::Dummy(ImVec2(effectiveSize.x, 0.0f));

	ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
	ImGui::SameLine(0.0f, 0.0f);
	ImGui::BeginGroup();
	ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
	ImGui::SameLine(0.0f, 0.0f);

	// Check if we should show the label and mask the rect
	bool shouldMask = !bHideLabel && Label.Len() > 0;

	if(shouldMask)
	{
		ImGui::TextUnformatted(name);
	}
	auto labelMin = ImGui::GetItemRectMin();
	auto labelMax = ImGui::GetItemRectMax();
	ImGui::SameLine(0.0f, 0.0f);
	ImGui::Dummy(ImVec2(0.0, frameHeight + itemSpacing.y));
	ImGui::BeginGroup();

	//ImGui::GetWindowDrawList()->AddRect(labelMin, labelMax, IM_COL32(255, 0, 255, 255));

	ImGui::PopStyleVar(2);

	// #if IMGUI_VERSION_NUM >= 17301
	// ImGui::GetCurrentWindow()->ContentRegionRect.Max.x -= frameHeight * 0.5f;
	// ImGui::GetCurrentWindow()->WorkRect.Max.x          -= frameHeight * 0.5f;
	// ImGui::GetCurrentWindow()->InnerRect.Max.x         -= frameHeight * 0.5f;
	// #else
	// ImGui::GetCurrentWindow()->ContentsRegionRect.Max.x -= frameHeight * 0.5f;
	// #endif
	// ImGui::GetCurrentWindow()->Size.x                   -= frameHeight;

	auto itemWidth = ImGui::CalcItemWidth();
	ImGui::PushItemWidth(ImMax(0.0f, itemWidth - frameHeight));

	s_GroupPanelLabelStack.push_back(ImRect(labelMin, labelMax));
	s_GroupPanelShouldMaskStack.push_back(shouldMask);
}

void UImGuiCustomWidgetFunctionLibrary::EndGroupPanel()
{
	 ImGui::PopItemWidth();

    auto itemSpacing = ImGui::GetStyle().ItemSpacing;

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

    auto frameHeight = ImGui::GetFrameHeight();

    ImGui::EndGroup();

    //ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 255, 0, 64), 4.0f);

    ImGui::EndGroup();

    ImGui::SameLine(0.0f, 0.0f);
    ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
    ImGui::Dummy(ImVec2(0.0, frameHeight - frameHeight * 0.5f - itemSpacing.y));

    ImGui::EndGroup();

    auto itemMin = ImGui::GetItemRectMin();
    auto itemMax = ImGui::GetItemRectMax();
    //ImGui::GetWindowDrawList()->AddRectFilled(itemMin, itemMax, IM_COL32(255, 0, 0, 64), 4.0f);

    auto labelRect = s_GroupPanelLabelStack.back();
    s_GroupPanelLabelStack.pop_back();
    bool shouldMask = s_GroupPanelShouldMaskStack.back();
    s_GroupPanelShouldMaskStack.pop_back();

    ImVec2 halfFrame = ImVec2(frameHeight * 0.25f, frameHeight) * 0.5f;
    ImRect frameRect = ImRect(itemMin + halfFrame, itemMax - ImVec2(halfFrame.x, 0.0f));

    if (shouldMask)
    {
        // Only mask the border if we have a visible label
        labelRect.Min.x -= itemSpacing.x;
        labelRect.Max.x += itemSpacing.x;
        for (int i = 0; i < 4; ++i)
        {
            switch (i)
            {
                // left half-plane
                case 0: ImGui::PushClipRect(ImVec2(-FLT_MAX, -FLT_MAX), ImVec2(labelRect.Min.x, FLT_MAX), true); break;
                // right half-plane
                case 1: ImGui::PushClipRect(ImVec2(labelRect.Max.x, -FLT_MAX), ImVec2(FLT_MAX, FLT_MAX), true); break;
                // top
                case 2: ImGui::PushClipRect(ImVec2(labelRect.Min.x, -FLT_MAX), ImVec2(labelRect.Max.x, labelRect.Min.y), true); break;
                // bottom
                case 3: ImGui::PushClipRect(ImVec2(labelRect.Min.x, labelRect.Max.y), ImVec2(labelRect.Max.x, FLT_MAX), true); break;
            }

            ImGui::GetWindowDrawList()->AddRect(
                frameRect.Min, frameRect.Max,
                ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Border)),
                halfFrame.x);

            ImGui::PopClipRect();
        }
    }
    else
    {
        // Draw complete border without masking
        ImGui::GetWindowDrawList()->AddRect(
            frameRect.Min, frameRect.Max,
            ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Border)),
            halfFrame.x);
    }

    ImGui::PopStyleVar(2);

// #if IMGUI_VERSION_NUM >= 17301
//     ImGui::GetCurrentWindow()->ContentRegionRect.Max.x += frameHeight * 0.5f;
//     ImGui::GetCurrentWindow()->WorkRect.Max.x          += frameHeight * 0.5f;
//     ImGui::GetCurrentWindow()->InnerRect.Max.x         += frameHeight * 0.5f;
// #else
//     ImGui::GetCurrentWindow()->ContentsRegionRect.Max.x += frameHeight * 0.5f;
// #endif
//     ImGui::GetCurrentWindow()->Size.x                   += frameHeight;

    ImGui::Dummy(ImVec2(0.0f, 0.0f));
	ImGui::Spacing();
    ImGui::EndGroup();
}

FImGuiDrawList UImGuiCustomWidgetFunctionLibrary::GetCurrentDrawList()
{
	FImGuiDrawList DrawList = FImGuiDrawList(ImGui::GetWindowDrawList());
	return DrawList;
}
