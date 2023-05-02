# Group_8-engg1340comp2113
This is Group 8 of the course engg1340andcomp2113.


## Contributors
Cao Nuanyu, Liu Yihan, Wu Haoyuan, Wu Lixin, Yang Liuqing

## Description
"Pac-Man+" is a game created by imitating the classic game Pac-Man. In this game, players take on the role of the titular character, a yellow circle that moves through a maze while eating small cookies, large cookies, and fruits to score points and progress to the next level. Along the way, they must avoid being caught by  ghosts that continually chase after them, losing a life if caught. The game ends when all lives are lost. Difficulty increases as levels progress, with faster and more numerous ghosts.

## Game Rules
1. At the beginning of the game, the player controls the role to move through the maze and eat all the cookies and fruits. Eating fruits can earn additional points. As the game progresses, fruits become more valuable.
2. There are several ghosts in the maze that constantly chase after the player. If the player is caught by a ghost, he loses a life. When all lives are lost, the game ends.
3. There are several big cookies in the maze. If the player eats it, all the ghosts become vulnerable. The player can eat the ghosts obtaining extra socres, and the ghost eaten will return to the starting point and wait to recover.
4. There are several warp tunnels in the maze. The player can move from one warp tunnel to the other, quickly moving to different parts of the maze.

## Key Features
1. Simple and easy-to-learn game rules that make it easy for everyone to get started.
2. Unique character design, each with its own personality.
3. Diverse map design, increasing the fun and challenge of the game.
4. Gradually increasing difficulty levels that require players to constantly improve their proficiency in this game.
5. Abundant carefully-designed toolsets giving players refreshed game experience every time.

## Requirement Implementation
### 1. Generation of random game sets or events

* The map of the maze at each level is random. Each level has 3 different maps. At the beginning of each game, the program will select a random map under that level where the player is currently on. Then the game starts.<br>
* The bonus tools are randomly generated and appear in random coordinates on the maze.<br>
* The movement of ghosts is partially random. They do not move in a fixed direction at each step, but are roughly close to the player's current position.<br>

### 2. Data structures for storing game status

The coordinates of the Pacman and ghosts are stored in the map's 2-D "vals" vector and updated by directly modifying the corresponding elements in the vector after every game loop. The parameters of the characters are stored in their classes, respectively. For example, the "Pacman" class stores variables including numbers of Pacman's lives and eaten ghosts.

### 3. Dynamic memory management

The information of pacman and ghosts are stored and updated in dynamic memory. In class Game, pacman and each ghost are allocated with new memory when initializing them under class constructors, and memory spaces are deleted everytime game terminates.

### 4. File input/output

File input and output are implemented in the game status storage. Game status is loaded by the constructor Game::Game() and saved by the saveToFile() function under class Game. All the information about the map coordinates and characters' status are stored in "temp.txt" under the data folder. When the player exits the game, data will be written to the file, and when restarting the game, previous data will be loaded from the file. 

### 5. Program codes in multiple files

Program codes are split into different files according to their functions and sorted under different folders.<br>
* The ***data*** folder contains a .txt file storing the records of players' scores. 
* The ***map*** folder holds ten .txt files storing all the initial game maps. <br>
Maps are divided into three levels, each level contains three .txt files under it. <br>
In addition, there is a bonus map.
* The ***src*** folder contains C++ and header files storing the primary game code. <br>
It contains 5 parts in total: <br>
characters.cpp & characters.h – Supports characteristics of ghosts and players <br>
game.cpp & game.h – Supports game flow <br>
main.cpp – Integrate the code needed to run the entire game <br>
tools.cpp & tools.h – Supports some of the functions often used in games including MapShow, ScoreUpdate, GetFilePath, etc.<br>
ui.cpp & ui.h – Supports operations related with ui <br>
* The ***ui*** folder stores all the designs of UI. <br>
Including the Start Menu window, How to Play window, High Scores window, In_Game window, and Pause window.

## Libraries

* C++ libraries being used include: <br>
ctime, chrono, iostream, fstream, sstream, filesystem, cstdlib, vector, string, cmath, algorithm

* Additional library being used include: <br>
ncurses-6.4

## Instructions
// Compilation and executions


