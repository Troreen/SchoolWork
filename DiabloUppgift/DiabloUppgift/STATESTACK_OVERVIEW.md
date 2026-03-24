# DiabloUppgift StateStack Overview

This document explains the StateStack system that was added to DiabloUppgift, including the files that changed, how state transitions work, and how per-state timers are handled.

## Goals of the implementation

The StateStack implementation was introduced to support:

- A stack-based game flow instead of one monolithic loop.
- At least 3 concrete states.
- Easy expansion to any number of states.
- A state that can be activated by multiple parent states.
- A timer owned by each state that resets correctly on activation.

## What was added

### Core framework

- `StateId.h`
  - Defines available state identifiers.
  - Current ids: `MainMenu`, `Gameplay`, `Combat`, `InventoryMenu`, `End`.

- `StateTimer.h` and `StateTimer.cpp`
  - Small real-time timer helper based on `std::chrono::steady_clock`.
  - API:
    - `Reset()`
    - `GetElapsedSeconds()`

- `IGameState.h` and `IGameState.cpp`
  - Base class for all states.
  - Provides lifecycle hooks:
    - `OnEnter()`
    - `OnExit()`
    - `Update(StateStack&)`
  - Owns one `StateTimer` per state instance.
  - Important behavior:
    - `OnEnter()` calls `myStateTimer.Reset()`.

- `StateStack.h` and `StateStack.cpp`
  - Manages active state stack (`std::vector<std::unique_ptr<IGameState>>`).
  - Supports deferred operations:
    - `PushState(StateId)`
    - `PopState()`
    - `ClearStates()`
  - Uses a registry/factory model:
    - `RegisterState(StateId, StateFactory)`
  - Deferred operations are applied in `ApplyPendingChanges()` before and after current-state update.

- `GameContext.h`
  - Shared references used by all states.
  - Holds:
    - `Player& player`
    - `std::vector<Room>& rooms`
    - `Room*& currentRoom`
    - `bool isRunning`
    - `std::string endMessage`

### Concrete states

- `MainMenuState.h` and `MainMenuState.cpp`
  - Entry state.
  - Shows menu and timer.
  - Transitions:
    - Start -> `Gameplay`
    - Exit -> `End`

- `GameplayState.h` and `GameplayState.cpp`
  - Exploration/safe-room flow.
  - Shows exploration timer.
  - Handles room actions, movement, pickups, chest, stats, drop.
  - Transitions:
    - If room has enemies -> `Combat`
    - Open inventory -> `InventoryMenu`
    - Quit -> `End`

- `CombatState.h` and `CombatState.cpp`
  - Encapsulates combat loop and uses `CombatComponent`.
  - Shows combat timer.
  - Transitions:
    - Open inventory -> `InventoryMenu`
    - Win room combat -> pop `Combat` and return to previous state
    - All rooms cleared -> `End`
    - Player defeated -> `End`
    - Exit combat -> `End`
  - Enemy turn visibility fix:
    - Keeps enemy turn output visible by pausing before next clear.

- `InventoryMenuState.h` and `InventoryMenuState.cpp`
  - Shared inventory/equip state.
  - Shows inventory timer.
  - Pops back to the calling state on `0`.

- `EndState.h` and `EndState.cpp`
  - Displays final message and end timer.
  - Clears states and sets `isRunning = false`.

## Existing files that were modified

- `GameManager.cpp`
  - Creates `GameContext` and `StateStack`.
  - Registers all state factories.
  - Pushes initial `MainMenu` state.
  - Runs stack update loop.
  - Includes fix for deferred first push:
    - The loop calls `stateStack.Update()` first, then checks `stateStack.Empty()`.

- `CombatComponent.cpp`
  - Removed `std::system("cls")` from `PerformPlayerAction`.
  - This prevented combat text from being instantly wiped.

- `DiabloUppgift.vcxproj`
  - Added new StateStack-related `.h/.cpp` files to build.

- `DiabloUppgift.vcxproj.filters`
  - Added new StateStack-related files to Visual Studio filters.

- Cleanup after migration
  - Removed unused legacy `InteractionController.h/.cpp`.
  - Removed unused `doors` reference from `GameContext`.
  - Removed unused debug-name storage/accessors from `IGameState`.

## Runtime flow

1. `GameManager::Run()` registers state factories and queues `MainMenu` push.
2. `StateStack::Update()` applies pending changes and enters `MainMenu`.
3. Active state `Update()` handles input and queues transitions.
4. Transitions are applied safely at update boundaries.
5. `EndState` clears stack and stops main loop.

## Timer behavior

- Every state instance has its own timer via `IGameState`.
- Timer reset point is centralized in `IGameState::OnEnter()`.
- Timers are independent between states.
- Because states are constructed on push, each activation gets a fresh timer baseline.

## Requirement mapping

- At least 3 states:
  - Implemented more than 3 (`MainMenu`, `Gameplay`, `Combat`, `InventoryMenu`, `End`).

- Supports expansion for any amount of states:
  - `StateStack::RegisterState` + `StateId` enum + factory function.

- At least one state can be activated by multiple states:
  - `InventoryMenu` is pushed from both `Gameplay` and `Combat`.
  - `End` can also be pushed from multiple states.

- Timer in each state that resets correctly:
  - Handled by `IGameState::OnEnter()` calling `StateTimer::Reset()`.

## How to add a new state

1. Add a new id in `StateId.h`.
2. Create `NewState.h/.cpp` deriving from `IGameState`.
3. Implement `Update(StateStack&)` and optional `OnEnter/OnExit`.
4. Register factory in `GameManager::Run()`.
5. Push it from any existing state where needed.

## Design notes

- The stack uses deferred changes to avoid mutating the stack while a state is mid-update.
- State ownership is clear:
  - `GameManager` owns core game objects.
  - `GameContext` exposes references.
  - `StateStack` owns state objects.
- If you want a leaner version, `CombatState` and `InventoryMenuState` can be folded back into `GameplayState` while still keeping the stack framework.
