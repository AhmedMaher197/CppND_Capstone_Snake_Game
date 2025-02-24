#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

class Controller {
 public:
  void HandleInput(bool &running, Snake &snake) const;

  Controller() = default;
  ~Controller() = default;
  
  //Rule of 5 Implementation
  Controller(const Controller& other) = delete; 
  Controller& operator=(const Controller& other) = delete;
  Controller(Controller&& other) noexcept = delete;
  Controller& operator=(Controller&& other) noexcept = delete;

 private:
  void ChangeDirection(Snake &snake, const Snake::Direction& input,
                       Snake::Direction opposite) const;
};

#endif



