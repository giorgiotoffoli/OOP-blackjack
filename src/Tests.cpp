#include "Blackjack.hpp"

#include <iostream>
#include <string>
#include <vector>

int runTests() {
  int failures = 0;
  const auto check = [&](bool ok, const std::string& name) {
    std::cout << (ok ? "PASS  " : "FAIL  ") << name << '\n';
    if (!ok) ++failures;
  };

  Hand hand;
  hand.addCard(Card(0));  // Ace
  hand.addCard(Card(12)); // King
  check(Blackjack::handValue(hand) == 21 && Blackjack::isNatural(hand),
        "Ace + King is a natural 21");

  hand.clear();
  hand.addCard(Card(0));
  hand.addCard(Card(5));
  check(Blackjack::handValue(hand) == 17 && Blackjack::isSoft(hand),
        "Ace + 6 is soft 17");

  hand.addCard(Card(9));
  check(Blackjack::handValue(hand) == 17 && !Blackjack::isSoft(hand),
        "Ace + 6 + 10 is hard 17");

  hand.clear();
  hand.addCard(Card(0));
  hand.addCard(Card(13));
  hand.addCard(Card(8));
  check(Blackjack::handValue(hand) == 21, "Ace + Ace + 9 totals 21");

  hand.clear();
  hand.addCard(Card(12));
  hand.addCard(Card(11));
  hand.addCard(Card(4));
  check(Blackjack::handValue(hand) > 21, "King + Queen + 5 busts");

  Blackjack softSeventeen;
  softSeventeen.setNextDeck(Deck({Card(9), Card(0), Card(8), Card(5)}));
  check(softSeventeen.startRound(5) && softSeventeen.stand(),
        "dealer soft 17 round completes");
  check(softSeventeen.dealerHand().size() == 2 &&
            Blackjack::handValue(softSeventeen.dealerHand()) == 17,
        "dealer stands on soft 17");

  Blackjack dealerSixteen;
  dealerSixteen.setNextDeck(Deck({Card(9), Card(9), Card(8), Card(5), Card(1)}));
  check(dealerSixteen.startRound(5) && dealerSixteen.stand(),
        "dealer 16 round completes");
  check(dealerSixteen.dealerHand().size() == 3 &&
            Blackjack::handValue(dealerSixteen.dealerHand()) == 18,
        "dealer hits 16");

  Blackjack natural;
  natural.setNextDeck(Deck({Card(0), Card(6), Card(12), Card(7)}));
  check(natural.startRound(10) && natural.phase() == Phase::RoundOver &&
            natural.outcome() == Outcome::PlayerBlackjack && natural.chips() == 115,
        "natural blackjack returns wager and pays 3:2");

  Blackjack doubleNatural;
  doubleNatural.setNextDeck(Deck({Card(0), Card(9), Card(12), Card(13)}));
  check(doubleNatural.startRound(10) && doubleNatural.outcome() == Outcome::Push &&
            doubleNatural.chips() == 100,
        "player and dealer naturals push");

  Blackjack invalidAction;
  const int originalChips = invalidAction.chips();
  check(!invalidAction.hit() && invalidAction.chips() == originalChips &&
            invalidAction.playerHand().size() == 0,
        "hit during betting is rejected without changing state");
  check(!invalidAction.startRound(0) && invalidAction.chips() == originalChips,
        "invalid wager is rejected without changing chips");

  return failures == 0 ? 0 : 1;
}
