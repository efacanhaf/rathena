// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef CONFIG_CUSTOM_DEFINES_POST_HPP
#define CONFIG_CUSTOM_DEFINES_POST_HPP

/**
 * rAthena configuration file (http://rathena.org)
 * For detailed guidance on these check http://rathena.org/wiki/SRC/config/
 **/

// DimensionsRO: disable per-packet XOR obfuscation in map-server.
// PACKETVER 20250604 enables it by default in src/config/packets.hpp;
// our WARP-patched Ragexe doesn't carry the matching keys (no CryptKeysGen),
// so the server side decoding produces noise. Disabling avoids the mismatch
// and shaves an XOR pass per packet (negligible CPU but cleaner pipeline).
#undef PACKET_OBFUSCATION

#endif /* CONFIG_CUSTOM_DEFINES_POST_HPP */
