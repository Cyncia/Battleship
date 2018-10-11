# Battleship
My first repository

Project 3 for CS 32 at UCLA

For this project, I had to complete a battle ship game in C++ that consists of four different player types and three different game modes. For each player, there exists a board that is private to him/her only. The game requires that all players to place all existing ships onto their respective board before starting. Once all ships are placed, the game begins with one player attacking a cell on the opponent's board. Each player alternates when attacking regardless of hit or miss. Two display boards are produced during each turn - one displays the opponent's board before any attack has occurred and the other after an attack. Instructions and statuses are updated and provided each turn.

The four player types are:
  1. Awful Player
  2. Mediocre Player
  3. Good Player
  4. Human Player

There are three different game modes to choose from:
  1. A mini-game between two mediocre players
  2. A mediocre player against a human player
  3. A 10-game consecutive match between a mediocre and an awful player

Board sizes can be adjusted when constructing a game in the main function within main.cpp
The current board size for a game is 10x10. The board size for a mini-game is 2x3. The board is formatted as a rxc rectangle with r number of rows and c number of columns. The rows and columns of the board are numbered from 0 to r-1 and 0 to c-1, respectively.

The number of ships can also be adjusted with the addShip function located in Game.cpp. There is also a addStandardShips function, within main.cpp, which uses a pre-existing set of ships for the game instead of manually adding ships one by one. Both functions can also be used simultaneously within the same game.

NOTE: Please do NOT copy and/or use this code
