#pragma once

class GameView {
public:
  virtual ~GameView() = default;
  virtual void run() = 0;
};
