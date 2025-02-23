#include "snake.h"
#include <cmath>
#include <iostream>

Snake::Snake(int grid_width_, int grid_height_)
      : grid_width_(grid_width_),
        grid_height_(grid_height_),
        growing_{},
        speed_{0.1f},
        size_{1},
        alive_{true},
        snake_head_position_{grid_width_ / 2.0f, grid_height_ / 2.0f},
        direction_{Direction::kUp}
        {}


void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(snake_head_position_.x),
      static_cast<int>(
          snake_head_position_.y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(snake_head_position_.x),
      static_cast<int>(snake_head_position_.y)};  // Capture the head's cell after updating.

  // Update all of the body_ vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead() {
  switch (direction_) {
    case Direction::kUp:
      snake_head_position_.y -= speed_;
      break;

    case Direction::kDown:
      snake_head_position_.y += speed_;
      break;

    case Direction::kLeft:
      snake_head_position_.x -= speed_;
      break;

    case Direction::kRight:
      snake_head_position_.x += speed_;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  snake_head_position_.x = fmod(snake_head_position_.x + grid_width_, grid_width_);
  snake_head_position_.y = fmod(snake_head_position_.y + grid_height_, grid_height_);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body_.push_back(prev_head_cell);

  if (!growing_) {
    // Remove the tail from the vector.
    body_.erase(body_.begin());
  } else {
    growing_ = false;
    size_++;
  }

  // Check if the snake has died.
  for (auto const &item : body_) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive_ = false;
    }
  }
}

void Snake::GrowBody() { growing_ = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(snake_head_position_.x) && y == static_cast<int>(snake_head_position_.y)) {
    return true;
  }
  for (auto const &item : body_) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}

bool Snake::SnakeCell(Snake::Position<float> pos)
{
  if (pos.x == static_cast<int>(snake_head_position_.x) && pos.y == static_cast<int>(snake_head_position_.y)) {
    return true;
  }
  for (auto const &item : body_) {
    if (pos.x == item.x && pos.y == item.y) {
      return true;
    }
  }
  return false;
}

bool Snake::SnakeCell(SDL_Point point) {
    if (point.x == static_cast<int>(snake_head_position_.x) && 
        point.y == static_cast<int>(snake_head_position_.y)) {
        return true;
    }
    for (auto const &item : body_) {
        if (point.x == item.x && point.y == item.y) {
            return true;
        }
    }
    return false;
}

void Snake::IncreaseSpeed()
{
  speed_ += 0.02;
}

int Snake::GetSize() const
{
   return size_;
}

bool Snake::IsSnakeAlive() const
{
  return alive_;
}

Snake::Position<float> Snake::GetSnakeHeadPosition () const
{
  return snake_head_position_;
}

Snake::Direction Snake::GetSnakeDirection() const
{
  return direction_;
}

void Snake::SetSnakeDirection (const Direction& direction)
{
  direction_ = direction;
}

