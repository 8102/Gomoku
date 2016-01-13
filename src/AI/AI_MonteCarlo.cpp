#include "AI_MonteCarlo.hh"

/*
** Constructor / Destructor
*/

AI_MonteCarlo::AI_MonteCarlo(char player, Referee &ref) :
    _player(player), _ref(ref), _distribution(0, MAX_HEIGHT * MAX_WIDTH - 1)
{

}

AI_MonteCarlo::~AI_MonteCarlo()
{

}

/*
** optimization: find a new way to get free cases. (very bad for now)
*/
std::unique_ptr<std::vector<std::pair<int, int> > >     AI_MonteCarlo::_getRandomFreePossibilities(unsigned int max)
{
    std::unique_ptr<std::vector<std::pair<int, int> > > possibilities(new std::vector<std::pair<int, int> >);
    int                                                 pos(0), x(0), y(0), attempt(0);
    unsigned int                                        i(0);
    char                                                piece;

    while (i < max)
    {
        pos = _distribution(_generator);
        x = pos % MAX_WIDTH;
        y = pos / MAX_WIDTH;
        if ((piece = _ref.getCase(x, y)) == EMPTY || piece + _player == 5)
            {
                possibilities->push_back(std::pair<int, int>(x, y));
                ++i;
            }
        else
            ++attempt;
        if (attempt > 2000)
            break;
    }
    return possibilities;
}

std::pair<int, int>                                    AI_MonteCarlo::_getBestMove(std::unique_ptr<std::vector<std::pair<int, int> > > possibilities)
{
    std::pair<int, int>                                bestPos;
    float                                              bestChance(0.0f), lastChance;

    for (auto it = possibilities->begin(); it != possibilities->end(); ++it)
    {
        if ((lastChance = _generateGames(it->first, it->second)) > bestChance)
        {
            bestChance = lastChance;
            bestPos.first = it->first;
            bestPos.second = it->second;
        }
    }
    return bestPos;
}

float                                                  AI_MonteCarlo::_generateGames(int x, int y, unsigned int max)
{
    float                                               winrate(0.0f);
    unsigned int                                        wins(0);

    _ref.putPieceOnBoard(x, y, _player);
    for (unsigned int i = 0; i < max; ++i)
    {
        if (this->_generateGame())
            ++wins;
    }
    _ref.putPieceOnBoard(x, y, EMPTY);
    winrate = static_cast<float>(wins) / static_cast<float>(max);
    return winrate;
}

bool                                                   AI_MonteCarlo::_generateGame()
{
    bool                                                AI_turn(false);
    int                                                 pos(0), x(0), y(0);
    // play for the other player first
    while (!_ref.getWinner())
    {
        pos = _distribution(_generator);
        x = pos % MAX_WIDTH;
        y = pos / MAX_WIDTH;
        if (AI_turn)
        {
            try
            {
                _ref.putPieceOnBoard(x, y, _player);
                AI_turn = !AI_turn;
            }
            catch (NotEmptyError &e)
            {
                // do nothing
            }
            catch (DoubleThreeRule &e)
            {
                // do nothing
            }
        }
        else
        {
            try
            {
                _ref.putPieceOnBoard(x, y, _player % 2 + 1);
                AI_turn = !AI_turn;
            }
            catch (NotEmptyError &e)
            {
                // do nothing
            }
            catch (DoubleThreeRule &e)
            {
                // do nothing
            }
        }
    }
    return _ref.getWinner() == _player;
}
