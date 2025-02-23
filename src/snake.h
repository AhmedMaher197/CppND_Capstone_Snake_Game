#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"



class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  template<typename T>
  struct Position
  {
    T x;
    T y;
  };

  Snake(int, int);
  ~Snake() = default;

  Snake(const Snake& other) = delete; 
  Snake& operator=(const Snake& other) = delete;
  Snake(Snake&& other) noexcept = delete;
  Snake& operator=(Snake&& other) noexcept = delete;

  void Update();
  void GrowBody();
  bool SnakeCell(int x, int y);
  bool SnakeCell(SDL_Point);
  bool SnakeCell(Snake::Position<float>);

  void IncreaseSpeed();
  int GetSize() const;
  bool IsSnakeAlive() const;
  Position<float> GetSnakeHeadPosition () const;
  Direction GetSnakeDirection() const;
  void SetSnakeDirection (const Direction&);
  std::vector<SDL_Point> body_;


 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  int grid_width_;
  int grid_height_;
  bool growing_;
  float speed_;
  int size_;
  bool alive_;
  Position<float> snake_head_position_;
  Direction direction_;
};

#endif