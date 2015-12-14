#ifndef _REFEREE_HH
# define _REFEREE_HH

# include <utility>
# include <string>
# include <iostream>
# include <vector>
# include "Board.hh"
# include "Error.hh"

# define DOUBLE_THREE_RULE	2
# define SEARCH_RADIUS		3

# define MIN(a, b)	(((a) < (b)) ? (a) : (b))
# define MAX(a, b)	(((a) > (b)) ? (a) : (b))

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
		SECONDARY_DIAGONAL,
		NONE
	};

private:
	void			_checkDoubleThree(int x, int y, char player) const;
	void			_checkWinner();
	void			_checkCapturedPawn(int x, int y) const;
	unsigned int	_countFreeThree(int x, int y, char player) const;
	bool			_freeThree(int x, int y, char player, enum direction dir) const;
	int				_lineSum(char c1, char c2, char c3, char c4, char player) const;
	bool			_fivePieceAligned(int x, int y, std::vector<enum direction> &dirs) const;
	bool			_isBreakable(int x, int y, enum direction dir) const;
	void			_removeDoubleThree(int x1, int y1, int x2, int y2, char player) const;

private:

	Board				&_board;
	int					_winner;
	std::pair<int, int> _lastPiece;
	std::vector<int>	_jail;

};

#endif // _REFEREE_HH