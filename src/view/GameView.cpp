#include "GameView.h"

#include <iostream>

GameView::GameView(std::shared_ptr<GameModel> model) : model(model), window(nullptr) {
    /* Initialize the library */
    glfwInit();

    this->window = glfwCreateWindow(this->model->windowX, this->model->windowY, "Asteroids: Remastered", NULL, NULL); //glfwGetPrimaryMonitor()

    if (!this->window)
    {
        glfwTerminate();
        //return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(this->window);

    /* Tie frame rate to monitor refresh rate */
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
        //return -1;
    }

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void GameView::setup() {
    Text::setup();

    //does not include all randomized asteroid textures
    std::vector<ActorType> typeList = { ActorType::Booster, ActorType::Player, ActorType::AsteroidLarge, ActorType::AsteroidMedium, ActorType::AsteroidSmall, ActorType::ShipLarge, ActorType::ShipSmall, ActorType::Projectile, ActorType::Character };

    for (auto& type : typeList) {
        ActorTypeData typeData = getActorDataFromType(type);
        std::shared_ptr<ActorDataView> renderData = std::make_shared<ActorDataView>(typeData.indices.data(), typeData.positions.data(), typeData.positions.size() / 2, typeData.positions.size(), typeData.texturePath);

        this->actorDataPerType.insert({type, std::move(renderData) });
    }

    this->AddActor(10000, ActorType::Booster);
    //this->AddText(0, std::to_string(this->model->score), this->model->windowX * 0.1f, this->model->windowY * 0.9f);
}

GameView::~GameView() {

}

HWND GameView::getHwnd() {
    return glfwGetWin32Window(this->window);
}


void GameView::updateResolution() {
    for (auto& actor : this->actors) {
        actor.second.setResolution(this->model->windowX, this->model->windowY);
    }
}

void GameView::checkWindowResize() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    if (width != this->model->windowX || height != this->model->windowY) {
        this->model->windowX = width;
        this->model->windowY = height;

        this->updateResolution();
    }
} 


bool GameView::ShouldWindowClose() const {
    return glfwWindowShouldClose(this->window);
}

void GameView::SwapBuffers() const {
    glfwSwapBuffers(this->window);
}

std::vector<std::string> GameView::GetInput() {
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


void GameView::AddText(unsigned int id, std::string text, float x, float y) {
    Text newText(id, text, x, y, this->model->windowX, this->model->windowY);
    this->texts.insert({id, std::move(newText)});
}

void GameView::removeText(unsigned int id) {
    this->texts.erase(id);
}

//TODO: Simplify
void GameView::AddActor(unsigned int id, ActorType actorType) {
    ActorTypeData typeData = getActorDataFromType(actorType);

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

    std::shared_ptr<ActorDataView> newActorType = std::make_shared<ActorDataView>(typeData.indices.data(), typeData.positions.data(), typeData.positions.size() / 2, typeData.positions.size(), typeData.texturePath);
    this->actorDataPerType.insert({ actorType, newActorType });

    ActorView newActor(newActorType, typeData.shaderPath, id, this->model->windowX, this->model->windowY);
    this->actors.insert({ id, std::move(newActor) });
}

void GameView::addPlayerLife(unsigned int id) {
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

void GameView::Render() {
    unsigned int count = 0;
    if (this->actors.count(10000) != 0) {
        auto booster = this->actors.find(10000);

        if (!booster->second.isHidden) {
            booster->second.data->texture.Bind();
            booster->second.shader.Bind();
            booster->second.data->va.Bind();
            booster->second.data->ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, booster->second.data->ib.GetCount(), GL_UNSIGNED_INT, nullptr));
            count++;
        }
    }

    for (auto& actor : this->actors) {
        if (actor.second.id == 10000) {
            continue;
        }

        actor.second.data->texture.Bind();
        actor.second.shader.Bind();
        actor.second.data->va.Bind();
        actor.second.data->ib.Bind();

        GLCall(glDrawElements(GL_TRIANGLES, actor.second.data->ib.GetCount(), GL_UNSIGNED_INT, nullptr));
        count++;
    }

    if (!this->playerLivesUIObjects.empty()) {
        this->playerLivesUIObjects.front().data->texture.Bind();
        this->playerLivesUIObjects.front().data->va.Bind();
        this->playerLivesUIObjects.front().data->ib.Bind();

        for (auto& object : this->playerLivesUIObjects) {
            object.shader.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, object.data->ib.GetCount(), GL_UNSIGNED_INT, nullptr));
        }
    }

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

void GameView::Update() {
    // delete actors in view
    for (auto actor = this->actors.cbegin(), next_actor = actor; actor != this->actors.cend(); actor = next_actor)
    {
        ++next_actor;
        if (model->actors.count(actor->first) != 1) {
            if (actor->first != 10000) {
                this->actors.erase(actor);
            }
        }
    }

    // add new actors in view
    for (auto& currentActorModel : model->actors) {
        if (this->actors.count(currentActorModel.first) != 1) {
            this->AddActor(currentActorModel.second->id, currentActorModel.second->actorType);
        }

        // update actor position in view
        this->actors.find(currentActorModel.first)->second.SetPosition(currentActorModel.second->position, currentActorModel.second->rotation);
    }

    //score
    auto score = this->texts.find(0);

    if (score != this->texts.end()) {
        this->removeText(0);
    }

    this->AddText(0, std::to_string(this->model->score), this->model->windowX * 0.1f, this->model->windowY * 0.9f);

    //lives
    int playerLivesDisplayedDifference = this->playerLivesUIObjects.size() - this->model->playerLives;

    if (playerLivesDisplayedDifference < 0) {
        for (int i = 0; i > playerLivesDisplayedDifference; i--) {
            this->addPlayerLife(9000 + this->playerLivesUIObjects.size());

            float uiDisplayPosition[3] = { this->model->windowX * 0.1f + 16.0f * this->playerLivesUIObjects.size(), this->model->windowY * 0.9f - 50.0f, 0.0f};
            this->playerLivesUIObjects.back().SetPosition(uiDisplayPosition, 0.0f);
        }
    } else if (playerLivesDisplayedDifference > 0) {
        for (int i = 0; i < playerLivesDisplayedDifference; i++) {
            this->playerLivesUIObjects.pop_back();
        }
    }

    //show player booster
    unsigned int boosterId = 10000;
    if (this->actors.count(boosterId) != 0) {
        if (this->model->player) {
            this->actors.find(boosterId)->second.setHidden(!this->model->player->isBoosterActive);

            if (!this->actors.find(boosterId)->second.isHidden) {
                float boosterPosition[3] = { this->model->player->position[0], this->model->player->position[1], this->model->player->position[2] };
                this->actors.find(boosterId)->second.SetPosition(boosterPosition, this->model->player->rotation + MY_PI);
            }
        }
        else {
            this->actors.find(boosterId)->second.setHidden(true);
        }
    }

}

void GameView::Clear() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}