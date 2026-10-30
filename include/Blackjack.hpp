#pragma once

#include "Deck.hpp"
#include "Hand.hpp"

#include <optional>
#include <string>

enum class Phase { Betting, PlayerTurn, RoundOver, GameOver };
enum class Outcome { None, PlayerBlackjack, PlayerWin, DealerWin, Push, PlayerBust, DealerBust };

class Blackjack {
public:
  explicit Blackjack(int startingChips = 100);

  bool startRound(int bet);
  bool hit();
  bool stand();
  bool nextRound();

  // Allows deterministic deals in rule checks. Production rounds use a newly
  // shuffled deck unless this one-round test deck has been supplied.
  bool setNextDeck(const Deck& deck);

  Phase phase() const;
  int chips() const;
  int currentBet() const;
  const Hand& playerHand() const;
  const Hand& dealerHand() const;
  bool dealerHoleHidden() const;
  Outcome outcome() const;

  static int handValue(const Hand& hand);
  static bool isSoft(const Hand& hand);
  static bool isNatural(const Hand& hand);
  static std::string outcomeName(Outcome outcome);

private:
  void settleDealerTurn();
  void finishRound(Outcome outcome);

  int chips_;
  int bet_ = 0;
  Phase phase_;
  Outcome outcome_ = Outcome::None;
  bool holeHidden_ = true;
  Deck deck_;
  Hand player_;
  Hand dealer_;
  std::optional<Deck> nextDeck_;
};
