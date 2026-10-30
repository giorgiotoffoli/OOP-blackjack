#include "Deck.hpp"

#include <algorithm>
#include <random>
#include <stdexcept>

Deck::Deck() { reset(); }

Deck::Deck(const std::vector<Card>& stacked) : cards_(stacked.rbegin(), stacked.rend()) {}

void Deck::reset() {
  cards_.clear();
  cards_.reserve(52);
  for (int value = 0; value < 52; ++value) {
    cards_.emplace_back(value);
  }
}

void Deck::shuffle() {
  static std::mt19937 generator(std::random_device{}());
  std::shuffle(cards_.begin(), cards_.end(), generator);
}

Card Deck::deal() {
  if (cards_.empty()) {
    throw std::runtime_error("Cannot deal from an empty deck");
  }
  Card card = cards_.back();
  cards_.pop_back();
  return card;
}

bool Deck::empty() const { return cards_.empty(); }
int Deck::size() const { return static_cast<int>(cards_.size()); }
