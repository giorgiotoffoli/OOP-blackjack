#pragma once

#include "Card.hpp"

#include <vector>

class Deck {
public:
  Deck();
  explicit Deck(const std::vector<Card>& stacked);

  void reset();
  void shuffle();
  Card deal();
  bool empty() const;
  int size() const;

private:
  std::vector<Card> cards_;
};
