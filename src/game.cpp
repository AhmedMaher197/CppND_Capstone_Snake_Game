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

//File I/O Operations
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

//File I/O Operations
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
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      score{0},
      poison_food_{},                    
      is_poison_food_active_{false},     
      is_snake_poisoned_{false},         
      original_speed_{0.1f},             
      poison_mutex_{},                   
      poison_cv_{},                      
      poison_food_thread_{} 
{
  PlaceFood();
}


void Game::PlacePoisonFood() 
{
    int x, y;
    while (true) 
    {
        x = random_w(engine);
        y = random_h(engine);
        
        // Thread-safe check for valid position
        std::lock_guard<std::mutex> lock(poison_mutex_);
        if (!snake.SnakeCell(x, y) && !(food.x == x && food.y == y)) {
            poison_food_ = {x, y};
            break;
        }
    }
}

void Game::StartPoisonFoodThread() 
{
    if (!is_poison_food_active_) 
    {
        PlacePoisonFood();
        is_poison_food_active_ = true;
        
        // Start the timer thread
        poison_food_thread_ = std::thread(&Game::PoisonFoodTimer, this);
    }
}

void Game::PoisonFoodTimer() {
    const int poison_duration = 5;  // Poison food stays for 5 seconds
    auto start_time = std::chrono::high_resolution_clock::now();
    
    std::unique_lock<std::mutex> lock(poison_mutex_);
    
    while (!should_terminate_ && is_poison_food_active_) {
        lock.unlock();
        
        auto current_time = std::chrono::high_resolution_clock::now();
        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(
            current_time - start_time).count();
            
        if (elapsed_seconds >= poison_duration) {
            std::lock_guard<std::mutex> remove_lock(poison_mutex_);
            is_poison_food_active_ = false;
            poison_food_ = {-1, -1};  // Move off screen
            break;
        }
        
        lock.lock();
        poison_cv_.wait_for(lock, std::chrono::milliseconds(800));
    }
}

Game::~Game() 
{
  {
      std::lock_guard<std::mutex> lock(poison_mutex_);
      should_terminate_ = true;
  }
  poison_cv_.notify_one();
  
  if (poison_food_thread_.joinable()) {
      poison_food_thread_.join();
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
    renderer.Render(snake, food, poison_food_, is_poison_food_active_);

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

// Modify Update() to include poison food logic
void Game::Update() {
    if (!snake.IsSnakeAlive()) return;

    snake.Update();
    
    // Check if it's time to spawn new poison food (every 10 seconds)
    static Uint32 last_poison_spawn = SDL_GetTicks();
    Uint32 current_time = SDL_GetTicks();
    
    if (current_time - last_poison_spawn >= 10000) 
    {
        StartPoisonFoodThread();
        last_poison_spawn = current_time;
    }

    // Handle regular food collision (existing code)
    auto snake_head_pos = snake.GetSnakeHeadPosition();
    int new_x = static_cast<int>(snake_head_pos.x);
    int new_y = static_cast<int>(snake_head_pos.y);

    if (food.x == new_x && food.y == new_y) {
        score++;
        PlaceFood();
        snake.GrowBody();
        snake.IncreaseSpeed();
    }

    // Add poison food collision handling
    {
        std::lock_guard<std::mutex> lock(poison_mutex_);
        if (is_poison_food_active_ && 
            poison_food_.x == new_x && poison_food_.y == new_y) {
            
            if (!is_snake_poisoned_) {
                is_snake_poisoned_ = true;
                original_speed_ = snake.GetSpeed();
                snake.SetSpeed(original_speed_ * 0.5f);  // Slow down by 50%
                
                // Create thread to remove poison effect after 3 seconds
                std::thread([this]() {
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                    is_snake_poisoned_ = false;
                    snake.SetSpeed(original_speed_);
                }).detach();
            }
            
            is_poison_food_active_ = false;
            poison_food_ = {-1, -1};
        }
    }
}


int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.GetSize(); }