#include "GameController.h"


GameController::GameController(std::shared_ptr<GameModel> model)
    : view(model) {

    this->model = model;
}

GameController::~GameController() {}

void GameController::Setup() {
    this->view.Update();
}

void GameController::Loop() {
    this->model->Setup();

    while (!view.ShouldWindowClose())
    {
        // Render here
        this->view.Clear();

        this->model->setCurrentTime();
        this->model->checkProjectileLifetimes();
        this->model->setShipDirection();
        this->model->checkShipLifetime();

        this->model->checkPlayerDeath();
        this->model->shipFireProjectile();

        std::vector<std::string> keyboardInput = this->view.GetInput();
        if (std::find(keyboardInput.begin(), keyboardInput.end(), "FORWARD") != keyboardInput.end()) {
            this->model->setPlayerAccelerating(true);
        }
        else {
            this->model->setPlayerAccelerating(false);
        }

        if (std::find(keyboardInput.begin(), keyboardInput.end(), "SPACE") != keyboardInput.end()) {
            this->model->playerFireProjectile();
        }

        if (std::find(keyboardInput.begin(), keyboardInput.end(), "RIGHT") != keyboardInput.end()) {
            this->model->RotatePlayerRight();
        }

        else if (std::find(keyboardInput.begin(), keyboardInput.end(), "LEFT") != keyboardInput.end()) {
            this->model->RotatePlayerLeft();
        }

        this->view.checkWindowResize();
        
        this->model->updatePositions();
        this->model->checkCollisions();
        
        this->view.Update();
        this->view.Render();

        // Swap front and back buffers
        this->view.SwapBuffers();
    }
}