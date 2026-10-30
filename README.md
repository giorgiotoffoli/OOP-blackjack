# Blackjack

A terminal Blackjack game for P01. The game rules and state live in `Blackjack`; `ConsoleView` and `TuiView` present that same game through different interfaces.

## Build and run

FTXUI is fetched by CMake the first time you configure the project. From this directory:

```sh
cmake -S . -B build
cmake --build build
./build/game --text
./build/game
./build/game --test
```

On Windows, run `build\game.exe` with the same arguments.

You can also play it without FTXUI! Configure the playable text version without that dependency:

```sh
cmake -S . -B build -DBLACKJACK_ENABLE_TUI=OFF
cmake --build build
./build/game
```

With `BLACKJACK_ENABLE_TUI=OFF`, the program starts `ConsoleView` by default. The normal configuration keeps FTXUI enabled and selects the TUI unless `--text` is passed.

The text version uses ASCII card names and accepts whole-number wagers, `h` for hit, and `s` for stand. The FTXUI version uses `1`–`9` or `m` to select a wager, `+`/`-` to adjust it, Enter to deal or continue, and `h`/`s` during the player turn.

## OOP

- `Card`, `Deck`, and `Hand` model the cards and hands.
- `Blackjack` owns the state machine, chip accounting, hand scoring, natural checks, and dealer rules. It does not perform input or output.
- `GameView` is the abstract interface. `main` selects a `ConsoleView` or `TuiView` through a `std::unique_ptr<GameView>`.
- `setNextDeck` supplies a deterministic deck for the built-in rule checks; normal rounds create and shuffle a fresh deck.
