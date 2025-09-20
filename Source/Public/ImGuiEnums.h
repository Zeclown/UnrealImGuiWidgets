#pragma once

#include "CoreMinimal.h"
#include "ImGuiCommon.h"
#include "ImGuiEnums.generated.h"

/* structs */

// same as "ImGuiCond_" from "imgui.h"
UENUM(BlueprintType)
enum class EImGuiConditions : uint8
{
	// No condition (always set the variable), same as _Always
	NoneCondition UMETA(DisplayName = "None", Hidden),
	// No condition (always set the variable)
	Always = 1 << 0,
	// Set the variable once per runtime session (only the first call will succeed)
	Once = 1 << 1,
	// Set the variable if the object/window has no persistently saved data (no entry in .ini file)
	FirstUseEver = 1 << 2,
	// Set the variable if the object/window is appearing after being hidden/inactive (or the first time)
	Appearing = 1 << 3			
};


UENUM(BlueprintType)
enum class ImGui_DragType : uint8
{
	Drag_Float		UMETA(DisplayName = "Float"),
	Drag_Int		UMETA(DisplayName = "Int"),
	Drag_Vector2D	UMETA(DisplayName = "Vector 2D"),
	Drag_Vector		UMETA(DisplayName = "Vector"),
	Drag_Vector4	UMETA(DisplayName = "Vector 4"),
	Drag_Rotator	UMETA(DisplayName = "Rotator"),
	Drag_Transform	UMETA(DisplayName = "Transform"),
	Drag_Color		UMETA(DisplayName = "Color")
};

UENUM(BlueprintType)
enum class EImGuiDrawFlags : uint8
{
    None                        = 0,
    Closed                      = 1 << 0, // PathStroke(), AddPolyline(): specify that shape should be closed (Important: this is always == 1 for legacy reason)
    RoundCornersTopLeft         = 1 << 4, // AddRect(), AddRectFilled(), PathRect(): enable rounding top-left corner only (when rounding > 0.0f, we default to all corners). Was 0x01.
    RoundCornersTopRight        = 1 << 5, // AddRect(), AddRectFilled(), PathRect(): enable rounding top-right corner only (when rounding > 0.0f, we default to all corners). Was 0x02.
    RoundCornersBottomLeft      = 1 << 6, // AddRect(), AddRectFilled(), PathRect(): enable rounding bottom-left corner only (when rounding > 0.0f, we default to all corners). Was 0x04.
    RoundCornersBottomRight     = 1 << 7, // AddRect(), AddRectFilled(), PathRect(): enable rounding bottom-right corner only (when rounding > 0.0f, we default to all corners). Wax 0x08.
};
