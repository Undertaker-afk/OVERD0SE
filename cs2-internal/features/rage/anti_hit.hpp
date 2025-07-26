#include <iostream>
#include <memory>

#include "../../utilities/common.h"
#include "../../utilities/draw.h"

#include "../../core/sdk.h"
#include "../../core/config.h"
#include "../../core/sdk.h"

#include "../../sdk/datatypes/usercmd.h"
#include "../../sdk/datatypes/qangle.h"
#include "../../sdk/entity.h"
#include "../../sdk/interfaces/iengineclient.h"
#include "../../sdk/interfaces/iglobalvars.h"
#include "../../sdk/interfaces/cgameentitysystem.h"
#include "../../sdk/datatypes/qangle.h"
#include "../../sdk/datatypes/vector.h"
#include "../../sdk/datatypes/transform.h"

#include "../misc/movement.h"

#include "../../sdk/interfaces/ccsgoinput.h"
#include "../../sdk/interfaces/ienginecvar.h"
#include "../../sdk/interfaces/events.h"
#include "../../sdk/interfaces/itrace.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))

class anti_hit
{
public:
	void create_move(CUserCmd* m_pcmd);

	float get_pitch(CUserCmd* m_pcmd);
	float get_yaw(CUserCmd* m_pcmd);

	int jitter_side = -1;
};
const auto g_anti_hit = std::make_unique<anti_hit>();