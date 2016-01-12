//
// Player.hh for gomoku in /home/guiz/School/rendu/gomoku/include/
//
// Made by Guiz
// Login   <briard_g@epitech.eu>
//
// Started on  Thu Dec 10 05:06:21 2015 Guiz
// Last update Thu Dec 10 05:19:04 2015 Guiz
//

#ifndef PLAYER_HH_
# define PLAYER_HH_

# include "IPlayer.hh"

class		Player : public IPlayer {
private:
  bool		_hasWon;

public:
  // Ctor & Dtor
  Player();
  virtual ~Player() {};

  // Getters
  bool		getHasWon() const;

  // Setters
  void		setHasWon(bool);

  // Player routines

};

#endif // !PLAYER_HH_
