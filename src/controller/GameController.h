#pragma once

#include "../view/GameView.h"

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