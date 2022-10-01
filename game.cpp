#include "game.h"
#include "stringhelper.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>


const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);
const float BLOCK_WIDTH = WIDTH / 10;
const float BLOCK_HEIGHT = HEIGHT / 40;


Game::Game(): mWindow(sf::VideoMode(1000, 600), "Apache Sieze", sf::Style::Close)
    , mTexture()
    , helicopter(sf::Vector2f(COPTER_WIDTH, COPTER_HEIGHT))
    , exp(sf::Vector2f(EXP_WIDTH, EXP_HEIGHT))
    , mStatisticsUpdateTime() //timer update
    , mStatisticsNumFrames(0)
    , mIsMovingUp(false)  //movement status up
    , mIsMovingDown(false) //movement status down
    , mIsMovingRight(false) //movement status right
    , mIsMovingLeft(false) //movement status left
    , PlayerSpeed(0)
    , mFont()
    , dead(false)
    , height(8)
    , mStatisticsText()
    , acc(9)
{
    //Loading sprites and checking status
    if (!mTexture.loadFromFile("heli.png"))
    {
        // Handle loading error
    }
    if (!smoke_texture.loadFromFile("smoke.png"))
    {
        // Handle loading error
    }
    if (!exp_texture.loadFromFile("explosion.png"))
    {
        // Handle loading error
    }
    exp.setTexture(&exp_texture);
    helicopter.setTexture(&mTexture);
    helicopter.setPosition(100.f, 100.f);
    srand(time(NULL));
    for (int i = 0; i < WIDTH / BLOCK_WIDTH + 1; i++)
        wall.push_back(height / 2);
    std::vector<sf::RectangleShape> v(height / 2, sf::RectangleShape(sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT)));
    //Loading wall,bottom and top bricks
    for (int i = 0; i <= wall.size(); i++)
    {
        top_bricks.push_back(v);
        bottom_bricks.push_back(v);
    }
    int l = 0, k = 0;
    for (auto &i : top_bricks)
    {
        k = 0;
        for (auto &j : i)
        {
            j.setPosition(l*BLOCK_WIDTH, k*BLOCK_HEIGHT);
            j.setFillColor(sf::Color(137, 232, 162));
                k++;
        }
        l++;
    }
    l = k = 0;
    for (auto &i : bottom_bricks)
    {
        k = 0;
        for (auto &j : i)
        {
            j.setPosition(l*BLOCK_WIDTH, HEIGHT - BLOCK_HEIGHT - k*BLOCK_HEIGHT);
            j.setFillColor(sf::Color(137, 232, 162));
            k++;
        }
        l++;
    }
    fortresses.push_back(sf::RectangleShape(sf::Vector2f(BLOCK_WIDTH, FORT_HEIGHT)));
    fortresses.push_back(sf::RectangleShape(sf::Vector2f(BLOCK_WIDTH, FORT_HEIGHT)));
    fortresses.front().setPosition(10 * BLOCK_WIDTH, height / 3 + rand() % (height / 2));
    fortresses.back().setPosition(18 * BLOCK_WIDTH, height / 3 + rand() % (height / 2));
    fortresses.back().setFillColor(sf::Color(137, 232, 162));
    fortresses.front().setFillColor(sf::Color(137, 232, 162));

    }

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            processEvents();
            if (!update(TimePerFrame))
                return;
        }

        updateStatistics(elapsedTime);
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        switch (event.type)
        //keybind setting for player status
        {
        case sf::Event::KeyPressed:
            handlePlayerInput(event.key.code, true);
            break;

        case sf::Event::KeyReleased:
            handlePlayerInput(event.key.code, false);
            break;

        case sf::Event::Closed:
            mWindow.close();
            exit(0);
            break;
        }
    }
}

bool Game::update(sf::Time elapsedTime)
{
    static int count = 0;
    static int w = 0;
    if (!dead)
    {
        if (mIsMovingUp)
        {
            acc = 20;
            w = (w + 1) % 4;
        }
        else
            acc = 0;

        static int trend = rand() % (height - 2) + 1;
        static int period = 0;
        static int counter = 1;
        if ((counter = (counter + 1) % 1000) == 0)
            if (height < MAX_HEIGHT)
                height++;
        for (auto &i : top_bricks)
        {
            for (auto &j : i)
            {
                j.move(-1 * BLOCK_WIDTH / 10, 0);
            }
        }
        for (auto &i : bottom_bricks)
        {
            for (auto &j : i)
            {
                j.move(-1 * BLOCK_WIDTH / 10, 0);
            }
        }
        if (top_bricks.front().front().getPosition().x <= -1 * BLOCK_WIDTH)
        {

            if (wall.back() < trend)
            {
                wall.pop_front();
                wall.push_back(wall.back() + 1);
            }
            else if (wall.back() > trend)
            {
                wall.pop_front();
                wall.push_back(wall.back() - 1);
            }
            else
                trend = rand() % (height - 2) + 1;

            top_bricks.pop_front();
            bottom_bricks.pop_front();
            std::vector<sf::RectangleShape> v1(wall.back(), sf::RectangleShape(sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT)));
            std::vector<sf::RectangleShape> v2(height - wall.back(), sf::RectangleShape(sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT)));
            for (int i = 0; i < wall.back(); i++)
            {
                v1[i].setPosition(top_bricks.back().front().getPosition().x + BLOCK_WIDTH, i*BLOCK_HEIGHT);
                v1[i].setFillColor(sf::Color(137, 232, 162));
            }
            for (int i = 0; i < height - wall.back(); i++)
            {
                v2[i].setPosition(top_bricks.back().front().getPosition().x + BLOCK_WIDTH, HEIGHT - BLOCK_HEIGHT - i*BLOCK_HEIGHT);
                v2[i].setFillColor(sf::Color(137, 232, 162));
            }
            top_bricks.push_back(v1);
            bottom_bricks.push_back(v2);

        }
        if (fortresses.front().getPosition().x < 0)
        {
            fortresses.pop_front();
            fortresses.push_back(sf::RectangleShape(sf::Vector2f(BLOCK_WIDTH, FORT_HEIGHT)));
            fortresses.back().setPosition(fortresses.front().getPosition().x + 8 * BLOCK_WIDTH, rand() % (HEIGHT - FORT_HEIGHT));
            fortresses.back().setFillColor(sf::Color(137, 232, 162));
        }
        fortresses.front().move(-1 * BLOCK_WIDTH / 10, 0);
        fortresses.back().move(-1 * BLOCK_WIDTH / 10, 0);

        //Now adding smoke to the heli's back
        for (auto &i : smoke)
        {
            i.body.move(sf::Vector2f(-1 * SMOKE_WIDTH, 0));
            i.w = (i.w + 1) % 10;
            i.body.setTextureRect(sf::IntRect(i.w * 128, 0, 128, 128));
        }
        PlayerSpeed += (GRAVITY - acc);
        period = (period + 1) % WALL_PERIOD;
        if (period == 0 && mIsMovingUp)
        {
            smk s;
            s.body = sf::RectangleShape(sf::Vector2f(SMOKE_WIDTH, SMOKE_HEIGHT));
            s.h = s.w = 0;
            s.body.setPosition(helicopter.getPosition());
            s.body.setTexture(&smoke_texture);
            smoke.push_back(s);
            if (smoke.front().body.getPosition().x < 0)
                smoke.pop_front();
        }

        sf::FloatRect helipos = helicopter.getGlobalBounds();
        for (auto &i : top_bricks)
        {
            for (auto &j : i)
            {
                if (helipos.intersects(j.getGlobalBounds()))
                {
                    dead = true;
                    exp.setPosition(helicopter.getPosition().x + 10, helicopter.getPosition().y);
                    return true;
                }
            }
        }
        for (auto &i : bottom_bricks)
        {
            for (auto &j : i)
            {
                if (helipos.intersects(j.getGlobalBounds()))
                {
                    dead = true;
                    exp.setPosition(helicopter.getPosition().x + 10, helicopter.getPosition().y);
                    return true;
                }
            }
        }
        if (helipos.intersects(fortresses.front().getGlobalBounds()) || helipos.intersects(fortresses.back().getGlobalBounds()))
        {
            dead = true;
            exp.setPosition(helicopter.getPosition().x + 10, helicopter.getPosition().y);
            return true;
        }
        helicopter.move(sf::Vector2f(0, PlayerSpeed) * elapsedTime.asSeconds());
        helicopter.setTextureRect(sf::IntRect((w + 1) * 132, 0, -132, 62));
    }
    else
    {
        static int w = 0;
        static int h = 0;
        if (h == 5)
        {
            h = w = 0;
            return false;
        }
        exp.setTextureRect(sf::IntRect(w * 64, h * 64, 64, 64));
        w = (w + 1) % 5;
        if (w == 0)
            h++;
    }
        return true;
}

void Game::render()
{
    mWindow.clear();
    for (auto &i : smoke)
    {
        mWindow.draw(i.body);
    }
    mWindow.draw(helicopter);
    for (auto &i : top_bricks)
    {
        for (auto &j : i)
        {
            mWindow.draw(j);
        }
    }
    for (auto &i : bottom_bricks)
    {
        for (auto &j : i)
        {
            mWindow.draw(j);
        }
    }

    mWindow.draw(fortresses.front());
    mWindow.draw(fortresses.back());
    if (dead)
        mWindow.draw(exp);
    mWindow.draw(mStatisticsText);
    mWindow.display();
}

void Game::wall_draw()
{
    sf::RectangleShape brick(sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT));
    brick.setFillColor(sf::Color::Green);
    int j=0;
    for (auto i = wall.begin(); i != wall.end(); i++)
    {
        for (int k = 0; k < *i; k++)
        {
            brick.setPosition(j*BLOCK_WIDTH, k*BLOCK_HEIGHT);
            mWindow.draw(brick);
        }
        for (int k = 0; k < (height - *i); k++)
        {
            brick.setPosition(j*BLOCK_WIDTH, HEIGHT - BLOCK_HEIGHT - k*BLOCK_HEIGHT);
            mWindow.draw(brick);
        }
        j++;
    }
}

void Game::updateStatistics(sf::Time elapsedTime)
{
    mStatisticsUpdateTime += elapsedTime;
    mStatisticsNumFrames += 1;

    if (mStatisticsUpdateTime >= sf::seconds(1.0f))
    {
        mStatisticsText.setString(
            "Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
            "Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
    if (key == sf::Keyboard::W)
        mIsMovingUp = isPressed;
    else if (key == sf::Keyboard::S)
        mIsMovingDown = isPressed;
    else if (key == sf::Keyboard::A)
        mIsMovingLeft = isPressed;
    else if (key == sf::Keyboard::D)
        mIsMovingRight = isPressed;
}
