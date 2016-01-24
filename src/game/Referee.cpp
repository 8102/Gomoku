#include "Referee.hh"

/*
** Constructor / Destructor
*/

Referee::Referee(Board &board) : _board(board), _cDbleThree(true), _cCapture(true)
{
	std::fill(std::begin(_save), std::begin(_save) + B_SIZE + JAIL_SIZE + WIN_SIZE, 0);
	_moves_played[PLAYER1] = std::vector<std::pair<int, int> >();
	_moves_played[PLAYER2] = std::vector<std::pair<int, int> >();
}

Referee::~Referee()
{

}

/*
** Getter / Setter
*/

int		Referee::getWinner() const
{
	return _board.getWinnerCase();
}

void 	Referee::resetWinner()
{
	_board.setWinnerCase(0);
}

void Referee::setDoubleThreeRule(bool const& b)
{
	_cDbleThree = b;
}

void Referee::setCaptureRule(bool const& b)
{
	_cCapture = b;
}

bool Referee::getDoubleThreeRule() const
{
	return _cDbleThree;
}

bool Referee::getCaptureRule() const
{
	return _cCapture;
}

char const *Referee::getJail()
{
	return _board.getJail();
}

char	Referee::getCase(int x, int y) const
{
	return _board.getCase(x, y);
}

std::vector<std::pair<int, int> > &Referee::getMovesPlayed(char player)
{
	return _moves_played[player];
}

/*
** Public methodes
*/

int		Referee::putPieceOnBoard(int x, int y, char player)
{
	char c = _board.getCase(x, y);
	if (c == PLAYER1 || c == PLAYER2)
		return NOT_EMPTY_ERROR;
	if (_board.getCase(x, y) == player + DOUBLE_THREE_RULE)
		return DOUBLE_THREE_ERROR;
	_board.setCase(x, y, player);
	if (_cDbleThree == true)
	{
		if (int error = _checkDoubleThree(x, y, player))
			return error;
	}
	if (_cCapture == true)
		_checkCapturedPawn(x, y);
	_checkWinner();
	return NO_ERROR;
}

void 	Referee::resetCell(int x, int y, char player)
{
	if (_board.getCase(x, y) == player)
		_board.setCase(x, y, EMPTY);
	else
		throw NotEmptyError("You're not allowed to reset this cell !");
}

void 	Referee::saveBoard()
{
	char const *save = _board.getBoard();
	std::memcpy(_save, save, B_SIZE + JAIL_SIZE + WIN_SIZE);
	_save_moves_played = _moves_played;
}

void 	Referee::resetBoardLastSave()
{
	_board.setBoard(_save);
	_moves_played = _save_moves_played;
}

void 	Referee::resetGame()
{
	_board.reset();
}

/*
** Private methodes
*/

int		Referee::_checkDoubleThree(int x, int y, char player)
{
	_moves_played[player].push_back(std::pair<int, int>(x, y));
	for (int i = 0; i <= 2 * SEARCH_RADIUS + 2; ++i)
	{
		for (int j = 0; j <= 2 * SEARCH_RADIUS + 2; ++j)
		{
			int current_x = x - SEARCH_RADIUS - 1 + j, current_y = y - SEARCH_RADIUS - 1 + i;
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
	return NO_ERROR;
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

bool	Referee::_freeThreePattern(char c1, char c2, char c3, char c4, char c5, char player) const
{
	if ((c1 == EMPTY || c1 + player == '5') && c2 == player && c3 == player && (c4 == EMPTY || c4 + player == '5'))
		return (true);
	if ((c1 == EMPTY || c1 + player == '5') && (c2 == EMPTY || c2 + player == '5') && c3 == player && c4 == player && (c5 == EMPTY || c5 + player == '5'))
		return (true);
	if ((c1 == EMPTY || c1 + player == '5') && c2 == player && (c3 == EMPTY || c3 + player == '5') && c4 == player && (c5 == EMPTY || c5 + player == '5'))
		return (true);
	return (false);
}

bool	Referee::_freeThree(int x, int y, char player, enum direction dir) const
{
	switch (dir)
	{
		case LINE:
			if (_freeThreePattern(_board.getCase(x - 1, y), _board.getCase(x + 1, y), _board.getCase(x + 2, y), _board.getCase(x + 3, y), _board.getCase(x + 4, y), player) ||
			    _freeThreePattern(_board.getCase(x - 4, y), _board.getCase(x - 3, y), _board.getCase(x - 2, y), _board.getCase(x - 1, y), _board.getCase(x + 1, y), player) ||
				_freeThreePattern(_board.getCase(x - 2, y), _board.getCase(x - 1, y), _board.getCase(x + 1, y), _board.getCase(x + 2, y), _board.getCase(x + 3, y), player))
				return (true);
			break;
		case COLUMN:
			if (_freeThreePattern(_board.getCase(x, y - 1), _board.getCase(x, y + 1), _board.getCase(x, y + 2), _board.getCase(x, y + 3), _board.getCase(x, y + 4), player) ||
			    _freeThreePattern(_board.getCase(x, y - 4), _board.getCase(x, y - 3), _board.getCase(x, y - 2), _board.getCase(x, y - 1), _board.getCase(x, y + 1), player) ||
				_freeThreePattern(_board.getCase(x, y - 2), _board.getCase(x, y - 1), _board.getCase(x, y + 1), _board.getCase(x, y + 2), _board.getCase(x, y + 3), player))
				return (true);
			break;
		case MAIN_DIAGONAL:
			if (_freeThreePattern(_board.getCase(x - 1, y - 1), _board.getCase(x + 1, y + 1), _board.getCase(x + 2, y + 2), _board.getCase(x + 3, y + 3), _board.getCase(x + 4, y + 4), player) ||
			    _freeThreePattern(_board.getCase(x - 4, y - 4), _board.getCase(x - 3, y - 3), _board.getCase(x - 2, y - 2), _board.getCase(x - 1, y - 1), _board.getCase(x + 1, y + 1), player) ||
				_freeThreePattern(_board.getCase(x - 2, y - 2), _board.getCase(x - 1, y - 1), _board.getCase(x + 1, y + 1), _board.getCase(x + 2, y + 2), _board.getCase(x + 3, y + 3), player))
				return (true);
			break;
		case SECONDARY_DIAGONAL:
			if (_freeThreePattern(_board.getCase(x - 4, y + 4), _board.getCase(x - 3, y + 3), _board.getCase(x - 2, y + 2), _board.getCase(x - 1, y + 1), _board.getCase(x + 1, y - 1), player) ||
			    _freeThreePattern(_board.getCase(x - 1, y + 1), _board.getCase(x + 1, y - 1), _board.getCase(x + 2, y - 2), _board.getCase(x + 3, y - 3), _board.getCase(x + 4, y - 4), player) ||
				_freeThreePattern(_board.getCase(x - 2, y + 2), _board.getCase(x - 1, y + 1), _board.getCase(x + 1, y - 1), _board.getCase(x + 2, y - 2), _board.getCase(x + 3, y - 3), player))
				return (true);
			break;
		default:
			std::cerr << "[ERROR]: In Referee instance -> _freeThree: unknown direction" << std::endl;
			break;
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
		if (_board.getCase(x - i, y + i) == player)
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
	char player = _board.getCase(x, y), enemy = (player - '0') % 2 + 1 + '0';

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

	if (_board.getJail(1) > 10)
		_board.setWinnerCase(PLAYER1);
	if (_board.getJail(2) > 10)
		_board.setWinnerCase(PLAYER2);
	for (int i = 0; i < MAX_HEIGHT; ++i)
	{
		for (int j = 0; j < MAX_WIDTH; ++j)
		{
			if (_fivePieceAligned(j, i, dirs))
			{
				while (!dirs.empty())
				{
					if (!_isBreakable(j, i, dirs.back()))
						_board.setWinnerCase(_board.getCase(j, i));
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

void		Referee::_checkCapturedPawn(int x, int y)
{
	char player = _board.getCase(x, y), enemy = (player -'0') % 2 + 1 + '0';

	if (_board.getCase(x + 1, y) == enemy && _board.getCase(x + 2, y) == enemy && _board.getCase(x + 3, y) == player)
	{
		_board.setCase(x + 1, y, EMPTY);
		_board.setCase(x + 2, y, EMPTY);
		if (player - '0' - 1 == 0)
			_board.addInJail(1);
		else
			_board.addInJail(2);
		_removeDoubleThree(x + 1, y, x + 2, y, enemy);
	}
	if (_board.getCase(x - 1, y) == enemy && _board.getCase(x - 2, y) == enemy && _board.getCase(x - 3, y) == player)
	{
		_board.setCase(x - 1, y, EMPTY);
		_board.setCase(x - 2, y, EMPTY);
		if (player - '0' - 1 == 0)
			_board.addInJail(1);
		else
			_board.addInJail(2);
		_removeDoubleThree(x - 1, y, x - 2, y, enemy);
	}
	if (_board.getCase(x, y + 1) == enemy && _board.getCase(x, y + 2) == enemy && _board.getCase(x, y + 3) == player)
	{
		_board.setCase(x, y + 1, EMPTY);
		_board.setCase(x, y + 2, EMPTY);
		if (player - '0' - 1 == 0)
			_board.addInJail(1);
		else
			_board.addInJail(2);
		_removeDoubleThree(x, y + 1, x, y + 2, enemy);
	}
	if (_board.getCase(x, y - 1) == enemy && _board.getCase(x, y - 2) == enemy && _board.getCase(x, y - 3) == player)
	{
		_board.setCase(x, y - 1, EMPTY);
		_board.setCase(x, y - 2, EMPTY);
		if (player - '0' - 1 == 0)
			_board.addInJail(1);
		else
			_board.addInJail(2);
		_removeDoubleThree(x, y - 1, x, y - 2, enemy);
	}
	if (_board.getCase(x + 1, y + 1) == enemy && _board.getCase(x + 2, y + 2) == enemy && _board.getCase(x + 3, y + 3) == player)
	{
		_board.setCase(x + 1, y + 1, EMPTY);
		_board.setCase(x + 2, y + 2, EMPTY);
		if (player - '0' - 1 == 0)
			_board.addInJail(1);
		else
			_board.addInJail(2);
		_removeDoubleThree(x + 1, y + 1, x + 2, y + 2, enemy);
	}
	if (_board.getCase(x - 1, y - 1) == enemy && _board.getCase(x - 2, y - 2) == enemy && _board.getCase(x - 3, y - 3) == player)
	{
		_board.setCase(x - 1, y - 1, EMPTY);
		_board.setCase(x - 2, y - 2, EMPTY);
		if (player - '0' - 1 == 0)
			_board.addInJail(1);
		else
			_board.addInJail(2);
		_removeDoubleThree(x - 1, y - 1, x - 2, y - 2, enemy);
	}
	if (_board.getCase(x + 1, y - 1) == enemy && _board.getCase(x + 2, y - 2) == enemy && _board.getCase(x + 3, y - 3) == player)
	{
		_board.setCase(x + 1, y - 1, EMPTY);
		_board.setCase(x + 2, y - 2, EMPTY);
		if (player - '0' - 1 == 0)
			_board.addInJail(1);
		else
			_board.addInJail(2);
		_removeDoubleThree(x + 1, y - 1, x + 2, y - 2, enemy);
	}
	if (_board.getCase(x - 1, y + 1) == enemy && _board.getCase(x - 2, y + 2) == enemy && _board.getCase(x - 3, y + 3) == player)
	{
		_board.setCase(x - 1, y + 1, EMPTY);
		_board.setCase(x - 2, y + 2, EMPTY);
		if (player - '0' - 1 == 0)
			_board.addInJail(1);
		else
			_board.addInJail(2);
		_removeDoubleThree(x - 1, y + 1, x - 2, y + 2, enemy);
	}
}
