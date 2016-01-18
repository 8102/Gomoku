#include "AI_MonteCarlo.hh"

/*
** Constructor / Destructor
*/

AI_MonteCarlo::AI_MonteCarlo(char player, Referee &ref) :
    _player(player), _ref(ref), _mt(_rd()), _distribution(0, MAX_HEIGHT * MAX_WIDTH - 1)
{

}

AI_MonteCarlo::~AI_MonteCarlo()
{

}

/*
** Public methodes
*/

bool            AI_MonteCarlo::playOneTurn()
{
    std::cout << "playOneTurn" << std::endl;
    std::shared_ptr<std::vector<std::pair<int, int> > > possibilities = this->_getRandomFreePossibilities();
    std::pair<int, int> bestMoveCoord = this->_getBestMove(possibilities);

    try
    {
        _ref.putPieceOnBoard(bestMoveCoord.first, bestMoveCoord.second, _player);
    }
    catch (NotEmptyError &e)
    {
        std::cerr << "[Error]: AI_MonteCarlo -> playOneTurn: Cell not empty (" + std::to_string(bestMoveCoord.first) + ", " + std::to_string(bestMoveCoord.second) + ")"<< std::endl;
        std::cout << "playOneTurn END" << std::endl;
        return false;
        // do nothing
    }
    catch (DoubleThreeRule &e)
    {
        std::cerr << "[Error]: AI_MonteCarlo -> playOneTurn: Double three" << std::endl;
        std::cout << "playOneTurn END" << std::endl;
        return false;
        // do nothing
    }
    std::cout << "playOneTurn END" << std::endl;
    return true;
}

/*
** Private methodes
*/

/*
** optimization: find a new way to get free cases. (very bad for now)
*/
std::shared_ptr<std::vector<std::pair<int, int> > >     AI_MonteCarlo::_getRandomFreePossibilities(unsigned int max)
{
    std::shared_ptr<std::vector<std::pair<int, int> > > possibilities(new std::vector<std::pair<int, int> >);
    int                                                 pos(0), x(0), y(0), attempt(0);
    unsigned int                                        i(0);
    char                                                piece;

    while (i < max)
    {
        pos = _distribution(_mt);
        x = pos % MAX_WIDTH;
        y = pos / MAX_WIDTH;
        if ((piece = _ref.getCase(x, y)) == EMPTY || piece + _player == '5')
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

std::pair<int, int>                                    AI_MonteCarlo::_getBestMove(std::shared_ptr<std::vector<std::pair<int, int> > > possibilities)
{
    std::pair<int, int>                                bestPos;
    float                                              bestChance(0.0f), lastChance;

    if (possibilities->size() > 0)
    {
        bestPos.first = possibilities->begin()->first;
        bestPos.second = possibilities->begin()->second;
    }
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
    using namespace std::chrono;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    _ref.putPieceOnBoard(x, y, _player);
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(t2 - t1).count();
    std::cout << duration << std::endl;
    _ref.saveBoard();
    for (unsigned int i = 0; i < max; ++i)
    {
        if (this->_generateGame())
            ++wins;
    }
    _ref.resetCell(x, y, _player);
    winrate = static_cast<float>(wins) / static_cast<float>(max);
    return winrate;
}

bool                                                   AI_MonteCarlo::_generateGame()
{
    bool                                                AI_turn(false);
    int                                                 pos(0), x(0), y(0);

    while (!_ref.getWinner())
    {
        pos = _distribution(_mt);
        x = pos % MAX_WIDTH;
        y = pos / MAX_WIDTH;
        if (AI_turn)
        {
            if (!(_ref.putPieceOnBoard(x, y, _player)))
                AI_turn = !AI_turn;
        }
        else
        {
            if (!(_ref.putPieceOnBoard(x, y, (_player - '0') % 2 + 1 + '0')))
                AI_turn = !AI_turn;
        }
    }
    _ref.resetBoardLastSave();
    return _ref.getWinner() == _player;
}
