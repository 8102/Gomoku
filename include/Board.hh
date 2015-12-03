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

// Macros
# define MAX_HEIGHT	19
# define MAX_WIDTH	19
# define B_SIZE		(MAX_HEIGHT * MAX_WIDTH)
# define EMPTY		'0'
# define PLAYER1	'1'
# define PLAYER2	'2'
# define INHIBITED	'3'
# define POS(x,y)	((MAX_WIDTH * y) + x)

class			Board {
private:
  char			_board[B_SIZE];
  //Graphic		_graphicBoard;

public:
  // Ctor & Dtor
  Board();
  ~Board();

  // Getters
  const char*		getBoard() const;
  char			getCase(int, int) const;

  // Setters
  bool			setCase(int, int, char);

  // Board routines
  bool			boardIsFull() const;
};

#endif // !BOARD_HH_
