#include "TuiView.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

using namespace ftxui;

namespace {
Element drawCard(const Card& card, bool faceDown = false) {
  if (faceDown) {
    return vbox({text("########"), filler(), text("##  ##") | center, filler(),
                 text("########")}) |
           color(Color::Blue) | size(WIDTH, EQUAL, 9) | size(HEIGHT, EQUAL, 7) | border;
  }
  const Color cardColor = card.isRed() ? Color::Red : Color::White;
  return vbox({hbox({text(card.rankSymbol()), filler()}), filler(),
               text(card.suitSymbol()) | bold | center, filler(),
               hbox({filler(), text(card.rankSymbol())})}) |
         color(cardColor) | size(WIDTH, EQUAL, 9) | size(HEIGHT, EQUAL, 7) | border;
}

Element drawHand(const char* title, const Hand& hand, bool hideHole) {
  Elements cards;
  for (int i = 0; i < hand.size(); ++i) {
    cards.push_back(drawCard(hand.at(i), hideHole && i == 1));
  }
  Element total = text(hideHole ? "Total: ?" : "Total: " + std::to_string(Blackjack::handValue(hand)));
  if (!hideHole && Blackjack::isSoft(hand)) total = hbox({total, text(" (soft)") | dim});
  return vbox({text(title) | bold, hbox(std::move(cards)), total});
}
} // namespace

TuiView::TuiView(Blackjack& game) : game_(game) {}

void TuiView::run() {
  auto screen = ScreenInteractive::TerminalOutput();
  auto renderer = Renderer([&] {
    Element status;
    if (!message_.empty()) status = text(message_);
    else if (game_.phase() == Phase::RoundOver) status = text(Blackjack::outcomeName(game_.outcome()));
    else if (game_.phase() == Phase::PlayerTurn) status = text("Your move.");
    else if (game_.phase() == Phase::GameOver) status = text("GAME OVER - out of chips.");
    else status = text("Place your bet to begin.");

    std::string keys;
    if (game_.phase() == Phase::Betting)
      keys = "1-9 choose bet   m max bet   +/- adjust   Enter deal   q quit";
    else if (game_.phase() == Phase::PlayerTurn)
      keys = "h hit   s stand   q quit";
    else if (game_.phase() == Phase::RoundOver)
      keys = game_.chips() == 0 ? "Enter finish   q quit" : "Enter next round   q quit";
    else
      keys = "q quit";

    Elements rows;
    rows.push_back(text("BLACKJACK") | bold | center);
    rows.push_back(separator());
    rows.push_back(drawHand("DEALER", game_.dealerHand(), game_.dealerHoleHidden()) | center);
    rows.push_back(separator());
    rows.push_back(drawHand("YOU", game_.playerHand(), false) | center);
    rows.push_back(separator());
    rows.push_back(status | bold | center);
    rows.push_back(hbox({text("Chips: " + std::to_string(game_.chips())), filler(),
                         text("Bet: " + (game_.currentBet() > 0 ? std::to_string(game_.currentBet()) : "-")),
                         filler(),
                         text(game_.phase() == Phase::Betting
                                  ? "Selected: " + std::to_string(std::max(1, selectedBet_))
                                  : "")}));
    rows.push_back(separator());
    rows.push_back(text(keys) | dim | center);
    return vbox(std::move(rows)) | border | size(WIDTH, GREATER_THAN, 44);
  });

  auto app = CatchEvent(renderer, [&](Event event) {
    if (event == Event::Character('q') || event == Event::Character('Q')) {
      screen.Exit();
      return true;
    }
    if (game_.phase() == Phase::Betting) {
      const int maxBet = game_.chips();
      if (event == Event::ArrowUp || event == Event::Character('+')) {
        selectedBet_ = std::min(maxBet, selectedBet_ + 1);
        return true;
      }
      if (event == Event::ArrowDown || event == Event::Character('-')) {
        selectedBet_ = std::max(1, selectedBet_ - 1);
        return true;
      }
      if (event == Event::Character('m') || event == Event::Character('M')) {
        selectedBet_ = maxBet;
        return true;
      }
      for (char digit = '1'; digit <= '9'; ++digit) {
        if (event == Event::Character(digit)) {
          selectedBet_ = std::min(maxBet, digit - '0');
          return true;
        }
      }
      if (event == Event::Return) {
        if (game_.startRound(selectedBet_)) message_.clear();
        else message_ = "That bet is not available.";
        return true;
      }
    } else if (game_.phase() == Phase::PlayerTurn) {
      if (event == Event::Character('h') || event == Event::Character('H')) {
        if (!game_.hit()) message_ = "Hit is not available now.";
        else message_.clear();
        return true;
      }
      if (event == Event::Character('s') || event == Event::Character('S')) {
        if (!game_.stand()) message_ = "Stand is not available now.";
        else message_.clear();
        return true;
      }
    } else if (game_.phase() == Phase::RoundOver && event == Event::Return) {
      if (game_.nextRound()) {
        selectedBet_ = std::min(selectedBet_, game_.chips());
        message_.clear();
      }
      return true;
    }
    return false;
  });

  screen.Loop(app);
}
