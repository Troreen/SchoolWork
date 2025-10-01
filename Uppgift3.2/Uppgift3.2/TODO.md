### 1. Refactor existing casino game

* [x] Rewrite the casino game so it uses **classes and objects**.
* [x] Create a **class for each game**, split into a header file (`.h`) and a source file (`.cpp`).
* [x] Each class should have a **member variable** that tracks how much the player has won or lost in that game.
* [x] Create a **Casino class** that contains the different games, with one member variable for each game.
* [x] Make member variables **private**.
* [x] Provide **getter functions** for values that must be read externally.
* [x] Use `const` and `static` where appropriate (e.g., if all instances share a variable).
* [x] Declare all functions (including getters) as `const` if they do not modify member variables.
* [x] Minimize constructor parameters:

  * Ask: does the value need to be passed every time?
  * Can it be calculated instead?
  * Can it be the same for every table?

---

### 2. Add variation in games

* [x] Modify the **Guess the Number** game so that it can support multiple tables (variants) with different betting limits.
* [x] The constructor must accept **minimum and maximum bet values**.
* [x] Use these parameters to define **Low Stakes** and **High Stakes** tables.
* [x] Display betting limits clearly so the player knows min and max bets.
* [x] Both tables should be handled identically outside the class — only their constructor values differ.
* [x] If the player no longer has enough money to place the minimum bet, return them to the main menu.

---

### 3. Add player name

* [x] Ask the player to enter their **name**.
* [x] Validate name: only letters, at least **two characters long**.
* [x] Use the player's name throughout the game (not just in the lobby).
* [x] Always pass the name as a **const reference** — never copy or allow it to be modified.
