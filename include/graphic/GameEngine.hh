#ifndef        ___GAME_ENGINE_HH___
# define       ___GAME_ENGINE_HH___

#include      <SFML/Graphics.hpp>

#include      "Board.hh"
#include      "common.hpp"
#include      "Referee.hh"

# define      DEFAULT_WIN_W  1600
# define      DEFAULT_WIN_H  900
# define      WIN_TITLE     "I wish I was clever"

class         GameEngine
{
public:
  GameEngine(Board& board);
  ~GameEngine();

public:

  void        loadAssets();
  void        initButtons();
  void        init(unsigned int const& winWidth = DEFAULT_WIN_W, unsigned int const& winHeight = DEFAULT_WIN_H, std::string const& title = WIN_TITLE);
  void        run();
  void        stop();

public:

  bool        treatEvent(std::string& s);
  bool        treatAction(std::string& s);
  bool        getTarget(sf::Vector2i& v);

public:

  bool        getButtonTarget();
  void        drawRulesState();
  void        resetBoard();
  void        changeCaptureRule();
  void        changeDbleThreeRule();

private:

  Board&      _board;
  Referee     _referee;

private:

  std::unique_ptr< sf::RenderWindow >   _win;
  std::map<std::string, sf::Texture>    _textures;
  sf::Font                              _font;
  std::vector<sf::RectangleShape>       _buttons;
  void                                  (GameEngine::*_buttonFct[4])();

private:

  unsigned int              _playerIndex;
  std::vector<sf::Color>    _playerColors;

private:

  bool      _isRunning;
  bool      _winner;
  std::string _info;
};

#endif         /* !___GAME_ENGINE_HH___ */
