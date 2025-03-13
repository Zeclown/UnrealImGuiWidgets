#pragma once
#include "ImGuiCommon.h"
#include "ImGuiEnums.h"
#include "Math/Vector2D.h"
#include "ImDrawList.generated.h"

USTRUCT(BlueprintType)
struct FImGuiDrawList
{
	GENERATED_BODY();

	FImGuiDrawList();
	FImGuiDrawList(ImDrawList* InDrawList);

	ImDrawList* DrawListImpl;
};

UCLASS(Meta = (ScriptMixin = "FImGuiDrawList"))
class UImGuiDrawListMixinLibrary : public UObject
{
	GENERATED_BODY()

	UFUNCTION(ScriptCallable)
	static void PushClipRect(const FImGuiDrawList& DrawList, FVector ClipRectMin, FVector ClipRectMax, bool bIntersectWithCurrentClipRect = false)
	{
		DrawList.DrawListImpl->PushClipRect(IMGUI_FVECTOR2D_TO_IMVEC2(ClipRectMin), IMGUI_FVECTOR2D_TO_IMVEC2(ClipRectMax), bIntersectWithCurrentClipRect);
	}

	UFUNCTION(ScriptCallable)
	static void PushClipRectFullScreen(const FImGuiDrawList& DrawList)
	{
		DrawList.DrawListImpl->PushClipRectFullScreen();
	} 

	UFUNCTION(ScriptCallable)
	static void PopClipRect(const FImGuiDrawList& DrawList)
	{
		DrawList.DrawListImpl->PopClipRect();
	}

	UFUNCTION(ScriptCallable)
	static FVector2D GetClipRectMin(const FImGuiDrawList& DrawList)
	{
		return IMGUI_IMVEC2_TO_FVECTOR2D(DrawList.DrawListImpl->GetClipRectMin());
	}

	UFUNCTION(ScriptCallable)
	static FVector2D GetClipRectMax(const FImGuiDrawList& DrawList)
	{
		return IMGUI_IMVEC2_TO_FVECTOR2D(DrawList.DrawListImpl->GetClipRectMax());
	}

	UFUNCTION(ScriptCallable)
	static void AddLine(const FImGuiDrawList& DrawList, const FVector2D& P1, const FVector2D& P2, FLinearColor Col, float Thickness = 1.0f)
	{
		ImVec4 ColVec4 = ImVec4(Col.R, Col.G, Col.B, Col.A);
		const ImU32 col32 = ImColor(ColVec4); 
		DrawList.DrawListImpl->AddLine(IMGUI_FVECTOR2D_TO_IMVEC2(P1), IMGUI_FVECTOR2D_TO_IMVEC2(P2), col32, Thickness);
	}

	UFUNCTION(ScriptCallable)
	static void AddRect(const FImGuiDrawList& DrawList, const FVector2D& P_Min, const FVector2D& P_Max, FLinearColor Col, float Rounding = 0.0f, EImGuiDrawFlags Flags = EImGuiDrawFlags::None, float Thickness = 1.0f)
	{
		ImVec4 ColVec4 = ImVec4(Col.R, Col.G, Col.B, Col.A);
		const ImU32 col32 = ImColor(ColVec4); 
		DrawList.DrawListImpl->AddRect(IMGUI_FVECTOR2D_TO_IMVEC2(P_Min), IMGUI_FVECTOR2D_TO_IMVEC2(P_Max), col32, Rounding, static_cast<ImDrawFlags>(Flags), Thickness);
	}

	UFUNCTION(ScriptCallable)
	static void AddRectFilled(const FImGuiDrawList& DrawList, const FVector2D& P_Min, const FVector2D& P_Max, FLinearColor Col, float Rounding = 0.0f, EImGuiDrawFlags Flags = EImGuiDrawFlags::None)
	{
		DrawList.DrawListImpl->AddRectFilled(IMGUI_FVECTOR2D_TO_IMVEC2(P_Min), IMGUI_FVECTOR2D_TO_IMVEC2(P_Max), IMGUI_FLINEARCOLOR_TO_U32(Col), Rounding, static_cast<ImDrawFlags>(Flags));
	}

	UFUNCTION(ScriptCallable)
	static void AddRectFilledMultiColor(const FImGuiDrawList& DrawList, const FVector2D& P_Min, const FVector2D& P_Max, FLinearColor Col_Upr_Left, FLinearColor Col_Upr_Right, FLinearColor Col_Bot_Right, FLinearColor Col_Bot_Left)
	{
		DrawList.DrawListImpl->AddRectFilledMultiColor(IMGUI_FVECTOR2D_TO_IMVEC2(P_Min), IMGUI_FVECTOR2D_TO_IMVEC2(P_Max), IMGUI_FLINEARCOLOR_TO_U32(Col_Upr_Left), IMGUI_FLINEARCOLOR_TO_U32(Col_Upr_Right), IMGUI_FLINEARCOLOR_TO_U32(Col_Bot_Right), IMGUI_FLINEARCOLOR_TO_U32(Col_Bot_Left));
	}

	UFUNCTION(ScriptCallable)
	static void AddQuad(const FImGuiDrawList& DrawList, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3, const FVector2D& P4, FLinearColor Col, float Thickness = 1.0f)
	{
		DrawList.DrawListImpl->AddQuad(IMGUI_FVECTOR2D_TO_IMVEC2(P1), IMGUI_FVECTOR2D_TO_IMVEC2(P2), IMGUI_FVECTOR2D_TO_IMVEC2(P3), IMGUI_FVECTOR2D_TO_IMVEC2(P4), IMGUI_FLINEARCOLOR_TO_U32(Col), Thickness);
	}

	UFUNCTION(ScriptCallable)
	static void AddQuadFilled(const FImGuiDrawList& DrawList, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3, const FVector2D& P4, FLinearColor Col)
	{
		DrawList.DrawListImpl->AddQuadFilled(IMGUI_FVECTOR2D_TO_IMVEC2(P1), IMGUI_FVECTOR2D_TO_IMVEC2(P2), IMGUI_FVECTOR2D_TO_IMVEC2(P3), IMGUI_FVECTOR2D_TO_IMVEC2(P4), IMGUI_FLINEARCOLOR_TO_U32(Col));
	}

	UFUNCTION(ScriptCallable)
	static void AddTriangle(const FImGuiDrawList& DrawList, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3, FLinearColor Col, float Thickness = 1.0f)
	{
		DrawList.DrawListImpl->AddTriangle(IMGUI_FVECTOR2D_TO_IMVEC2(P1), IMGUI_FVECTOR2D_TO_IMVEC2(P2), IMGUI_FVECTOR2D_TO_IMVEC2(P3), IMGUI_FLINEARCOLOR_TO_U32(Col), Thickness);
	}

	UFUNCTION(ScriptCallable)
	static void AddTriangleFilled(const FImGuiDrawList& DrawList, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3, FLinearColor Col)
	{
		DrawList.DrawListImpl->AddTriangleFilled(IMGUI_FVECTOR2D_TO_IMVEC2(P1), IMGUI_FVECTOR2D_TO_IMVEC2(P2), IMGUI_FVECTOR2D_TO_IMVEC2(P3), IMGUI_FLINEARCOLOR_TO_U32(Col));
	}

	UFUNCTION(ScriptCallable)
	static void AddCircle(const FImGuiDrawList& DrawList, const FVector2D& Center, float Radius, FLinearColor Col, int Num_Segments = 0, float Thickness = 1.0f)
	{
		DrawList.DrawListImpl->AddCircle(IMGUI_FVECTOR2D_TO_IMVEC2(Center), Radius, IMGUI_FLINEARCOLOR_TO_U32(Col), Num_Segments, Thickness);
	}

	UFUNCTION(ScriptCallable)
	static void AddCircleFilled(const FImGuiDrawList& DrawList, const FVector2D& Center, float Radius, FLinearColor Col, int Num_Segments = 0)
	{
		DrawList.DrawListImpl->AddCircleFilled(IMGUI_FVECTOR2D_TO_IMVEC2(Center), Radius, IMGUI_FLINEARCOLOR_TO_U32(Col), Num_Segments);
	}

	UFUNCTION(ScriptCallable)
	static void AddNgon(const FImGuiDrawList& DrawList, const FVector2D& Center, float Radius, FLinearColor Col, int Num_Segments, float Thickness = 1.0f)
	{
		DrawList.DrawListImpl->AddNgon(IMGUI_FVECTOR2D_TO_IMVEC2(Center), Radius, IMGUI_FLINEARCOLOR_TO_U32(Col), Num_Segments, Thickness);
	}

	UFUNCTION(ScriptCallable)
	static void AddNgonFilled(const FImGuiDrawList& DrawList, const FVector2D& Center, float Radius, FLinearColor Col, int Num_Segments)
	{
		DrawList.DrawListImpl->AddNgonFilled(IMGUI_FVECTOR2D_TO_IMVEC2(Center), Radius, IMGUI_FLINEARCOLOR_TO_U32(Col), Num_Segments);
	}

	UFUNCTION(ScriptCallable)
	static void AddText(const FImGuiDrawList& DrawList, const FVector2D& Pos, FLinearColor Col, FString Text_Begin, FString Text_End = "")
	{
		DrawList.DrawListImpl->AddText(IMGUI_FVECTOR2D_TO_IMVEC2(Pos), IMGUI_FLINEARCOLOR_TO_U32(Col), IMGUI_STR(Text_Begin), IMGUI_STR(Text_End));
	}

	UFUNCTION(ScriptCallable)
	static void AddPolyline(const FImGuiDrawList& DrawList, const TArray<FVector2D> Points, int Num_Points, FLinearColor Col, EImGuiDrawFlags Flags, float Thickness)
	{
		std::vector<ImVec2> PointsVec;
		PointsVec.reserve(Points.Num());
		for (const FVector2D& Point : Points)
		{
			PointsVec.push_back(IMGUI_FVECTOR2D_TO_IMVEC2(Point));
		}
		DrawList.DrawListImpl->AddPolyline(PointsVec.data(), Num_Points, IMGUI_FLINEARCOLOR_TO_U32(Col), static_cast<ImDrawFlags>(Flags), Thickness);
	}

	UFUNCTION(ScriptCallable)
	static void AddConvexPolyFilled(const FImGuiDrawList& DrawList, const TArray<FVector2D> Points, int Num_Points, FLinearColor Col)
	{
		std::vector<ImVec2> PointsVec;
		PointsVec.reserve(Points.Num());
		for (const FVector2D& Point : Points)
		{
			PointsVec.push_back(IMGUI_FVECTOR2D_TO_IMVEC2(Point));
		}
		
		DrawList.DrawListImpl->AddConvexPolyFilled(PointsVec.data(), Num_Points, IMGUI_FLINEARCOLOR_TO_U32(Col));
	}

	UFUNCTION(ScriptCallable)
	static void AddBezierCubic(const FImGuiDrawList& DrawList, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3, const FVector2D& P4, FLinearColor Col, float Thickness, int Num_Segments = 0)
	{
		DrawList.DrawListImpl->AddBezierCubic(IMGUI_FVECTOR2D_TO_IMVEC2(P1), IMGUI_FVECTOR2D_TO_IMVEC2(P2), IMGUI_FVECTOR2D_TO_IMVEC2(P3), IMGUI_FVECTOR2D_TO_IMVEC2(P4), IMGUI_FLINEARCOLOR_TO_U32(Col), Thickness, Num_Segments);
	}

	UFUNCTION(ScriptCallable)
	static void AddBezierQuadratic(const FImGuiDrawList& DrawList, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3, FLinearColor Col, float Thickness, int Num_Segments = 0)
	{
		DrawList.DrawListImpl->AddBezierQuadratic(IMGUI_FVECTOR2D_TO_IMVEC2(P1), IMGUI_FVECTOR2D_TO_IMVEC2(P2), IMGUI_FVECTOR2D_TO_IMVEC2(P3), IMGUI_FLINEARCOLOR_TO_U32(Col), Thickness, Num_Segments);
	}

	UFUNCTION(ScriptCallable)
	static void AddImage(const FImGuiDrawList& DrawList, int User_Texture_Id, const FVector2D& P_Min, const FVector2D& P_Max, const FVector2D& Uv_Min = FVector2D(0, 0), const FVector2D& Uv_Max = FVector2D(1, 1), FLinearColor Col = FLinearColor::White)
	{
		DrawList.DrawListImpl->AddImage(reinterpret_cast<ImTextureID>(User_Texture_Id), IMGUI_FVECTOR2D_TO_IMVEC2(P_Min), IMGUI_FVECTOR2D_TO_IMVEC2(P_Max), IMGUI_FVECTOR2D_TO_IMVEC2(Uv_Min), IMGUI_FVECTOR2D_TO_IMVEC2(Uv_Max), IMGUI_FLINEARCOLOR_TO_U32(Col));
	}

	UFUNCTION(ScriptCallable)
	static void AddImageQuad(const FImGuiDrawList& DrawList, int User_Texture_Id, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3, const FVector2D& P4, const FVector2D& Uv1 = FVector2D(0, 0), const FVector2D& Uv2 = FVector2D(1, 0), const FVector2D& Uv3 = FVector2D(1, 1), const FVector2D& Uv4 = FVector2D(0, 1), FLinearColor Col = FLinearColor::White)
	{
		DrawList.DrawListImpl->AddImageQuad(reinterpret_cast<ImTextureID>(User_Texture_Id), IMGUI_FVECTOR2D_TO_IMVEC2(P1), IMGUI_FVECTOR2D_TO_IMVEC2(P2), IMGUI_FVECTOR2D_TO_IMVEC2(P3), IMGUI_FVECTOR2D_TO_IMVEC2(P4), IMGUI_FVECTOR2D_TO_IMVEC2(Uv1), IMGUI_FVECTOR2D_TO_IMVEC2(Uv2), IMGUI_FVECTOR2D_TO_IMVEC2(Uv3), IMGUI_FVECTOR2D_TO_IMVEC2(Uv4), IMGUI_FLINEARCOLOR_TO_U32(Col));
	}

	UFUNCTION(ScriptCallable)
	static void AddImageRounded(const FImGuiDrawList& DrawList, int User_Texture_Id, const FVector2D& P_Min, const FVector2D& P_Max, const FVector2D& Uv_Min, const FVector2D& Uv_Max, FLinearColor Col, float Rounding, EImGuiDrawFlags Flags = EImGuiDrawFlags::None)
	{
		DrawList.DrawListImpl->AddImageRounded(reinterpret_cast<ImTextureID>(User_Texture_Id), IMGUI_FVECTOR2D_TO_IMVEC2(P_Min), IMGUI_FVECTOR2D_TO_IMVEC2(P_Max), IMGUI_FVECTOR2D_TO_IMVEC2(Uv_Min), IMGUI_FVECTOR2D_TO_IMVEC2(Uv_Max), IMGUI_FLINEARCOLOR_TO_U32(Col), Rounding, static_cast<ImDrawFlags>(Flags));
	}

	UFUNCTION(ScriptCallable)
	static void PathClear(const FImGuiDrawList& DrawList)
	{
		DrawList.DrawListImpl->PathClear();
	}

	UFUNCTION(ScriptCallable)
	static void PathLineTo(const FImGuiDrawList& DrawList, const FVector2D& Pos)
	{
		DrawList.DrawListImpl->PathLineTo(IMGUI_FVECTOR2D_TO_IMVEC2(Pos));
	}

	UFUNCTION(ScriptCallable)
	static void PathLineToMergeDuplicate(const FImGuiDrawList& DrawList, const FVector2D& Pos)
	{
		DrawList.DrawListImpl->PathLineToMergeDuplicate(IMGUI_FVECTOR2D_TO_IMVEC2(Pos));
	}

	UFUNCTION(ScriptCallable)
	static void PathFillConvex(const FImGuiDrawList& DrawList, FLinearColor Col)
	{
		DrawList.DrawListImpl->PathFillConvex(IMGUI_FLINEARCOLOR_TO_U32(Col));
	}

	UFUNCTION(ScriptCallable)
	static void PathStroke(const FImGuiDrawList& DrawList, FLinearColor Col, EImGuiDrawFlags Flags = EImGuiDrawFlags::None, float Thickness = 1.0f)
	{
		DrawList.DrawListImpl->PathStroke(IMGUI_FLINEARCOLOR_TO_U32(Col), static_cast<ImDrawFlags>(Flags), Thickness);
	}

	UFUNCTION(ScriptCallable)
	static void PathArcTo(const FImGuiDrawList& DrawList, const FVector2D& Center, float Radius, float A_Min, float A_Max, int Num_Segments = 0)
	{
		DrawList.DrawListImpl->PathArcTo(IMGUI_FVECTOR2D_TO_IMVEC2(Center), Radius, A_Min, A_Max, Num_Segments);
	}

	UFUNCTION(ScriptCallable)
	static void PathArcToFast(const FImGuiDrawList& DrawList, const FVector2D& Center, float Radius, int A_Min_Of_12, int A_Max_Of_12)
	{
		DrawList.DrawListImpl->PathArcToFast(IMGUI_FVECTOR2D_TO_IMVEC2(Center), Radius, A_Min_Of_12, A_Max_Of_12);
	}

	UFUNCTION(ScriptCallable)
	static void PathBezierCubicCurveTo(const FImGuiDrawList& DrawList, const FVector2D& P2, const FVector2D& P3, const FVector2D& P4, int Num_Segments = 0)
	{
		DrawList.DrawListImpl->PathBezierCubicCurveTo(IMGUI_FVECTOR2D_TO_IMVEC2(P2), IMGUI_FVECTOR2D_TO_IMVEC2(P3), IMGUI_FVECTOR2D_TO_IMVEC2(P4), Num_Segments);
	}

	UFUNCTION(ScriptCallable)
	static void PathBezierQuadraticCurveTo(const FImGuiDrawList& DrawList, const FVector2D& P2, const FVector2D& P3, int Num_Segments = 0)
	{
		DrawList.DrawListImpl->PathBezierQuadraticCurveTo(IMGUI_FVECTOR2D_TO_IMVEC2(P2), IMGUI_FVECTOR2D_TO_IMVEC2(P3), Num_Segments);
	}

	UFUNCTION(ScriptCallable)
	static void PathRect(const FImGuiDrawList& DrawList, const FVector2D& RectMin, const FVector2D& RectMax, float Rounding = 0.0f, EImGuiDrawFlags Flags = EImGuiDrawFlags::None)
	{
		DrawList.DrawListImpl->PathRect(IMGUI_FVECTOR2D_TO_IMVEC2(RectMin), IMGUI_FVECTOR2D_TO_IMVEC2(RectMax), Rounding, static_cast<ImDrawFlags>(Flags));
	}

	UFUNCTION(ScriptCallable)
	static void AddDrawCmd(const FImGuiDrawList& DrawList)
	{
		DrawList.DrawListImpl->AddDrawCmd();
	}

	UFUNCTION(ScriptCallable)
	static void ChannelsSplit(const FImGuiDrawList& DrawList, int Count)
	{
		DrawList.DrawListImpl->ChannelsSplit(Count);
	}

	UFUNCTION(ScriptCallable)
	static void ChannelsMerge(const FImGuiDrawList& DrawList)
	{
		DrawList.DrawListImpl->ChannelsMerge();
	}

	UFUNCTION(ScriptCallable)
	static void ChannelsSetCurrent(const FImGuiDrawList& DrawList, int N)
	{
		DrawList.DrawListImpl->ChannelsSetCurrent(N);
	}

	UFUNCTION(ScriptCallable)
	static void PrimReserve(const FImGuiDrawList& DrawList, int idx_count, int vtx_count)
	{
		DrawList.DrawListImpl->PrimReserve(idx_count, vtx_count);
	}

	UFUNCTION(ScriptCallable)
	static void PrimUnreserve(const FImGuiDrawList& DrawList, int idx_count, int vtx_count)
	{
		DrawList.DrawListImpl->PrimUnreserve(idx_count, vtx_count);
	}

	UFUNCTION(ScriptCallable)
	static void PrimRect(const FImGuiDrawList& DrawList, const FVector2D& A, const FVector2D& B, FLinearColor Col)
	{
		DrawList.DrawListImpl->PrimRect(IMGUI_FVECTOR2D_TO_IMVEC2(A), IMGUI_FVECTOR2D_TO_IMVEC2(B), IMGUI_FLINEARCOLOR_TO_U32(Col));
	}

	UFUNCTION(ScriptCallable)
	static void PrimRectUV(const FImGuiDrawList& DrawList, const FVector2D& A, const FVector2D& B, const FVector2D& UVA, const FVector2D& UVB, FLinearColor Col)
	{
		DrawList.DrawListImpl->PrimRectUV(IMGUI_FVECTOR2D_TO_IMVEC2(A), IMGUI_FVECTOR2D_TO_IMVEC2(B), IMGUI_FVECTOR2D_TO_IMVEC2(UVA), IMGUI_FVECTOR2D_TO_IMVEC2(UVB), IMGUI_FLINEARCOLOR_TO_U32(Col));
	}

	UFUNCTION(ScriptCallable)
	static void PrimQuadUV(const FImGuiDrawList& DrawList, const FVector2D& A, const FVector2D& B, const FVector2D& C, const FVector2D& D, const FVector2D& UVA, const FVector2D& UVB, const FVector2D& UVC, const FVector2D& UVD, FLinearColor Col)
	{
		DrawList.DrawListImpl->PrimQuadUV(IMGUI_FVECTOR2D_TO_IMVEC2(A), IMGUI_FVECTOR2D_TO_IMVEC2(B), IMGUI_FVECTOR2D_TO_IMVEC2(C), IMGUI_FVECTOR2D_TO_IMVEC2(D), IMGUI_FVECTOR2D_TO_IMVEC2(UVA), IMGUI_FVECTOR2D_TO_IMVEC2(UVB), IMGUI_FVECTOR2D_TO_IMVEC2(UVC), IMGUI_FVECTOR2D_TO_IMVEC2(UVD), IMGUI_FLINEARCOLOR_TO_U32(Col));
	}

	UFUNCTION(ScriptCallable)
	static void PrimWriteVtx(const FImGuiDrawList& InDrawList, const FVector2D& InPos, const FVector2D& InUv, FLinearColor InCol)
	{
		InDrawList.DrawListImpl->PrimWriteVtx(IMGUI_FVECTOR2D_TO_IMVEC2(InPos), IMGUI_FVECTOR2D_TO_IMVEC2(InUv), IMGUI_FLINEARCOLOR_TO_U32(InCol));
	}

	UFUNCTION(ScriptCallable)
	static void PrimWriteIdx(const FImGuiDrawList& InDrawList, int InIdx)
	{
		InDrawList.DrawListImpl->PrimWriteIdx(InIdx);
	}

	UFUNCTION(ScriptCallable)
	static void PrimVtx(const FImGuiDrawList& InDrawList, const FVector2D& InPos, const FVector2D& InUv, FLinearColor InCol)
	{
		InDrawList.DrawListImpl->PrimVtx(IMGUI_FVECTOR2D_TO_IMVEC2(InPos), IMGUI_FVECTOR2D_TO_IMVEC2(InUv), IMGUI_FLINEARCOLOR_TO_U32(InCol));
	}
};
