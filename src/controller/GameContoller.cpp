#include "GameController.h"

/**
 * Initializes the MVC controller that manages the main game loop.
 * 
 * \param model
 */
GameController::GameController(std::shared_ptr<GameModel> model)
    : view(model) {

    this->model = model;
}

GameController::~GameController() {}

/**
 * Intializes DirectSound for the game's window.
 * 
 * DirectSound needs a HWND window context, which we can get from view.
 */
void GameController::setupSound() {
    HWND hwnd = this->view.getHwnd();
    this->soundApi.initialize(hwnd);

    //Pre-load every sound that will be used
    for (std::string& fileName : this->model->soundFileList) {
        this->soundApi.loadSound(fileName, "res/sounds/" + fileName + ".wav");
    }
}

/**
 * Forwards all sound-related events from model to DirectSound.
 */
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

    this->model->clearSoundEvents(); // Empty the list of sound events since they were handled
}

/**
 * Executes the main game loop.
 * 
 * Handles game ticks for update frequency of model, sound and view. It uses a fixed tick rate for model, and a non-fixed refresh rate for view,
 * meaning a constant game speed over variable rendered frames per second (unless the computer is much, much slower than the tick rate).
 */
void GameController::update() {
    this->model->setup();
    this->view.setup();
    this->setupSound();

    const int TICKS_PER_SECOND = 144;
    const int SKIP_TICKS_IN_MS = 1000 / TICKS_PER_SECOND;
    const int MAX_FRAMESKIP = 10;

    auto nextGameTick = this->model->currentTickTime;
    auto lastRenderTick = this->model->currentTickTime;

    bool isGameOver = false;

    while (!view.shouldWindowClose())
    {
        if (isGameOver) {
            break; //TODO: END SCREEN
        }

        // update model at a fixed tick rate (TICKS_PER_SECOND), independently from view 
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

            std::vector<std::string> keyboardInput = this->view.getInput();
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
                this->model->playerRotateRight();
            }

            else if (std::find(keyboardInput.begin(), keyboardInput.end(), "LEFT") != keyboardInput.end()) {
                this->model->playerRotateLeft();
            }

            //this->view.checkWindowResize();

            this->model->updatePositions();
            this->model->checkCollisions();

            this->updateSound();

            nextGameTick += milliseconds(SKIP_TICKS_IN_MS);
            loops++;
        }

        this->view.update();

        // Render here
        this->view.clearFrameBuffer();
        this->view.render();

        // Swap front and back buffers
        this->view.swapFrameBuffers();
    }

    this->soundApi.shutdown();
}