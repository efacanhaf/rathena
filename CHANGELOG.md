# DimensionsRO — Changelog

All notable changes to the DimensionsRO server (rAthena fork) are tracked here.

Format inspired by [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).
Sections: **Added**, **Changed**, **Fixed**, **Removed**, **Deprecated**, **Security**.

---

## [Unreleased] — develop branch (not yet in production)

Changes accumulated on `develop` since last `main` release. When merging
`develop → main`, move this whole block under a new versioned heading
(e.g. `## [1.1.0] — 2026-05-XX`) and start a fresh `[Unreleased]` block.

### Added

- **Phase 5 — mid-rate baseline drop & exp config** (`conf/import-tmpl/battle_conf.txt`):
  EXP 5x base/job, 3x quest. Equip 5x normal / 10x boss / 15x MVP.
  Cards 3x / 2x MVP. Common/Heal/Use 5x. Pace ~30-40h casual to max 3rd class;
  events (weekend +5x, anniversary +10x) lift effective rate to 10-20x.
- **Phase 3 — buffs to 9 dead 3rd-class PvE skills** (`db/import-tmpl/skill_damage_db.txt`):
  LG_BANISHINGPOINT, LG_EARTHDRIVE, NC_BOOSTKNUCKLE (+50%);
  NC_ARMSCANNON, SR_KNUCKLEARROW, WL_CHAINLIGHTNING_ATK, KO_BAKURETSU (+30%);
  SC_TRIANGLESHOT (+50%); PR_MAGNUS / Magnus Exorcismus (+75%, Adoramus rescue).
  PvE only (vs_players unchanged), normal maps.
- **Episode 17.1 content lockdown** (`src/custom/ep17_1.hpp`,
  `npc/re/scripts_post_ep17_1.conf`): server stops at Ep 17.1 — no 4th classes,
  no post-17.1 quests/instances/items.
- **Web `GET /status` endpoint** (`src/web/status_controller.cpp`):
  returns `{online: bool, players: int}` for launcher integration.
- **CI: build rAthena binaries on Oracle Linux 9 x86_64** (`.github/workflows/build_dimensionsro_prod.yml`).
- **Dev environment isolation:** ports `72xx` instead of `62xx`,
  `PACKET_OBFUSCATION` off, separate `ragnarok_dev` DB.

### Changed

- **Server-side input lag tuning** (phase 2): adjusted `min_skill_delay_limit`,
  `default_walk_delay`, etc. for snappier feel.
- **Phase 4 — PvE damage caps on 5 dominant 3rd-class skills** (`db/import-tmpl/skill_damage_db.txt`):
  - `GN_CARTCANNON` -15% — caps Genetic farming spam
  - `WL_COMET` -20% — controls AoE one-shot (~4500% → ~3600%)
  - `SR_TIGERCANNON` -25% — Sura output cap (formula already non-HP%-based in modern rAthena)
  - `RA_ARROWSTORM` -15% — caps White Wing Set farming loop
  - `AB_ADORAMUS` -15% — differentiates from buffed Magnus (Phase 3)
  All PvE-only (vs_players=0).

### Draft (not yet enabled)

- **Phase 5.2 — Adventurer Starter Kit NPC** (`npc/custom/dro_starter_kit.txt`):
  vendor at `prontera 156,200` selling baseline 3rd-class kit (class-specific
  Scarlet weapon +9 + Diabolus armor set +7 + Variant Shoes +7 + 2× Brisingamen)
  for 500k zeny, once per character. All item IDs confirmed against rAthena DB
  (Scarlet series, ids 1498/1839/13127/13327/15313/16040/18130/19550/20844/21015/28007/28102/28106/28705/29287/31925/2630). NOT enabled in
  `scripts_custom.conf` until first GM playtest. Doram (Summoner) falls back to
  Scarlet Mace pending Doram-specific weapon decision.

### Fixed

- _(none yet on this Unreleased)_

---

## [1.0.0] — 2026-04-26 (production v1)

Initial DimensionsRO production server. Baseline rAthena upstream + custom changes:

### Added

- DimensionsRO client v1.0 (Vanilla/HD mode swap, custom launcher with
  GitHub patcher, Oracle Cloud login at `163.176.144.14:6266`)
- Server-side input lag reduction phase 1
- (Other Phase 1 customizations from initial fork — see git history before
  this changelog was introduced)

---

## How to use this changelog

1. **Every commit on `develop`** that introduces user-visible behavior
   (gameplay, balance, quest, instance, drop rate, exp, GM commands,
   client behavior) MUST add an entry under `[Unreleased]`.
2. **Internal changes** (refactors, build fixes that don't change behavior,
   doc-only) can skip the changelog.
3. **At merge time `develop → main`**, the `[Unreleased]` block becomes
   a new versioned heading. Bump the version per [SemVer](https://semver.org/):
   - **MAJOR** (1.x.x → 2.0.0): breaking changes (e.g. wipe, char migration)
   - **MINOR** (1.0.x → 1.1.0): new features (new instances, classes, systems)
   - **PATCH** (1.0.0 → 1.0.1): fixes and balance tweaks
4. **Production release** = the `main` branch tag matching that version.
   Player-facing changelog (Discord announcement) derived from this file.

---

_Maintained by the DimensionsRO team. Last updated: 2026-04-30._
