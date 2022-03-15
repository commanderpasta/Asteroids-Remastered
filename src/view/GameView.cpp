#include "GameView.h"

#include <iostream>

/**
 * Creates a view manager for this game instance.
 * 
 * Attempts to initialize GLFW, OpenGL, and GLEW.
 * 
 * \param model The MVC model used for this game instance.
 */
GameView::GameView(std::shared_ptr<GameModel> model) : model(model), window(nullptr), boosterId(UINT_MAX), gameOverId(UINT_MAX) {
    glfwInit(); // Attempt to intialize the library

    this->window = glfwCreateWindow(this->model->windowX, this->model->windowY, "Asteroids: Remastered", NULL, NULL); //glfwGetPrimaryMonitor()

    if (!this->window)
    {
        glfwTerminate();
    }

    glfwMakeContextCurrent(this->window); // Make the window's context current

    glfwSwapInterval(0); // Disable V-sync (meaning no frame limiter)

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    // Enable texture blending in OpenGL
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

/**
 * Preloads buffer data for each ActorType.
 * 
 * Avoids performance hiccups when having to load new textures and buffers during gameplay.
 */
void GameView::setup() {
    Text::setup();

    //does not include all randomized asteroid textures
    std::vector<ActorType> typeList = { ActorType::Booster, ActorType::Player, ActorType::AsteroidLarge, ActorType::AsteroidMedium, ActorType::AsteroidSmall, ActorType::ShipLarge, ActorType::ShipSmall, ActorType::Projectile, ActorType::Character };

    for (auto& type : typeList) {
        ActorTypeData typeData = getActorDataFromType(type);
        std::shared_ptr<ActorDataView> renderData = std::make_shared<ActorDataView>(typeData.indices.data(), typeData.positions.data(), typeData.positions.size() / 2, typeData.positions.size(), typeData.texturePath);

        this->actorDataPerType.insert({type, std::move(renderData) });
    }

    this->addBooster();
}

GameView::~GameView() {

}

/**
 * Gets the HWND context for this game instance.
 * 
 * Necessary for the DirectSound API to know the window to attach to.
 * 
 * \return The HWND context of the window for this game instance.
 */
HWND GameView::getHwnd() {
    return glfwGetWin32Window(this->window);
}

void GameView::updateFrameTime() {
    this->currentFrameTime = steady_clock::now();
}

/**
 * Changes the resolution of the game while it is active.
 */
void GameView::updateResolution() {
    for (auto& actor : this->actors) {
        actor.second.setResolution(this->model->windowX, this->model->windowY);
    }
}

/**
 * Checks whether the window size has changed.
 * 
 * Updates the resolution if it is the case.
 */
void GameView::checkWindowResize() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    if (width != this->model->windowX || height != this->model->windowY) {
        this->model->windowX = width;
        this->model->windowY = height;

        this->updateResolution();
    }
} 

/**
 * Checks with GLFW whether the window should be closed.
 * 
 * \return A boolean stating whether it should be closed.
 */
bool GameView::shouldWindowClose() const {
    return glfwWindowShouldClose(this->window);
}

/**
 * Directs the GPU to swap the currently displayed frame with the newly drawn frame.
 */
void GameView::swapFrameBuffers() const {
    glfwSwapBuffers(this->window);
}

/**
 * Gets all mapped inputs the player is currently holding.
 * 
 * \return A list with the names of the pressed (mapped) keys.
 */
std::vector<std::string> GameView::getInput() {
    glfwPollEvents();

    std::vector<std::string> keyboardEvents;

    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
        keyboardEvents.push_back("FORWARD");
    }
    if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        keyboardEvents.push_back("SPACE");
    }
    if (glfwGetKey(this->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        keyboardEvents.push_back("SHIFT");
    }
    if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
        keyboardEvents.push_back("RIGHT");
    }
    else if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
        keyboardEvents.push_back("LEFT");
    }

    return keyboardEvents;
}

/**
 * Adds a new UI text to be drawn on the next frame.
 * 
 * \param id The new id of the text.
 * \param text The text to be display
 * \param x The x coordinate of the lower left corner of the text.
 * \param y The y coordinate of the lower left corner of the text.
 */
void GameView::addText(unsigned int id, std::string text, float x, float y) {
    Text newText(id, text, x, y, this->model->windowX, this->model->windowY);
    this->texts.insert({id, std::move(newText)});
}

/**
 * Removes an existing UI text from the list of displayed texts.
 * 
 * \param id The id of the text to be removed
 */
void GameView::removeText(unsigned int id) {
    this->texts.erase(id);
}

void GameView::addBooster() {
    ActorTypeData boosterRenderData = getActorDataFromType(ActorType::Booster);
    auto renderCache = this->actorDataPerType.find(ActorType::Booster);

    ActorView booster(renderCache->second, boosterRenderData.shaderPath, 0, this->model->windowX, this->model->windowY);

    booster.setFlickerFrequency(0.1f);

    this->boosterId = booster.id;
    this->actors.insert({ booster.id, std::move(booster) });
}

/**
 * Adds a new game object to be displayed.
 * 
 * \param id The id of the new game object
 * \param actorType The type of the game object
 */
void GameView::addActor(unsigned int id, ActorType actorType) {
    ActorTypeData typeData = getActorDataFromType(actorType);

    // Check <actorDataPerType> whether the buffers for this actor type are cached
    if (this->actorDataPerType.count(actorType) >= 1) {
        auto range = this->actorDataPerType.equal_range(actorType);

        for (auto& it = range.first; it != range.second; ++it) {
            if (it->second->texture.getFilePath() == typeData.texturePath) { //To support random textures
                ActorView newActor(it->second, typeData.shaderPath, id, this->model->windowX, this->model->windowY);
                this->actors.insert({ id, std::move(newActor) });

                return;
            }
        }
    }

    // ActorData for this type doesn't exist yet, so it needs to be created
    std::shared_ptr<ActorDataView> newActorType = std::make_shared<ActorDataView>(typeData.indices.data(), typeData.positions.data(), typeData.positions.size() / 2, typeData.positions.size(), typeData.texturePath);
    this->actorDataPerType.insert({ actorType, newActorType });

    ActorView newActor(newActorType, typeData.shaderPath, id, this->model->windowX, this->model->windowY);
    this->actors.insert({ id, std::move(newActor) });
}

/**
 * Draw a new bonus life in the UI.
 * 
 * Should be called when a player has gained a bonus life, so it can be
 * displayed on the screen.
 * 
 * \param id The id for the new UI element
 */
void GameView::addPlayerLifeUI(unsigned int id) {
    ActorTypeData typeData = getActorDataFromType(ActorType::Player);

    if (this->actorDataPerType.count(ActorType::Player) >= 1) {
        auto range = this->actorDataPerType.equal_range(ActorType::Player);
        for (auto& it = range.first; it != range.second; ++it) {
            if (it->second->texture.getFilePath() == typeData.texturePath) {
                ActorView newActor(it->second, typeData.shaderPath, id, this->model->windowX, this->model->windowY);
                this->playerLivesUIObjects.push_back(std::move(newActor));
                return;
            }
        }
    }

    std::shared_ptr<ActorDataView> newActorType = std::make_shared<ActorDataView>(typeData.indices.data(), typeData.positions.data(), typeData.positions.size() / 2, typeData.positions.size(), typeData.texturePath);
    this->actorDataPerType.insert({ ActorType::Player, newActorType });

    ActorView newActor(newActorType, typeData.shaderPath, id, this->model->windowX, this->model->windowY);
    this->playerLivesUIObjects.push_back(std::move(newActor));
}

/**
 * Draws all objects to the new frame.
 */
void GameView::render() {
    // Booster
    // Special case as it's the only object that uses the isHidden property
    /*if (this->booster) {

        if (!booster->isHidden) {
            booster->data->texture.Bind();
            booster->shader.Bind();
            booster->data->va.Bind();
            booster->data->ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, booster->data->ib.GetCount(), GL_UNSIGNED_INT, nullptr));
        }
    }*/

    // Game objects
    for (auto& actor : this->actors) {
        if (actor.second.isVisible) {
            actor.second.data->texture.Bind();
            actor.second.shader.Bind();
            actor.second.data->va.Bind();
            actor.second.data->ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, actor.second.data->ib.GetCount(), GL_UNSIGNED_INT, nullptr));
        }
    }

    // UI - Player lives
    if (!this->playerLivesUIObjects.empty()) {
        this->playerLivesUIObjects.front().data->texture.Bind();
        this->playerLivesUIObjects.front().data->va.Bind();
        this->playerLivesUIObjects.front().data->ib.Bind();

        for (auto& object : this->playerLivesUIObjects) {
            object.shader.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, object.data->ib.GetCount(), GL_UNSIGNED_INT, nullptr));
        }
    }
    
    // UI - Texts
    if (Text::texture) {
        Text::texture->Bind();

        for (auto& text : this->texts) {
            int counter = 0;

            for (auto& character : text.second.characters) {
                int row = charCoordinates[text.second.text[counter]].first;
                int column = charCoordinates[text.second.text[counter]].second;

                character.shader.Bind();
                character.data->va.Bind();
                character.data->ib.Bind();

                GLCall(glDrawElements(GL_TRIANGLES, character.data->ib.GetCount(), GL_UNSIGNED_INT, nullptr));
                counter++;
            }
        }
    }
}

/**
 * Updates the view manager to mirror the model component.
 */
void GameView::update() {
    this->updateFrameTime(); 

    // delete actors in view
    for (auto actor = this->actors.cbegin(), next_actor = actor; actor != this->actors.cend(); actor = next_actor)
    {
        ++next_actor;
        if (model->actors.count(actor->first) != 1) {
            if (actor->first != this->boosterId) {
                this->actors.erase(actor);
            }
        }
    }

    // add new actors in view
    for (auto& currentActorModel : model->actors) {
        if (this->actors.count(currentActorModel.first) != 1) {
            this->addActor(currentActorModel.second->id, currentActorModel.second->actorType);
        }

        // update actor position in view
        this->actors.find(currentActorModel.first)->second.setPosition(currentActorModel.second->position, currentActorModel.second->rotation);
    }

    //score
    auto score = this->texts.find(0);

    if (score != this->texts.end()) {
        this->removeText(0);
    }

    this->addText(0, std::to_string(this->model->score), this->model->windowX * 0.1f, this->model->windowY * 0.9f);

    //lives
    int playerLivesDisplayedDifference = this->playerLivesUIObjects.size() - this->model->playerLives;

    if (playerLivesDisplayedDifference < 0) {
        for (int i = 0; i > playerLivesDisplayedDifference; i--) {
            this->addPlayerLifeUI(9000 + this->playerLivesUIObjects.size());

            float uiDisplayPosition[2] = { this->model->windowX * 0.1f + 16.0f * this->playerLivesUIObjects.size(), this->model->windowY * 0.9f - 50.0f};
            this->playerLivesUIObjects.back().setPosition(uiDisplayPosition, 0.0f);
        }
    } else if (playerLivesDisplayedDifference > 0) {
        for (int i = 0; i < playerLivesDisplayedDifference; i++) {
            this->playerLivesUIObjects.pop_back();
        }
    }

    //flickering for player booster
    if (this->model->player) {
        auto player = this->actors.find(this->model->player->id);

        if (player != this->actors.end()) {
            // Flickering for player invincibility
            duration<double> timeSpan = duration_cast<duration<double>>(this->currentFrameTime - this->model->lastPlayerDeath);

            if (timeSpan.count() < 5.0) {
                player->second.setFlickerFrequency(0.25f);
                player->second.updateFlickering(this->currentFrameTime);
            }
            else {
                player->second.setFlickerFrequency(0.0f);
                player->second.isVisible = true;
            }

            // Booster flickering
            auto booster = this->actors.find(this->boosterId);
            if (booster != this->actors.end()) {
                if (this->model->player->isAccelerating && player->second.isVisible) {
                    booster->second.updateFlickering(this->currentFrameTime);

                    float boosterPosition[2] = { this->model->player->position[0], this->model->player->position[1] };
                    booster->second.setPosition(boosterPosition, this->model->player->rotation + MY_PI);
                }
                else {
                    booster->second.isVisible = false;
                }
            }
        }
    } else {
        auto booster = this->actors.find(this->boosterId);
        if (booster != this->actors.end()) {
            booster->second.isVisible = false;
        }
    }
}

/**
 * Clear the new frame.
 * 
 * Is used remove contents of the last frame after it is displayed to the screen.
 */
void GameView::clearFrameBuffer() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void GameView::gameOver() {
    if (this->gameOverId == UINT_MAX) {
        this->gameOverId = generateId();
        this->addText(this->gameOverId, "GAME OVER", this->model->windowX / 2.0f - 4.5f * 30.0f, 2.0f * this->model->windowY / 3.0f);
    }
}
