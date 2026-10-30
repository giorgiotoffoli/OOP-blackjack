#pragma once

#include <string>

// Card values follow the assignment's representation: 0-12 are Clubs,
// 13-25 Diamonds, 26-38 Hearts, and 39-51 Spades. Ace has rank 0.
class Card {
public:
  explicit Card(int value = 0);

  int getValue() const;
  int getRank() const;
  int getSuit() const;
  std::string rankSymbol() const;
  std::string suitSymbol() const;
  char suitLetter() const;
  bool isRed() const;
  std::string toString() const;

private:
  int value_;
};
