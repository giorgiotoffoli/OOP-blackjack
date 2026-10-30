#pragma once

#include "Blackjack.hpp"
#include "GameView.hpp"

class ConsoleView : public GameView {
public:
  explicit ConsoleView(Blackjack& game);
  void run() override;

private:
  void showState() const;
  void showHand(const char* label, const Hand& hand, bool hideHole) const;
  Blackjack& game_;
};
