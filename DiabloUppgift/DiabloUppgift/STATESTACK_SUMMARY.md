The project now uses a stack-based state system where gameplay flow is controlled by push/pop transitions between game states.

Core framework files:

- `StateStack.h/.cpp`
- `IGameState.h/.cpp`
- `StateTimer.h/.cpp`
- `StateId.h`
- `GameContext.h`

Concrete states:

- `MainMenuState`
- `GameplayState`
- `CombatState`
- `InventoryMenuState`
- `EndState`

## How It Works

1. `GameManager::Run()` creates `StateStack` and registers state factories
2. Initial state `MainMenu` is pushed
3. Each frame/tick, `StateStack::Update()` runs the top state
4. States queue transitions with:
   - `PushState(...)`
   - `PopState()`
   - `ClearStates()`
5. Pending transitions are applied safely before/after state update

## Assignment Requirements Mapping

- At least 3 states:
  - Implemented 5 states

- Supports expansion for any number of states:
  - New states are added by extending `StateId`, creating a class derived from `IGameState`, and registering a factory

- One state activated by multiple states:
  - `InventoryMenuState` is pushed from both `GameplayState` and `CombatState`
  - `EndState` is also reachable from multiple states

- Timer in each state that resets correctly:
  - Each `IGameState` owns a `StateTimer`
  - Timer reset happens in `IGameState::OnEnter()`