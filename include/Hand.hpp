#pragma once

#include "Card.hpp"

#include <vector>

class Hand {
public:
  void addCard(const Card& card);
  void clear();
  int size() const;
  const Card& at(int index) const;
  const std::vector<Card>& cards() const;

private:
  std::vector<Card> cards_;
};
