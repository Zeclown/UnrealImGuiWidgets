#pragma once

#ifdef IMGUI_API
#define WITH_IMGUI 1
#else
#define WITH_IMGUI 0
#endif // IMGUI_API

#if WITH_IMGUI
#include <imgui.h>
#endif // WITH_IMGUI

#define IMGUI_IMVEC2_TO_FVECTOR2D(ImVec2D) FVector2D(ImVec2D.x, ImVec2D.y)
#define IMGUI_FVECTOR2D_TO_IMVEC2(FVector2D) ImVec2(FVector2D.X, FVector2D.Y)
#define IMGUI_STR(x) TCHAR_TO_ANSI(*x)
#define IMGUI_FLINEARCOLOR_TO_IMVEC4(LinearColor) ImVec4(LinearColor.R, LinearColor.G, LinearColor.B, LinearColor.A)
#define IMGUI_FLINEARCOLOR_TO_U32(LinearColor) ImColor(IMGUI_FLINEARCOLOR_TO_IMVEC4(LinearColor))