#pragma once

#include "../view/GameView.h"

class GameController {
public:
    std::shared_ptr<GameModel> model;
    GameView view;

    GameController(std::shared_ptr<GameModel> model);
    ~GameController();

    void Setup();
    void Loop();
};