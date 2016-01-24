#ifndef _AI_MONTE_CARLO_
# define _AI_MONTE_CARLO_

# include <vector>
# include <memory>
# include <random>
# include "IAI.hh"
# include "Referee.hh"
# include "ThreadPool.h"
# include "Heuristic.hh"

# define RAND_DIAMETER                      7
# define NUMBER_OF_POSSIBILITIES_MAX        8
# define NUMBER_OF_SIMULATIONS_PER_PIECE    1000
# define NUMBER_OF_THREADS                  8

class AI_MonteCarlo : public IAI
{

public:
    AI_MonteCarlo(char player, Referee &ref);
    AI_MonteCarlo(char player, Referee &ref, bool child);
    ~AI_MonteCarlo();

public:
    bool        playOneTurn();

private:
    std::shared_ptr<std::vector<std::pair<int, int> > >    _getRandomFreePossibilities(unsigned int max, char for_player);
    float                                                  _getBestMove(std::vector<std::pair<int, int> > const &possibilities, std::pair<int, int> *move);
    float                                                  _generateGames(int x, int y, unsigned int max = NUMBER_OF_SIMULATIONS_PER_PIECE);
    bool                                                   _generateGame();

private:
    char                                    _player;
    Referee &                               _ref;
    std::random_device                      _rd;
    std::mt19937                            _mt;
    std::uniform_int_distribution<int>      _distribution;
    ThreadPool                              _pool;
    bool                                    _child;
    std::vector<std::future<float> >        _results;
    std::vector<std::pair<int, int> >      _results_coord;
    std::vector<std::pair<Board *, Referee *> >           _vgames;
    std::vector<AI_MonteCarlo *>                          _ais;
    std::vector<std::vector<std::pair<int, int> > >       _args;

};

#endif // _AI_MONTE_CARLO_
