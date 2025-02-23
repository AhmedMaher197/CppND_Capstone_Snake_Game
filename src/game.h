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

  Game(const Game& other) = delete; 
  Game& operator=(const Game& other) = delete;
  Game(Game&& other) noexcept = delete;
  Game& operator=(Game&& other) noexcept = delete;

  void Run(Controller const &controller, Renderer &renderer,
           std::size_t& target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:

  struct FoodPosition 
  {
    int x;
    int y;
    bool valid;  
  };

  Snake snake;
  SDL_Point food;
  std::atomic<bool> running_; 

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  
  std::future<FoodPosition> food_future_;  // Holds the future food position
  mutable std::mutex food_mutex_;          // Protects food position updates

  // Food placement methods
  void StartFoodPlacement();               // Initiates async food calculation
  FoodPosition CalculateNextFoodPosition();// Worker thread function
  void UpdateFoodPosition();               // Checks and updates food position
  void PlaceFood();
  void Update();
};

#endif