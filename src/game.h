#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <fstream>  
#include <sstream>  
#include <future>
#include <thread>   
#include <chrono>

struct GameSettings
{
  std::size_t frames_per_second;
  std::size_t ms_per_frame;
  std::size_t screen_width;
  std::size_t screen_height;
  std::size_t grid_width;
  std::size_t grid_height;
};

class GameConfig
{
  public:
   GameConfig(const std::string&);
   ~GameConfig() = default;

  GameConfig(const GameConfig& other) = delete; 
  GameConfig& operator=(const GameConfig& other) = delete;
  GameConfig(GameConfig&& other) noexcept = delete;
  GameConfig& operator=(GameConfig&& other) noexcept = delete;

   void SaveConfig(const std::string&); 
   void LoadConfig(const std::string&); 
   GameSettings GetGameSettings() const;
   int GetHighestScore() const;
   void SetNewHighScore(int);
  private:
    int highest_score_;
    GameSettings game_settings_;
};


class Game {
 public:
  Game(std::size_t& grid_width, std::size_t& grid_height);
  ~Game();

  //Rule of 5 Implementation
  Game(const Game& other) = delete; 
  Game& operator=(const Game& other) = delete;
  Game(Game&& other) noexcept = delete;
  Game& operator=(Game&& other) noexcept = delete;

  void Run(Controller const &controller, Renderer &renderer,
           std::size_t& target_frame_duration);
  
  //Setters & Getters
  int GetScore() const;
  int GetSize() const;

 private:

  Snake snake;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score;

  // New member variables for poison food mechanics
    SDL_Point poison_food_;              
    bool is_poison_food_active_;         
    bool is_snake_poisoned_;             
    float original_speed_;
    bool terminate_;           
    
    // Thread synchronization members
    std::mutex poison_mutex_;            
    std::condition_variable poison_cv_;  
    std::thread poison_food_thread_;

  // New member functions for poison food logic
    void PoisonFoodTimer();             
    void StartPoisonFoodThread();       
    void PlacePoisonFood();             

  void PlaceFood();
  void Update();
};

#endif