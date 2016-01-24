#include      "GameEngine.hh"
#include      "Heuristic.hh"

#ifdef _DEBUG
void  printInfluencialGOBAN(std::array<Heuristic::Cell, 361>& goban, int const& playerInfluence);
#endif
GameEngine::GameEngine(Board& board)
  : _board(board), _referee(board), _ai('2', _referee), _playerIndex(0), _isRunning(false), _winner(false), _info("") {

    _playerColors.push_back(sf::Color::White);
    _playerColors.push_back(sf::Color::Black);
}

GameEngine::~GameEngine() {}

void            GameEngine::init(unsigned int const& winWidth, unsigned int const& winHeight, std::string const& title) {

  _win = std::make_unique< sf::RenderWindow >(sf::VideoMode(winWidth, winHeight), title);
  loadAssets();
  _isRunning = true;
}

void            GameEngine::loadAssets() {

  _textures["button1"].loadFromFile("./assets/capture.png");
  _textures["button2"].loadFromFile("./assets/doubleThree.png");
  _textures["button3"].loadFromFile("./assets/reset.png");
  _textures["button4"].loadFromFile("./assets/quit.png");
  _textures["paper"].loadFromFile("./assets/paper.jpg");
  _textures["paper"].setRepeated(true);
  _textures["background"].loadFromFile("./assets/background.jpg");
  _textures["valid"].loadFromFile("./assets/valid.png");
  _font.loadFromFile("./assets/font.ttf");
}

void            GameEngine::resetBoard() {

  _winner = false;
  _info = "";
  _referee.resetGame();
  _playerIndex = 0;
}

void            GameEngine::changeCaptureRule() {

  _referee.setCaptureRule(!_referee.getCaptureRule());
}

void            GameEngine::changeDbleThreeRule() {

  _referee.setDoubleThreeRule(!_referee.getDoubleThreeRule());
}

void            GameEngine::initButtons() {

  sf::RectangleShape  button(sf::Vector2f(488.0f, 295.0f));
  button.setScale(sf::Vector2f(0.25f, 0.25f));
  button.setTexture(&_textures["button1"]);
  button.setPosition(sf::Vector2f(45.0F * 21, 45.0f * 18.0f));
  _buttons.push_back(button);
  _buttonFct[0] = &GameEngine::changeCaptureRule;
  button.setTexture(&_textures["button2"]);
  button.setPosition(sf::Vector2f(45.0F * 21 + 150.0f, 45.0f * 18.0f));
  _buttons.push_back(button);
  _buttonFct[1] = &GameEngine::changeDbleThreeRule;
  button.setTexture(&_textures["button3"]);
  button.setPosition(sf::Vector2f(45.0F * 21 + 300.0f, 45.0f * 18.0f));
  _buttons.push_back(button);
  _buttonFct[2] = &GameEngine::resetBoard;
  button.setTexture(&_textures["button4"]);
  button.setPosition(sf::Vector2f(45.0F * 21 + 450.0f, 45.0f * 18.0f));
  _buttons.push_back(button);
  _buttonFct[3] = &GameEngine::stop;

}

void            GameEngine::drawRulesState() {

  sf::RectangleShape v(sf::Vector2f(100.0f, 100.0f));
  v.setScale(sf::Vector2f(0.5f, 0.5f));
  v.setTexture(&_textures["valid"]);

  if (_referee.getCaptureRule() == true)
    {
      v.setPosition(sf::Vector2f(45 * 20 + 100, 45.0f * 16.0f));
      _win->draw(v);
    }
  if (_referee.getDoubleThreeRule() == true)
    {
      v.setPosition(sf::Vector2f(45 * 20 + 250, 45.0f * 16.0f));
      _win->draw(v);
    }

}

void            GameEngine::setJailsText(std::string& s1, std::string& s2) {

  std::stringstream ss;
  char const *v = _referee.getJail();

  ss << static_cast<int>(v[0]);
  s1 = "Player 1 captured pawns : ";
  s1 += ss.str();
  ss.str("");
  ss << static_cast<int>(v[1]);
  s2 = "Player 2 captured pawns : ";
  s2 += ss.str();
}


void            GameEngine::run() {

  sf::Text      text("", _font, 60);
  sf::Text      errorText("", _font, 60);
  sf::Text      jailsP1("toto", _font, 30);
  sf::Text      jailsP2("titi", _font, 30);

  text.setColor(sf::Color::Black);
  errorText.setColor(sf::Color::Black);
  jailsP1.setColor(sf::Color::White);
  jailsP1.setPosition(sf::Vector2f(45 * 20.0f + 100.0f, 45.0f * 5.0f));
  jailsP2.setColor(sf::Color::Black);
  jailsP2.setPosition(sf::Vector2f(45 * 20.0f + 100.0f, 45.0f * 7.0f));

  sf::VertexArray   a(sf::Lines, 21 * 2);
  sf::VertexArray   b(sf::Lines, 21 * 2);
  sf::RectangleShape r(sf::Vector2f(45.0f * 20.0f, 45.0f * 20.0f));
  sf::RectangleShape r2(sf::Vector2f(45.0f * 18.0f, 45.0f * 18.0f));
  sf::RectangleShape menu(sf::Vector2f(45.0f * 16, 45.0f * 21.0f));

  menu.setTexture(&_textures["background"]);
  menu.setPosition(sf::Vector2f(45.0f * 20.0f, 0.0f));
  sf::RectangleShape button(sf::Vector2f(488.0f, 295.0f));
  button.setScale(sf::Vector2f(0.25f, 0.25f));
  r.setPosition(sf::Vector2f(0.0f, 0.0f));
  r2.setPosition(sf::Vector2f(45.0f, 45.0f));
  r.setTexture(&_textures["paper"]);
  r.setFillColor(sf::Color(220, 220, 220));
  r2.setTexture(&_textures["paper"]);
  text.setFont(_font);
  errorText.setFont(_font);
  initButtons();
  text.setPosition(45 * 21 + 100, 45);
  errorText.setPosition(45 * 21 + 25.0f, 450);
  for (auto i = 0; i <= 20; i++) {
        a[i * 2].position = vf(i * 45 + 1, 0);
        a[(i * 2) + 1].position = vf(i * 45, 900);
        b[i * 2].position = vf(0, i * 45 - (i == 20 ? 1 : (i == 0 ? -1 : 0)));
        b[(i * 2) + 1].position = vf(900, i * 45 - (i == 20 ? 1 : 0));
      }
  std::string s("");
  while (_isRunning == true)
    {
      _win->clear(sf::Color::Blue);
      _win->draw(r);
      _win->draw(menu);
      _win->draw(r2);
      _win->draw(a);
      _win->draw(b);
      _win->draw(jailsP1);
      _win->draw(jailsP2);
      std::string s1, s2;
      setJailsText(s1, s2);
      jailsP1.setString(s1);
      jailsP2.setString(s2);
      for (auto it = _buttons.begin(); it != _buttons.end(); it++)
        _win->draw(*it);
      drawRulesState();
      if (_playerIndex % 2 == 1 && !_winner)
      {
        auto finalGoban = _referee._board.getBoard();
        auto evaluatedGoban = Heuristic::translateGoban(finalGoban);

        auto plays = Heuristic::listRelevantPlays(evaluatedGoban, 2);
        if (plays.size() > 0)
        {
          auto p = plays[0];
          _referee.putPieceOnBoard(p.x, p.y, 50);
          evaluatedGoban = Heuristic::translateGoban(finalGoban);

          plays = Heuristic::listRelevantPlays(evaluatedGoban, 2);
#ifdef _DEBUG
          printInfluencialGOBAN(evaluatedGoban, 2);
#endif /* !_DEBUG */
        } else _referee.putPieceOnBoard(10, 10, 50);
        ++_playerIndex;
        if (char winner = _referee.getWinner())
        {
          s = std::string("Winner : Player ");
          s += winner;
          _winner = true;
        }
      }
      if (treatEvent(s) == true)
          stop();
      sf::CircleShape pawn(10.0f);
      for (auto i = 0; i < MAX_WIDTH; i++)
        for (auto j = 0; j < MAX_HEIGHT; j++) {
          char c = '0';
          if ((c = _board.getCase(i, j)) != EMPTY) {
            pawn.setFillColor(c == PLAYER1 ? sf::Color::White : (c == PLAYER2 ? sf::Color::Black : (c == '3' ? sf::Color::Red : sf::Color::Yellow)));
            pawn.setPosition(sf::Vector2f(45.0f + i * 45.0f - 10.0f, 45.0f + j * 45.0f - 10.0f));
            _win->draw(pawn);
          }
        }
      errorText.setString(s);
      if (_info.size() < 1) {
        text.setColor(_playerIndex % 2 == 0 ? sf::Color::White : sf::Color::Black);
        text.setString(_playerIndex % 2 == 0 ? "Player 1" : "Player 2");
      }
      else text.setString(_info);
      _win->draw(errorText);
      _win->draw(text);
      _win->display();
    }
}

bool            GameEngine::getTarget(sf::Vector2i& target)
{
  sf::Vector2f  location = sf::Vector2f(sf::Mouse::getPosition(*_win));
  sf::FloatRect bounds(sf::FloatRect(location.x - 5, location.y - 5, location.x + 5, location.y + 5));

  if (_winner == true) return false;
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

bool            GameEngine::treatEvent(std::string& s) {

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
          treatAction(s);
          break;
        default:
          break;
      }
    }
  return false;
}

bool            GameEngine::getButtonTarget() {

  int i = 0;
  for (auto it = _buttons.begin(); it != _buttons.end(); it++)
  {
    if (i < 4 && (*it).getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*_win)))) {
    (this->*_buttonFct[i])();
    return true;
  }
    ++i;
  }
  return false;
}

bool            GameEngine::treatAction(std::string& s) {

  sf::Vector2i  p(0, 0);

  if (_winner == true)
    _info = std::string("Press Reset Game to restart");
  if (getTarget(p) == false)
    return getButtonTarget();
  s = "";
    if (int error = _referee.putPieceOnBoard(p.x - 1, p.y - 1, (_playerIndex % 2) + 1 + 48))
      {
        if (error == NOT_EMPTY_ERROR)
          s = "Cell not Empty";
        else if (error == DOUBLE_THREE_ERROR)
          s = "Double three";
      }
    else
      ++_playerIndex;
  if (char winner = _referee.getWinner())
  {
    s = std::string("Winner : Player ");
    s += winner;
    _winner = true;
  }

  return true;
}
