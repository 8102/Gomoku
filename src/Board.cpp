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
  if (x < 0 || y < 0 || x >= MAX_WIDTH || y >= MAX_HEIGHT)
    return (-1);
  return (_board[POS(x,y)]);
}

char&    Board::operator[](unsigned int position) {

  return _board[position];
}

void		Board::setCase(int x, int y, char status) {
  _board[POS(x,y)] = status;
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
