#include "Card.hpp"

#include <array>
#include <stdexcept>

Card::Card(int value) : value_(value) {
  if (value < 0 || value > 51) {
    throw std::out_of_range("Card value must be between 0 and 51");
  }
}

int Card::getValue() const { return value_; }
int Card::getRank() const { return value_ % 13; }
int Card::getSuit() const { return value_ / 13; }

std::string Card::rankSymbol() const {
  static const std::array<std::string, 13> ranks = {
      "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
  return ranks.at(static_cast<std::size_t>(getRank()));
}

std::string Card::suitSymbol() const {
  static const std::array<std::string, 4> suits = {"♣", "♦", "♥", "♠"};
  return suits.at(static_cast<std::size_t>(getSuit()));
}

char Card::suitLetter() const {
  static const std::array<char, 4> suits = {'C', 'D', 'H', 'S'};
  return suits.at(static_cast<std::size_t>(getSuit()));
}

bool Card::isRed() const { return getSuit() == 1 || getSuit() == 2; }

std::string Card::toString() const { return rankSymbol() + suitLetter(); }
