#pragma once

#include "../view/GameView.h"

/**
 * Controller component of the Asteroids implementation following the MVC pattern.
 *
 * Contains and manages the other components and include the main game loop.
 */
class GameController {
public:
    std::shared_ptr<GameModel> model;
    GameView view;
    DirectSound soundApi;

    GameController(std::shared_ptr<GameModel> model);
    ~GameController();

    void setupSound();
    void updateSound();
    void update();
};