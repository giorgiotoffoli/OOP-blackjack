#pragma once

#include "Blackjack.hpp"
#include "GameView.hpp"

#include <string>

class TuiView : public GameView {
public:
  explicit TuiView(Blackjack& game);
  void run() override;

private:
  Blackjack& game_;
  int selectedBet_ = 1;
  std::string message_;
};
