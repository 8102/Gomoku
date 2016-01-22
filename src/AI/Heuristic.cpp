#include "Heuristic.hh"
#include "common.hpp"

std::array<Heuristic::Cell, 361>      Heuristic::translateGoban(char const * const goban)
{
  std::array<Heuristic::Cell, 361>    translated = {0};

  for (size_t i = 0; i < 361; i++)
    translated[i] = goban[i] - '0';

  for (auto y = 0; y < 19; y++)
    for (auto x = 0; x < 19; x++)
      translated[POS(x, y)] = Heuristic::mesureInfluence(x, y, translated);

  return translated;
}

/*
** used by std::vector<>::sort() */
bool                              Heuristic::operator<(Heuristic::study const& r, Heuristic::study const& l)
{
  if (l.best == 4 && r.best < 4)
    return true;
  if (r.best < l.best)
    return true;
  if (r.relevance < l.relevance)
    return true;
  return false;
}


std::vector<Heuristic::study>     Heuristic::listRelevantPlays(std::array<Cell, 361>const& goban, int player)
{
  std::vector<Heuristic::study>   plays;

  for (auto y = 0; y < 19; y++)
    for (auto x = 0; x < 19; x++)
      {
        /* if cell if free and playable for player X */
        if (((goban[POS(x, y)] & 0x07) == 0) && ((goban[POS(x, y)] & 0x0700) != (unsigned int)player))
          {
              Heuristic::influence  scoring = Heuristic::decryptData(goban[POS(x, y)]);
              Heuristic::study      evl =  {0, 0, x, y};

              for (size_t i = 0; i < 8; i++)
                {
                  int playerInfluence = (scoring.paths[i] >> (4 * (player - 1))) & 0x07;
                  if (playerInfluence > evl.best)
                  {
                    evl.relevance += evl.best;
                    evl.best = playerInfluence;
                  }
                  else
                    evl.relevance += playerInfluence;
                }
            if (evl.best || evl.relevance > 0)
            {
              plays.push_back(evl);
            }
          }
      }
  std::sort(plays.begin(), plays.end());
  std::reverse(plays.begin(), plays.end());
  return plays;
}

Heuristic::TNode*                 Heuristic::createNode(std::array<Cell, 361>const& initialGoban, int player, int depth)
{
  TNode*                          node = nullptr;

  if (depth > 0)
    {
      if ((node = new TNode) == nullptr)
        return nullptr;
      node->goban = initialGoban;
      node->x = -1;
      node->y = -1;
      node->advantage = -1;
      node->floor = player;
      for (size_t i = 0;i < node->children.size(); i++)
        node->children[i] = nullptr;
    }
  return node;
}

/*
** Evaluate what play this node should represent, and record its impact on the cell goban
** If the play grants victory the function return true and node's children won't be
** evaluated by Heuristic::createTree
*/
bool                                Heuristic::evaluateNode(__attribute__((unused)) TNode& node)
{
  return false;
}

/*
** compute average gain of a node children, in order to be used in Heuristic::resolveTree
** If an average gain is significantly different when compared to others, we may decide
** to evaluate it only or not evaluate it
*/
int                                 Heuristic::averageAdvantage(TNode& node)
{
  int                               mediane(0);
  int                               cnt(0);

  for (size_t i = 0; i < node.children.size(); i++)
  {
    if (node.children[i] != nullptr) {
      mediane += node.children[i]->advantage;
      ++cnt;
    }
  }

  return mediane / cnt;
}

/*
** Create a Tree of possibility
** In order to reduce computing-time as well as use of ressources, each level of the tree shoud not excede
** its floor ^ 5. We do that by using std::array<X, 5> and stocking the 5-bests answers for each node.
** For example, level 0 will be 1 node, level 1 will be 5 and level 2 will be 25.
*/

Heuristic::TNode*                  Heuristic::createTree(std::array<Cell, 361>const& initialGoban, int player)
{
  TNode*                           node(nullptr);

  if ((node = Heuristic::createNode(initialGoban, player, 1)) == nullptr)
    return node;
  /* evaluate Node */
  if (Heuristic::evaluateNode(*node) == true)
    return node;
  /* createCHildren if Node is not a win condition */
  for (size_t i = 0; i < node->children.size(); i++)
    node->children[i] = Heuristic::createTree(node->goban, ((player + 1) % 2) + 1);
  return node;
}

/*
** Recursively delete Each Node of the Tree to free memory */
void                              Heuristic::deleteTree(Heuristic::TNode* Tree)
{
  if (Tree == nullptr)
    return;
  for (size_t i = 0; i < Tree->children.size(); i++)
    deleteTree(Tree->children[i]);
  delete Tree;
}


/*
** For a given position, search for a given direction a contiguous streak of up to 4 pawns and encode it on either
** bitwise party of the result corresponding to the said-streak colour. */
unsigned char                      Heuristic::searchDirection(int x, int y, int vx, int vy, std::array<Heuristic::Cell, 361>& goban)
{
  unsigned char                    content(0);
  int                              streak(0), pX(x), pY(y);
  unsigned char                    evaluated(0);

  for (;;)
  {
    pX += vx;
    pY += vy;
    if (pX < 0 || pY < 0 || pX > 18 || pY > 18)
      return content;
    if ((evaluated == 0 && (evaluated = (goban[POS(pX, pY)] & 0x07)) == 0) || (evaluated != (goban[POS(pX, pY)] & 0x07)))
      return content;
    ++streak;
    content += (evaluated == 1 ? 1 : (evaluated == 2 ? 16 : 0));
    if (streak >= 4)
      break;
  }
  return content;
}

/*
** Encode streaks of colours and value to a Heuritic::Cell .
** Format is as follow (each [] is a byte, streaks are coded on 3 bits to a total of 24 bits for 8 sides):
**      [8] [7] [6] _ [5] [4] [3] _ [2]  _   [1]     (64 bits)
**     {player2}  -  {player1} -  {free} - {value}
*******************************************************/
Heuristic::Cell                    Heuristic::encryptData(unsigned char value, std::array<unsigned char, 8>& influence)
{
  Heuristic::Cell                  resultingCell = value;

  for (int i = 0; i < 8; i++)
  {
    resultingCell |= (static_cast<Heuristic::Cell>((influence[i] & 0xF0) >> 4) << (40 + (8 - (i + 1)) * 3));
    resultingCell |= (static_cast<Heuristic::Cell>((influence[i] & 0x0F)) << (16 + (8 - (i + 1)) * 3));
  }
  return resultingCell;
}

/*
** Split a Cell-encrypted data into an array of value defined as follow :
** value (8 bits) [8]             [7][6][5]               [4]             [3][2][1]
**         {should be 0}  {streak value for color 2}  { should be 0}    { streak value for color 1}
** (Nota) : As a goban state where a streak would be greater than 4 should not be a thing, only 3 bits are necessary to encore data
** The eight values are corresponding to each possible directions a streak could stand */
Heuristic::influence             Heuristic::decryptData(Heuristic::Cell const c)
{
  Heuristic::influence           localInfluence = { {0} };
  int                            p1((c >> (2 * 8)) & 0x00ffffff), p2((c >> (5 * 8)));

  for (int i = 0; i < 8; i++)
  {
   localInfluence.paths[i] = (((((p2>>(((8-(i+1))*3)))&0x07))<<4) + ((p1>>(((8-(i+1))*3)))&0x07));
  }
  return localInfluence;
}

/*
** For a given position, mesures the contiguous streak of colour it creates or break */
Heuristic::Cell                    Heuristic::mesureInfluence(int x, int y, std::array<Heuristic::Cell, 361>& goban)
{
    std::array<unsigned char, 8>   localInfluence = {0};
    int                            i(0);


    for (auto vY = -1; vY <= 1; vY++)
      for (auto vX = -1; vX <= 1; vX++)
      {
        if (vX != 0 || vY != 0)
        {
          localInfluence[i] = searchDirection(x, y, vX, vY, goban);
          ++i;
        }
      }
    return Heuristic::encryptData(goban[POS(x, y)], localInfluence);
}

/*
** Fill an array with the value of surrounding cell of (x, y). if no Piece have been found, return false */
bool                    Heuristic::perimeter(int x, int y, std::array<char, 8>& dataCollector,std::array<char, 361>const& goban) {

  int                   bounds[4] = {x <= 0 ? 0 : (x - 1), y <= 0 ? 0 : (y - 1), x >= 18 ? 18 : (x + 1), y >= 18 ? 18 : (y + 1)};
  int                   i = 0;
  bool                  hasFoundPiece = false;

  for (auto yIt = bounds[1]; yIt <= bounds[3]; yIt++) {
    for (auto xIt = bounds[0]; xIt <= bounds[2];  xIt++) {

      if (xIt != x || yIt != y)
      {
        if (((dataCollector[i] = goban[POS(xIt, yIt)]) & 3) != 0)
          hasFoundPiece = true;
        ++i; /* ( i = bounds[0] + (bounds[1] * 3) ? */
      }
    }
  }
  return hasFoundPiece;
}

unsigned char         Heuristic::AknowledgeCell(int x, int y, std::array<char, 361>const& goban)
{
  std::array<char, 8> dataCollector{0};
  char                sides[2] = {0, 0};

  if (Heuristic::perimeter(x, y, dataCollector, goban) == true)
  {
    for (int i = 0; i < 8; i++)
    {
        if (sides[0] < 4)
          sides[0] += !(!(dataCollector[i] & 1));
        if (sides[1] < 4)
          sides[1] += !(!(dataCollector[i] & 2));
    }
  }
  return goban[POS(x, y)] + (sides[0] << 5) + (sides[1] << 2);
}

bool                    Heuristic::operator<=(Heuristic::stateNode const& l, Heuristic::stateNode const& r) {

  return l.score <= r.score;
}
