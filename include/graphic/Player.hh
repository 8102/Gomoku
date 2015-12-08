#ifndef       ___PLAYER_HH___
# define      ___PLAYER_HH___

# include     "Board.hh"
# include     "common.hpp"

class         Player
{
public:

  Player(bool const& isControlled = false);
  ~Player();

public:

  bool        isControlled() const;

private:

  bool        _controlled;

};

#endif        /* !___PLAYER_HH___ */
