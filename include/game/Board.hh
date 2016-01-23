//
// Board.hpp for gomoku in /home/guiz/School/rendu/gomoku/include/
//
// Made by Guiz
// Login   <briard_g@epitech.eu>
//
// Started on  Thu Dec  3 14:17:41 2015 Guiz
// Last update Thu Dec  3 17:19:17 2015 Guiz
//

#ifndef BOARD_HPP_
# define BOARD_HPP_

# include <cstring>
# include <algorithm>

// DEBUG
# include <chrono>

// Macros
# define MAX_HEIGHT	19
# define MAX_WIDTH	19
# define B_SIZE     (MAX_HEIGHT * MAX_WIDTH)
# define JAIL_SIZE  2
# define WIN_SIZE   1
# define EMPTY		  '0'
# define PLAYER1    '1'
# define PLAYER2    '2'
# define INHIBITED1	'3'
# define INHIBITED2 '4'
# define POS(x,y)	  ((MAX_WIDTH * y) + x)

class			Board {
private:
  char			_board[B_SIZE + JAIL_SIZE + WIN_SIZE];

public:
  // Ctor & Dtor
  Board();
  ~Board();

  // Getters
  const char*		getBoard() const;
  char			    getCase(int, int) const;
  char          getWinnerCase() const;
  const char*   getJail() const;
  char          getJail(char player) const;

  // Setters
  void			setCase(int, int, char);
  void      setBoard(char const *);
  void      setWinnerCase(char winner);

  // Board routines
  bool			boardIsFull() const;
  void      addInJail(unsigned short int player);
  void      reset();

public:

  char&      operator[](unsigned int position);
};

#endif // !BOARD_HH_
