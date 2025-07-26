#include "visuals.h"

#include "../../sdk/interfaces/iengineclient.h"
#include "../../sdk/entity.h"

#include "overlay.h"
#include "chams.h"

#include "../../core/sdk.h"

using namespace F;

bool F::VISUALS::Setup()
{
	if (!ams::chams->initialize())
	{
		printf(CXOR("failed to initialize DrawObject chams | swapping to K3V"));
		return true;
	}	

	return true;
}

bool F::VISUALS::OnDrawObject(void* pAnimatableSceneObjectDesc, void* pDx11, material_data_t* arrMeshDraw, int nDataCount, void* pSceneView, void* pSceneLayer, void* pUnk, void* pUnk2)
{
	return ams::chams->draw_object(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
}