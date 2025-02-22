#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <fstream>  
#include <sstream>  

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
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void Update();
};

#endif