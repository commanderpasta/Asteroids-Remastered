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
    std::chrono::steady_clock::time_point projectileCooldown = std::chrono::steady_clock::now() - std::chrono::seconds(1);

    while (!view.ShouldWindowClose())
    {
        // Render here
        this->view.Clear();

        std::vector<std::string> keyboardInput = this->view.GetInput();
        if (std::find(keyboardInput.begin(), keyboardInput.end(), "FORWARD") != keyboardInput.end()) {
            this->model->setPlayerAccelerating(true);
        }
        else {
            this->model->setPlayerAccelerating(false);
        }

        if (std::find(keyboardInput.begin(), keyboardInput.end(), "SPACE") != keyboardInput.end()) {
            using namespace std::chrono;
            steady_clock::time_point currentTime = steady_clock::now();

            duration<double> timeSpan = duration_cast<duration<double>>(currentTime - projectileCooldown);
            if (timeSpan.count() > 0.25) {
                projectileCooldown = currentTime;
                this->model->fireProjectile();
            }
        }

        if (std::find(keyboardInput.begin(), keyboardInput.end(), "RIGHT") != keyboardInput.end()) {
            this->model->RotatePlayerRight();
        }

        else if (std::find(keyboardInput.begin(), keyboardInput.end(), "LEFT") != keyboardInput.end()) {
            this->model->RotatePlayerLeft();
        }
        
        this->model->updatePositions();
        this->model->checkCollisions();
        
        this->view.Update();
        this->view.Render();

        // Swap front and back buffers
        this->view.SwapBuffers();
    }
}