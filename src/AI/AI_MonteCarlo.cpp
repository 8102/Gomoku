#include <unistd.h>
#include "AI_MonteCarlo.hh"

/*
** Constructor / Destructor
*/

AI_MonteCarlo::AI_MonteCarlo(char player, Referee &ref) :
    _player(player), _ref(ref), _mt(_rd()), _distribution(0, MAX_HEIGHT * MAX_WIDTH - 1),
    _pool(NUMBER_OF_THREADS), _child(false)
{
    for (short i = 0; i < NUMBER_OF_THREADS; ++i)
    {
        Board *newBoard = new Board;
        Referee *newReferee = new Referee(*newBoard);
        AI_MonteCarlo *newAi = new AI_MonteCarlo(player, *newReferee, true);
        _vgames.push_back(std::pair<Board *, Referee *>(newBoard, newReferee));
        _ais.push_back(newAi);
    }
}

AI_MonteCarlo::AI_MonteCarlo(char player, Referee &ref, bool child) :
    _player(player), _ref(ref), _mt(_rd()), _distribution(0, MAX_HEIGHT * MAX_WIDTH - 1), _pool(0), _child(child)
{

}

AI_MonteCarlo::~AI_MonteCarlo()
{
    if (!_child)
    {
        while (! _ais.empty())
        {
            delete _ais.back();
            _ais.pop_back();
            delete _vgames.back().first;
            delete _vgames.back().second;
            _vgames.pop_back();
        }
    }
}

/*
** Public methodes
*/

bool            AI_MonteCarlo::playOneTurn()
{
    std::cout << "playOneTurn" << std::endl;
    std::shared_ptr<std::vector<std::pair<int, int> > > possibilities = this->_getRandomFreePossibilities(NUMBER_OF_POSSIBILITIES_MAX, _player);
    std::pair<int, int> bestMoveCoord;
    float bestChance(0.0f), lastChance;
    int pack = possibilities->size() / NUMBER_OF_THREADS;
    int rest = possibilities->size() % NUMBER_OF_THREADS;
    _results_coord = std::vector<std::pair<int, int> >(NUMBER_OF_THREADS + 1, std::pair<int, int>(0, 0));

    std::cout << "size: " << possibilities->size() << std::endl;
    auto it_begin = possibilities->begin();
    auto it_end = it_begin + pack;
    for (short i = 0; i < NUMBER_OF_THREADS; ++i)
    {
        _args.push_back(std::vector<std::pair<int, int> >(it_begin, it_end));
        it_begin = it_end;
        it_end += pack;
    }
    if (rest)
        _args.push_back(std::vector<std::pair<int, int> >(it_begin, possibilities->end()));
    std::cout << _ais.size() << " " << _args.size() << " " << _results_coord.size() << std::endl;
    std::cout << "args" << std::endl;
    for (auto it = _args.begin(); it != _args.end(); ++it)
    {
        std::cout << it->size() << std::endl;
    }
    std::cout << "end" << std::endl;
    for (short i = 0; i < NUMBER_OF_THREADS; ++i)
    {
        _results.emplace_back(_pool.enqueue(&AI_MonteCarlo::_getBestMove, _ais[i], _args[i], &(_results_coord[i])));
    }
    std::cout << "coucou" << std::endl;
    if (rest)
        _results.emplace_back(_pool.enqueue(&AI_MonteCarlo::_getBestMove, this, _args.back(), &(_results_coord.back())));
    short i(0);
    for (auto it = _results.begin(); it != _results.end(); ++it)
    {
        if ((lastChance = it->get()) > bestChance)
        {
            bestChance = lastChance;
            bestMoveCoord.first = _results_coord[i].first;
            bestMoveCoord.second = _results_coord[i].second;
        }
        ++i;
    }
    std::cout << "results coord" << std::endl;
    for (auto it = _results_coord.begin(); it != _results_coord.end(); ++it)
    {
        std::cout << "(" << it->first << ", " << it->second << ")"<< std::endl;
    }
    std::cout << "end" << std::endl;
    std::cout << "best chance -> " << bestChance << " coord: " << bestMoveCoord.first << ", " << bestMoveCoord.second << std::endl;
    // this->_getBestMove(possibilities, bestMoveCoord);
    _ref.putPieceOnBoard(bestMoveCoord.first, bestMoveCoord.second, _player);
    std::cout << "playOneTurn END" << std::endl;
    _results.clear();
    _args.clear();
    return true;
}

/*
** Private methodes
*/

std::shared_ptr<std::vector<std::pair<int, int> > >     AI_MonteCarlo::_getRandomFreePossibilities(unsigned int max, char for_player)
{
    std::shared_ptr<std::vector<std::pair<int, int> > > free_possibilities(new std::vector<std::pair<int, int> >);
    const char *finalGoban = _ref._board.getBoard();
    std::array<Heuristic::Cell, 361> evaluatedGoban = Heuristic::translateGoban(finalGoban);
    std::vector<Heuristic::study> plays = Heuristic::listRelevantPlays(evaluatedGoban, for_player - '0');
    unsigned int i(0);

    // std::cout << "j'ai " << plays.size() << " cases à tester" << std::endl;

    for (auto it = plays.begin(); it != plays.end(); ++it)
    {
        if (i == max)
            break;
        free_possibilities->push_back(std::pair<int, int>(it->x, it->y));
        ++i;
    }
    return free_possibilities;
}

// std::shared_ptr<std::vector<std::pair<int, int> > >     AI_MonteCarlo::_getRandomFreePossibilities(unsigned int max, char for_player)
// {
//     std::shared_ptr<std::vector<std::pair<int, int> > > free_possibilities(new std::vector<std::pair<int, int> >);
//     std::vector<std::pair<int, int> >                   possibilities;
//     int                                                 pos(0);
//     unsigned int                                        m(0);
//     char                                                piece;
//     std::vector<std::pair<int, int> >                 & moves = _ref.getMovesPlayed((for_player - '0') % 2 + 1 + '0');
//     std::pair<int, int>                                 tmp;
//
//     for (auto it = moves.begin(); it != moves.end(); ++it)
//     {
//         for (short i = 0; i < RAND_DIAMETER; ++i)
//         {
//             for (short j = 0; j < RAND_DIAMETER; ++j)
//             {
//                 if ((piece = _ref.getCase(it->first + j - 3, it->second + i - 3)) == EMPTY || piece + _player == '5')
//                     possibilities.push_back(std::pair<int, int>(it->first + j - 3, it->second + i - 3));
//             }
//         }
//     }
//     unsigned int poss_size = possibilities.size();
//     if (poss_size > max)
//     {
//         while (m < max)
//         {
//             pos = _distribution(_mt) % poss_size;
//             tmp = possibilities[pos];
//             free_possibilities->push_back(tmp);
//             ++m;
//         }
//     }
//     else if (poss_size < max && poss_size > 0)
//     {
//         *free_possibilities = possibilities;
//     }
//     else
//     {
//         // trouver des frees places
//     }
//     return free_possibilities;
// }

float                                                 AI_MonteCarlo::_getBestMove(std::vector<std::pair<int, int> > const &possibilities, std::pair<int, int> *move)
{
    std::pair<int, int>                                bestPos;
    float                                              bestChance(0.0f), lastChance;

    if (possibilities.size() > 0)
    {
        bestPos.first = possibilities.begin()->first;
        bestPos.second = possibilities.begin()->second;
    }
    for (auto it = possibilities.begin(); it != possibilities.end(); ++it)
    {
        if ((lastChance = _generateGames(it->first, it->second)) > bestChance)
        {
            std::cout << "chance: " << lastChance << std::endl;
            bestChance = lastChance;
            bestPos.first = it->first;
            bestPos.second = it->second;
        }
    }
    move->first = bestPos.first;
    move->second = bestPos.second;
    return bestChance;
}

float                                                  AI_MonteCarlo::_generateGames(int x, int y, unsigned int max)
{
    float                                               winrate(0.0f);
    unsigned int                                        wins(0);

    _ref.putPieceOnBoard(x, y, _player);
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
    std::shared_ptr<std::vector<std::pair<int, int> > > random_possibilitie;
    int                                                 pos(0), x(0), y(0);

    while (!_ref.getWinner())
    {
        pos = _distribution(_mt);
        x = pos % MAX_WIDTH;
        y = pos / MAX_WIDTH;
        if (AI_turn)
        {
            // random_possibilitie = this->_getRandomFreePossibilities(1, _player);
            if (!(_ref.putPieceOnBoard(x, y, _player)))
                AI_turn = !AI_turn;
        }
        else
        {
            // random_possibilitie = this->_getRandomFreePossibilities(1, (_player - '0') % 2 + 1 + '0');
            if (!(_ref.putPieceOnBoard(x, y, (_player - '0') % 2 + 1 + '0')))
                AI_turn = !AI_turn;
        }
    }
    char winner = _ref.getWinner();
    _ref.resetBoardLastSave();
    return winner == _player;
}
