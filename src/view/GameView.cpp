#include "GameView.h"

#include <iostream>

int counterr = 0;

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

    Text::setup();
}

GameView::~GameView() {

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
    if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
        keyboardEvents.push_back("RIGHT");
    }
    else if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
        keyboardEvents.push_back("LEFT");
    }
    if (glfwGetKey(this->window, GLFW_KEY_H) == GLFW_PRESS) {
        keyboardEvents.push_back("H");
        if (counterr == 0) {
            this->AddText(900, "DAVID IST EIN KEK", 50.0f, 50.0f);
            counterr++;
        }
    }

    return keyboardEvents;
}


void GameView::AddText(unsigned int id, std::string text, float x, float y) {
    Text newText(id, text, x, y, this->model->windowX, this->model->windowY);
    this->texts.insert({id, std::move(newText)});
}

//TODO: Simplify
void GameView::AddActor(unsigned int id, ActorType actorType) {
    ActorTypeData typeData = getActorDataFromType(actorType);

    if (this->actorDataPerType.count(actorType) == 1) {
        auto test = this->actorDataPerType[actorType];

        if (typeData.texturePath == test->texture.getFilePath()) {
            ActorView newActor(actorDataPerType.find(actorType)->second, typeData.shaderPath, id, this->model->windowX, this->model->windowY);
            this->actors.insert({ id, std::move(newActor) });
        }
        else {
            if (actorType == ActorType::Triangle) {
                std::shared_ptr<ActorDataView> newActorType = std::make_shared<ActorDataView>(typeData.indices.data(), typeData.positions.data(), 6, typeData.positions.size(), typeData.texturePath);
                this->actorDataPerType.insert({ actorType, newActorType });

                ActorView newActor(newActorType, typeData.shaderPath, id, this->model->windowX, this->model->windowY);
                this->actors.insert({ id, std::move(newActor) });
            }
            else {
                std::shared_ptr<ActorDataView> newActorType = std::make_shared<ActorDataView>(typeData.indices.data(), typeData.positions.data(), 8, typeData.positions.size(), typeData.texturePath);
                this->actorDataPerType.insert({ actorType, newActorType });

                ActorView newActor(newActorType, typeData.shaderPath, id, this->model->windowX, this->model->windowY);
                this->actors.insert({ id, std::move(newActor) });
            }
        }
    } else {
        if (actorType == ActorType::Triangle) {
            std::shared_ptr<ActorDataView> newActorType = std::make_shared<ActorDataView>(typeData.indices.data(), typeData.positions.data(), 6, typeData.positions.size(), typeData.texturePath);
            this->actorDataPerType.insert({ actorType, newActorType });

            ActorView newActor(newActorType, typeData.shaderPath, id, this->model->windowX, this->model->windowY);
            this->actors.insert({ id, std::move(newActor) });
        } else {
            std::shared_ptr<ActorDataView> newActorType = std::make_shared<ActorDataView>(typeData.indices.data(), typeData.positions.data(), 8, typeData.positions.size(), typeData.texturePath);
            this->actorDataPerType.insert({ actorType, newActorType });

            ActorView newActor(newActorType, typeData.shaderPath, id, this->model->windowX, this->model->windowY);
            this->actors.insert({ id, std::move(newActor) });
        }
    }
}

void GameView::Render()  {
    //GLCall(glTexCoord2i(0, 0));
    unsigned int count = 0;
    for (auto& actor : this->actors) {
        actor.second.data->texture.Bind();
        actor.second.shader.Bind();
        actor.second.data->va.Bind();
        actor.second.data->ib.Bind();

        GLCall(glDrawElements(GL_TRIANGLES, actor.second.data->ib.GetCount(), GL_UNSIGNED_INT, nullptr));
        count++;
    }

    if (Text::texture) {
        Text::texture->Bind();
    }

    for (auto& text : this->texts) {
        int counter = 0;
      
        for (auto& character : text.second.characters) {
            int row = charCoordinates[text.second.text[counter]].first;
            int column = charCoordinates[text.second.text[counter]].second;
      
            //GLCall(glTexCoord2i(row * 16, column * 32));
            character.shader.Bind();
            character.data->va.Bind();
            character.data->ib.Bind();
      
            GLCall(glDrawElements(GL_TRIANGLES, character.data->ib.GetCount(), GL_UNSIGNED_INT, nullptr));
            counter++;
        }
    }
}

void GameView::Update() {
    // delete actors in view
    for (auto actor = this->actors.cbegin(), next_actor = actor; actor != this->actors.cend(); actor = next_actor)
    {
        ++next_actor;
        if (model->actors.count(actor->first) != 1) {
            this->actors.erase(actor);
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
}

void GameView::Clear() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}