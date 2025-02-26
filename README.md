
## Project Introduction
This project creates a modern implementation of the classic Snake game using C++ and SDL2. The game features a snake that players control to collect food and grow longer while avoiding collisions. The implementation demonstrates various C++ programming concepts including object-oriented design, file operations, and user input handling.


# Snake Game Features Documentation
## Game Overview
The Snake game is implemented as a modern C++ application that combines classic gameplay mechanics with advanced technical features. The game provides a responsive and smooth gaming experience through its multi-threaded architecture and efficient resource management.

## Core Gameplay Features

### Snake Movement and Control
The game implements smooth snake movement using floating-point position tracking, allowing for precise control and fluid animation. The snake's movement is controlled using the arrow keys, with the following characteristics:
- Continuous movement in the current direction
- Immediate response to direction changes
- Prevention of 180-degree turns when the snake is longer than one segment
- Wrap-around movement when reaching screen boundaries

### Growth and Scoring System
The snake grows longer each time it consumes food, with the following mechanics:
- Each food item increases the snake's length by one segment
- Score increases with each food item collected
- The snake's speed gradually increases as it grows longer, adding to the challenge
- High scores are persistently stored between gaming sessions

### Collision Detection
The game features comprehensive collision detection that monitors:
- Collisions between the snake and itself
- Boundary interactions for wrap-around movement
- Food collection detection with precise positioning

### Posion Food
The sanke speed will be decrease by 50 % as if the user eats posionous food.

This implementation provides a solid foundation for a classic game while incorporating modern programming practices and features that enhance both the gaming experience and code maintainability.
## Loops, Functions, and I/O Implementation Details

### 1. Understanding of C++ Functions and Control Structures
The project demonstrates comprehensive understanding of C++ functions and control structures through several key implementations:

Game Loop Implementation:
[game.cpp][Line: 205]
[Game::Run]
This method implements the main game loop with sophisticated control structures for managing frame timing, game state updates, and rendering coordination. The function demonstrates mastery of loop control, conditional statements, and function composition.

### 2. File I/O Operations
The project implements file operations through its configuration management system:

Configuration Loading:
[game.cpp][Line: 20]
[GameConfig::LoadConfig]
This method demonstrates file input operations by reading game settings from a configuration file. It implements error handling, data validation, and proper file stream management.

Score Saving:
[game.cpp][Line: 57]
[GameConfig::SaveConfig]
This method handles file output operations by saving game settings and high scores. It showcases proper file handling with error checking and formatted data writing.

### 3. User Input Processing
The project features robust input handling through its controller system:

Input Handling:
[controller.cpp][Line: 12]
[Controller::HandleInput]
This method processes user input in a thread-safe manner, demonstrating proper input validation and game state updates based on user actions.

### 4. Data Structures and Variables
Snake Data Management:
[snake.h][Line: 48]
[Snake::body_]
The implementation uses vectors to store the snake's body segments and employs constant variables for grid dimensions, demonstrating effective use of STL containers and immutable variables.


## Object Oriented Programming Implementation Details

### 1. Class Access Specifiers and Organization
The project implements multiple classes with appropriate access specifiers and well-organized data members:

Snake Class Structure:
[snake.h][Line: 7-63]
[Snake Class]
The Snake class demonstrates proper encapsulation with private member variables for internal state (grid dimensions, speed, size) and public methods for external interaction. All data members are explicitly specified with appropriate access levels.

Game Class Organization:
[game.h][Line: 47-97]
[Game Class]
The Game class shows clear separation of public interface and private implementation details. Member data subject to invariants, such as the random number generator and game state, is properly encapsulated with private access.

### 2. Constructor Implementation
The constructors in the project utilize member initialization lists effectively:

Game Constructor:
[game.cpp][Line: 106]
[Game::Game]
The constructor demonstrates proper member initialization using initialization lists, setting up game state and random number generation components.

Snake Constructor:
[snake.cpp][Line: 4]
[Snake::Snake]
Shows comprehensive member initialization, establishing the initial state of the snake with all necessary parameters.

### 4. Function Overloading
The project demonstrates effective use of function overloading:

Snake Position Checking:
[snake.h][Line: 31-34]
[Snake::SnakeCell]
Implements three overloaded versions of SnakeCell to handle different parameter types (coordinates, SDL_Point, and Position template), showing proper function overloading.

### 5. Templates
The project utilizes templates for generic programming:

Position Template:
[snake.h][Line: 11-17]
[Position Template]
Implements a generic Position template that can work with different numeric types, demonstrating effective use of templates for flexible coordinate handling.

## Memory Management Implementation Details

### 1. Use of References in Function Declarations
The project extensively uses references to avoid unnecessary copying and ensure efficient memory usage:

Game Run Method:
[game.cpp][Line: 196]
[Game::Run]
This method takes both the controller and renderer as references, demonstrating efficient parameter passing for large objects. The target_frame_duration parameter is also passed by reference to allow modifications.

Snake Direction Control:
[controller.cpp][Line: 6]
[Controller::ChangeDirection]
Takes the snake object as a reference parameter, allowing direct manipulation of the snake's state without copying. The direction parameters are passed as const references to prevent modifications while avoiding copies.

### 2. Destructor Implementation
The project implements destructors appropriately for classes managing resources:

Renderer Cleanup:
[renderer.cpp][Line: 36]
[Renderer::~Renderer]
Handles the proper destruction of SDL resources, demonstrating RAII principles by cleaning up window and renderer objects.

### 4. Rule of Five Implementation
The project properly implements move semantics and prevents unwanted copying:

Game Class:
[game.h][Line: 52]
[Game Class]
Explicitly deletes copy constructor and assignment operator while providing proper resource management through destructor implementation.

Controller Class:
[controller.h][Line: 13]
[Controller Class]
Implements the Rule of Five by explicitly deleting copy operations and move operations, ensuring proper resource management for thread-handling components.

## Concurrency Implementation Details

### 1. Multithreading Implementation
The project implements multithreading to handle user input processing separately from the main game loop, ensuring responsive gameplay:

Posinous Food Placement:
[game.cpp][Line: 154]
[StartPoisonFoodThread]
This method start a thread to execute PoisonFoodTimer which start conting how long the poisonous food should be available on the map then its position shall be changed.

### 3. Mutex and Lock Implementation
The project uses mutexes to protect shared resources:

[game..h][Line: 86]
[Game::poison_mutex_]
Implements a mutex to protect shared input state data between the main thread and posion food thread.


### 3. Use condition Variable
The project use condition variable to synch thread execution

[game.h][Line: 87]
[Game::poison_cv_]
The lock parameter makes sure no other part of the program interferes with our poison food state while we're checking it.

## Dependencies
- SDL2 library
- C++17 or higher
- CMake for building

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.


## CC Attribution-ShareAlike 4.0 International
Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
