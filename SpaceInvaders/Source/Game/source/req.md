# GAME_FEATURES_SPEC

// G1 - G10 done 

## G1_PLAYER_MOVEMENT [DONE]

- Entity: `Player`
- Position:
  - `y` fixed at bottom of playfield.
  - `x` ∈ [PLAYFIELD_MIN_X, PLAYFIELD_MAX_X].
- Input:
  - `Left` key → negative horizontal velocity.
  - `Right` key → positive horizontal velocity.
- Constraint:
  - Movement logic MUST clamp `x` to valid range each update.

---

## G2_PLAYER_FIRE_BASIC [DONE]

- Entity: `PlayerShot` (projectile owned by player).
- Behaviour:
  - On fire input, spawn new projectile with upward velocity.
  - Projectile moves strictly in `-y` direction.
  - When projectile leaves playfield bounds, it is destroyed and no longer blocks new shots.

---

## G3_PLAYER_FIRE_LIMIT ]

- Max active `PlayerShot` count: 3.
- Additional constraint:
  - `reload_timer` per player:
    - Fire command is ignored while `reload_timer > 0`.
    - After each valid shot, set `reload_timer = RELOAD_TIME`.
    - Decrease `reload_timer` by `dt` each update.

---

## G4_ENEMY_KILLABLE

- Entities: `Enemy`, `PlayerShot`.
- On collision `PlayerShot` vs `Enemy`:
  - `Enemy` transitions to dead state and is removed from active enemy list.
  - `PlayerShot` is destroyed.
  - No dangling references; memory must be reclaimed.

---

## G5_SCORE_BASIC

- Variable: `score:int`.
- On enemy death:
  - `score += enemy_score_value`.
- UI:
  - `score` is rendered to screen each frame in HUD.

---

## G6_HISCORE_BASIC

- Precondition: G5 implemented.
- Variable: `hiscore:int`.
- Initialization:
  - `hiscore = DEFAULT_HISCORE` (e.g. 20000) on first run.
- Behaviour:
  - On any `score` change:
    - If `score > hiscore` then `hiscore = score`.
- UI:
  - `hiscore` rendered at top-center of screen.

---

## G7_ENEMY_TYPES

- Introduce multiple `EnemyType` definitions with distinct attributes:
  - `health:int` (≥ 1).
  - `score_value:int`.
- At least one type MUST require `health > 1` hits to die and yield higher `score_value`.

---

## G8_ENEMY_FORMATION_MOVEMENT

- Formation:
  - Enemies arranged in 2D grid; all share global formation offset.
- Horizontal stepping:
  - Movement state includes `direction ∈ {LEFT, RIGHT}`.
  - At regular step intervals, exactly one enemy at a time moves horizontally in `direction`.
  - Step continues until the outermost enemy in `direction` reaches screen edge.
- Edge behaviour:
  - When any enemy would move beyond horizontal bounds:
    - Entire formation moves one “row step” downward.
    - `direction` toggles (`LEFT` ↔ `RIGHT`).
- While moving horizontally, only one enemy position is updated per step tick. :contentReference[oaicite:1]{index=1}  

---

## G9_ENEMY_REACH_BOTTOM_GAMEOVER [DONE]

- Condition:
  - If any `Enemy`’s `y` position ≥ PLAYER_Y (same vertical level as player), then:
    - Set game state to `GAME_OVER`.

---

## G10_ENEMY_FIRE [DONE]

- Entity: `EnemyShot`.
- Behaviour:
  - Enemies may emit projectiles with downward velocity.
  - Only `Player` is a valid damage recipient for `EnemyShot`.
  - Enemy projectiles MUST NOT damage enemies or shields unless extended explicitly.

---

## G11_ENEMY_DYNAMIC_SPEED [DONE]

- On each enemy death:
  - Remaining enemies’ parameters are updated:
    - horizontal/vertical movement speed increased.
    - firing frequency increased (if enemy shooting implemented).
- Increase function:
  - Monotonic with respect to number of dead enemies.

---

## G12_PLAYER_LIVES [DONE]

- Variable: `lives:int`.
- Initialization: `lives = 3`.
- On collision `EnemyShot` vs `Player`:
  - `lives -= 1`.
  - If `lives == 0` → set game state to `GAME_OVER`.
- UI:
  - `lives` visualized on screen (e.g. icons or text).

---

## G13_SHIELDS_BLOCK [DONE]

- Entities: `ShieldBlock` (static obstacles between player and enemies).
- Collision rules:
  - `EnemyShot` vs `ShieldBlock`:
    - Projectile destroyed.
    - Shield damage applied.
  - `PlayerShot` vs `ShieldBlock`:
    - Projectile destroyed.
    - Shield damage applied.
- Shields do not move.

---

## G14_SHIELDS_DAMAGE_STATES [DONE]

- Each `ShieldBlock` has:
  - `max_hits > 0`.
  - `hit_count` ∈ [0, max_hits].
- On every projectile collision:
  - `hit_count++`.
  - If `hit_count >= max_hits` → shield block destroyed.
- Visual mapping:
  - Damage state controls sprite selection using the four textures:
    - `shelter1.dds`, `shelter2.dds`, `shelter3.dds`, `shelter4.dds`.

---

## G15_ENEMY_EXPLOSION_ANIMATION [DONE]

- On enemy death:
  - Spawn temporary `Explosion` entity at enemy position.
  - `Explosion` uses provided explosion images as animation frames.
  - Lifetime:
    - After `EXPLOSION_DURATION`, `Explosion` is removed.

---

## G16_BACKGROUND_RENDERING [NOT DONE]

- Asset: provided background texture.
- Rendering:
  - Background drawn behind all other sprites.
  - No stretch or crop; game resolution matches background resolution exactly.
- All coordinates and layout assume this base resolution.

---

## G17_MYSTERY_SHIP

- Entity: `MysteryShip`.
- Spawn:
  - Created off-screen at top of playfield, above enemy formation.
  - Spawn times randomized within configured interval.
- Movement:
  - Moves horizontally across the screen at fixed `y` level.
- Health:
  - Requires exactly 2 hits to be destroyed.
- Score:
  - Awards large bonus score on destruction (greater than regular enemies).
- Despawn:
  - When leaving screen bounds or being destroyed, instance is removed.

---

## G18_TITLE_SCREEN

- Game state: `TITLE`.
- On entering:
  - Render:
    - game name,
    - author name,
    - input/control instructions (movement, fire, pause, etc.).
- Input:
  - `Escape` key → terminate application.
  - `Enter` (or alternative configured key) → transition to `PLAYING` (start game).

---

## G19_GAME_OVER_SCREEN

- Trigger: player reaches `GAME_OVER` either by `lives == 0` or G9 condition.
- UI:
  - Display:
    - highest level reached,
    - total number of enemies killed,
    - final score.
- Input:
  - On any key press (selectable key set):
    - If `TITLE` state exists → transition to `TITLE`.
    - Else → restart directly into `PLAYING`.

---

## G20_SESSION_HISCORES_TOP3

- During a single run of the executable (session-local):
  - Maintain list `session_hiscores[3]`:
    - entries sorted by score descending.
- Title screen UI:
  - Display three entries with highest scores from the current session.
- At program startup:
  - All three entries initialized with default values (name + score).

---

## G21_PERSISTENT_HISCORES

- Persistent storage:
  - Hi-score table is stored externally (e.g. file) and loaded on startup.
- First run:
  - If no persistent data found → initialize with default hi-score entries.
- Subsequent runs:
  - Do NOT reset to defaults; use persisted scores instead.

---

## G22_PAUSE_FUNCTIONALITY

- Key: `P`.
- When in `PLAYING`:
  - On `P` press → transition to `PAUSED`.
- When in `PAUSED`:
  - On `P` press → transition back to `PLAYING`.
- While paused:
  - Update loop for game logic is halted (or uses zero dt).
  - UI overlay text `"paused"` rendered on screen.

---

## G23_CODE_QUALITY (non-runtime, still part of spec)

- No memory leaks.
- Code conforms to course coding standard (naming, formatting, etc.).

---

## VG1_TYPE_OBJECT_PATTERN

- All game object categories (player, enemy types, projectiles, shields, explosion) use shared **type data**:
  - Instances reference a `Type` object instead of duplicating config.
- Common attributes (health, speed, score value, sprites, etc.) live in these type objects. :contentReference[oaicite:2]{index=2}  

---

## VG2_FACTORY_CONSTRUCTION

- All runtime game objects (player, enemies, mystery ship, projectiles, shields, explosions) are created via dedicated factory methods/functions.
- No direct `new` calls for game entities outside factories.

---

## VG3_MULTITHREADED_LOGIC_RENDER

- Threads:
  - `logic_thread` executes game update loop.
  - `render_thread` executes rendering loop (TGE frame begin/end).
- Synchronization:
  - Use double-buffered render data structure.
  - Logic writes to back buffer; render reads from front buffer.
  - Swaps are synchronized (mutex/atomic) and handled safely on shutdown.

---

## VG4_JSON_CONFIG_VALUES

- A JSON config file defines at minimum:
  - `player_move_speed` (left/right),
  - `player_lives`,
  - `projectile_speed_player`,
  - `projectile_speed_enemy`,
  - `enemy_move_speed`,
  - `default_hiscore`.
- At runtime startup:
  - JSON is parsed and values applied without recompilation.

---

## VG5_DELTATIME_AND_INPUT_MANAGER

- All movement and time-based behaviour uses:
  - `dt` from custom `Timer` (not engine’s delta or fixed-step hacks).
  - Logic must be hardware-independent (same behaviour across frame rates).
- Input:
  - Custom `InputManager` / `InputHandler` library is used.
  - TGE’s `GetAsyncKeyState` is NOT used.

---

## VG6_WAVE_RESET_AND_DIFFICULTY

- Condition:
  - When all enemies in current formation are dead.
- Sequence:
  - Generate new enemy wave (rebuild formation).
  - Reset shields to initial state.
  - Grant fixed bonus to player score.
  - Increase:
    - enemy movement speed,
    - enemy fire rate,
    - by small amount per wave.

---

## VG7_RESPAWN_BEHAVIOUR

- On player death while `lives > 0`:
  - Reset player position to center of bottom area.
  - Temporarily freeze all enemies:
    - no movement,
    - no shooting.
- Freeze ends when:
  - player moves horizontally OR
  - player fires a shot.

---

## VG8_HISCORE_NAME_ENTRY

- On achieving a score that enters hi-score table:
  - Enter state `HISCORE_NAME_ENTRY`.
  - Accept alphanumeric name input without using console I/O.
- Stored hi-score entries include:
  - `name:string`,
  - `score:int`.
- Title screen displays names next to scores.
