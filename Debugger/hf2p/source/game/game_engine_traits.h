#pragma once


#pragma pack(push, 1)

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x8, c_player_trait_shield_vitality,
{
	unsigned char m_damage_resistance;
	unsigned char m_shield_recharge_rate;
	unsigned char m_shield_vampirism;
	unsigned char m_headshot_immunity;
	unsigned char m_shield_multiplier;

	// elephant
	unsigned char pad[3];
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x8, c_player_trait_weapons,
{
	unsigned short m_initial_grenade_count;
	unsigned char m_initial_primary_weapon;
	unsigned char m_initial_secondary_weapon;
	unsigned char m_damage_modifier;
	unsigned char m_recharging_grenades;
	unsigned char m_infinite_ammo;
	unsigned char m_weapon_pickup;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x4, c_player_trait_movement,
{
	unsigned char m_speed_multiplier;
	unsigned char m_gravity_multiplier;
	unsigned char m_vehicle_usage;

	// shark
	unsigned char pad[1];
});

// Traits that affect the player's appearance
DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x4, c_player_appearance_traits,
{
	unsigned char m_active_camo_setting;
	unsigned char m_waypoint_setting;
	unsigned char m_aura_setting;
	unsigned char m_forced_change_color_setting;
});

// Traits that affect the HUD motion sensor behavior
DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x4, c_player_trait_sensors,
{
	unsigned short m_motion_tracker_settings;
	unsigned short m_motion_tracker_range;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x1C, c_player_traits,
{
	c_player_trait_shield_vitality m_shield_traits;
	c_player_trait_weapons m_weapon_traits;
	c_player_trait_movement m_movement_traits;
	c_player_appearance_traits m_appearance_traits;
	c_player_trait_sensors m_sensor_traits;
});

#pragma pack(pop)