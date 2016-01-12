//
// Player.cpp for gomoku in /home/guiz/School/rendu/gomoku/src/
//
// Made by Guiz
// Login   <briard_g@epitech.eu>
//
// Started on  Thu Dec 10 05:17:13 2015 Guiz
// Last update Thu Dec 10 05:19:38 2015 Guiz
//

#include "Player.hh"

Player::Player() {
  _hasWon = false;
}

bool	Player::getHasWon() const {
  return (_hasWon);
}

void	Player::setHasWon(bool result) {
  _hasWon = result;
}
