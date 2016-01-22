#ifndef               ___HEURISTIC_HH___
# define              ___HEURISTIC_HH___

# include     <vector>
# include     <list>
# include     <memory>
# include     <array>
# include     <algorithm>

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
    ** A Cell, once evaluated by Influence Mapping, will be constitued as follow :
    ** bit representation of a 64-bits of a Cell.
    ** numbers indicate bit idex, [] indicate a set of 3 bits, {} indicate a byte (8-bits)
    **  64 [][][][][][][][]  40 [][][][][][][][]    16 {}             8 {}
    **   player 2 influence   player 1 influence   various data     cell value
    ** An influence of 8 x 3 bits corresponds to the streaks of a given player in each of 8-directions arround the cell
    */
    typedef           long unsigned int        Cell;

    /*
    ** Node of the tree generated then evaluated for each play
    */
    typedef struct    TKnee_s
    {
      std::array<Cell, 361>           goban;  /* State of the current goban after the play, once it has been evaluated */
      int                             x;  /* X position of the play, once the node has been evaluated */
      int                             y;  /* Y position of the play, once the node has been evaluated */
      int                             advantage;  /* Heuristically-determined advantage of the play */
      int                             floor;  /* Depth of the node, indicating which player the play is for */
      std::array<struct TKnee_s *, 5> children;  /* Lower-levels of the tree starting from this node. */
    }                 TNode;


    TNode*             createNode(std::array<Cell, 361>const& initialGoban, int Player, int depth);
    bool               evaluateNode(TNode& node);
    int                averageAdvantage(TNode& node);
    TNode*             createTree(std::array<Cell, 361> const& goban, int depth);
    void               deleteTree(Heuristic::TNode* Tree);

    typedef           struct        influence_s
    {

      unsigned char   paths[8];

    }                 influence;

    typedef           struct        study_s
    {
      int             best;
      int             relevance;
      int             x;
      int             y;
    }                 study;

    bool              operator<(study const& r, study const& l);

    std::vector<study>  listRelevantPlays(std::array<Cell, 361>const& goban, int player);

    /*
    ** Collect data to mesure the influence in both players series a cell can have and record it on the cell*/
    Cell              mesureInfluence(int x, int y, std::array<Cell, 361>& goban);
    /*
    ** iterative search from (x, y) using vector (xv, vy) to record a streak in this direction */
    unsigned char     searchDirection(int x, int y, int vx, int vy, std::array<Cell, 361>& goban);
    /*
    ** use data collected trhough streaks of each directions to mark the influence mapping on the cell*/
    Cell              encryptData(unsigned char value, std::array<unsigned char, 8>& influence);
    /*
    extract the influence mapping hints recorded in the given Cell */
    influence         decryptData(Cell const c);

    /*
    ** <--High-importance plays determination functions --> */
    u_NodePtr         winPossibility(std::unique_ptr<char> const& actualBoard, Player const& evaluated, eLevel const& goal);
    u_NodePtr         alignFour(std::unique_ptr<char> const& actualBoard, Player const& evaluated);

    /*
    ** <- knowledge representation inscribed in a cell --> */
    /* inscribe Influencial Mapping Weight on a cell according to the links it can provide */
    bool              perimeter(int x, int y, std::array<char, 8>& dataCollector, std::array<char, 361>const& goban);

#define               EXTRACT_P1(x)     (((x) >> 5))
#define               EXTRACT_P2(x)     (((x) >> 2) & 0x7)

    unsigned char     AknowledgeCell(int x, int y, std::array<char, 361>const& goban);
    /* Inscribe Knowledge representation in the current Boards Cell */ /* Heavy on first call, undoubtably much lighter after */
    bool              AknowledgeBoard(std::unique_ptr<char>& board);


    /*
    ** */
    bool              operator<=(stateNode const& l, stateNode const& r);
};

#endif                /* !___HEURISTIC_HH___ */
