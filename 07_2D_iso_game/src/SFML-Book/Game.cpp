#include <SFML-Book/Game.hpp>
#include <iostream>

#include <SFML-Book/Helpers.hpp>

namespace book
{
    Game::Game(int X, int Y) : _window(sf::VideoMode(X,Y),"07 2D Iso Game"), _level(nullptr)
    {
        _window.setFramerateLimit(65);

        onPickup = [this](Level::Param& param){
            std::cout<<"pickup on "<<param.coord.x<<" "<<param.coord.y<<std::endl;
        };

        _team = new Team(_window,1,sf::Color(255,255,255,255));

    }

    Game::~Game()
    {
        delete _level;
    }

    bool Game::load(const std::string& level)
    {
        bool res = true;
        try{
            delete _level;
            _level = new Level(_window,level);
            _level->onPickup = onPickup;

            for(int i=0;i<4;++i)
            {
                Entity& e = _level->createEntity(sf::Vector2i(i,i));
                makeAsMain(e,_team);
            }

        }catch(...)
        {
            std::cerr<<"impossible to create level "<<level<<std::endl;
            res = false;
        }
        return res;
    }

    void Game::run(int minimum_frame_per_seconds)
    {
        sf::Clock clock;
        sf::Time timeSinceLastUpdate;
        sf::Time TimePerFrame = sf::seconds(1.f/minimum_frame_per_seconds);

        while (_window.isOpen())
        {
            processEvents();

            timeSinceLastUpdate = clock.restart();
            while (timeSinceLastUpdate > TimePerFrame)
            {
                timeSinceLastUpdate -= TimePerFrame;
                update(TimePerFrame);
            }

            update(timeSinceLastUpdate);
            render();
        }
    }

    void Game::processEvents()
    {
        //to store the events
        sf::Event event;
        //events loop
        while(_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                _window.close();
            else if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Escape)
                _window.close();
            else
            {
                bool used = false;
                if(_team)
                    used = _team->processEvent(event);
                if(_level and not used)
                    used = _level->processEvent(event);
            }
        }
        if(_team)
            _team->processEvents();
        if(_level)
            _level->processEvents();
    }

    
    void Game::update(sf::Time deltaTime)
    {
        if(_level)
            _level->update(deltaTime);
    }

    void Game::render()
    {
        _window.clear();

        if(_level)
            _level->draw(_window);
        if(_team)
            _team->draw(_window);

        _window.display();
    }
}