//
// Board.cpp for gomoku in /home/guiz/School/rendu/gomoku/src/
//
// Made by Guiz
// Login   <briard_g@epitech.eu>
//
// Started on  Thu Dec  3 14:21:21 2015 Guiz
// Last update Thu Dec  3 17:19:23 2015 Guiz
//

#include <iostream>
#include "Board.hh"

// Ctor & Dtor
Board::Board() {
  std::memset(_board, EMPTY, B_SIZE);
  std::memset(_board + B_SIZE, 0, JAIL_SIZE + WIN_SIZE);
}

Board::~Board() {}

// Getters
const char*	Board::getBoard() const {
  return (_board);
}

char		Board::getCase(int x, int y) const {
  if (x < 0 || y < 0 || x >= MAX_WIDTH || y >= MAX_HEIGHT)
    return (-1);
  return (_board[POS(x,y)]);
}

char    Board::getWinnerCase() const {
  return (_board[B_SIZE + JAIL_SIZE]);
}

const char*   Board::getJail() const {
  return (_board + B_SIZE);
}

char          Board::getJail(char player) const {
  return (_board[B_SIZE + player - 1]);
}

// Setters
void		Board::setCase(int x, int y, char status) {
  if (x < 0 || y < 0 || x >= MAX_WIDTH || y >= MAX_HEIGHT)
    return;
  _board[POS(x,y)] = status;
}

void    Board::setBoard(char const *board) {
  std::memcpy(_board, board, B_SIZE + JAIL_SIZE + WIN_SIZE);
}

void    Board::setWinnerCase(char winner) {
  _board[B_SIZE + JAIL_SIZE] = winner;
}

// Board routines
char&    Board::operator[](unsigned int position) {
  return _board[position];
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

void      Board::addInJail(unsigned short int player) {
  if (player > 2)
    return;
  _board[B_SIZE + player - 1] += 2;
}

void      Board::reset() {
  std::memset(_board, EMPTY, B_SIZE);
  std::memset(_board + B_SIZE, 0, JAIL_SIZE + WIN_SIZE);
}
