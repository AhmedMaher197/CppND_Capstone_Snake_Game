#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

//Multithreading Implementation
Controller::Controller() : input_thread_{&Controller::InputThreadFunction, this} {}

//Destructor Implementation
Controller::~Controller() 
{
    running_ = false;
    if (input_thread_.joinable()) 
    {
        input_thread_.join();
    }
}

void Controller::InputThreadFunction() 
{
    while (running_) 
    {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            //Mutex and Lock Implementation
            std::lock_guard<std::mutex> lock(input_mutex_);
            
            if (e.type == SDL_QUIT) 
            {
                input_state_.quit = true;
            } 
            else if (e.type == SDL_KEYDOWN) 
            {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        input_state_.next_direction = Snake::Direction::kUp;
                        input_state_.direction_changed = true;
                        break;
                    case SDLK_DOWN:
                        input_state_.next_direction = Snake::Direction::kDown;
                        input_state_.direction_changed = true;
                        break;
                    case SDLK_LEFT:
                        input_state_.next_direction = Snake::Direction::kLeft;
                        input_state_.direction_changed = true;
                        break;
                    case SDLK_RIGHT:
                        input_state_.next_direction = Snake::Direction::kRight;
                        input_state_.direction_changed = true;
                        break;
                }
           }
        }
        
        // Add small delay to prevent excessive CPU usage
        SDL_Delay(1);
    }
}

//Use of References in Function Declarations
void Controller::ChangeDirection(Snake &snake, const Snake::Direction& input, const Snake::Direction& opposite) const 
{
  if (snake.GetSnakeDirection() != opposite || snake.GetSize() == 1) 
    snake.SetSnakeDirection(input);
}

//User Input Processing
void Controller::HandleInput(bool &running, Snake &snake) const 
{
    //Mutex and Lock Implementation
    std::lock_guard<std::mutex> lock(input_mutex_);
    
    if (input_state_.quit) 
    {
        running = false;
        return;
    }
    
    if (input_state_.direction_changed) 
    {
        Snake::Direction opposite;
        
        switch (input_state_.next_direction) 
        {
            case Snake::Direction::kUp:
                opposite = Snake::Direction::kDown;
                break;
            case Snake::Direction::kDown:
                opposite = Snake::Direction::kUp;
                break;
            case Snake::Direction::kLeft:
                opposite = Snake::Direction::kRight;
                break;
            case Snake::Direction::kRight:
                opposite = Snake::Direction::kLeft;
                break;
        }
        
        ChangeDirection(snake, input_state_.next_direction, opposite);
        input_state_.direction_changed = false;
    }
}