//
// main.cpp for gomoku in /home/guiz/School/rendu/gomoku/src/
//
// Made by Guiz
// Login   <briard_g@epitech.eu>
//
// Started on  Thu Dec  3 17:15:18 2015 Guiz
// Last update Thu Dec  3 17:16:41 2015 Guiz
//

#include "Board.hh"
#include "GameEngine.hh"
#include "Heuristic.hh"

#ifdef _DEBUG
#include <iomanip>
void  printInfluencialGOBAN(std::array<Heuristic::Cell, 361>& goban)
{
  Heuristic::influence  tab[361];
  std::string s[7] = {"\033[0m", "\033[031m", "\033[032m", "\033[033m", "\033[034m", "\033[035m", "\033[037m"};


  for (int y = 0; y < 19; y++)
    for (int x = 0; x < 19; x++)
      tab[y * 19 + x] = Heuristic::decryptData(Heuristic::mesureInfluence(x, y, goban));

  std::cout << std::endl << "Influencial Map of actual GOBAN state : " << std::endl << std::endl;

  for (int y = 0; y < 19; y++)
    {
      for (int x = 0; x < 19; x++)
        std::cout << "{" << std::setw(2) << x << ", " << std::setw(2) << y << "}    ";

      std::cout << std::endl;
      for (int i = 0; i < 3; i++)
      {
        for (int x = 0; x < 19; x++)
        {
          int  cellValue = goban[POS(x, y)] & 0x07;
          int  cellColor = cellValue;
          if (cellColor == 0)
            for (int k = 0; k < 8; k++)
              {
                int maxStreak = tab[y * 19 + x].paths[k] & 0x07;
                cellColor = (cellColor > maxStreak ? cellColor : maxStreak);
                if (cellColor == 4)
                  break;
              }
          else cellColor = cellValue;
          for (int j = 0; j < 3; j++)
              {
                if (i != 1 || j != 1)
                {
                  int cellV = (int)(tab[y * 19 + x].paths[(i * 3 + j) >= 4 ? (i * 3 + j -1) : (i * 3 + j) ] & 0x07);
                  std::cout <<  s[cellValue > 0 ? cellValue : (cellColor == 0 ? 0 : cellColor +  2)] << "[" << cellV  << "]"<< s[0];
                }
                else
                  std::cout <<  s[cellValue] << "[" << cellValue  << "]"<< s[0];
              }
            std::cout << "   ";
        }
        std::cout << std::endl;
      }
    std::cout << std::endl;
    }
    std::cout << std::endl;
    tab[POS(7, 8)] = Heuristic::decryptData(Heuristic::mesureInfluence(7, 8, goban));

}

#endif /* _DEBUG */


int main() {

  Board obj;

  GameEngine    engine(obj);

#ifdef _DEBUG
  std::array<Heuristic::Cell, 361> goban = {0};

  goban[POS(8, 8)] = 1;
  goban[POS(0, 0)] = 1;
  goban[POS(1, 1)] = 1;
  goban[POS(2, 6)] = 1;
  goban[POS(3, 3)] = 1;
  goban[POS(4, 4)] = 1;
  goban[POS(5, 5)] = 1;
  goban[POS(15, 6)] = 1;
  goban[POS(6, 6)] = 1;
  goban[POS(2, 3)] = 1;
  goban[POS(15, 15)] = 1;
  goban[POS(15, 18)] = 1;

  goban[POS(15, 6)] = 1;
  printInfluencialGOBAN(goban);
#endif
  engine.init();
  engine.run();
  return (0);
}
