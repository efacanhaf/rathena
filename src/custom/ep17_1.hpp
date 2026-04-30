// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// "feels like Episode 17.1" content lockdown — DimensionsRO
//
// Master toggle is `EP17_1_LOCK` (defined in src/custom/defines_pre.hpp).
// When undefined, all functions in this header are no-ops at the call sites
// thanks to the surrounding #ifdef guards in pc.cpp / mob.cpp.
//
// Five fronts, each with its own sub-flag so individual content categories
// can be re-enabled without rebuilding everything:
//   - EP17_1_DISABLE_4TH_JOBS    - block jobchange to 4th-class jobs
//   - EP17_1_DISABLE_POST_MAPS   - block entry to maps released after Ep 17.1
//   - EP17_1_DISABLE_POST_MOBS   - block spawning of mobs added after Ep 17.1
//   - EP17_1_DISABLE_POST_ITEMS  - block use of items added after Ep 17.1
//   - EP17_1_DISABLE_POST_QUESTS - script-side; toggled in npc/scripts_main.conf
//                                  (no C++ symbol needed)
//
// To re-enable everything: comment out `#define EP17_1_LOCK` in defines_pre.hpp
// and rebuild. To re-enable a specific front: comment out only its sub-flag
// below.
//
// To extend the locked content lists, edit the unordered_sets inside this
// header (search for "TODO: populate"). This is header-only so no project
// file changes are needed.

#ifndef CONFIG_CUSTOM_EP17_1_HPP
#define CONFIG_CUSTOM_EP17_1_HPP

// Toggles (EP17_1_LOCK + EP17_1_DISABLE_*) live in src/custom/defines_pre.hpp.
// This header carries only the runtime lookups — pulled from pc.cpp / mob.cpp
// where it is safe to include common/mmo.hpp.

#if defined(EP17_1_DISABLE_4TH_JOBS) || defined(EP17_1_DISABLE_POST_MAPS) || \
    defined(EP17_1_DISABLE_POST_MOBS) || defined(EP17_1_DISABLE_POST_ITEMS)

#include <cctype>
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_set>

#include "../common/mmo.hpp"

namespace ep17_1 {

// -----------------------------------------------------------------------
// Front 1 — 4th class job IDs (Episode 17.2 / Sage of Magic onwards).
// rAthena groups them in three blocks in mmo.hpp:
//   4252-4264  primary 4th jobs (Dragon Knight..Trouvere)
//   4278-4283  mounted/peco variants (Windhawk2, Meister2, ...)
//   4302-4316  expanded 4th (Sky Emperor, Soul Ascetic, Hyper Novice, ...)
// We block every id in those windows; any class outside is allowed.
// -----------------------------------------------------------------------

inline bool is_post_ep17_1_job(int32_t class_) {
    if (class_ >= JOB_DRAGON_KNIGHT && class_ <= JOB_TROUVERE)        return true;
    if (class_ >= JOB_WINDHAWK2 && class_ <= JOB_IMPERIAL_GUARD2)     return true;
    if (class_ >= JOB_SKY_EMPEROR && class_ <= JOB_SKY_EMPEROR2)      return true;
    return false;
}

// -----------------------------------------------------------------------
// Front 2 — locked maps. Names are lowercase, no extension. Add maps
// released after Ep 17.1 (Sage of Magic = Ep 17.2 dungeons, Tirfing = Ep 18,
// Wandering = Ep 19, Iceflow = Ep 20, Demonio = Ep 21, plus instances).
// -----------------------------------------------------------------------

inline const std::unordered_set<std::string> &locked_maps_() {
    static const std::unordered_set<std::string> s = {
        // --- Ep 17.2 Sage of Magic
        "1@hht",        // Heart Hunter War Base
        "1@gef_in",     // Geffen Magic Tournament (4th-job related)
        // --- Ep 18 Tirfing
        "tf_in01",
        "tf_fild01",
        "tf_fild02",
        // --- Ep 19 Wandering Minstrel / Adelheid
        "wand_fild01",
        "1@wand",
        // --- Ep 20 Iceflow
        "ice_dun01",
        "ice_dun02",
        "1@ice",
        // --- Ep 21 Demonio
        "dem_fild01",
        "1@dem",
        // TODO: extend with the full kRO post-17.1 map list as needed.
    };
    return s;
}

inline bool is_post_ep17_1_map(std::string_view map_name) {
    std::string n(map_name);
    for (char &c : n) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    auto dot = n.find('.');
    if (dot != std::string::npos) n.resize(dot);
    return locked_maps_().find(n) != locked_maps_().end();
}

// -----------------------------------------------------------------------
// Front 3 — locked mob IDs. Mobs added in Ep 17.2+ (Heart Hunter base
// bosses, 4th-job test mobs, Tirfing/Wandering/Iceflow/Demonio creatures).
// -----------------------------------------------------------------------

inline const std::unordered_set<int32_t> &locked_mobs_() {
    static const std::unordered_set<int32_t> s = {
        // TODO: populate from post-17.1 mob_db diff.
        // Example placeholders (replace with real ids once curated):
        // 3795, 3796,
    };
    return s;
}

inline bool is_post_ep17_1_mob(int32_t mob_id) {
    return locked_mobs_().find(mob_id) != locked_mobs_().end();
}

// -----------------------------------------------------------------------
// Front 4 — locked item IDs. Mostly 4th-job equipment, illusion-armour
// upgrades, post-17.1 cards and quest reward gear.
// -----------------------------------------------------------------------

inline const std::unordered_set<int32_t> &locked_items_() {
    static const std::unordered_set<int32_t> s = {
        // TODO: populate from post-17.1 item_db diff.
    };
    return s;
}

inline bool is_post_ep17_1_item(int32_t nameid) {
    return locked_items_().find(nameid) != locked_items_().end();
}

} // namespace ep17_1

#endif // any sub-flag enabled

#endif // CONFIG_CUSTOM_EP17_1_HPP
