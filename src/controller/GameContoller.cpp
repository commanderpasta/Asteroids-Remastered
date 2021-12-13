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

        std::vector<std::string> keyboardInput = this->view.GetInput();
        if (std::find(keyboardInput.begin(), keyboardInput.end(), "FORWARD") != keyboardInput.end()) {
            this->model->MovePlayerForward();
        }

        if (std::find(keyboardInput.begin(), keyboardInput.end(), "RIGHT") != keyboardInput.end()) {
            this->model->RotatePlayerRight();
        }

        else if (std::find(keyboardInput.begin(), keyboardInput.end(), "LEFT") != keyboardInput.end()) {
            this->model->RotatePlayerLeft();
        }
        
        // test removing an actor
        /*if (std::find(keyboardInput.begin(), keyboardInput.end(), "H") != keyboardInput.end()) {
            this->model->actors.erase(this->model->actors.rbegin()->first);
        }*/

        for (auto& asteroid : this->model->asteroids) {
            asteroid->Move();
        }
        
        this->view.Update();
        this->view.Render();

        // Swap front and back buffers
        this->view.SwapBuffers();
    }
}