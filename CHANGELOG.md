# DimensionsRO — Changelog

All notable changes to the DimensionsRO server (rAthena fork) are tracked here.

Format inspired by [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

---

## [1.0.0] — 2026-04-30

Release inicial consolidado da DimensionsRO. Esta versão agrega todas as
modificações desde o fork do upstream rAthena (`master` original) até o estado
atual em `develop`. Após este ponto, mudanças seguem o ciclo normal `develop →
main` com versionamento incremental (1.0.x patches, 1.x.0 features).

### Stylist (@stylist) — kRO 2024+ Compatibility

Cliente kRO Aug/2025 introduziu novo packet de Apply (`0x0bf7`) que rAthena
upstream ainda não parsea. Fix consome o packet, reproduz a UI sem cobrar por
categorias não alteradas e suporta o Costume Change.

- **Packet `CZ_REQ_STYLE_CHANGE3` (0xbf7) registrado** (`src/map/packets.hpp`,
  `src/map/clif_packetdb.hpp`): variable-length, layout `count + records[count]`
  de 8 bytes (`[category, _, value, _]`). Resolve o disconnect ao apertar Apply
  na janela do estilista em clientes 2024+.
- **Mapa de categorias kRO → rAthena**: 0=Hair_Color (palette), 1=Hair (style),
  2=Clothes_Color (palette), 3=Head_Top, 4=Head_Mid, 5=Head_Bottom, 9=Body2.
  Paletas (0/1/2) usam lookup por `Value`; acessórios (3/4/5) usam lookup por
  `Index` no DB.
- **Atomic 2-pass dispatch** (`clif.cpp:clif_parse_stylist_buy`): Pass 1 valida
  zeny/itens de todas categorias em dry-run; Pass 2 commita. Evita gastar zeny/
  cupom quando uma categoria posterior falha (a UI do kRO 2024+ envia o estado
  atual de todas categorias junto, então sem isso o jogador era cobrado por
  estilos que não pediu).
- **No-op skip**: categorias cuja `Value` já é a atual (`status.hair`,
  `hair_color`, `clothes_color`, `body`) retornam sem cobrar. Necessário pelo
  comportamento do cliente que sempre revalida tudo no Apply.
- **Costume Change (toggle de model alternativo, kRO category 9)**: para 3rd
  classes não-4th, alterna entre `status.class_` (default) e
  `alternate_outfits[0]` do `job_db.find()` (ex.: GC_T 4065 ↔
  JOB_GUILLOTINE_CROSS_2ND 4334). Consome 1 `Costume_Ticket` (id 6959) só ao
  trocar para o alternativo — voltar pra default é grátis.
- **Clothes_Color Value 1 adicionada ao DB** (`db/re/stylist.yml`): paleta 1
  estava ausente entre Value 0 e Value 2; cliente kRO aceita 0..6 mas a entrada
  intermediária faltava. Custo igual aos outros (`Clothing_Dye_Coupon`).
- **`max_cloth_color: 7 → 6`** (`conf/battle/client.conf`): valor é o
  índice máximo, não a contagem; range correto é 0..6 (7 paletas totais).

### NPCs (Player-Facing)

- **Job Master NPC ativado** em `prontera` (`npc/custom/jobmaster.txt`): NPC
  oficial rAthena, posicionado no spot do Valerie. Bloqueia advancement pra 4th
  jobs via runtime hook.
- **Eden Group iRO VIP NPCs habilitados** em `moc_para01`
  (`npc/re/quests/eden/eden_iro.txt`): npcs auxiliares (refine helpers, board
  proxies) que vinham comentados.
- **Adventurer Starter Kit reescrito** (`npc/custom/dro_starter_kit.txt`): 4
  tiers (1st/2nd/Trans/3rd), gear vanilla apropriado por classe (sem Booster
  endgame), validação de equipabilidade por classe, gating per-tier via flags
  (`DRO_STARTER_T1`/`T2`/`T3`/`TAKEN`) — cada tier só pode ser claimado uma
  vez. Sem consumíveis no kit.

### Infra

- **Map-server isolado em segunda instância Oracle** (136.248.101.89): login +
  char + DB ficam na primeira (163.176.144.14); map sobe na segunda e
  conversa com a DB via SSH tunnel (`/usr/local/bin/mysql-tunnel.sh`,
  systemd-managed) pelo VCN privado. Reduz contenção CPU em surtos de map-side.

### Server Identity & Scope

- **Episode 17.1 content lockdown** (`src/custom/ep17_1.hpp`,
  `npc/re/scripts_post_ep17_1.conf`): server trava em Ep 17.1. Sem 4th classes,
  sem quests/instances/items pós-17.1. `pc_jobchange` bloqueia advancement pra
  classes 4th com mensagem in-game.
- **3rd class caps travados pra match iRO oficial 17.1** (`db/re/job_exp.yml`,
  `conf/import-tmpl/battle_conf.txt`):
  - Base Level max: 175 (era 200 default rathena)
  - Job Level max: 60 (era 70 default)
  - Stat max: 120 (era 130, default rathena assumia progressão pra 4th)

### Rates & Drop Configuration

- **Mid-rate baseline** (`conf/import-tmpl/battle_conf.txt`):
  - EXP: 5x base / 5x job / 3x quest
  - Equip: 5x normal / 10x boss / 15x MVP
  - Cards: 3x normal / 3x boss / 2x MVP
  - Common/Heal/Use: 5x
  - MVP-specific drops (Old Card Album etc): 3x
  - Pace: ~30-40h casual to max 3rd class. Eventos (weekend +5x, anniversary
    +10x) elevam efetivo pra 10-20x para sessões curtas.

### Skill Rebalance — PvE Only (vs_players=0)

Estratégia tiered: 3rds são fracas pro conteúdo de 17.1, todas skills sobem,
escalado pela "fraqueza original". Aplicado via `db/import-tmpl/skill_damage_db.txt`.

**Tier C — skills "dead" (buff alto, +45% a +90%):**

| Skill | Buff | Classe |
|---|---|---|
| LG_BANISHINGPOINT | +65% | Royal Guard |
| LG_EARTHDRIVE | +65% | Royal Guard |
| NC_BOOSTKNUCKLE | +65% | Mechanic non-Mado |
| NC_ARMSCANNON | +45% | Mechanic |
| SR_KNUCKLEARROW | +45% | Sura |
| SC_TRIANGLESHOT | +65% | Shadow Chaser |
| WL_CHAINLIGHTNING_ATK | +45% | Warlock |
| PR_MAGNUS | +90% | Arch Bishop / Priest |
| KO_BAKURETSU | +45% | Kagerou |

**Tier A — main skills sub-tier (buff moderado, +30%):**

| Skill | Classe |
|---|---|
| LG_HESPERUSLIT | Royal Guard |
| RK_DRAGONBREATH | Rune Knight |
| RK_DRAGONBREATH_WATER | Rune Knight |
| NC_VULCANARM | Mechanic |
| NC_AXEBOOMERANG | Mechanic |
| SR_RAMPAGEBLASTER | Sura |
| GN_DEMONIC_FIRE | Genetic |
| AB_JUDEX | Arch Bishop |
| SC_FATALMENACE | Shadow Chaser |
| WL_TETRAVORTEX | Warlock |
| WL_JACKFROST | Warlock |
| RA_AIMEDBOLT | Ranger |

**Tier S — top-tier farming/burst skills (buff suave, +15%):**

| Skill | Classe |
|---|---|
| GN_CARTCANNON | Genetic |
| WL_COMET | Warlock |
| SR_TIGERCANNON | Sura |
| RA_ARROWSTORM | Ranger |
| AB_ADORAMUS | Arch Bishop |

PvP intacto — todas adjustments têm `vs_players=0`.

### Defensive Skill Buffs (source-level, requer recompile)

Skills defensivas universais buffadas pra player taxa de sobrevivência vs
conteúdo de 17.1. Aplicado em `src/map/battle.cpp`, `src/map/status.cpp` e
`db/re/skill_db.yml`.

- **HP_ASSUMPTIO**: agora dá -50% damage taken em PvE renewal (era só DEF
  buff). Mantém scaling por skill level. PvP fica em -33%.
- **MG_ENERGYCOAT**: damage reduction 6%/12%/18%/24%/30% → 10%/20%/30%/40%/50%
  por SP interval. SP cost por hit reduzido pela metade (1%+0.5% → 0.5%+0.3%).
- **LK_TENSIONRELAX**: HP regen rate 200 → 400. Adicionado SP regen 100 (skill
  agora é viável pra solo farm).
- **PR_MAGNIFICAT**: SP regen multiplier 100 → 150 (regen 2.5x base). Duração
  estendida 30-90s → 60-180s.
- **AL_BLESSING**: duração estendida x1.5 (lvl 10: 240s → 360s). Beneficia
  todas classes da Acolyte tree.
- **LG_KINGS_GRACE**: duração 5s → 8s. Cooldown reduzido pela metade
  (100s → 50s lvl 1, 60s → 30s lvl 5).
- **SC_AUTOGUARD**: block rate scaling +30% (lvl 10: 30% → 40%).
- **GC_HALLUCINATIONWALK**: physical Flee 50/lvl → 100/lvl, magic Flee
  10/lvl → 20/lvl.
- **SC__SHADOWFORM**: duração +30s em todos lvls (30-70s → 60-100s).
- **MI_RUSH_WINDMILL**: duração 3min → 4min.

**Wave 2 — defensivas avançadas:**

- **LK_PARRYING**: block chance scaling 20+3/lvl → 30+5/lvl (lvl 5: 35% → 55%).
- **CR_DEFENDER**: penalidade de aspd reduzida pela metade
  (val4 250-50/lvl → 125-25/lvl). Penalidade de movimento aliviada
  (`max(speed,200)` → `max(speed,175)`). Damage reduction (vs ranged weapon)
  inalterada (5+15/lvl, max 80%).
- **RK_MILLENNIUMSHIELD**: HP por shield 1000 → 2500. Escala melhor com
  conteúdo endgame onde MVPs batem >1k por hit.
- **SO_FIRE/WATER/WIND/EARTH_INSIGNIA**: cada insígnia agora dá -25% de
  resistência ao dano do elemento correspondente (somado ao bonus já existente
  em battle.cpp para dano de elemento oposto).
- **SR_GENTLETOUCH_REVITALIZE**: scaling buffado em todos os val:
  - MaxHP %: 2/lvl → 4/lvl (max 10% → 20%)
  - HP regen: 30/lvl+50 → 40/lvl+100 (lvl 5: 200% → 300%)
  - STAT DEF: 20/lvl → 30/lvl (max 100 → 150)
  - Duração: 240s → 360s (4 → 6 min)
- **GN_MANDRAGORA**: rate base 25+10/lvl → 35+12/lvl (max ~95% antes de
  mitigação por VIT/LUK). Duração 10-30s → 15-35s. Skill agora viável pra
  controle de cast em mobs/MVPs com mais consistência.
- **RA_CAMOUFLAGE**: removida a penalidade de DEF/DEF2 que crescia 5%/segundo
  (`status.cpp:7825-7826` e `:7909-7910`). Removido o aumento de 5%/seg de
  dano ranged recebido (`battle.cpp:4763-4769`). Bonus ofensivos (ATK +30/seg
  até 300, CRIT +100/seg até 1000) preservados — Camouflage agora é puro
  buff de prep.
- **NC_SHAPESHIFT**: duração 5min → 10min. Reduz o overhead de re-cast em
  Mado long-runs.
- **WA_MOONLIT_SERENADE / MI_RUSH_WINDMILL**: agora adicionam DEF/MDEF flat
  (10 por skill level, em ambos os lados). Antes só ATK/MATK; agora também
  endurecem a party.

### Server-Side Tuning

- **Input lag tuning Phase 1 + 2** (`conf/battle/battle.conf`,
  `conf/battle/client.conf`, `conf/battle/skill.conf`):
  - `damage_walk_delay_rate: 100 → 80` — recovery mais snappy após hits
  - `snap_dodge: yes` — snap skills (Asura/Body Relocation) dodge corretamente
  - `max_walk_path: 17 → 25` — menos pathfind recalcs em click-walks longos
  - `multihit_delay: 200 → 100` — menos stunlock pós multi-hit
  - `player_damage_delay_rate: 100 → 50` — walk-delay menor após dano
  - `default_walk_delay: 300 → 200` — "can't walk" reduzido pós-skill
- **`nc_madogear_no_fuel` battle config** (`src/map/battle.{cpp,hpp}`,
  `src/map/skill.cpp`, `conf/battle/skill.conf`): nova flag server-wide pra
  desabilitar consumo de Magic_Gear_Fuel. Default `no` (vanilla). Quando `yes`,
  remove o burden de fuel-management do Mechanic/Mado sem quebrar item bonuses
  per-player. Reusa infra `sd->special_state.no_mado_fuel`.

### Web / Infra

- **Web `GET /status` endpoint** (`src/web/status_controller.cpp`): retorna
  `{online: bool, players: int}` pro launcher consumir status do server.
- **CI: build automatizado** (`.github/workflows/build_dimensionsro_prod.yml`):
  builda binários rAthena no Oracle Linux 9 x86_64 pra prod deployment.
- **Dev environment isolation**: ports `72xx` (vs prod `62xx`),
  `PACKET_OBFUSCATION` off em map-server dev, DB `ragnarok_dev` separado.

### Decisões Estratégicas (não-implementadas, registradas)

- **Phase 4 nerfs revertidos**: estratégia inicial de cap nas top-tier skills
  (CartCannon -15%, Comet -20%, TigerCannon -25%, ArrowStorm -15%, Adoramus
  -15%) foi revertida. 3rds são fracas pro conteúdo de 17.1; cortar topo
  agravaria. Substituído pela strategy "all-buff progressivo" descrita em
  Skill Rebalance acima.
- **Phase 5.3 mob HP overrides**: scaffold pra reduzir HP/DEF de mobs
  Ep 17.1 (FACEWORM, TIMEHOLDER, HEART_HUNTER) abandonado. Decisão: nunca
  tocar em mobs do conteúdo, ajustes ficam só do lado do player.
- **Phase 6.1 Royal Guard Banding refactor**: deferred. Requer rewrite de
  `skill_check_pc_partner` semantics, risk de quebrar outras Royal Guard
  skills (Hesperus Lit, Inspiration, Banishing Buster) sem playtest. Phase 3
  buffs já endereçam parte do solo-pain do RG.
- **Phase 6.2 Shadow Chaser Reproduce**: investigado, OBSOLETO. Modern rAthena
  (`skill.cpp:801`) não tem o "lock" que a comunidade reportava em clientes
  antigos. Reproduce já copia continuamente skills do target.

### Client (DimensionsRO 1.0)

(Tracked em repo separado do client; resumo aqui)

- Vanilla/HD mode swap funcional
- Custom launcher com GitHub patcher
- Login point: Oracle Cloud `163.176.144.14:6266`
- Server name: DimensionsRO
- IP de prod escrito em `server.grf/data/clientinfo.xml`

---

## How to use this changelog

1. **Cada commit em `develop`** que introduz user-visible behavior (gameplay,
   balance, quest, instance, drop rate, exp, GM commands, client behavior)
   adiciona entry sob `[Unreleased]`.
2. **Internal changes** (refactors, build fixes que não mudam comportamento,
   doc-only) podem skipar o changelog.
3. **No merge `develop → main`**, bloco `[Unreleased]` vira nova versão.
   Bump por [SemVer](https://semver.org/):
   - **MAJOR** (1.x.x → 2.0.0): breaking (ex: wipe, char migration)
   - **MINOR** (1.0.x → 1.1.0): novas features (instances, classes, sistemas)
   - **PATCH** (1.0.0 → 1.0.1): fixes e balance tweaks
4. **Production release** = tag em `main` com a versão. Player-facing
   changelog (Discord announcement) derivado deste arquivo.

---

_Maintained by the DimensionsRO team. Last updated: 2026-05-02._
