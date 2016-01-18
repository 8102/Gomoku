#ifndef _AI_MONTE_CARLO_
# define _AI_MONTE_CARLO_

# include <vector>
# include <memory>
# include <random>
# include "IAI.hh"
# include "Referee.hh"

# define NUMBER_OF_POSSIBILITIES_MAX        300
# define NUMBER_OF_SIMULATIONS_PER_PIECE    10

class AI_MonteCarlo : public IAI
{

public:
    AI_MonteCarlo(char player, Referee &ref);
    ~AI_MonteCarlo();

public:
    bool        playOneTurn();

private:
    std::shared_ptr<std::vector<std::pair<int, int> > >    _getRandomFreePossibilities(unsigned int max = NUMBER_OF_POSSIBILITIES_MAX);
    std::pair<int, int>                                    _getBestMove(std::shared_ptr<std::vector<std::pair<int, int> > > possibilities);
    float                                                  _generateGames(int x, int y, unsigned int max = NUMBER_OF_SIMULATIONS_PER_PIECE);
    bool                                                   _generateGame();

private:
    char                                    _player;
    Referee &                               _ref;
    std::random_device                      _rd;
    std::mt19937                            _mt;
    std::uniform_int_distribution<int>      _distribution;
};

#endif // _AI_MONTE_CARLO_
