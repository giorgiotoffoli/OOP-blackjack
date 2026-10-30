#include "ConsoleView.hpp"

#include <iostream>
#include <string>

ConsoleView::ConsoleView(Blackjack& game) : game_(game) {}

void ConsoleView::showHand(const char* label, const Hand& hand, bool hideHole) const {
  std::cout << label << ": ";
  for (int i = 0; i < hand.size(); ++i) {
    if (i > 0) std::cout << ' ';
    if (hideHole && i == 1) std::cout << "??";
    else std::cout << hand.at(i).toString();
  }
  if (!hideHole && hand.size() > 0) {
    std::cout << "  (" << Blackjack::handValue(hand) << ')';
    if (Blackjack::isSoft(hand)) std::cout << " soft";
  }
  std::cout << '\n';
}

void ConsoleView::showState() const {
  std::cout << "\nChips: " << game_.chips();
  if (game_.currentBet() > 0) std::cout << "   Bet: " << game_.currentBet();
  std::cout << '\n';
  showHand("Dealer", game_.dealerHand(), game_.dealerHoleHidden());
  showHand("You   ", game_.playerHand(), false);
  if (game_.phase() == Phase::RoundOver) {
    std::cout << Blackjack::outcomeName(game_.outcome()) << '\n';
  }
}

void ConsoleView::run() {
  std::string line;
  while (game_.phase() != Phase::GameOver) {
    showState();

    if (game_.phase() == Phase::Betting) {
      std::cout << "Bet 1-" << game_.chips() << " chips, or q to quit: ";
      if (!std::getline(std::cin, line) || line == "q" || line == "Q") return;
      try {
        std::size_t parsed = 0;
        const int bet = std::stoi(line, &parsed);
        if (parsed != line.size() || !game_.startRound(bet)) {
          std::cout << "Please enter a whole-number bet from 1 to " << game_.chips() << ".\n";
        }
      } catch (const std::exception&) {
        std::cout << "Please enter a whole-number bet.\n";
      }
    } else if (game_.phase() == Phase::PlayerTurn) {
      std::cout << "Hit or stand? (h/s, q to quit): ";
      if (!std::getline(std::cin, line) || line == "q" || line == "Q") return;
      if (line == "h" || line == "H") game_.hit();
      else if (line == "s" || line == "S") game_.stand();
      else std::cout << "Please type h, s, or q.\n";
    } else if (game_.phase() == Phase::RoundOver) {
      std::cout << "Press Enter for the next round, or q to quit: ";
      if (!std::getline(std::cin, line) || line == "q" || line == "Q") return;
      game_.nextRound();
    }
  }
  showState();
  std::cout << "Out of chips. Game over.\n";
}
