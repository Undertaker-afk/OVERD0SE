#include "utilities/crt.h"
#include "utilities/memory.h"
#include "utilities/inputsystem.h"
#include "utilities/draw.h"

#include "core/interfaces.h"
#include "core/sdk.h"
#include "core/hooks.h"
#include "core/schema.h"
#include "core/convars.h"

#include "features/configs/configs_var.h"
#include "features/visuals/visuals.h"

#include "security/security.h"
#include "features/skins/inventorychanger.h"

uintptr_t CS_STDCALL start_address(const HMODULE h_module)
{
#ifdef _DEBUG
	AllocConsole( );

	freopen_s( reinterpret_cast< FILE** >( stdin ), CXOR( "CONIN$" ), CXOR( "r" ), stdin );
	freopen_s( reinterpret_cast< FILE** >( stdout ), CXOR( "CONOUT$" ), CXOR( "w" ), stdout );

	SetConsoleTitleA( CXOR( "debug" ) );
#endif

	if (!MEM::Setup()) /*memory*/
	{
		printf(CXOR("failed to setup memory.\n"));

		CS_ASSERT(false);

		return 0;
	}

	if (!MATH::Setup()) /*math*/
	{
		printf(CXOR("failed to setup math.\n"));

		CS_ASSERT(false);

		return 0;
	}

	if (!I::Setup()) /*interfaces*/
	{
		printf(CXOR("failed to setup interfaces."));

		CS_ASSERT(false);

		return false;
	}

	if (!SDK::Setup()) /*sdk*/
	{
		printf(CXOR("failed to setup sdk.\n"));

		CS_ASSERT(false);

		return 0;
	}

	if (!IPT::Setup()) /*input system*/
	{
		printf(CXOR("failed to setup input system.\n"));

		CS_ASSERT(false);

		return 0;
	}

	if (!F::VISUALS::Setup()) /*features*/
	{
		printf(CXOR("failed to setup features.\n"));

		CS_ASSERT(false);

		return 0;
	}

	if (!SCHEMA::Setup()) /*schema*/
	{
		printf(CXOR("failed to setup schema.\n"));

		CS_ASSERT(false);

		return 0;
	}

	if (!CONVAR::Setup()) /*convar*/
	{
		printf(CXOR("failed to setup convar.\n"));

		CS_ASSERT(false);

		return 0;
	}

	if (!H::Setup()) /*hooks*/
	{
		printf(CXOR("failed to setup hooks.\n"));

		CS_ASSERT(false);

		return 0;
	}
	changers::dumpitems = true;
	if (!C::Setup())
		printf(CXOR("failed to setup configs.\n"));

	printf(CXOR("Success!\n"));

	while (!cheat->unload)
	{
		Sleep(100);
	}

	// unhook
	IPT::Destroy();
	H::Destroy();
	D::Destroy();
	changers::Shutdown();
	// @credits: makstv8888
#ifdef _DEBUG
	const HWND hConsoleWindow = GetConsoleWindow();

	FreeConsole();

	if (hConsoleWindow != nullptr)
		PostMessageW(hConsoleWindow, WM_CLOSE, 0U, 0L);

	Sleep(100);
#endif

	FreeLibraryAndExitThread(h_module, 0);

	return 0;
}

extern "C" BOOL WINAPI _CRT_INIT(HMODULE h_module, DWORD ul_reason_for_call, LPVOID lp_reserved);

BOOL APIENTRY dllmain(HMODULE h_module, DWORD ul_reason_for_call, LPVOID lp_reserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) 
	{
		// fix msvc error's
		if (!_CRT_INIT(h_module, ul_reason_for_call, lp_reserved))
			return false;

		// start security
#ifndef _DEBUG
		if (!security::checker())
			return false;
#endif

		// call cheat 
		CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(start_address), h_module, NULL, NULL);

		return true;
	}

	return false;
}