//
// Game.hh for gomoku in /home/guiz/School/rendu/gomoku/include/
//
// Made by Guiz
// Login   <briard_g@epitech.eu>
//
// Started on  Thu Dec 10 05:30:54 2015 Guiz
// Last update Tue Dec 15 05:18:28 2015 Guiz
//

#ifndef GAME_HH_
# define GAME_HH_

# include "Board.hh"
# include "IPlayer.hh"
# include "Referee.hh"

class		Game {
public:
  typedef enum	gameMode {
    oneOnOne = 0,
    playerOneIA = 1,
    playerTwoIA = 2
  }		gm;

private:
  Board*		_board;
  bool		_oneOnOne;
  IPlayer*	_playerOne;
  IPlayer*	_playerTwo;
  Referee*	_referee;
  gm		_gameMode;
  bool		_gameOver;

public:
  // Ctor & Dtor
  Game();
  ~Game();

  // Getters
  gm		getGameMode() const;

  // Setters
  void		setGameMode(Game::gm);
  void		setGameOver(bool);

  // Game routines
};

#endif // !GAME_HH_
