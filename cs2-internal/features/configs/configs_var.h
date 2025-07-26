#include "configs.h"
#include "../core/config.h"
#include "../../sdk/datatypes/color.h"

namespace cfg_var 
{
	struct cfg_t {
		//aim
		struct aim
		{
			bool enablerage, auto_fire;
			int cur_wep;

			struct weapon
			{
				bool autostop, autoscope;
				int chance, mindmg, point_scale{ 1 }, rage_target_select;

				bool m_head = false;
				bool m_chest = false;
				bool m_stomach = false;
				bool m_arms = false;
				bool m_legs = false;
				bool m_foot = false;

				bool p_head = false;
				bool p_chest = false;
				bool p_stomach = false;
				bool p_arms = false;
				bool p_legs = false;
				bool p_foot = false;

				int is_mindmg_override, mindmg_override_val, is_hitchan_override, hitchan_override_val;
			} weapon[10];
		} g_aim;

		struct legit_aim
		{
			bool enablelegit, aimfov;

			float aimrange{ 10.f }, aimsmooth{ 10.f };

			int legit_key;

			ColorPickerVar_t aimfovcolor{ 255, 255, 255 };
		} g_legit_aim;

		//anti
		struct antiaim
		{
			bool hideshots;
			int left_anti, right_anti, backward_anti, baseyaw, pitchtype, custompitch, customyaw;
		} g_antiaim;

		//visuals
		struct visuals
		{
			bool enablevis, enablevisarrows, skeleton, removelegs, localbullet;
			bool box, name, hbar, abar;
			bool textweap, hkflag, kitflag;
			bool Flags[5];

			ColorPickerVar_t arrows_color{ 255,0,0 };

			struct chams_enemie
			{
				bool vischam, invischam;
				int chamstype{ 1 };
				ColorPickerVar_t colVisCham{ 94, 255, 0 }, colInvisCham{ 101, 193, 255 };
			} chams_enemie;

			struct chams_local
			{
				bool chamLocal, weapcham_local, LocalGlow;
				int chamstype, chamstypeWeap;
				ColorPickerVar_t colLocalCham, colWeapCham, colLocalGlow;
			} chams_local;

			struct box_sets
			{
				bool outlined{ true };
				float rounding, thickness;
				ColorPickerVar_t col{ 255,255,255 };
			} box_sets;

			struct name_sets
			{
				ColorPickerVar_t col{ 255,255,255 };
			} name_sets;

			struct hbar_sets
			{
				bool outlined{ true };
				float thickness{ 2.0f };
				ColorPickerVar_t col{ 128,255,0 };
			} hbar_sets;

			struct abar_sets
			{
				bool outlined{ true };
				float thickness{ 2.0f };
				ColorPickerVar_t col{ 0,153,255 };
			} abar_sets;

			struct weap_sets
			{
				ColorPickerVar_t col{ 255,255,255 };
			} weap_sets;

			struct skelet_sets
			{
				bool outlined{ true };
				ColorPickerVar_t col{ 255,255,255 };
			} skelet_sets;
		} g_visuals;

		//misc
		struct misc
		{
			bool antiuntrusted, bunny, strafe, nightmode, viewover, smokeremove, granade_cam, m_buy_bot_enable, eventlogs[2], enable_aspectratio;
			float strafesmootch, ifov{ 90 }, map_tone, viewfov{ 60 }, aspectratio{ 1 }, viewX, viewY, viewZ;
		
			ColorPickerVar_t colNightmode, colLighting, colSkyBox;
		
			int m_buybot1, m_buybot2, thirdpersondist{ 120 }, thirdperson{ 0 }, m_scopelocal;
			int scope_thickness{ 1 }, scope_size{ 150 }, scope_gap;

			ColorPickerVar_t colCustomScope1{ 255, 35, 150, 255 }, colCustomScope2{ 255, 255, 255, 0 };

			bool m_buybot3[4];
		} g_misc;

		//skins
		struct skins
		{
			bool model_changer;
		} g_skins;
	};

	inline config_sdk::cfg_var_t< cfg_t > m_cfg{ 0x14881488, {} };
}