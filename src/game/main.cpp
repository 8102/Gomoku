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

//#ifdef      _WIN32
//int         WINAPI    WinMain() {}
//#elif       _linux
int main() {
//#endif      /* !PlatForm */

  Board obj;

  GameEngine    engine(obj);
  engine.init();
  engine.run();
  return (0);
}
