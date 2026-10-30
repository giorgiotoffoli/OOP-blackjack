#include "Blackjack.hpp"

#include <algorithm>

Blackjack::Blackjack(int startingChips)
    : chips_(std::max(0, startingChips)),
      phase_(chips_ == 0 ? Phase::GameOver : Phase::Betting) {}

bool Blackjack::startRound(int bet) {
  if (phase_ != Phase::Betting || bet < 1 || bet > chips_) {
    return false;
  }

  bet_ = bet;
  chips_ -= bet_;
  outcome_ = Outcome::None;
  player_.clear();
  dealer_.clear();
  holeHidden_ = true;

  if (nextDeck_) {
    deck_ = *nextDeck_;
    nextDeck_.reset();
  } else {
    deck_.reset();
    deck_.shuffle();
  }

  // The deal order is player, dealer, player, dealer.
  player_.addCard(deck_.deal());
  dealer_.addCard(deck_.deal());
  player_.addCard(deck_.deal());
  dealer_.addCard(deck_.deal());
  phase_ = Phase::PlayerTurn;

  const bool playerNatural = isNatural(player_);
  const bool dealerNatural = isNatural(dealer_);
  if (playerNatural && dealerNatural) {
    chips_ += bet_; // Push: return the original wager.
    finishRound(Outcome::Push);
  } else if (playerNatural) {
    chips_ += bet_ + (bet_ * 3) / 2; // Return wager plus 3:2 winnings.
    finishRound(Outcome::PlayerBlackjack);
  } else if (dealerNatural) {
    finishRound(Outcome::DealerWin);
  }
  return true;
}

bool Blackjack::hit() {
  if (phase_ != Phase::PlayerTurn) {
    return false;
  }
  player_.addCard(deck_.deal());
  if (handValue(player_) > 21) {
    finishRound(Outcome::PlayerBust);
  }
  return true;
}

bool Blackjack::stand() {
  if (phase_ != Phase::PlayerTurn) {
    return false;
  }
  settleDealerTurn();
  return true;
}

bool Blackjack::nextRound() {
  if (phase_ != Phase::RoundOver) {
    return false;
  }
  bet_ = 0;
  phase_ = chips_ == 0 ? Phase::GameOver : Phase::Betting;
  return true;
}

bool Blackjack::setNextDeck(const Deck& deck) {
  if (phase_ != Phase::Betting) {
    return false;
  }
  nextDeck_ = deck;
  return true;
}

Phase Blackjack::phase() const { return phase_; }
int Blackjack::chips() const { return chips_; }
int Blackjack::currentBet() const { return bet_; }
const Hand& Blackjack::playerHand() const { return player_; }
const Hand& Blackjack::dealerHand() const { return dealer_; }
bool Blackjack::dealerHoleHidden() const { return holeHidden_; }
Outcome Blackjack::outcome() const { return outcome_; }

int Blackjack::handValue(const Hand& hand) {
  int total = 0;
  int aces = 0;
  for (const Card& card : hand.cards()) {
    const int rank = card.getRank();
    if (rank == 0) {
      total += 11;
      ++aces;
    } else if (rank >= 10) {
      total += 10;
    } else {
      total += rank + 1;
    }
  }
  while (total > 21 && aces > 0) {
    total -= 10;
    --aces;
  }
  return total;
}

bool Blackjack::isSoft(const Hand& hand) {
  int total = 0;
  int aces = 0;
  for (const Card& card : hand.cards()) {
    const int rank = card.getRank();
    if (rank == 0) {
      total += 11;
      ++aces;
    } else if (rank >= 10) {
      total += 10;
    } else {
      total += rank + 1;
    }
  }
  while (total > 21 && aces > 0) {
    total -= 10;
    --aces;
  }
  return aces > 0;
}

bool Blackjack::isNatural(const Hand& hand) {
  if (hand.size() != 2) {
    return false;
  }
  const int first = hand.at(0).getRank();
  const int second = hand.at(1).getRank();
  const bool firstAce = first == 0;
  const bool secondAce = second == 0;
  const bool firstTenValue = first >= 9;
  const bool secondTenValue = second >= 9;
  return (firstAce && secondTenValue) || (secondAce && firstTenValue);
}

std::string Blackjack::outcomeName(Outcome outcome) {
  switch (outcome) {
  case Outcome::PlayerBlackjack: return "Blackjack! You win 3:2.";
  case Outcome::PlayerWin: return "You win.";
  case Outcome::DealerWin: return "Dealer wins.";
  case Outcome::Push: return "Push. Your bet is returned.";
  case Outcome::PlayerBust: return "Bust. Dealer wins.";
  case Outcome::DealerBust: return "Dealer busts. You win.";
  case Outcome::None: return "";
  }
  return "";
}

void Blackjack::settleDealerTurn() {
  holeHidden_ = false;
  while (handValue(dealer_) <= 16 && !deck_.empty()) {
    dealer_.addCard(deck_.deal());
  }

  const int playerTotal = handValue(player_);
  const int dealerTotal = handValue(dealer_);
  if (dealerTotal > 21) {
    chips_ += bet_ * 2;
    finishRound(Outcome::DealerBust);
  } else if (playerTotal > dealerTotal) {
    chips_ += bet_ * 2;
    finishRound(Outcome::PlayerWin);
  } else if (playerTotal < dealerTotal) {
    finishRound(Outcome::DealerWin);
  } else {
    chips_ += bet_;
    finishRound(Outcome::Push);
  }
}

void Blackjack::finishRound(Outcome outcome) {
  outcome_ = outcome;
  holeHidden_ = false;
  phase_ = Phase::RoundOver;
}
