// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// Public unauthenticated GET /status endpoint.
// Responds with JSON: {"online": true, "players": <count>}.
//
// Reaching this endpoint at all means the web-server is running, which
// implies the rest of the rathena process tree is alive (web-server is
// launched by the same supervisor as login/char/map). The launcher uses
// this to flip its ONLINE/OFFLINE indicator.

#include "status_controller.hpp"

#include <nlohmann/json.hpp>

#include <common/sql.hpp>

#include "http.hpp"
#include "sqllock.hpp"
#include "web.hpp"

HANDLER_FUNC(status_get) {
	int32 player_count = 0;

	SQLLock sl(CHAR_SQL_LOCK);
	sl.lock();
	auto handle = sl.getHandle();
	if (SQL_SUCCESS == Sql_Query(handle, "SELECT COUNT(*) FROM `char` WHERE `online` = 1")) {
		if (SQL_SUCCESS == Sql_NextRow(handle)) {
			char *data = nullptr;
			size_t len = 0;
			if (SQL_SUCCESS == Sql_GetData(handle, 0, &data, &len) && data != nullptr) {
				player_count = std::atoi(data);
			}
		}
		Sql_FreeResult(handle);
	}
	sl.unlock();

	nlohmann::json out = {
		{ "online", true },
		{ "players", player_count },
	};
	res.set_header("Access-Control-Allow-Origin", "*");
	res.set_content(out.dump(), "application/json");
}
