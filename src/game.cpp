#include "game.h"
#include <iostream>
#include "SDL.h"


GameConfig::GameConfig(const std::string& config_file) : highest_score_{},
                                                         game_settings_{
                                                                         60,                   // frames_per_second - standard refresh rate
                                                                         1000/60,              // ms_per_frame - calculated from FPS
                                                                         640,                  // screen_width - default window width
                                                                         640,                  // screen_height - default window height
                                                                         32,                   // grid_width - default game grid width
                                                                         32                    // grid_height - default game grid height
                                                                     }

{ 
  LoadConfig(config_file);
}

void GameConfig::LoadConfig(const std::string& file_name) 
{
    std::ifstream config_file(file_name);
    std::string line;

    if (!config_file.is_open()) {
        std::cerr << "Could not open config file: " << file_name << "\n";
        std::cerr << "Using default values\n";
        return;
    }

    while (std::getline(config_file, line)) 
    {
        if (line.empty() || line[0] == '-') continue;
            
        std::istringstream line_stream(line);
        std::string key;
            
        std::getline(line_stream, key, ':');
            
        if (key == "FramePerSeconds") {
            line_stream >> game_settings_.frames_per_second;
        } else if (key == "ScreenWidth") {
            line_stream >> game_settings_.screen_width;
        } else if (key == "ScreenHeight") {
            line_stream >> game_settings_.screen_height;
        } else if (key == "GridWidth") {
            line_stream >> game_settings_.grid_width;
        } else if (key == "GridHeight") {
            line_stream >> game_settings_.grid_height;
        } else if (key == "HighestScore") {
            line_stream >> highest_score_;
        }
    }
}

void GameConfig::SaveConfig(const std::string& filename) 
{
        std::ofstream config_file(filename, std::ios::out | std::ios::trunc);
    
    if (!config_file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << "\n";
        return;
    }

    // Write content with error checking
    config_file << "Game Settings:\n"
               << "--------------\n"
               << "FramePerSeconds: " << game_settings_.frames_per_second << "\n"
               << "ScreenWidth: " << game_settings_.screen_width << "\n"
               << "ScreenHeight: " << game_settings_.screen_height << "\n"
               << "GridWidth: " << game_settings_.grid_width << "\n"
               << "GridHeight: " << game_settings_.grid_width << "\n\n\n";

    // Write the score section separately to ensure it's not being skipped
    config_file << "Game Score:\n"
               << "------------\n"
               << "HighestScore: " << highest_score_ << "\n";

    // Force the write to disk
    config_file.flush();
    
    // Check if any errors occurred during writing
    if (config_file.fail()) {
        std::cerr << "Error occurred while writing to file\n";
    }

    config_file.close();
}

void GameConfig::SetNewHighScore(int new_score)
{
  highest_score_ = new_score;
} 

int GameConfig::GetHighestScore() const
{
  return highest_score_;
}

GameSettings GameConfig::GetGameSettings() const
{
  return game_settings_;
}

Game::Game(std::size_t& grid_width, std::size_t& grid_height)
    : snake(grid_width, grid_height),
      food{},
      running_{true},
      engine{dev()},
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      food_future_{},
      food_mutex_{}
{
  StartFoodPlacement();
}

Game::~Game() 
{
  running_ = false;
  if (food_future_.valid()) 
  {
    food_future_.wait();  
  }
}

void Game::StartFoodPlacement() 
{
  food_future_ = std::async(std::launch::async, 
                           &Game::CalculateNextFoodPosition, 
                           this);
}

Game::FoodPosition Game::CalculateNextFoodPosition() 
{
  FoodPosition position;
  position.valid = false;

  // Keep trying positions until we find a valid one or the game is stopping
  while (running_) 
  {
    position.x = random_w(engine);
    position.y = random_h(engine);

    // Check if position is valid (not occupied by snake)
    if (!snake.SnakeCell(position.x, position.y)) 
    {
      position.valid = true;
      return position;
    }
  }

  return position;  // Return invalid position if game is stopping
}

void Game::UpdateFoodPosition() 
{
  if (food_future_.valid() && 
      food_future_.wait_for(std::chrono::seconds(0)) == std::future_status::ready) 
  {
    
    auto new_position = food_future_.get();
    if (new_position.valid) {
      std::lock_guard<std::mutex> lock(food_mutex_);
      food.x = new_position.x;
      food.y = new_position.y;
    }
  }
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t& target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() 
{
  if (!snake.IsSnakeAlive()) return;

  snake.Update();

  auto snake_head_pos = snake.GetSnakeHeadPosition();
  int new_x = static_cast<int>(snake_head_pos.x);
  int new_y = static_cast<int>(snake_head_pos.y);

  // Check if snake has found food
  bool food_eaten = false;
  {
    std::lock_guard<std::mutex> lock(food_mutex_);
    if (food.x == new_x && food.y == new_y) {
      food_eaten = true;
    }
  }

  if (food_eaten) {
    score++;
    // Start calculating next food position asynchronously
    StartFoodPlacement();
    
    // Grow snake and increase speed
    snake.GrowBody();
    snake.IncreaseSpeed();
  }
  
  // Check if we need to update food position from async calculation
  UpdateFoodPosition();
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.GetSize(); }