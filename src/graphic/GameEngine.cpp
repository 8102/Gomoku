#include      "GameEngine.hh"

GameEngine::GameEngine(Board& board)
  : _board(board), _referee(board), _playerIndex(0), _isRunning(false) {

    _playerColors.push_back(sf::Color::White);
    _playerColors.push_back(sf::Color::Black);
}

GameEngine::~GameEngine() {}

void            GameEngine::init(unsigned int const& winWidth, unsigned int const& winHeight, std::string const& title) {

  _win = make_unique< sf::RenderWindow >(sf::VideoMode(winWidth, winHeight), title);
  _isRunning = true;
}

void            GameEngine::run() {

  sf::VertexArray   a(sf::Lines, 21 * 2);
  sf::VertexArray   b(sf::Lines, 21 * 2);

  for (auto i = 0; i <= 20; i++) {
        a[i * 2].position = vf(i * 45 + 1, 0);
        a[(i * 2) + 1].position = vf(i * 45, 900);
        b[i * 2].position = vf(0, i * 45 - (i == 20 ? 1 : (i == 0 ? -1 : 0)));
        b[(i * 2) + 1].position = vf(900, i * 45 - (i == 20 ? 1 : 0));
      }
  while (_isRunning == true)
    {
      _win->clear(sf::Color::Blue);
      _win->draw(a);
      _win->draw(b);
      if (treatEvent() == true)
          stop();
      for (auto it = _pawns.begin(); it != _pawns.end(); it++)
        _win->draw(*it);
      _win->display();
    }
}

bool            GameEngine::getTarget(sf::Vector2i& target)
{
  sf::Vector2f  location = sf::Vector2f(sf::Mouse::getPosition(*_win));
  sf::FloatRect bounds(sf::FloatRect(location.x - 5, location.y - 5, location.x + 5, location.y + 5));

  for (auto i = 1; i < 20; i++)
    for (auto j = 1; j < 20; j++)
      {
        if (bounds.contains(vf(i * 45, j * 45))) {
          target = sf::Vector2i(i, j);
          return true;
        }
      }
  return false;
}

void            GameEngine::stop() {

  _isRunning = false;
}

bool            GameEngine::treatEvent() {

  sf::Event     e;

  while (_win->pollEvent(e))
    {
      switch (e.type)
      {
        case  sf::Event::Closed:
          return true;
        case sf::Event::KeyPressed:
          if (e.key.code == sf::Keyboard::Escape)
            return true;
          else return false;
        case sf::Event::MouseButtonPressed:
          treatAction();
          break;
        default:
          break;
      }
    }
  return false;
}

bool            GameEngine::treatAction() {

  sf::Vector2i  p(0, 0);

  if (getTarget(p) == false)
    return false;
  // if (_board.getCase(p.x - 1, p.y - 1) != EMPTY)
  //   return false;


  /* decision de l'arbitre */

//  _board.setCase(p.x - 1, p.y - 1, (_playerIndex % 2) + 1 + 48);
  try
  {
    _referee.putPieceOnBoard(p.x - 1, p.y - 1, (_playerIndex % 2) + 1 + 48);
    sf::CircleShape     c(10.0f);
   c.setFillColor(_playerColors[_playerIndex % 2]);
   c.setPosition(p.x * 45 - 10, p.y * 45 - 10);
   _pawns.push_back(c);
   ++_playerIndex;
  }
  catch (NotEmptyError &e)
  {
    std::cerr << "Cell not empty..." << std::endl;
    std::cerr << e.what() << std::endl;
  }
  catch (DoubleThreeRule &e)
  {
    std::cerr << "[WARNING]: GameEngine -> treatAction: You're not allowed to put your piece here (double three rule) !" << std::endl;
    std::cerr << e.what() << std::endl;
  }
  if (char winner = _referee.getWinner())
  {
    std::cout << "WINNER: " << winner << std::endl; 
  }
  return true;
}
