#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "GameController.h"

#define WINDOW_X 960
#define WINDOW_Y 540

int main(void) {
    std::shared_ptr<GameModel> model_ptr = std::make_shared<GameModel>(WINDOW_X, WINDOW_Y);
    GameController controller(model_ptr);

    controller.Loop();

    return 0;
}