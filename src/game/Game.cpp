//
// Game.cpp for gomoku in /home/guiz/School/rendu/gomoku/src/
//
// Made by Guiz
// Login   <briard_g@epitech.eu>
//
// Started on  Tue Dec 15 01:24:20 2015 Guiz
// Last update Tue Dec 15 05:13:39 2015 Guiz
//

#include "Game.hh"

Game::Game() {
  _gameMode = Game::oneOnOne;
}

Game::~Game() {}

Game::gm	Game::getGameMode() const {
  return (_gameMode);
}

void	Game::setGameMode(Game::gm mode) {
  _gameMode = mode;
}

void	Game::setGameOver(bool gameOver) {
  _gameOver = gameOver;
}
