#ifndef _REFEREE_HH
# define _REFEREE_HH

# include <utility>
# include <string>
# include <iostream>
# include "Board.hh"
# include "Error.hh"

# define DOUBLE_THREE_RULE	2
# define SEARCH_RADIUS		3

class Referee
{
public:

	Referee(Board &board);
	~Referee();

public:

	int		getWinner() const;


public:

	void	putPieceOnBoard(int x, int y, char player);

private:
	enum direction
	{
		LINE,
		COLUMN,
		MAIN_DIAGONAL,
		SECONDARY_DIAGONAL
	};

private:
	void			_checkDoubleThree(int x, int y, char player) const;
	void			_checkBreakableFive() const;
	void			_checkCapturedPawn() const;
	unsigned int	_countFreeThree(int x, int y, char player) const;
	bool			_freeThree(int x, int y, char player, enum direction dir) const;
	int				_lineSum(char c1, char c2, char c3, char c4, char player) const;

private:

	Board				&_board;
	int					_winner;
	std::pair<int, int> _lastPiece;

};

#endif // _REFEREE_HH