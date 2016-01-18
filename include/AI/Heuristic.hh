#ifndef               ___HEURISTIC_HH___
# define              ___HEURISTIC_HH___

# include     <vector>
# include     <list>
# include     <memory>
# include     <array>

# ifdef       _DEBUG
#  include      <iostream>
# endif       /* !_DEBUG */

# ifdef       _BENCHMARK
#  include      <chrono>
#  include      <ctime>
# endif       /* !_BENCHMARK */

# include     "Referee.hh"
# include     "Player.hh"
# include     "Board.hh"

# define              TREE_LEVEL_WIDTH   (5)

# define              u_NodePtr           std::unique_ptr<struct stateNode_s>

namespace             Heuristic
{
    enum              eLevel
    {
      MinLevel = 0,
      MaxLevel
    };

    /**/
    typedef struct    stateNode_s {

      std::unique_ptr<char>         board;  /* inherited board through previous levels, including evaluated play */
      int                           x;      /* x position of this node's play */
      int                           y;      /* y position of this node's play */
      int                           score;  /* Heuristic score of this action */
      char                          mode;   /* define wether the play is agressive or passive, and at wich rate */ /* ( ??? ) */
      std::array<u_NodePtr, TREE_LEVEL_WIDTH>  nodes; /* lower-level nodes of the tree */
    }                 stateNode;

    /*
    ** <--High-importance plays determination functions --> */
    u_NodePtr         winPossibility(std::unique_ptr<char> const& actualBoard, Player const& evaluated, eLevel const& goal);
    u_NodePtr         alignFour(std::unique_ptr<char> const& actualBoard, Player const& evaluated);

    /*
    ** <- knowledge representation inscribed in a cell --> */
    /* inscribe Influencial Mapping Weight on a cell according to the links it can provide */
    char              AknowledgedCell(int x, int y);
    /* Inscribe Knowledge representation in the current Boards Cell */ /* Heavy on first call, undoubtably much lighter after */
    bool              AknowledgeBoard(std::unique_ptr<char>& )

    /*
    ** */
    bool              operator<=(stateNode const& l, stateNode const& r);
};

#endif                /* !___HEURISTIC_HH___ */
