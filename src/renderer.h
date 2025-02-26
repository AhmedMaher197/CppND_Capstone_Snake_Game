#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include <memory>
class Renderer {
 public:
  Renderer(const std::size_t& screen_width, const std::size_t& screen_height,
           const std::size_t& grid_width, const std::size_t& grid_height);
  ~Renderer();

  Renderer(const Renderer& other) = delete; 
  Renderer& operator=(const Renderer& other) = delete;
  Renderer(Renderer&& other) noexcept = delete;
  Renderer& operator=(Renderer&& other) noexcept = delete;

  void Render(Snake const& snake, 
                SDL_Point const& food,
                SDL_Point const& poison_food,
                bool is_poison_food_active);
  void UpdateWindowTitle(int& score, int& fps);

 private:
  SDL_Window* sdl_window;
  SDL_Renderer* sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif