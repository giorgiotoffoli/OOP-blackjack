#include "Hand.hpp"

#include <stdexcept>

void Hand::addCard(const Card& card) { cards_.push_back(card); }
void Hand::clear() { cards_.clear(); }
int Hand::size() const { return static_cast<int>(cards_.size()); }

const Card& Hand::at(int index) const {
  if (index < 0 || index >= size()) {
    throw std::out_of_range("Hand card index is out of range");
  }
  return cards_[static_cast<std::size_t>(index)];
}

const std::vector<Card>& Hand::cards() const { return cards_; }
