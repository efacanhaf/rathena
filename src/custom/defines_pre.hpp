// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef CONFIG_CUSTOM_DEFINES_PRE_HPP
#define CONFIG_CUSTOM_DEFINES_PRE_HPP

/**
 * rAthena configuration file (http://rathena.org)
 * For detailed guidance on these check http://rathena.org/wiki/SRC/config/
 **/

#define PACKETVER 20250604

// "feels like Episode 17.1" content lockdown (DimensionsRO).
// Master toggle — comment the next line to fully disable the lockdown.
// Each sub-flag below can also be commented individually to re-enable a
// single front (4th jobs / maps / mobs / items / quests).
// The runtime helpers (lists + lookup functions) live in src/custom/ep17_1.hpp
// and are pulled only by the .cpp files that need them, so this header stays
// macro-only and safe to include from defines_pre.hpp.
#define EP17_1_LOCK
#ifdef EP17_1_LOCK
  #define EP17_1_DISABLE_4TH_JOBS
  #define EP17_1_DISABLE_POST_MAPS
  #define EP17_1_DISABLE_POST_MOBS
  #define EP17_1_DISABLE_POST_ITEMS
  #define EP17_1_DISABLE_POST_QUESTS
#endif

#endif /* CONFIG_CUSTOM_DEFINES_PRE_HPP */
