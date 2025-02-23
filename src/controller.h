#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include <mutex>
#include <thread>
#include <atomic>

class Controller {
 public:
  Controller();
  ~Controller();
  //Rule of 5 Implementation
  Controller(const Controller& other) = delete; 
  Controller& operator=(const Controller& other) = delete;
  Controller(Controller&& other) noexcept = delete;
  Controller& operator=(Controller&& other) noexcept = delete;
  

  void HandleInput(bool &running, Snake &snake) const;

 private:
  struct InputState 
  {
    bool quit{false};
    Snake::Direction next_direction{Snake::Direction::kUp};
    bool direction_changed{false};
  };

  void InputThreadFunction();

  void ChangeDirection(Snake &snake, const Snake::Direction& input,
                       const Snake::Direction& opposite) const;
  //Mutex and Lock Implementation
  mutable std::mutex input_mutex_;    
  std::atomic<bool> running_{true};

  
  mutable InputState input_state_;
  
  // Input handling thread
  std::thread input_thread_;
};

#endif