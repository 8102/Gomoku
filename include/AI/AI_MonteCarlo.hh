#ifndef _AI_MONTE_CARLO_
# define _AI_MONTE_CARLO_

# include <vector>
# include <memory>
# include <random>
# include "IAI.hh"
# include "Referee.hh"

# define NUMBER_OF_POSSIBILITIES_MAX        200
# define NUMBER_OF_SIMULATIONS_PER_PIECE    500

class AI_MonteCarlo : public IAI
{

public:
    AI_MonteCarlo(char player, Referee const &ref);
    ~AI_MonteCarlo();

private:
    std::unique_ptr<std::vector<std::pair<int, int> > >    _getRandomFreePossibilities(unsigned int max = NUMBER_OF_POSSIBILITIES_MAX);
    std::pair<int, int>                                    _getBestMove(std::unique_ptr<std::vector<std::pair<int, int> > > possibilities);
    float                                                  _generateGames(int x, int y, unsigned int max = NUMBER_OF_SIMULATIONS_PER_PIECE);
    bool                                                   _generateGame();

private:
    char            _player;
    Referee const & _ref;
};

#endif // _AI_MONTE_CARLO_
