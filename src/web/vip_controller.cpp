// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// Authenticated GET /vips endpoint.
//
// Cross-references the `login` table's `vip_time` (UNIX timestamp; > now()
// means VIP active) with the registration bot's `account_oauth` link table
// to surface only the *Discord* identifiers of currently-active VIPs. The
// Kaori bot polls this every 12h to reconcile the VIP role on the guild.
//
// Auth: shared secret in `X-API-Key` header, configured via `vip_api_key`
// in web_athena.conf and stored on the bot side in
// .dro-secrets/discord-kaori-bot.env. Plaintext over HTTP is acceptable
// here because the bot lives on the same VM as the web-server (loopback)
// in production; in dev you can tunnel.

#include "vip_controller.hpp"

#include <nlohmann/json.hpp>

#include <common/sql.hpp>

#include "http.hpp"
#include "sqllock.hpp"
#include "web.hpp"

HANDLER_FUNC(vip_get) {
	if (web_config.vip_api_key.empty()) {
		res.status = 503;
		res.set_content("{\"error\":\"vip_api_key not configured\"}", "application/json");
		return;
	}
	auto provided = req.get_header_value("X-API-Key");
	if (provided != web_config.vip_api_key) {
		res.status = 401;
		res.set_content("{\"error\":\"unauthorized\"}", "application/json");
		return;
	}

	nlohmann::json discord_ids = nlohmann::json::array();
	SQLLock sl(LOGIN_SQL_LOCK);
	sl.lock();
	auto handle = sl.getHandle();
	if (SQL_SUCCESS == Sql_Query(handle,
			"SELECT DISTINCT o.discord_id "
			"FROM `login` l "
			"JOIN `account_oauth` o ON o.account_id = l.account_id "
			"WHERE l.vip_time > UNIX_TIMESTAMP()")) {
		while (SQL_SUCCESS == Sql_NextRow(handle)) {
			char *data = nullptr;
			size_t len = 0;
			if (SQL_SUCCESS == Sql_GetData(handle, 0, &data, &len) && data != nullptr) {
				// discord_id is BIGINT UNSIGNED; serialize as string to avoid
				// JSON number precision loss on the consumer side.
				discord_ids.push_back(std::string(data, len));
			}
		}
		Sql_FreeResult(handle);
	}
	sl.unlock();

	nlohmann::json out = { { "discord_ids", discord_ids } };
	res.set_content(out.dump(), "application/json");
}
