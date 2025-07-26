#pragma once

#include <vector>

#include "../utilities/common.h"
#include "../utilities/draw.h"

namespace MENU
{
	struct ClickData {
		ImVec2 pos;
		float value = 0.0f;
	};

	void RenderMainWindow();

	inline bool bMainActive = false;

	inline int rage_subtab = 0;
	inline int legit_subtab = 0;
	inline int visuals_subtab = 0;
	inline int skins_subtab = 0;
	inline int misc_subtab = 0;
	inline int cloud_subtab = 0;
}