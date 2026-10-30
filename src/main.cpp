#include "Blackjack.hpp"
#include "ConsoleView.hpp"
#include "GameView.hpp"
#ifndef BLACKJACK_TEXT_ONLY
#include "TuiView.hpp"
#endif

#include <memory>
#include <string>

int runTests();

int main(int argc, char** argv) {
  const std::string mode = argc > 1 ? argv[1] : "";
  if (mode == "--test") return runTests();

  Blackjack game;
  std::unique_ptr<GameView> view;
#ifdef BLACKJACK_TEXT_ONLY
  view = std::make_unique<ConsoleView>(game);
#else
  if (mode == "--text") view = std::make_unique<ConsoleView>(game);
  else view = std::make_unique<TuiView>(game);
#endif
  view->run();
  return 0;
}
