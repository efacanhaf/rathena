// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom Script Commands
 * Place the body of custom script commands in this file.
 * Format:
 *	BUILDIN_FUNC(command_name)
 *	{
 *		<code>;
 *	}
 **/

//BUILDIN_FUNC(example)
//{
//	ShowInfo("buildin_example: It works!\n");
//	script_pushint(st,1);
//	return 0;
//}

// slide <x>,<y>; — moves attached player to (x,y) on current map without fade.
// Wraps unit_movepos. Returns 1 on success, 0 on failure.
BUILDIN_FUNC(slide)
{
	map_session_data *sd = map_id2sd(st->rid);
	if (sd == nullptr) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}
	int16 x = (int16)script_getnum(st, 2);
	int16 y = (int16)script_getnum(st, 3);
	script_pushint(st, unit_movepos(sd, x, y, 0, false) ? 1 : 0);
	return SCRIPT_CMD_SUCCESS;
}

// setnpcwalkthrough <flag>; — toggles current NPC's walk_through flag,
// which excludes it from cell stack-limit counts (players can stand on
// the same tile). Call from OnInit on the master script — duplicates
// inherit the OnInit and each gets its own flag set.
BUILDIN_FUNC(setnpcwalkthrough)
{
	npc_data *nd = map_id2nd(st->oid);
	if (nd == nullptr) {
		script_pushint(st, 0);
		return SCRIPT_CMD_SUCCESS;
	}
	nd->walk_through = (script_getnum(st, 2) != 0);
	script_pushint(st, 1);
	return SCRIPT_CMD_SUCCESS;
}

// ep17_feelslike() — returns 1 if the server is configured as a "Ep 17.1
// feels-like" build (post-17.1 maps/items/jobs blocked). Useful for scripts
// that want to relocate or disable NPCs depending on the canonical episode.
BUILDIN_FUNC(ep17_feelslike)
{
#ifdef EP17_1_DISABLE_POST_MAPS
	script_pushint(st, 1);
#else
	script_pushint(st, 0);
#endif
	return SCRIPT_CMD_SUCCESS;
}
