//
// Board.cpp for gomoku in /home/guiz/School/rendu/gomoku/src/
//
// Made by Guiz
// Login   <briard_g@epitech.eu>
//
// Started on  Thu Dec  3 14:21:21 2015 Guiz
// Last update Thu Dec  3 17:19:23 2015 Guiz
//

#include "Board.hh"

Board::Board() {
  memset(_board, EMPTY, B_SIZE);
  //_graphicBoard;
}

Board::~Board() {}

const char*	Board::getBoard() const {
  return (_board);
}

char		Board::getCase(int x, int y) const {
  return (_board[POS(x,y)]);
}

char    Board::operator[](unsigned int position) const {

  return _board[position];
}

bool		Board::setCase(int x, int y, char status) {
  if (_board[POS(x,y)] == INHIBITED)
    return (false);
  _board[POS(x,y)] = status;
  return (true);
}

bool		Board::boardIsFull() const {
  bool		check;

  check = true;
  for (int i = 0 ; i <= B_SIZE ; i++) {
    if (_board[i] == EMPTY)
      check = false;
  }
  return (check);
}
