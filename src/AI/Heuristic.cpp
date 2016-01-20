#include "Heuristic.hh"

bool                    Heuristic::operator<=(Heuristic::stateNode const& l, Heuristic::stateNode const& r) {

  return l.score <= r.score;
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
    if ((evaluated == 0 && (evaluated = goban[POS(pX, pY)]) == 0) || (evaluated != goban[POS(pX, pY)]))
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
    unsigned char p1_influence = (influence[i] & 0x0F);
    unsigned char p2_influence = (influence[i] & 0xF0);

    resultingCell |= (static_cast<Heuristic::Cell>(p2_influence) << (40 + (8 - (i + 2)) * 3));
    resultingCell |= (static_cast<Heuristic::Cell>(p1_influence) << (16 + (8 - (i + 1)) * 3));
#ifdef _DEBUG
  //   std::cout << "p1 = " << (int)p1_influence << " ,  p2 = " << (int)p2_influence << std::endl;
  //   std::cout << "value of c = " << resultingCell << std::endl;
  //   for (int i = 64; i >= 0; i--)
  //   {
  //     std::cout << ((resultingCell >> i) & 1);
  //     if ((i % 3) == 0)
  //       std::cout << " ";
  //     if (i == 40 || i == 16)
  //       std::cout << " | ";
  //   }
  // std::cout << std::endl;
#endif /* !DEBUG*/
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
  int                            p1((c >> (2 * 8)) & 0x00ffffff), p2(c >> (5 * 8));

// #ifdef _DEBUG
//   std::cout << "DEBUG : decryptData " << std::endl;
// #endif /* !_DEBUG */
  for (int i = 0; i < 8; i++)
  {
    localInfluence.paths[i] = (((((p2 >> (((8 - (i+1)) * 3))) & 0x07) >> 1) << 4) + ((p1 >> (((8-(i+1)) *3))) & 0x07));
// #ifdef _DEBUG
//     {
//       if (i == 0 || i == 3 || i == 5 || i == 8)
//         std::cout << std::endl;
//       if (i == 4)
//         std::cout << "[  "<< (c & 0x07) <<  "  ]";
//       std::cout << "[" << (localInfluence.paths[i] & 0x0f) << " , " << ((localInfluence.paths[i] >> 4) & 0x07) << "]";
//     }
// #endif /* !_DEBUG */
  }
// #ifdef _DEBUG
//   std::cout << std::endl;
// #endif /* !_DEBUG */
  // std::cout << "{" << (int)(localInfluence.paths[2] >> 4) <<  "}" << std::endl;
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
// #ifdef _DEBUG
//     {
//         std::cout << "DEBUG : Heuristic::mesureInfluence(" << x << ", " << y << ")" << std::endl;
//         for (auto k = 0; k < 8; k++)
//         {
//           if (k == 0 || k == 3 || k == 5 || k == 8)
//             std::cout << std::endl << "    ";
//           if (k == 4)
//             std::cout << "[X]";
//           std::cout << "[" << (int)localInfluence[k] << "]";
//         }
//       std::cout << std::endl << "-------------------" << std::endl;
//     }
// #endif  /* !_DEBUG */

    return Heuristic::encryptData(goban[POS(x, y)], localInfluence);
}

/*
** Fill an array with the value of surrounding cell of (x, y). if no Piece have been found, return false */
bool                    Heuristic::perimeter(int x, int y, std::array<char, 8>& dataCollector,std::array<char, 361>const& goban) {

  int                   bounds[4] = {x <= 0 ? 0 : (x - 1), y <= 0 ? 0 : (y - 1), x >= 18 ? 18 : (x + 1), y >= 18 ? 18 : (y + 1)};
  int                   i = 0;
  bool                  hasFoundPiece = false;

#ifdef  _DEBUG
        {
          std::cout << "_DEBUG : Heuristic::PerimetericData() -- call on ("<< x << ", " << y << ") / POS(" << POS(x, y) << ")" << std::endl;
          std::cout << "Bounds : x = {" << bounds[0] << ", " << bounds[2] << "} -- y = {" << bounds[1] << ", " << bounds[3] << "}" << std::endl;
        }
#endif  /* !_DEBUG */
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
