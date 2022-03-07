#include "GameController.h"

GameController::GameController(std::shared_ptr<GameModel> model)
    : view(model) {

    this->model = model;
}

GameController::~GameController() {}

void GameController::setupSound() {
    HWND hwnd = this->view.getHwnd();
    this->soundApi.initialize(hwnd);

    //load every sound that will be used
    for (std::string& fileName : this->model->soundFileNames) {
        this->soundApi.loadSound(fileName, "res/sounds/" + fileName + ".wav");
    }
}

void GameController::updateSound() {
    for (auto& change : this->model->soundEvents) {
        auto it = std::find_if(this->soundApi.sounds.begin(), this->soundApi.sounds.end(), [change](DirectSound::Sound& s) {return s.name == change.soundName;} );
        
        if (it != this->soundApi.sounds.end()) {
            switch (change.action) {
                case GameModel::SoundAction::PLAY:
                    this->soundApi.playSound(std::move(*it), false);
                    break;
                case GameModel::SoundAction::STOP:
                    this->soundApi.stopSound(std::move(*it));
                    break;
                case GameModel::SoundAction::LOOP:
                    if (!it->isLooping) {
                        this->soundApi.playSound(std::move(*it), true);
                    }
                    break;
                default:
                    break;
            }
        }
    }

    this->model->clearSoundChanges();
}

void GameController::update() {
    this->model->Setup();
    this->view.setup();
    this->setupSound();

    const int TICKS_PER_SECOND = 144;
    const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
    const int MAX_FRAMESKIP = 10;

    auto nextGameTick = this->model->currentFrameTime;
    auto lastRenderTick = this->model->currentFrameTime;

    bool isGameOver = false;

    while (!view.ShouldWindowClose())
    {
        // Render here
        this->view.Clear();

        if (isGameOver) {
            break; //TODO: END SCREEN
        }

        // update model at a fixed rate (TICKS_PER_SECOND), independently from view 
        // reference https://web.archive.org/web/20160328091806/http://www.koonsolo.com/news/dewitters-gameloop/
        int loops = 0;
        while (steady_clock::now() > nextGameTick && loops < MAX_FRAMESKIP) {
            this->model->setCurrentTime();

            if (this->model->playerLives == 0) {
                isGameOver = true; //TODO: END SCREEN
            }

            this->model->checkParticleLifetimes();
            this->model->checkProjectileLifetimes();
            this->model->setShipDirection();
            this->model->checkShipLifetime();

            this->model->checkLevel();

            this->model->checkPlayerDeath();
            this->model->checkPlayerHyperSpace();
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

            if (std::find(keyboardInput.begin(), keyboardInput.end(), "SHIFT") != keyboardInput.end()) {
                this->model->activateHyperSpace();
            }

            if (std::find(keyboardInput.begin(), keyboardInput.end(), "RIGHT") != keyboardInput.end()) {
                this->model->RotatePlayerRight();
            }

            else if (std::find(keyboardInput.begin(), keyboardInput.end(), "LEFT") != keyboardInput.end()) {
                this->model->RotatePlayerLeft();
            }

            //this->view.checkWindowResize();

            this->model->updatePositions();
            this->model->checkCollisions();

            this->updateSound();

            nextGameTick += milliseconds(SKIP_TICKS);
            loops++;
        }

        this->view.Update();
        this->view.Render();

        // Swap front and back buffers
        this->view.SwapBuffers();
    }

    //TODO: Shutdown directsound
}