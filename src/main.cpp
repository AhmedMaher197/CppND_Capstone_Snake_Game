#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {

  GameConfig config("snake_config.txt");
  auto game_settings = config.GetGameSettings();

  Renderer renderer(game_settings.screen_width,
                    game_settings.screen_height, 
                    game_settings.grid_width, 
                    game_settings.grid_height);

  Controller controller;

  Game game(game_settings.grid_width, game_settings.grid_height);
  game.Run(controller, renderer, game_settings.ms_per_frame);

  std::cout << "Game has terminated successfully!\n";

  if(config.GetHighestScore() < game.GetScore())
  {
    std::cout << "Congrats, a New Score Has been achieved.!\n";
    config.SetNewHighScore(game.GetScore());
  }

  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  
  config.SaveConfig("snake_config.txt");
  return 0;
}