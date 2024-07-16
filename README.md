# Snake Game in Console

## Description
This is a simple Snake game implemented in C++ for the Windows console. The game involves controlling a snake to eat apples, grow in length, and avoid running into walls or itself. It is designed to demonstrate basic game development principles, including real-time input handling, game loop, and collision detection in a console environment.

## How to Run
0. Make sure you are using Windows.
1. Clone the repository.
2. Make sure your terminal has a size of 120 x 40.
3. Compile the code by running `g++ -std=c++17 *.cpp -o ./build/main` in the terminal from the root directory.
4. Start the game by running `./build/main`.

## Game Structure
### main.cpp
The `main.cpp` file contains the core logic of the game, including the game loop, input handling, and rendering.

#### Classes
1. **Snake**: The `Snake` class represents the snake in the game. It manages the snake's body parts, movement direction, and growth when it eats an apple.

    - **Methods**:
        - `getHead()`: Returns the position of the snake's head.
        - `getTail()`: Returns the position of the snake's tail.
        - `setDirection(std::pair<int, int> newDir)`: Sets the snake's movement direction, ensuring it doesn't reverse.
        - `move()`: Moves the snake in the current direction.
        - `getParts()`: Returns the list of the snake's body parts.
        - `insertPart()`: Adds a new part to the snake's body, simulating growth.
        - `pointBelongs(std::pair<int, int> point)`: Checks if a given point is part of the snake's body.

### Game Mechanics
- **Initialization**: The game initializes the console screen buffer, hides the cursor, and sets up random number generation for placing apples.
- **Main Loop**: The game runs in an infinite loop, controlling the frame rate and handling player input, game updates, and rendering.
- **Input Handling**: Player controls are managed using the `W`, `A`, `S`, `D` keys for movement in up, left, down, and right directions, respectively. Input is processed asynchronously.
- **Game Updates**: The snake moves in the current direction, checks for collisions with walls or itself, and grows when it eats an apple.
- **Rendering**: The game renders the snake, apples, and other elements on the console screen using ASCII characters.
- **Game Over**: If the snake collides with the wall or itself, the game ends, displaying a game over message and the snake's final length.

### Main Function
- **Setup Console**: Initializes the console screen buffer and hides the cursor.
- **Random Apple Placement**: Uses random number generation to place the apple on the screen, ensuring it doesn't overlap with the snake's body.
- **Game Loop**:
    - **Frame Rate Control**: Ensures the game runs at a consistent frame rate by controlling the duration of each frame.
    - **Input Handling**: Reads player input to change the snake's direction.
    - **Game Update**: Moves the snake, checks for collisions, and handles apple consumption.
    - **Rendering**: Draws the snake, apples, and other elements on the console screen.
- **Game Over Handling**: Displays a game over message and waits for the player to press 'E' to exit.

## Conclusion
This Snake game in the console is a basic demonstration of game development principles using C++ and ASCII art. It provides a foundation for further development and experimentation with more complex game mechanics and features.
