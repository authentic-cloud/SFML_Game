#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <list>
#include <vector>

#define WIDTH 640
#define HEIGHT  480
#define COPTER_HEIGHT HEIGHT/13
#define COPTER_WIDTH HEIGHT/(62.f*13) * 132.f
#define MAX_HEIGHT 12
#define WALL_PERIOD 6
#define GRAVITY 8
#define SMOKE_WIDTH 15
#define SMOKE_HEIGHT 15
#define FORT_HEIGHT HEIGHT/10*3
#define EXP_WIDTH 60
#define EXP_HEIGHT 60
struct smk
{
    sf::RectangleShape body;
    int w;
    int h;
};


class Game : private sf::NonCopyable
{
public:
    Game();
    void run();


private:
    void processEvents();
    bool update(sf::Time elapsedTime);
    void render();

    void updateStatistics(sf::Time elapsedTime);
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
    void wall_draw();

private:
    float PlayerSpeed;
    static const sf::Time	TimePerFrame;

    sf::RenderWindow mWindow;
    sf::Texture	mTexture;
    sf::Texture	smoke_texture;
    sf::Texture	exp_texture;
    sf::RectangleShape helicopter;
    bool dead;
    sf::RectangleShape exp;
    std::list<int> wall;
    std::list<std::vector<sf::RectangleShape>> top_bricks;
    std::list<std::vector<sf::RectangleShape>> bottom_bricks;
    std::list<smk> smoke;
    std::list<sf::RectangleShape> fortresses;
    int	height;
    float acc;
    sf::Font mFont;
    sf::Text mStatisticsText;
    sf::Time mStatisticsUpdateTime;
    sf::Vector2f wall_pos;

    std::size_t	mStatisticsNumFrames;
    bool mIsMovingUp;
    bool mIsMovingDown;
    bool mIsMovingRight;
    bool mIsMovingLeft;
};

#endif // GAME_H

