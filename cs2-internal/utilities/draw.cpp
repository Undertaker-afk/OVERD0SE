#include "draw.h"

#include "../core/sdk.h"

#include "math.h"
#include "crt.h"
#include "easing.h"
#include "inputsystem.h"

#include <d3dcompiler.h>

#include "../imgui/imgui_freetype.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_settings.h"

#include "../sdk/interfaces/iengineclient.h"
#include "../sdk/interfaces/iswapchaindx11.h"
#include "../sdk/interfaces/iinputsystem.h"

#include "../core/interfaces.h"
#include "../core/hooks.h"

#include "../features/visuals/overlay.h"
#include "../features/configs/configs_var.h"

#include "../menu/widgets/widgets.h"
#include "../menu/menu.h"

void ImGui_ImplDX11_RenderDrawData(ImDrawData* draw_data);
static SRWLOCK drawLock = {};

static void* CS_CDECL ImGuiAllocWrapper(const std::size_t nSize, [[maybe_unused]] void* pUserData = nullptr)
{
	return MEM::HeapAlloc(nSize);
}

static void CS_CDECL ImGuiFreeWrapper(void* pMemory, [[maybe_unused]] void* pUserData = nullptr) noexcept
{
	MEM::HeapFree(pMemory);
}

bool D::InitImGui()
{
	if (bInitialized)
		return false;

	ImGui::SetAllocatorFunctions(ImGuiAllocWrapper, ImGuiFreeWrapper);
	ImGui::CreateContext();

	if (!ImGui_ImplWin32_Init(IPT::hWindow))
		return false;

	if (!ImGui_ImplDX11_Init(I::Device, I::DeviceContext))
		return false;

	ImGuiIO& io = ImGui::GetIO();

	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	pDrawListActive = IM_NEW(ImDrawList)(ImGui::GetDrawListSharedData());
	pDrawListSafe = IM_NEW(ImDrawList)(ImGui::GetDrawListSharedData());
	pDrawListRender = IM_NEW(ImDrawList)(ImGui::GetDrawListSharedData());

	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImFontConfig verdana_cfg, name_cfg, seg_cfg, tahoma_cfg, icons_config;

	verdana_cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_LightHinting;
	tahoma_cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_Bitmap;
	seg_cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_Monochrome | ImGuiFreeTypeBuilderFlags_NoHinting;
	name_cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint;

	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };

	icons_config.MergeMode = true; 
	icons_config.PixelSnapH = true;

	io.Fonts->AddFontFromMemoryTTF(roboto, sizeof(roboto), 14.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
	io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 12.0f, &icons_config, icons_ranges);
	io.Fonts->AddFontFromMemoryTTF(roboto, sizeof(roboto), 16.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
	io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 16.0f, &icons_config, icons_ranges);

	FONT::pEspName = io.Fonts->AddFontFromFileTTF(CXOR("C:\\Windows\\Fonts\\Verdana.ttf"), 11.5f, &name_cfg, io.Fonts->GetGlyphRangesCyrillic());
	FONT::pEspHealth = io.Fonts->AddFontFromFileTTF(CXOR("C:\\Windows\\Fonts\\Verdana.ttf"), 11.5f, &verdana_cfg, io.Fonts->GetGlyphRangesCyrillic());
	FONT::pEspWepName = io.Fonts->AddFontFromFileTTF(CXOR("C:\\Windows\\Fonts\\Verdana.ttf"), 11.5f, &name_cfg, io.Fonts->GetGlyphRangesDefault());
	FONT::pEspFlagsName = io.Fonts->AddFontFromFileTTF(CXOR("C:\\Windows\\Fonts\\Verdana.ttf"), 11.5f, &verdana_cfg, io.Fonts->GetGlyphRangesDefault());

	io.Fonts->FontBuilderFlags = ImGuiFreeTypeBuilderFlags_LightHinting;
	bInitialized = io.Fonts->Build();

	ImGui::StyleColorsDark();

	// new style
	auto& ss = ImGui::GetStyle();
	ss.WindowRounding = 10.f;
	ss.ChildRounding = 6.f;
	ss.PopupRounding = 5.f;
	ss.FramePadding = ImVec2(4, 1);
	ss.FrameRounding = 3.f;
	ss.FrameBorderSize = 1.f;
	ss.ScrollbarSize = 6.f;
	ss.ScrollbarRounding = 0.f;

	auto c = ss.Colors;
	c[ImGuiCol_TextDisabled] = ImVec4(1.00f, 1.00f, 1.00f, 0.25f);
	c[ImGuiCol_PopupBg] = ImColor(14, 14, 16);
	c[ImGuiCol_Border] = ImColor(27, 27, 32);
	c[ImGuiCol_Button] = ImColor(27, 27, 32);
	c[ImGuiCol_ButtonHovered] = ImColor(14, 14, 16);
	c[ImGuiCol_ButtonActive] = ImColor(27, 27, 32);
	c[ImGuiCol_FrameBg] = ImColor(155, 150, 219);
	c[ImGuiCol_Header] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	c[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
	c[ImGuiCol_HeaderActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.15f);
	c[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

	return bInitialized;
}

void D::Destroy()
{
	if (!bInitialized)
		return;

	//idk shit crashing
	//ImGui::GetIO().Fonts->Clear();
	//ImGui::GetIO().Fonts->TexID = 0;

	IM_DELETE(pDrawListActive);
	IM_DELETE(pDrawListSafe);
	IM_DELETE(pDrawListRender);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	//idk shit crashing
	//ImGui::DestroyContext();

	bInitialized = false;
}

void D::Render(float remaining, bool init)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (init)
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(io.DisplaySize, ImGuiCond_Once);
		ImGui::Begin(CXOR("##remaining"), NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
		{
			ImVec2 pos = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);

			float rad_circle = 60.0f;

			// indicator circle
			ImGui::GetWindowDrawList()->AddCircle(pos, rad_circle, IM_COL32(255, 255, 255, 80), 32, 2.0f);

			// remaining idicator
			ImGui::GetWindowDrawList()->PathArcTo(pos, rad_circle, 0.0f, remaining * 2 * IM_PI, 32);
			ImGui::GetWindowDrawList()->PathStroke(ImGui::GetColorU32(ImGuiCol_FrameBg), ImDrawFlags_None, 3.0f);

			char remaining_text[32];
			sprintf_s(remaining_text, CXOR("%.0f%%"), remaining * 100.0f);

			// text
			ImVec2 text_size = ImGui::CalcTextSize(remaining_text);
			ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x - text_size.x * 0.5f, pos.y - text_size.y * 0.5f), IM_COL32(255, 255, 255, 255), remaining_text);
		}

		ImGui::End();
		ImGui::PopStyleVar(2);
	}
	else
	{
		MENU::RenderMainWindow();
		g_visuals_overlay::Render();
	}

	ImGui::EndFrame();
	ImGui::Render();

	I::DeviceContext->OMSetRenderTargets(1, &I::RenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool D::OnWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!bInitialized)
		return false;

	IPT::OnWndProc(hWnd, uMsg, wParam, lParam);

	if (IPT::IsKeyReleased(VK_INSERT))
	{
		cheat->menu_open = !cheat->menu_open;

		const auto oIsRelativeMouseMode = H::hkIsRelativeMouseMode.GetOriginal();
		oIsRelativeMouseMode(I::InputSystem, cheat->menu_open ? false : MENU::bMainActive);
	}

	return ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam) || cheat->menu_open;
}

void D::RenderDrawData(ImDrawData* pDrawData)
{
	if (::TryAcquireSRWLockExclusive(&drawLock))
	{
		*pDrawListRender = *pDrawListSafe;
		::ReleaseSRWLockExclusive(&drawLock);
	}

	if (pDrawListRender->CmdBuffer.empty())
		return;

	// remove trailing command if unused
	// @note: equivalent to  pDrawList->_PopUnusedDrawCmd()
	if (const ImDrawCmd& lastCommand = pDrawListRender->CmdBuffer.back(); lastCommand.ElemCount == 0 && lastCommand.UserCallback == nullptr)
	{
		pDrawListRender->CmdBuffer.pop_back();
		if (pDrawListRender->CmdBuffer.empty())
			return;
	}

	ImGuiContext* pContext = ImGui::GetCurrentContext();
	ImGuiViewportP* pViewport = pContext->Viewports[0];
	ImVector<ImDrawList*>* vecDrawLists = pViewport->DrawDataBuilder.Layers[0];
	vecDrawLists->push_front(pDrawListRender); // this one being most background

	pDrawData->CmdLists.push_front(pDrawListRender);
	pDrawData->CmdListsCount = vecDrawLists->Size;

	pDrawData->TotalVtxCount += pDrawListRender->VtxBuffer.Size;
	pDrawData->TotalIdxCount += pDrawListRender->IdxBuffer.Size;
}

void D::ResetDrawData()
{
	pDrawListActive->_ResetForNewFrame();
	pDrawListActive->PushTextureID(ImGui::GetIO().Fonts->TexID);
	pDrawListActive->PushClipRectFullScreen();
}

void D::SwapDrawData()
{
	::AcquireSRWLockExclusive(&drawLock);
	*pDrawListSafe = *pDrawListActive;
	::ReleaseSRWLockExclusive(&drawLock);
}

bool D::WorldToScreen(const Vector_t& in, ImVec2& out) {
	if (!ImGui::GetCurrentContext()) return false;

	auto z = SDK::ViewMatrix[3][0] * in.x + SDK::ViewMatrix[3][1] * in.y + SDK::ViewMatrix[3][2] * in.z + SDK::ViewMatrix[3][3];
	if (z < 0.001f) return false;

	out = ImGui::GetIO().DisplaySize * 0.5f;
	out.x *= 1.0f + (SDK::ViewMatrix[0][0] * in.x + SDK::ViewMatrix[0][1] * in.y + SDK::ViewMatrix[0][2] * in.z + SDK::ViewMatrix[0][3]) / z;
	out.y *= 1.0f - (SDK::ViewMatrix[1][0] * in.x + SDK::ViewMatrix[1][1] * in.y + SDK::ViewMatrix[1][2] * in.z + SDK::ViewMatrix[1][3]) / z;
	out = ImFloor(out);

	return true;
}

void D::AddDrawListRect(ImDrawList* pDrawList, const ImVec2& vecMin, const ImVec2& vecMax, const Color_t& colRect, const unsigned int uFlags, const Color_t& colOutline, const float flRounding, const ImDrawFlags roundingCorners, float flThickness, const float flOutlineThickness)
{
	if (pDrawList == nullptr)
		pDrawList = pDrawListActive;

	const ImU32 colRectPacked = colRect.GetU32();
	const ImU32 colOutlinePacked = colOutline.GetU32();

	if (uFlags & DRAW_RECT_FILLED)
		pDrawList->AddRectFilled(vecMin, vecMax, colRectPacked, flRounding, roundingCorners);
	else
	{
		pDrawList->AddRect(vecMin, vecMax, colRectPacked, flRounding, roundingCorners, flThickness);
		flThickness *= 0.5f;
	}

	const float flHalfOutlineThickness = flOutlineThickness * 0.5f;
	const ImVec2 vecThicknessOffset = { flThickness + flHalfOutlineThickness, flThickness + flHalfOutlineThickness };

	if (uFlags & DRAW_RECT_BORDER)
		pDrawList->AddRect(vecMin + vecThicknessOffset, vecMax - vecThicknessOffset, colOutlinePacked, flRounding, roundingCorners, flOutlineThickness);

	if (uFlags & DRAW_RECT_OUTLINE)
		pDrawList->AddRect(vecMin - vecThicknessOffset, vecMax + vecThicknessOffset, colOutlinePacked, flRounding, roundingCorners, flOutlineThickness);
}

void D::AddDrawListRectMultiColor(ImDrawList* pDrawList, const ImVec2& vecMin, const ImVec2& vecMax, const Color_t& colUpperLeft, const Color_t& colUpperRight, const Color_t& colBottomRight, const Color_t& colBottomLeft)
{
	if (pDrawList == nullptr)
		pDrawList = pDrawListActive;

	pDrawList->AddRectFilledMultiColor(vecMin, vecMax, colUpperLeft.GetU32(), colUpperRight.GetU32(), colBottomRight.GetU32(), colBottomLeft.GetU32());
}

void D::AddDrawListCircle(ImDrawList* pDrawList, const ImVec2& vecCenter, const float flRadius, const Color_t& colCircle, const int nSegments, const unsigned int uFlags, const Color_t& colOutline, float flThickness, const float flOutlineThickness)
{
	if (pDrawList == nullptr)
		pDrawList = pDrawListActive;

	const ImU32 colCirclePacked = colCircle.GetU32();

	if (uFlags & DRAW_CIRCLE_FILLED)
	{
		pDrawList->AddCircleFilled(vecCenter, flRadius, colCirclePacked, nSegments);
		flThickness = 0.0f;
	}
	else
		pDrawList->AddCircle(vecCenter, flRadius, colCirclePacked, nSegments, flThickness);

	if (uFlags & DRAW_CIRCLE_OUTLINE)
		pDrawList->AddCircle(vecCenter, flRadius + flOutlineThickness, colOutline.GetU32(), nSegments, flThickness + flOutlineThickness);
}

void D::AddDrawListArc(ImDrawList* pDrawList, const ImVec2& vecPosition, const float flRadius, const float flMinimumAngle, const float flMaximumAngle, const Color_t& colArc, const float flThickness)
{
	if (pDrawList == nullptr)
		pDrawList = pDrawListActive;

	pDrawList->PathArcTo(vecPosition, flRadius, M_DEG2RAD(flMinimumAngle), M_DEG2RAD(flMaximumAngle), 32);
	pDrawList->PathStroke(colArc.GetU32(), false, flThickness);
}

void D::AddDrawListLine(ImDrawList* pDrawList, const ImVec2& vecFirst, const ImVec2& vecSecond, const Color_t& colLine, const float flThickness)
{
	if (pDrawList == nullptr)
		pDrawList = pDrawListActive;

	pDrawList->AddLine(vecFirst, vecSecond, colLine.GetU32(), flThickness);
}

void D::AddDrawListTriangle(ImDrawList* pDrawList, const ImVec2& vecFirst, const ImVec2& vecSecond, const ImVec2& vecThird, const Color_t& colTriangle, const unsigned int uFlags, const Color_t& colOutline, const float flThickness)
{
	if (pDrawList == nullptr)
		pDrawList = pDrawListActive;

	const ImU32 colTrianglePacked = colTriangle.GetU32();

	if (uFlags & DRAW_TRIANGLE_FILLED)
		pDrawList->AddTriangleFilled(vecFirst, vecSecond, vecThird, colTrianglePacked);
	else
		pDrawList->AddTriangle(vecFirst, vecSecond, vecThird, colTrianglePacked, flThickness);

	if (uFlags & DRAW_TRIANGLE_OUTLINE)
		pDrawList->AddTriangle(vecFirst, vecSecond, vecThird, colOutline.GetU32(), flThickness + 1.0f);
}

void D::AddDrawListQuad(ImDrawList* pDrawList, const ImVec2& vecFirst, const ImVec2& vecSecond, const ImVec2& vecThird, const ImVec2& vecFourth, const Color_t& colQuad, const unsigned int uFlags, const Color_t& colOutline, const float flThickness)
{
	if (pDrawList == nullptr)
		pDrawList = pDrawListActive;

	const ImU32 colQuadPacked = colQuad.GetU32();

	if (uFlags & DRAW_QUAD_FILLED)
		pDrawList->AddQuadFilled(vecFirst, vecSecond, vecThird, vecFourth, colQuadPacked);
	else
		pDrawList->AddQuad(vecFirst, vecSecond, vecThird, vecFourth, colQuadPacked, flThickness);

	if (uFlags & DRAW_QUAD_OUTLINE)
		pDrawList->AddQuad(vecFirst, vecSecond, vecThird, vecFourth, colOutline.GetU32(), flThickness + 1.0f);
}

void D::AddDrawListPolygon(ImDrawList* pDrawList, const ImVec2* vecPoints, const int nPointsCount, const Color_t& colPolygon, unsigned int uFlags, const Color_t& colOutline, const bool bClosed, const float flThickness)
{
	if (pDrawList == nullptr)
		pDrawList = pDrawListActive;

	const ImU32 colPolygonPacked = colPolygon.GetU32();

	if (uFlags & DRAW_POLYGON_FILLED)
		pDrawList->AddConvexPolyFilled(vecPoints, nPointsCount, colPolygonPacked);
	else
		pDrawList->AddPolyline(vecPoints, nPointsCount, colPolygonPacked, bClosed, flThickness);

	if (uFlags & DRAW_POLYGON_OUTLINE)
		pDrawList->AddPolyline(vecPoints, nPointsCount, colOutline.GetU32(), bClosed, flThickness + 1.0f);
}

void D::AddDrawListText(ImDrawList* pDrawList, const ImFont* pFont, const ImVec2& vecPosition, const char* szText, const Color_t& colText, const unsigned int uFlags, const Color_t& colOutline, const float flThickness)
{
	if (pDrawList == nullptr)
		pDrawList = pDrawListActive;

	// set font texture
	pDrawList->PushTextureID(pFont->ContainerAtlas->TexID);

	const ImU32 colOutlinePacked = colOutline.GetU32();

	if (uFlags & DRAW_TEXT_DROPSHADOW)
		pDrawList->AddText(pFont, pFont->FontSize, vecPosition + ImVec2(flThickness, flThickness), colOutlinePacked, szText);
	else if (uFlags & DRAW_TEXT_OUTLINE)
	{
		pDrawList->AddText(pFont, pFont->FontSize, vecPosition + ImVec2(flThickness, -flThickness), colOutlinePacked, szText);
		pDrawList->AddText(pFont, pFont->FontSize, vecPosition + ImVec2(-flThickness, flThickness), colOutlinePacked, szText);
	}

	pDrawList->AddText(pFont, pFont->FontSize, vecPosition, colText.GetU32(), szText);
	pDrawList->PopTextureID();
}

void D::AddDrawListShadowRect(ImDrawList* pDrawList, const ImVec2& vecMin, const ImVec2& vecMax, const Color_t& colShadow, float flThickness, float flRounding, ImDrawFlags roundingCorners)
{
	if (pDrawList == nullptr)
		pDrawList = pDrawListActive;

	pDrawList->AddShadowRect(vecMin, vecMax, colShadow.GetU32(), flThickness, ImVec2(0, 0), roundingCorners, flRounding);
}

void AnimationHandler_t::Update(const float flDeltaTime, const float flDuration)
{
	if (fnEaseIn == nullptr)
		fnEaseIn = &EASING::InSine;

	if (fnEaseOut == nullptr)
		fnEaseOut = &EASING::OutSine;

	flElapsedTime = 0;
	flElapsedTime = MATH::Max(0.0f, MATH::Min(flElapsedTime, flDuration));

	float flTime = flElapsedTime / flDuration;
	float flInitialValue = bSwitch ? 0.1f : flValue;
	float flTargetValue = bSwitch ? 1.0f : 0.1f;

	EasingFunction_t fnCurrentEase = bSwitch ? fnEaseIn : fnEaseOut;
	flValue = (flInitialValue + (flTargetValue - flInitialValue)) * (float)fnCurrentEase(flTime);
	flValue = MATH::Clamp(flValue, 0.1f, 1.0f);

	flElapsedTime += flDeltaTime;
	bLastSwitch = bSwitch;
}