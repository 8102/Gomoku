#include "Referee.hh"

/*
** Constructor / Destructor
*/

Referee::Referee(Board &board) : _board(board), _winner(0), _lastPiece(-1, -1), _jail(2, 0)
{

}

Referee::~Referee()
{

}

/*
** Getter / Setter
*/

int		Referee::getWinner() const
{
	return (_winner);
}

/*
** Public methodes
*/

void	Referee::putPieceOnBoard(int x, int y, char player)
{
	if (_board.getCase(x, y) == PLAYER1 || _board.getCase(x, y) == PLAYER2)
		throw NotEmptyError("This case is already use by player " + std::string(&player, 1));
	_checkDoubleThree(x, y, player);
	_checkCapturedPawn(x, y);
	_checkWinner();
}

/*
** Private methodes
*/

void	Referee::_checkDoubleThree(int x, int y, char player) const
{
	if (_board.getCase(x, y) == player + DOUBLE_THREE_RULE)
		throw DoubleThreeRule("Forbidden position on (" + std::to_string(x) + ", " + std::to_string(y) + ").");
	_board.setCase(x, y, player);
	for (int i = 0; i <= 2 * SEARCH_RADIUS; ++i)
	{
		for (int j = 0; j <= 2 * SEARCH_RADIUS; ++j)
		{
			int current_x = x - SEARCH_RADIUS + j, current_y = y - SEARCH_RADIUS + i;
			if (current_x >= 0 && current_y >= 0 && current_x < MAX_WIDTH && current_y < MAX_HEIGHT)
			{
				if (_board.getCase(current_x, current_y) ==  EMPTY)
				{
					_board.setCase(current_x, current_y, player); // simulation
					if  (_countFreeThree(current_x, current_y, player) >= 2)
						_board.setCase(current_x, current_y, player + DOUBLE_THREE_RULE);
					else
						_board.setCase(current_x, current_y, EMPTY); // annulation
				}				
			}
		}
	}
}

int		Referee::_lineSum(char c1, char c2, char c3, char c4, char player) const
{
	int sum = 0;

	if (c1 == PLAYER1 || c1 == PLAYER2)
		sum += (c1 == player ? 1 : -3);
	if (c2 == PLAYER1 || c2 == PLAYER2)
		sum += (c2 == player ? 1 : -3);
	if (c3 == PLAYER1 || c3 == PLAYER2)
		sum += (c3 == player ? 1 : -3);
	if (c4 == PLAYER1 || c4 == PLAYER2)
		sum += (c4 == player ? 1 : -3);
	return (sum);
}

bool	Referee::_freeThree(int x, int y, char player, enum direction dir) const
{
	int sp_x = x, sp_y = y;

	switch (dir)
	{
		case LINE:
			sp_x = x - SEARCH_RADIUS;
			break;
		case COLUMN:
			sp_y = y - SEARCH_RADIUS;
			break;
		case MAIN_DIAGONAL:
			sp_x = x - SEARCH_RADIUS;
			sp_y = y - SEARCH_RADIUS;
			break;
		case SECONDARY_DIAGONAL:
			sp_x = x + SEARCH_RADIUS;
			sp_y = y + SEARCH_RADIUS;
			break;
		default:
			std::cerr << "[ERROR]: In Referee instance -> _freeThree: unknown direction" << std::endl;
			break;
	}
	while (sp_x != x && sp_y != y)
	{
		switch (dir)
		{
			case LINE:
				if (_lineSum(_board.getCase(sp_x, sp_y),
				    		_board.getCase(sp_x + 1, sp_y),
				    		_board.getCase(sp_x + 2, sp_y),
				    		_board.getCase(sp_x + 3, sp_y),
				    		player) == 3 &&
					(_board.getCase(sp_x - 1, sp_y) == EMPTY ||
					_board.getCase(sp_x - 1, sp_y) + player == 5) &&
					(_board.getCase(sp_x + 4, sp_y) == EMPTY ||
					 _board.getCase(sp_x + 4, sp_y) + player == 5))
					return (true);
				sp_x += 1;
				break;
			case COLUMN:
				if (_lineSum(_board.getCase(sp_x, sp_y),
				    		_board.getCase(sp_x, sp_y + 1),
				    		_board.getCase(sp_x, sp_y + 2),
				    		_board.getCase(sp_x, sp_y + 3),
				    		player) == 3 &&
					(_board.getCase(sp_x, sp_y - 1) == EMPTY ||
					_board.getCase(sp_x, sp_y - 1) + player == 5) &&
					(_board.getCase(sp_x, sp_y + 4) == EMPTY ||
					 _board.getCase(sp_x, sp_y + 4) + player == 5))
					return (true);
				sp_y += 1;
				break;
			case MAIN_DIAGONAL:
				if (_lineSum(_board.getCase(sp_x, sp_y),
				    		_board.getCase(sp_x + 1, sp_y + 1),
				    		_board.getCase(sp_x + 2, sp_y + 1),
				    		_board.getCase(sp_x + 3, sp_y + 1),
				    		player) == 3 &&
					(_board.getCase(sp_x - 1, sp_y - 1) == EMPTY ||
					_board.getCase(sp_x - 1, sp_y - 1) + player == 5) &&
					(_board.getCase(sp_x + 4, sp_y + 4) == EMPTY ||
					 _board.getCase(sp_x + 4, sp_y + 4) + player == 5))
					return (true);
				sp_x += 1;
				sp_y += 1;
				break;
			case SECONDARY_DIAGONAL:
				if (_lineSum(_board.getCase(sp_x, sp_y),
				    		_board.getCase(sp_x - 1, sp_y - 1),
				    		_board.getCase(sp_x - 2, sp_y - 2),
				    		_board.getCase(sp_x - 3, sp_y - 3),
				    		player) == 3 &&
					(_board.getCase(sp_x + 1, sp_y + 1) == EMPTY ||
					_board.getCase(sp_x + 1, sp_y + 1) + player == 5) &&
					(_board.getCase(sp_x - 4, sp_y - 4) == EMPTY ||
					 _board.getCase(sp_x - 4, sp_y - 4) + player == 5))
					return (true);
				sp_x -= 1;
				sp_y -= 1;
				break;
			default:
				std::cerr << "[ERROR]: In Referee instance -> _freeThree: unknown direction" << std::endl;
				break;
		}		
	}
	return (false);
}

unsigned int	Referee::_countFreeThree(int x, int y, char player) const
{
	unsigned int	nbFreeThree = 0;

	if (_freeThree(x, y, player, LINE))
		nbFreeThree += 1;
	if (_freeThree(x, y, player, COLUMN))
		nbFreeThree += 1;
	if (_freeThree(x, y, player, MAIN_DIAGONAL))
		nbFreeThree += 1;
	if (_freeThree(x, y, player, SECONDARY_DIAGONAL))
		nbFreeThree += 1;
	return (nbFreeThree);
}

bool		Referee::_fivePieceAligned(int x, int y, std::vector<enum direction> &dirs) const
{
	unsigned char line(0), column(0), mainDiag(0), secondDiag(0);
	char player = _board.getCase(x, y);

	dirs.clear();
	if (player != PLAYER1 && player != PLAYER2)
		return (false);
	for (unsigned char i = 0; i < 5; ++i)
	{
		if (_board.getCase(x + i, y) == player)
			line += 1;
		if (_board.getCase(x, y + i) == player)
			column += 1;
		if (_board.getCase(x + i, y + i) == player)
			mainDiag += 1;
		if (_board.getCase(x - i, y - i) == player)
			secondDiag += 1;
	}
	if (line == 5)
		dirs.push_back(LINE);
	else if (column == 5)
		dirs.push_back(COLUMN);
	else if (mainDiag == 5)
		dirs.push_back(MAIN_DIAGONAL);
	else if (secondDiag == 5)
		dirs.push_back(SECONDARY_DIAGONAL);
	return (dirs.size());
}

bool			Referee::_isBreakable(int x, int y, enum direction dir) const
{
	int sp_x = x, sp_y = y;
	char player = _board.getCase(x, y), enemy = player % 2 + 1;

	for (unsigned char i = 0; i < 5; ++i)
	{
		if (dir != LINE)
		{
			if ((_board.getCase(sp_x + 1, sp_y) == player && (_board.getCase(sp_x + 2, sp_y) == EMPTY || _board.getCase(sp_x + 2, sp_y) == player + 2) && _board.getCase(sp_x - 1, sp_y) == enemy) ||
			    (_board.getCase(sp_x + 1, sp_y) == player && _board.getCase(sp_x + 2, sp_y) == enemy && (_board.getCase(sp_x - 1, sp_y) == EMPTY || _board.getCase(sp_x - 1, sp_y) == player + 2)) ||
			    (_board.getCase(sp_x + 1, sp_y) == enemy && _board.getCase(sp_x - 1, sp_y) == player && (_board.getCase(sp_x - 2, sp_y) == EMPTY || _board.getCase(sp_x - 2, sp_y) == player + 2)) ||
			    ((_board.getCase(sp_x + 1, sp_y) == EMPTY || _board.getCase(sp_x + 1, sp_y) == player + 2) && _board.getCase(sp_x - 1, sp_y) == player && _board.getCase(sp_x - 2, sp_y) == enemy))
			    return (true);
		}
		if (dir != COLUMN)
		{
			if ((_board.getCase(sp_x, sp_y + 1) == player && (_board.getCase(sp_x, sp_y + 2) == EMPTY || _board.getCase(sp_x, sp_y + 2) == player + 2) && _board.getCase(sp_x, sp_y - 1) == enemy) ||
			    (_board.getCase(sp_x, sp_y + 1) == player && _board.getCase(sp_x, sp_y + 2) == enemy && (_board.getCase(sp_x, sp_y - 1) == EMPTY || _board.getCase(sp_x, sp_y - 1) == player + 2)) ||
			    (_board.getCase(sp_x, sp_y + 1) == enemy && _board.getCase(sp_x, sp_y - 1) == player && (_board.getCase(sp_x, sp_y - 2) == EMPTY || _board.getCase(sp_x, sp_y - 2) == player + 2)) ||
			    ((_board.getCase(sp_x, sp_y + 1) == EMPTY || _board.getCase(sp_x, sp_y + 1) == player + 2) && _board.getCase(sp_x, sp_y - 1) == player && _board.getCase(sp_x, sp_y - 2) == enemy))
				return (true);
		}
		if (dir != MAIN_DIAGONAL)
		{
			if ((_board.getCase(sp_x + 1, sp_y + 1) == player && (_board.getCase(sp_x + 2, sp_y + 2) == EMPTY || _board.getCase(sp_x + 2, sp_y + 2) == player + 2) && _board.getCase(sp_x - 1, sp_y - 1) == enemy) ||
			    (_board.getCase(sp_x + 1, sp_y + 1) == player && _board.getCase(sp_x + 2, sp_y + 2) == enemy && (_board.getCase(sp_x - 1, sp_y - 1) == EMPTY || _board.getCase(sp_x - 1, sp_y - 1) == player + 2)) ||
			    (_board.getCase(sp_x + 1, sp_y + 1) == enemy && _board.getCase(sp_x - 1, sp_y - 1) == player && (_board.getCase(sp_x - 2, sp_y - 2) == EMPTY || _board.getCase(sp_x - 2, sp_y - 2) == player + 2)) ||
			    ((_board.getCase(sp_x + 1, sp_y + 1) == EMPTY || _board.getCase(sp_x + 1, sp_y + 1) == player + 2) && _board.getCase(sp_x - 1, sp_y - 1) == player && _board.getCase(sp_x - 2, sp_y - 2) == enemy))
				return (true);
		}
		if (dir != SECONDARY_DIAGONAL)
		{
			if ((_board.getCase(sp_x - 1, sp_y + 1) == player && (_board.getCase(sp_x - 2, sp_y + 2) == EMPTY || _board.getCase(sp_x - 2, sp_y + 2) == player + 2) && _board.getCase(sp_x + 1, sp_y - 1) == enemy) ||
			    (_board.getCase(sp_x - 1, sp_y + 1) == player && _board.getCase(sp_x - 2, sp_y + 2) == enemy && (_board.getCase(sp_x + 1, sp_y - 1) == EMPTY || _board.getCase(sp_x + 1, sp_y - 1) == player + 2)) ||
			    (_board.getCase(sp_x - 1, sp_y + 1) == enemy && _board.getCase(sp_x + 1, sp_y - 1) == player && (_board.getCase(sp_x + 2, sp_y - 2) == EMPTY || _board.getCase(sp_x + 2, sp_y + 2) == player + 2)) ||
			    ((_board.getCase(sp_x - 1, sp_y + 1) == EMPTY || _board.getCase(sp_x - 1, sp_y + 1) == player + 2) && _board.getCase(sp_x + 1, sp_y - 1) == player && _board.getCase(sp_x + 2, sp_y - 2) == enemy))
				return (true);
		}
		switch (dir)
		{
			case LINE:
				sp_x += 1;
				break;
			case COLUMN:
				sp_y += 1;
				break;
			case MAIN_DIAGONAL:
				sp_x += 1;
				sp_y += 1;
				break;
			case SECONDARY_DIAGONAL:
				sp_x -= 1;
				sp_y -= 1;
				break;
			default:
				std::cerr << "[ERROR]: In Referee instance -> _freeThree: unknown direction" << std::endl;
				break;
		}
	}
	return (false);
}

void	Referee::_checkWinner()
{
	std::vector<enum direction>	dirs;

	for (int i = 0; i < MAX_HEIGHT; ++i)
	{
		for (int j = 0; j < MAX_WIDTH; ++j)
		{
			if (_fivePieceAligned(j, i, dirs))
			{
				while (!dirs.empty())
				{
					if (!_isBreakable(j, i, dirs.back()))
						_winner = _board.getCase(j, i);
					dirs.pop_back();
				}
			}
		}
	}
}

void	Referee::_removeDoubleThree(int x1, int y1, int x2, int y2, char player) const
{
	int sp_x1(MIN(x1, x2) - 3), sp_y1(MIN(y1, y2) - 3), ep_x2(MAX(x1, x2) + 3), ep_y2(MAX(y1, y2) + 3);

	for (int i = sp_y1; i <= ep_y2; ++i)
	{
		for (int j = sp_x1; j <= ep_x2; ++j)
		{
			if (_board.getCase(j, i) == player + 2 && _countFreeThree(j, i, player) < 2)
				_board.setCase(j, i, EMPTY);
		}
	}
}

void	Referee::_checkCapturedPawn(int x, int y) const
{
	char player = _board.getCase(x, y), enemy = player % 2 + 1;

	if (_board.getCase(x + 1, y) == enemy && _board.getCase(x + 2, y) == enemy && _board.getCase(x + 3, y) == player)
	{
		_board.setCase(x + 1, y, EMPTY);
		_board.setCase(x + 2, y, EMPTY);
		_removeDoubleThree(x + 1, y, x + 2, y, enemy);
	}
	if (_board.getCase(x - 1, y) == enemy && _board.getCase(x - 2, y) == enemy && _board.getCase(x - 3, y) == player)
	{
		_board.setCase(x - 1, y, EMPTY);
		_board.setCase(x - 2, y, EMPTY);		
		_removeDoubleThree(x - 1, y, x - 2, y, enemy);
	}
	if (_board.getCase(x, y + 1) == enemy && _board.getCase(x, y + 2) == enemy && _board.getCase(x, y + 3) == player)
	{
		_board.setCase(x, y + 1, EMPTY);
		_board.setCase(x, y + 2, EMPTY);
		_removeDoubleThree(x, y + 1, x, y + 2, enemy);
	}
	if (_board.getCase(x, y - 1) == enemy && _board.getCase(x, y - 2) == enemy && _board.getCase(x, y - 3) == player)
	{
		_board.setCase(x, y - 1, EMPTY);
		_board.setCase(x, y - 2, EMPTY);
		_removeDoubleThree(x, y - 1, x, y - 2, enemy);
	}
	if (_board.getCase(x + 1, y + 1) == enemy && _board.getCase(x + 2, y + 2) == enemy && _board.getCase(x + 3, y + 3) == player)
	{
		_board.setCase(x + 1, y + 1, EMPTY);
		_board.setCase(x + 2, y + 2, EMPTY);
		_removeDoubleThree(x + 1, y + 1, x + 2, y + 2, enemy);
	}
	if (_board.getCase(x - 1, y - 1) == enemy && _board.getCase(x - 2, y - 2) == enemy && _board.getCase(x - 3, y - 3) == player)
	{
		_board.setCase(x - 1, y - 1, EMPTY);
		_board.setCase(x - 2, y - 2, EMPTY);
		_removeDoubleThree(x - 1, y - 1, x - 2, y - 2, enemy);
	}
	if (_board.getCase(x + 1, y - 1) == enemy && _board.getCase(x + 2, y - 2) == enemy && _board.getCase(x + 3, y - 3) == player)
	{
		_board.setCase(x + 1, y - 1, EMPTY);
		_board.setCase(x + 2, y - 2, EMPTY);
		_removeDoubleThree(x + 1, y - 1, x + 2, y - 2, enemy);
	}
	if (_board.getCase(x - 1, y + 1) == enemy && _board.getCase(x - 2, y + 2) == enemy && _board.getCase(x - 3, y + 3) == player)
	{
		_board.setCase(x - 1, y + 1, EMPTY);
		_board.setCase(x - 2, y + 2, EMPTY);
		_removeDoubleThree(x - 1, y + 1, x - 2, y + 2, enemy);
	}
}
