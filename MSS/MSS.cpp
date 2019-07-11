// MSS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <miles_header.h>
#include <vector>
#include <iomanip>
#include <Timeapi.h>
#include "MSS.h"

constexpr unsigned int sound_data_size = 1024 * 1024 * 100; // 100 MB
const char* names[] = {
	"general",
	"1PTailFilter",
	"1PTailSwitch",
	"1PTailVolume",
	"3PTailSwitch",
	"AmmoClipPct",
	"CodeValue",
	"ConvolutionPreset1P",
	"ConvolutionSend1P",
	"CurrentAngle",
	"CurrentAngleDistance",
	"CurrentAngleOcclusion",
	"Dia_AngleLS",
	"Dia_AngleSL",
	"Dia_OcclFreq",
	"Dia_OcclusionFreq",
	"Dia_OcclusionFreq_Convert",
	"Occlusion",
	"Dia_OcclusionVolWet",
	"DialogueVolume",
	"Enmity",
	"FadeInTime",
	"FloorAndRoof_FunctionController",
	"GameMusicVolume",
	"Importance",
	"InitialAngle",
	"InitialAngleDan",
	"InitialAngleDistance",
	"InitialAngleDistanceDan",
	"InitialAngleInitialOcclusion",
	"InitialAngleOcclusion",
	"InitialOcclusion",
	"InitialOcclusionDan",
	"InitialOcclusionDistance",
	"Interiorness",
	"IsTitan",
	"ListenerSoundscapeRoof",
	"LobbyMusicVolume",
	"LowHealth",
	"ManualDistance",
	"OcclusionDistance",
	"OcclusionDistance_Long",
	"OcclusionDistance_ReverbLarge",
	"OcclusionDistance_ReverbOutdoor",
	"OcclusionDistance_ReverbSmall",
	"OcclusionDistanceEven",
	"OcclusionTest",
	"PlayerSpeed",
	"PriorityOffset_minus100",
	"PriorityOffset_minus200",
	"PriorityOffset_plus25",
	"RandomLFOTest",
	"ReverbLarge",
	"ReverbOutdoor",
	"ReverbSmall",
	"SfxVolume",
	"Sidechain",
	"SlowOcclusion",
	"SoundscapeFloor",
	"Speed",
	"SpeedDistance",
	"TitanHealth",
	"VerticalDistance",
	"VerticalOcclusion",
	"VoiceCommVolume",
	"Master",
	"Voice_Comm_bus",
	"Submaster",
	"Compress",
	"SFX_Footsteps_Titan_Stealth_Cloaked_3P",
	"SFX_ui",
	"SFX",
	"ParametricEQ",
	"Reverb_bus_2",
	"Highpass",
	"Reverb",
	"Reverb_bus_1",
	"Reverb_1P_Bus",
	"Convolution",
	"NoDuck",
	"MUSIC_FD_POSTGAME",
	"MUSIC",
	"Lowpass_bus_1",
	"Lowpass",
	"HiFocusEq",
	"DIAL",
	"Delay_Bus",
	"Chorus",
	"sfx_ui_voStingers",
	"SFX_ui_menu",
	"SFX_ui_InGame",
	"SFX_PreBuss",
	"SFX_pilotHelm_pickup",
	"SFX_1P",
	"SFX_Reverb_bus_2",
	"SFX_Playback_Timescrubbers_Reverb_bus_2",
	"DIAL_Reverb_bus_2",
	"SFX_Reverb_bus_1",
	"SFX_Playback_Timescrubbers_Reverb_bus_1",
	"DIAL_Reverb_bus_1",
	"SFX_Reverb_1P_Bus",
	"DIAL_Reverb_1P_Bus",
	"MUSIC_SP",
	"MUSIC_MP",
	"MUSIC_Lobby",
	"SFX_Lowpass_bus_1",
	"DIAL_Lowpass_bus_1",
	"DIAL_ui",
	"DIA_Titan_Int",
	"DIA_Titan_Ext",
	"DIA_AP_HostPA_Ext",
	"Bandpass",
	"SFX_Delay_Bus",
	"DIAL_Delay_Bus",
	"SFX_Scripted",
	"SFX_General",
	"SFX_Combat_Default",
	"SFX_amb",
	"SFX_Scripted_ForTitanInt",
	"SFX_impacts_3p_hits_1p",
	"SFX_impacts_1p_hits_3p",
	"SFX_1P_Titan_PreBuss",
	"SFX_1P_Pilot_PreBuss",
	"MUSIC_Wilds",
	"MUSIC_Training",
	"MUSIC_Timeshift_Timestop_Wet",
	"MUSIC_Timeshift_Timestop_Dry",
	"MUSIC_Timeshift",
	"MUSIC_TDay",
	"MUSIC_Skyway",
	"MUSIC_ShipToShip",
	"MUSIC_Sewers",
	"MUSIC_CloudCity",
	"MUSIC_Boomtown",
	"DIAL_ui_readyup",
	"DIAL_ui_killQuips",
	"DIAL_ui_champIntro",
	"DIAL_vdu_deprecated",
	"DIAL_tos_1p_pa_far",
	"DIAL_tos_1p_pa",
	"DIAL_tos_1p_coms",
	"DIAL_tos_1p_cockpit",
	"DIAL_titanos_deprecated",
	"DIAL_SP_deprecated",
	"DIAL_scripted_dial_deprecated",
	"DIAL_MP_WraithVoid",
	"DIAL_MP_GM",
	"DIAL_MP_Faction_deprecated",
	"DIAL_MP_CausticGasBreath",
	"DIAL_GasMaskBreath",
	"DIAL_FacilityPA_deprecated",
	"DIAL_SP_Ext_deprecated",
	"DIA_ForLowHealth_MenuVol",
	"DIAL_AP_HostPA_ToneSfx",
	"DIAL_AP_HostPA",
	"SFX_sp_scripted",
	"SFX_Survival_gameplay_looping",
	"SFX_Survival_gameplay",
	"SFX_Interactions",
	"SFX_Weaponfire_3P_Enemy",
	"SFX_Combat_3P",
	"SFX_amb_emitters_sp",
	"SFX_amb_emitters_mp",
	"SFX_amb_beds",
	"SFX_impacts_titan_3p_hits_1P",
	"SFX_impacts_3p_hits_1p_explosions",
	"SFX_impacts_3p_hits_1p_bullets",
	"SFX_impacts_titandamagestates_1p_hits_3p",
	"SFX_impacts_1p_hits_environment",
	"SFX_impacts_1p_hits_3p_explosions",
	"SFX_impacts_1p_hits_3p_bullets",
	"SFX_titan_abilities_1p",
	"SFX_1P_Titan_Weapons",
	"SFX_1P_Movement_Titans",
	"SFX_pilot_abilities_1p",
	"SFX_1P_Pilot_Weapons",
	"SFX_1P_Movement_Pilots",
	"DIAL_ui_readyup_wraith",
	"DIAL_ui_readyup_wattson",
	"DIAL_ui_readyup_pathfinder",
	"DIAL_ui_readyup_octane",
	"DIAL_ui_readyup_mirage",
	"DIAL_ui_readyup_lifeline",
	"DIAL_ui_readyup_gibraltar",
	"DIAL_ui_readyup_caustic",
	"DIAL_ui_readyup_bangalore",
	"DIAL_ui_readup_bloodhound",
	"DIAL_ui_champIntro_wraith",
	"DIAL_ui_champIntro_wattson",
	"DIAL_ui_champIntro_pathfinder",
	"DIAL_ui_champIntro_octane",
	"DIAL_ui_champIntro_mirage",
	"DIAL_ui_champIntro_lifeline",
	"DIAL_ui_champIntro_gibraltar",
	"DIAL_ui_champIntro_caustic",
	"DIAL_ui_champIntro_bloodhound",
	"DIAL_ui_champIntro_bangalore",
	"DIAL_tos_1p_pa_bt_far",
	"DIAL_tos_1p_pa_bt",
	"DIAL_tos_1p_coms_gnrc_noduck",
	"DIAL_tos_1p_coms_gnrc",
	"DIAL_tos_1p_coms_bt",
	"DIAL_tos_1p_cockpit_gnrc_noduck",
	"DIAL_tos_1p_cockpit_gnrc",
	"DIAL_tos_1p_cockpit_bt",
	"DIAL_SP_sts",
	"DIAL_MP_Faction_Marvin",
	"DIAL_MP_Faction_generic",
	"DIA_ForLowHealth_Medium",
	"DIAL_AP_HostPA_Near",
	"DIAL_AP_HostPA_Far",
	"SFX_scripted_wilds",
	"SFX_scripted_training",
	"SFX_scripted_timeshift",
	"SFX_Scripted_Thunderdome",
	"SFX_scripted_tday",
	"SFX_scripted_story",
	"SFX_scripted_special_S2S",
	"SFX_scripted_skyway",
	"SFX_scripted_skits",
	"SFX_scripted_shiptoship",
	"SFX_Scripted_Ship2Ship",
	"SFX_scripted_S2S",
	"SFX_scripted_reclamation",
	"SFX_Scripted_Playback",
	"SFX_Scripted_Exception_S2S_3",
	"SFX_Scripted_Exception_S2S_2",
	"SFX_Scripted_Exception_S2S",
	"SFX_scripted_cloudcity",
	"SFX_Scripted_Boomtown_Ducked",
	"SFX_scripted_boomtown",
	"SFX_scripted_beacon_Switch_Activate",
	"SFX_scripted_beacon",
	"FX_scripted_boomtown_looping",
	"Survival_Circle",
	"SFX_Weapons_3P",
	"SFX_vehicles",
	"SFX_Titan_Embarks_3P",
	"SFX_titan_abilities_3p_friendly",
	"SFX_titan_abilities_3p_enemy",
	"SFX_Reloads_Titan_3P",
	"SFX_Reloads_Pilot_3P",
	"SFX_Reloads_3P",
	"SFX_pilot_abilities_3p_friendly",
	"SFX_pilot_abilities_3p_enemy",
	"SFX_pickups_3p",
	"SFX_Movement_3P_Friendly",
	"SFX_Movement_3P_Enemy",
	"SFX_impacts_thermiteflamewall",
	"SFX_impacts_friendly",
	"SFX_impacts_enemy",
	"SFX_Footsteps_HiPass",
	"SFX_Executions_3P",
	"SFX_creatures",
	"SFX_amb_slowrings_timeshift",
	"SFX_amb_fastrings_timeshift",
	"SFX_amb_emitters_wilds",
	"SFX_amb_emitters_training",
	"SFX_amb_emitters_timeshift_EXT",
	"SFX_amb_emitters_timeshift",
	"SFX_amb_emitters_tday",
	"SFX_amb_emitters_skyway",
	"SFX_amb_emitters_shiptoship",
	"SFX_amb_emitters_S2S",
	"SFX_amb_emitters_reclamation",
	"SFX_amb_emitters_cloudcity",
	"SFX_amb_emitters_boomtown",
	"SFX_amb_emitters_beacon",
	"SFX_amb_emitters_mp_thaw",
	"SFX_amb_emitters_mp_oasis",
	"SFX_amb_emitters_mp_kodai",
	"SFX_amb_emitters_mp_homestead",
	"SFX_amb_emitters_mp_grave",
	"SFX_amb_emitters_mp_glitch",
	"SFX_amb_emitters_mp_eden",
	"SFX_amb_emitters_mp_drydock",
	"SFX_amb_emitters_mp_crashsite",
	"SFX_amb_emitters_mp_complex",
	"SFX_amb_emitters_mp_blackwater",
	"SFX_amb_emitters_cloudcity_underbelly",
	"SFX_amb_Thunderdome_Crowds_Survival",
	"SFX_amb_beds_wilds",
	"SFX_amb_beds_training",
	"SFX_amb_beds_timeshift_present",
	"SFX_amb_beds_timeshift_past",
	"SFX_amb_beds_thunderdome",
	"SFX_amb_beds_tday",
	"SFX_amb_beds_skyway",
	"SFX_amb_beds_shiptoship",
	"SFX_amb_beds_S2S",
	"SFX_amb_beds_reclamation",
	"SFX_amb_beds_mp_wargames",
	"SFX_amb_beds_mp_thaw",
	"SFX_amb_beds_mp_kodai",
	"SFX_amb_beds_mp_homestead",
	"SFX_amb_beds_mp_grave",
	"SFX_amb_beds_mp_glitch",
	"SFX_amb_beds_mp_eden",
	"SFX_amb_beds_mp_drydock",
	"SFX_amb_beds_mp_crashsite",
	"SFX_amb_beds_mp_complex",
	"SFX_amb_beds_mp_blackwater",
	"SFX_amb_beds_cloudcity",
	"SFX_amb_beds_boomtown",
	"SFX_amb_beds_beacon",
	"SFX_impacts_1p_hits_environment_explosions",
	"SFX_impacts_1p_hits_environment_bullets",
	"SFX_weapons_titan_1p_reloads",
	"SFX_weapons_titan_1p_handling",
	"SFX_weapons_titan_1p_gunfire",
	"SFX_Reloads_Titan_1P",
	"SFX_titan_1p_tone",
	"SFX_titan_1p_survival",
	"SFX_titan_1p_Stealth_Normal",
	"SFX_titan_1p_stealth_cloaked",
	"SFX_titan_1p_scorch",
	"SFX_titan_1p_ronin",
	"SFX_titan_1p_northstar",
	"SFX_titan_1p_legion",
	"SFX_titan_1p_ion",
	"SFX_player_titan_movement",
	"SFX_player_titan_footsteps",
	"SFX_player_titan_dash",
	"SFX_player_titan_cockpit",
	"SFX_weapons_pilot_1p_gunfire_loops",
	"SFX_weapons_pilot_1p_gunfire",
	"SFX_weapons_pilot_1p_reloads",
	"SFX_weapons_pilot_1p_handling",
	"SFX_Player_RoboPilot_1P",
	"SFX_player_pilot_slide",
	"SFX_player_pilot_jumpjet",
	"SFX_player_pilot_footsteps_lowpass",
	"SFX_player_pilot_foley",
	"SFX_Pickups_1P",
	"DIAL_MP_Faction_Marvin_Vocals",
	"DIAL_MP_Faction_Marvin_Foley",
	"DIAL_MP_Callouts",
	"DIAL_levelcommentary_deprecated",
	"DIAL_ai",
	"SFX_Scripted_Playback_Timescrubbers",
	"SFX_CloudCity_Gunship_Thrusters",
	"SFX_CloudCity_Gunship",
	"SFX_weapons_turrets",
	"SFX_weapons_titan_3p_gunfire_enemy",
	"SFX_weapons_titan_3p_gunfire",
	"SFX_weapons_pilot_3p_gunfire_tails",
	"SFX_weapons_pilot_3p_gunfire_loops",
	"SFX_weapons_pilot_3p_gunfire_enemy",
	"SFX_weapons_pilot_3p_gunfire",
	"SFX_weapons_ai_tails",
	"SFX_weapons_ai_gunfire",
	"SFX_vehicles_train",
	"SFX_vehicles_redeye",
	"SFX_vehicles_hovertank",
	"SFX_vehicles_goblins",
	"SFX_vehicles_droppods",
	"SFX_vehicles_crows",
	"SFX_harvester",
	"SFX_Titan_Embark_Phase_3P",
	"SFX_Titan_Embark_3P",
	"SFX_titan_abilities_3p",
	"SFX_pilot_abilities_3p",
	"SFX_Movement_3P",
	"SFX_impacts_temp",
	"SFX_impacts",
	"SFX_impacts_whizbys_enemy",
	"SFX_Footsteps_3P_Friendly",
	"SFX_Footsteps_3P_Enemy",
	"SFX_creatures_prowlers_vocals",
	"SFX_creatures_prowlers_mvmt",
	"SFX_creatures_loottick_oneshots",
	"SFX_creatures_loottick_idleloop",
	"SFX_creatures_leviathans",
	"SFX_creatures_flyers",
	"SFX_player_pilot_footsteps",
	"SFX_Player_Pathfinder_Footsteps",
	"SFX_Player_Pathfinder_Foley",
	"SFX_Player_Octane_Stim_Footsteps",
	"DIAL_MP_Callouts_3p_Near",
	"DIAL_MP_Callouts_3p_Far",
	"DIAL_MP_Callouts_1p",
	"DIAL_levelcommentary_militia",
	"DIAL_levelcommentary_imc",
	"DIAL_ai_impacts",
	"DIAL_ai_highcombat_militia_spectre",
	"DIAL_ai_highcombat_imc_spectre",
	"DIAL_ai_battle",
	"SFX_Titan_BT_Embark_Phase_3P",
	"SFX_Titan_BT_Embark_3P",
	"SFX_Abilities_WattsonTactical_3p",
	"SFX_Abilities_DroneHeal_3P",
	"SFX_Movement_Wraith_3P",
	"SFX_Movement_Titans_3P",
	"SFX_Movement_Spectres_3P",
	"SFX_Movement_Pilots_3P_Enemy",
	"SFX_Movement_Pilots_3P",
	"SFX_Movement_Pathfinder_3P",
	"SFX_Movement_Marvins_3P",
	"SFX_Movement_Lifeline_3P",
	"SFX_Movement_Grunts_3P",
	"SFX_Movement_Gibraltar_3P",
	"SFX_Movement_Drones_Looping_3P",
	"SFX_Movement_Drones_3P",
	"SFX_Movement_Caustic_3P",
	"SFX_Movement_BT_3P",
	"SFX_Movement_Bloodhound_3P",
	"SFX_Movement_Bangalore_3P",
	"SFX_Jumpjet_3P_Enemy",
	"SFX_Jumpjet_3P",
	"SFX_Bodyfalls_3P",
	"SFX_3p_stalkers_vocals_attacks_destruction",
	"SFX_3p_reapers_vocals_attacks_destruction",
	"SFX_impacts_whizbys",
	"SFX_impacts_titandamagestates",
	"SFX_impacts_physics",
	"SFX_impacts_deaths",
	"SFX_impacts_3p_hits_environment",
	"SFX_impacts_3p_hits_3p",
	"SFX_Footsteps_3P",
	"DIAL_MP_Efforts_3p_Near",
	"DIAL_MP_Callouts_3p_Near_Wraith",
	"DIAL_MP_Callouts_3p_Near_Wattson",
	"DIAL_MP_Callouts_3p_Near_Pathfinder",
	"DIAL_MP_Callouts_3p_Near_Octane",
	"DIAL_MP_Callouts_3p_Near_Mirage",
	"DIAL_MP_Callouts_3p_Near_Lifeline",
	"DIAL_MP_Callouts_3p_Near_Gibraltar",
	"DIAL_MP_Callouts_3p_Near_Caustic",
	"DIAL_MP_Callouts_3p_Near_Bloodhound",
	"DIAL_MP_Callouts_3p_Near_Bangalore",
	"DIAL_MP_Callouts_3p_Far_Wraith",
	"DIAL_MP_Callouts_3p_Far_Wattson",
	"DIAL_MP_Callouts_3p_Far_Pathfinder",
	"DIAL_MP_Callouts_3p_Far_Octane",
	"DIAL_MP_Callouts_3p_Far_Mirage",
	"DIAL_MP_Callouts_3p_Far_Lifeline",
	"DIAL_MP_Callouts_3p_Far_Gibraltar",
	"DIAL_MP_Callouts_3p_Far_Caustic",
	"DIAL_MP_Callouts_3p_Far_Bloodhound",
	"DIAL_MP_Callouts_3p_Far_Bangalore",
	"DIAL_MP_Efforts_1p",
	"DIAL_MP_Callouts_1p_Wraith",
	"DIAL_MP_Callouts_1p_Wattson",
	"DIAL_MP_Callouts_1p_Pathfinder",
	"DIAL_MP_Callouts_1p_Octane",
	"DIAL_MP_Callouts_1p_Mirage",
	"DIAL_MP_Callouts_1p_Lifeline",
	"DIAL_MP_Callouts_1p_Gibraltar",
	"DIAL_MP_Callouts_1p_Caustic",
	"DIAL_MP_Callouts_1p_Bloodhound",
	"DIAL_MP_Callouts_1p_Bangalore",
	"DIAL_ai_bc_imc",
	"DIAL_ai_battle_militia",
	"DIAL_ai_battle_imc",
	"SFX_Bodyfalls_Spectres_3P",
	"SFX_impacts_titandamagestates_3p_hits_3p",
	"SFX_impacts_titandamagestates_3p_hits_1p",
	"SFX_impacts_3p_hits_environment_explosions",
	"SFX_impacts_3p_hits_environment_bullets",
	"SFX_impacts_3p_hits_3p_explosions",
	"SFX_impacts_3p_hits_3p_bullets",
	"SFX_RoofFS_Wraith_3P",
	"SFX_RoofFS_Pathfinder_3P",
	"SFX_RoofFS_Octane_3p",
	"SFX_RoofFS_Lifeline_3P",
	"SFX_RoofFS_Gibraltar_3P",
	"SFX_RoofFS_Caustic_3p",
	"SFX_RoofFS_Bloodhound_3P",
	"SFX_RoofFS_Bangalore_3P",
	"SFX_Footsteps_Wraith_3P",
	"SFX_Footsteps_Titans_3P_Enemy",
	"SFX_Footsteps_Titan_3P",
	"SFX_Footsteps_Ticks_3P",
	"SFX_Footsteps_Stalkers_3P",
	"SFX_Footsteps_Spectres_3P",
	"SFX_Footsteps_Robopilots_3P_Enemy",
	"SFX_Footsteps_RoboPilot_3P",
	"SFX_Footsteps_Reapers_3P",
	"SFX_Footsteps_Pilots_3P_Enemy",
	"SFX_Footsteps_Pilots_3P",
	"SFX_Footsteps_Pathfinder_3P",
	"SFX_Footsteps_Octane_3p",
	"SFX_Footsteps_Lifeline_3P",
	"SFX_Footsteps_Grunts_3P",
	"SFX_Footsteps_Gibraltar_3P",
	"SFX_Footsteps_Caustic_3p",
	"SFX_Footsteps_Bloodhound_3P",
	"SFX_Footsteps_Bangalore_3P",
	"SFX_FloorFS_Wraith_3P",
	"SFX_FloorFS_Pathfinder_3P",
	"SFX_FloorFS_Octane_3p",
	"SFX_FloorFS_Lifeline_3P",
	"SFX_FloorFS_Gibraltar_3P",
	"SFX_FloorFS_Caustic_3p",
	"SFX_FloorFS_Bloodhound_3P",
	"SFX_FloorFS_Bangalore_3P",
	"DIAL_SP_MarderLogs",
	"DIAL_ai_bc_imc_deaths",
	"SFX_Footsteps_Titan_Tone_3P",
	"SFX_Footsteps_Titan_Survival_3P",
	"SFX_Footsteps_Titan_Stealth_Normal_3P",
	"SFX_Footsteps_Titan_Scorch_3P",
	"SFX_Footsteps_Titan_Ronin_3P",
	"SFX_Footsteps_Titan_Northstar_3P",
	"SFX_Footsteps_Titan_Legion_3P",
	"SFX_Footsteps_Titan_Ion_3P",
	"Duck_CloudCity_forUnderbellyMachinery",
	"Default",
	"Duck",
	"Duck_For_1P_Embarks",
	"Embark",
	"Duck_For_1P_Hits_3P_Impacts",
	"Duck_For_1P_Stim",
	"Duck_For_1P_Weaponfire",
	"Duck_for_AP_Host_ElimPlacement",
	"Duck_for_AP_Host_Notify",
	"Duck_For_AP_Wraith_VoidVoice",
	"Duck_For_BeastOfTheHunt",
	"Duck_For_ChampIntro",
	"Duck_For_ChampionVictory",
	"Duck_for_CircleDamage",
	"Duck_For_CloudCity_UpperCity",
	"Duck_For_CreditsFadeoutToMusic",
	"Duck_For_DeathSurvival",
	"Duck_For_Doubletime",
	"Duck_For_DroneHeal_3P",
	"Duck_For_Explosions",
	"Duck_For_GameSFX_inLobby",
	"Duck_For_HalfTime",
	"Duck_For_LevelEndFade_0second",
	"Duck_For_LevelEndFade_1second",
	"Duck_For_LevelEndFade_2seconds",
	"Duck_For_LevelEndFade_3seconds",
	"Duck_For_LevelEndFade_4seconds",
	"Duck_For_Movies",
	"Duck_For_MP_TitanCockpit",
	"Duck_For_MP_VO_TOS",
	"Duck_For_MuteGameSFXandVO_AdjustVOBalance_inLobby",
	"Duck_For_MuteGameSFXandVO_EndMatch",
	"Duck_For_PlayerHit",
	"Duck_For_PodLaunchVo",
	"Duck_For_QuickDeath_to_Respawn",
	"Duck_For_SP_TitanCockpit",
	"Duck_for_sp_vo_bt",
	"Duck_For_StagingArea",
	"Duck_For_SurvivalIntro",
	"Duck_For_Vo_inLobbyMenus",
	"Duck_For_VoQuips",
	"Duck_ForCriticalTemp",
	"Duck_ForPhaseShift",
	"Duck_ForRegularDia_sts",
	"Duck_ForStalkerBulletImpacts",
	"Duck_HalfDuck_ForRegularDia_sts",
	"Duck_Playback_PausedState",
	"Duck_Playback_TimescrubberReverb",
	"Duck_Spectator_FriendlyWpnFire_Boost",
	"english",
	"french",
	"german",
	"spanish",
	"italian",
	"japanese",
	"polish",
	"russian",
	"tchinese"
};
int names_length = 565;
struct unk {
	__int64* sound_function = 0; 
	LPCWSTR* endpointID = 0; // Used inside MilesDriverCreate when calling CreateDirectSound
	INT32 channel_count = 0;
	INT32 maybe_sample_rate = 0;
	WORD field18 = 0;
	WORD field1A = 0;
	__int64 hrtf_buffer = 0; // Pointer to HRTF Data
};
byte* sound_data = (byte*) malloc(sound_data_size);
unsigned int sound_data_cursor = 0;
byte* buffer_addr;
int write_size;
__int64 (*original_GET_AUDIO_BUFFER_AND_SET_SIZE)(__int64*, byte**, int);
__int64 (*original_TRANSFER_MIXED_AUDIO_TO_SOUND_BUFFER)(__int64*);
__int64 (*REFRESH_DSBUFFERS)(__int64*, int);


__int64 hook_REFRESH_DSBUFFERS(__int64* a, int b) {
	auto return_val = REFRESH_DSBUFFERS(a, b);
	hook_directsoundcom((byte*)a);
	std::cout << "updated hooks" << std::endl;
	return return_val;
}

void hook_directsoundcom(byte* DIRECTSOUNDCOM) {
	__int64* addr = *(long long**)(DIRECTSOUNDCOM + 0x30);
	original_GET_AUDIO_BUFFER_AND_SET_SIZE = (__int64 (*)(__int64*, byte * *, int))addr;

	addr = *(long long**)(DIRECTSOUNDCOM + 0x38);
	original_TRANSFER_MIXED_AUDIO_TO_SOUND_BUFFER = *(__int64 (*)(__int64*))addr;

	addr = *(long long**)(DIRECTSOUNDCOM + 0x20);
	REFRESH_DSBUFFERS = *(__int64 (*)(__int64*, int))addr;

	*(long long**)(DIRECTSOUNDCOM + 0x20) = (long long*)& hook_REFRESH_DSBUFFERS;
	*(long long**)(DIRECTSOUNDCOM + 0x30) = (long long*)& hook_GET_AUDIO_BUFFER_AND_SET_SIZE;
	*(long long**)(DIRECTSOUNDCOM + 0x38) = (long long*)& hook_TRANSFER_MIXED_AUDIO_TO_SOUND_BUFFER;
}

__int64 hook_GET_AUDIO_BUFFER_AND_SET_SIZE(__int64* a1, byte** BUFFER, int size) {
	write_size = size;
	auto return_val = original_GET_AUDIO_BUFFER_AND_SET_SIZE(a1, BUFFER, size);
	buffer_addr = *BUFFER;
	return return_val;
}

bool valid_data(byte* buffer, int size) {
	for (int i = 0; i < size; i++) {
		if (buffer[i] != 0) { return true; }
	}

	return false;
}
void dump_data() {
	byte header[44];
	MilesFillWavHeader((long long)&header, 48000, 2, sound_data_cursor);

	FILE* file;
	fopen_s(&file, "test13.wav", "wb");
	fwrite(header, 1, 44, file);
	fwrite(sound_data, 1, sound_data_cursor, file);
	fclose(file);
	sound_data_cursor = 0;
}
int empty_count = 0;
__int64 hook_TRANSFER_MIXED_AUDIO_TO_SOUND_BUFFER(__int64* a1) {
	if (valid_data(buffer_addr, write_size)) {
		memcpy(sound_data + sound_data_cursor, buffer_addr, write_size);
		sound_data_cursor += write_size;
	}
	else if (sound_data_cursor > 0) {
		if (empty_count == 100) {
			empty_count = 0;
			dump_data();
		}
		else {
			empty_count += 1;
		}
		dump_data();
	}

	return original_TRANSFER_MIXED_AUDIO_TO_SOUND_BUFFER(a1);
}


void setuphooks(__int64* driver) {
	byte* DS_CONTAINER = (byte*)driver + 0x28;
	byte* DIRECTSOUNDCOM = (byte*) *(__int64**)(DS_CONTAINER + 0x20);

	hook_directsoundcom(DIRECTSOUNDCOM);
}


void WINAPI logM(int number, char* message)
{
	std::cout << "Message received: " << message << "\r\n";
}

void print_bus_volumes(__int64 driver) {
	for (int i = 0; i < names_length; i++) {
		__int64 test = MilesProjectGetBus(driver, names[i]);
		if (test) {
			auto vol = MilesBusGetVolumeLevel(test);
			std::cout << "BANK:" << names[i] << " = " << std::setprecision(16) << vol.m128_f32[0] << std::endl;
		}
	}
}

void full_all_buses(__int64 driver) {
	for (int i = 0; i < names_length; i++) {
		__int64 test = MilesProjectGetBus(driver, names[i]);
		if (test) {
			MilesBusSetVolumeLevel(test, 1);
		}
	}
}

void print_source_names(__int64 bank) {
	std::ofstream file;
	file.open("source_names.txt");
	auto srcCount = MilesBankGetSourceCount(bank);
	//cout << "MilesBankGetSourceCount: " << srcCount << std::endl;
	for (int i = 0; i < srcCount; i++) {
		auto meh = MilesBankGetSourceName(bank, i);
		//cout << i << " " << meh << std::endl;
		file << i << " " << meh << std::endl;
	}
	file.close();
}

int main()
{
	int i;
	int startup_parameters = 0;

	MilesSetStartupParameters(&startup_parameters);
	MilesAllocTrack(2);
	__int64 startup = MilesStartup(&logM);
	std::cout << "Start up: " << startup << "\r\n";
	std::cout << "Buffer ptr " << std::hex << &buffer_addr << std::endl;

	auto output = MilesOutputDirectSound();
	unk a1;
	a1.sound_function = (long long*)output;
	//a1.endpointID = new LPCWSTR(L"{0.0.0.00000000}.{9b88f291-a276-47ad-8a75-28fbc49360e9}");
	a1.endpointID = NULL; // Default audio Device 
	a1.maybe_sample_rate = 48000;
	a1.channel_count = 2;
	__int64 driver = MilesDriverCreate((long long*)&a1); // ERROR <--
	MilesDriverRegisterBinkAudio(driver);
	MilesEventSetStreamingCacheLimit(driver, 0x4000000);
	MilesDriverSetMasterVolume(driver, 0.5);
	auto queue = MilesQueueCreate(driver);
	MilesEventInfoQueueEnable(driver);
	setuphooks((long long *)driver);

	__int64 project_load = MilesProjectLoad(driver, "D:\\Miles SS10\\apex data - april 9\\audio.mprj", "english", "audio");

	__int64 status = MilesProjectGetStatus(driver); //5 MB of data - entire file is loaded
	while (status == 0) {
		Sleep(500);
		status = MilesProjectGetStatus(driver);
	}
	status = MilesProjectGetStatus(driver);
	std::cout << "status: " << MilesProjectStatusToString(status) << std::endl;

	__int64 bank = MilesBankLoad(driver, "D:\\Miles SS10\\apex data - april 9\\general.mbnk", "D:\\Miles SS10\\apex data - april 9\\general_stream.mstr", "D:\\Miles SS10\\apex data - april 9\\general_english.mstr", 0); // 136 MB data
	MilesBankPatch(bank, "D:\\Miles SS10\\apex data - april 9\\general_stream_patch_1.mstr", "D:\\Miles SS10\\apex data - april 9\\general_english_patch_1.mstr");

	int bs_ptr = -1;
	int bank_status = MilesBankGetStatus(bank, &bs_ptr);
	while (bank_status == 0) {
		bank_status = MilesBankGetStatus(bank, &bs_ptr);
	}
	std::cout << "bank_status: " << MilesBankStatusToString(bank_status) << std::endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//print_bus_volumes(driver); 

	//MilesBusSetVolumeLevel(bus, 1);
	//MilesBusGetVolumeLevel(bus);


	/* Needed to be *. Miles returns a pointer which is an integer, shouldn't that be fine? */
	
	//__int64 *sample = MilesSampleCreate(driver, bus, 0); // target 

	/*FILE* handle;
	fopen_s(&handle, "D:\\Miles SS10\\apex data - april 9\\general_stream.mstr", "r");
	fseek(handle, 0, SEEK_END);
	long size = ftell(handle);
	fseek(handle, 0, SEEK_SET);
	auto buffer = new char[size];
	fread(buffer, 1, size, handle);
	fclose(handle);*/

	auto events = MilesBankGetEventCount(bank);
	/*cout << "events: " << events << std::endl;
	for (int i = 0; i < 0x100; i++) {
		auto meh = MilesBankGetEventName(bank, i);
		cout << i << " " << meh << std::endl;
	}*/
	//MilesBusGetVolumeLevel(bus);
	struct {
		int fielda;
		int fieldb;
	} out;
	while (true) {

		std::cout << "n: ";
		std::cin >> i;

		if (i >= events) {
			std::cout << "invalid" << std::endl;
			continue;
		}

		if (i == -1) {
			full_all_buses(driver);
			continue;
		}

		if (i == -2) {
			print_bus_volumes(driver);
			continue;
		}

		if (i == -3) {
			MilesQueueEventRunByTemplateId(queue, (int*)& out);
			MilesQueueEventRun(queue, "STOPNOW");
			MilesQueueSubmit(queue);
			continue;
		}
		if (i == -4) {
			continue;
		}

		auto meh = MilesBankGetEventName(bank, i);
		std::cout << meh << std::endl;


		MilesBankGetEventTemplateId(bank, i, (long long*)& out);
		MilesQueueEventVolume(queue, 0.5);
		MilesQueueControllerValue(queue, "GameMusicVolume", 1);
		MilesQueueControllerValue(queue, "LobbyMusicVolume", 1);
		MilesQueueControllerValue(queue, "DialogueVolume", 1);
		MilesQueueControllerValue(queue, "SfxVolume", 1);
		MilesQueueControllerValue(queue, "VoiceCommVolume", 1);
		MilesQueueControllerValue(queue, "1PTailVolume", 1);
		/*MilesQueueEventControllerValue(queue, "GameMusicVolume", 1);
		MilesQueueEventControllerValue(queue, "LobbyMusicVolume", 1);
		MilesQueueEventControllerValue(queue, "SoundscapeFloor", 0);
		MilesQueueEventControllerValue(queue, "ListenerSoundscapeRoof", 0);
		MilesQueueEventInfoMask(queue, 1);*/
		MilesQueueEventRunByTemplateId(queue, (int*)& out);
		//MilesQueueEvent3DPosition(queue, 50000, 500, 0.2);
		MilesQueueSubmit(queue);

		//MilesQueueEventFilterId(queue, 0x3);
		//MilesQueueEventRun(queue, "RESUME"); // RESUME, STOP, STOPNOW
	}
	

	//bool set_sample_result = MilesSampleSetSourceRaw(sample, (long long)buffer+0x20, 0, 0x2B11LL, 1); // target
	/*bool set_sample_result = MilesSampleSetSource(sample, (long long)buffer + 0x20, 0x69, 2); // target
	cout << "set sample result: " << set_sample_result << endl;
	MilesSamplePlay(sample); // target*/

	
	//delete[] buffer;
	
	std::cin >> i;
	return 0;
}

