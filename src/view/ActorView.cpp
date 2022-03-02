#include "ActorView.h"

ActorView::ActorView(std::shared_ptr<ActorDataView> data, std::string shaderPath, unsigned int id, float x, float y)
    : data(data), shader(shaderPath), id(id) {

    this->shader.Bind();

    this->isHidden = false;

    this->setResolution(x, y);

    float identityMatrix[4][4] = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };

    this->shader.SetUniformMat4f("u_T", identityMatrix);
    this->shader.SetUniformMat4f("u_R", identityMatrix);

    this->shader.SetUniform1i("u_Texture", 0);
    this->shader.Unbind();
}

void ActorView::setResolution(float x, float y) {
    float LEFT = 0.0f;
    float RIGHT = x;
    float BOTTOM = 0.0f;
    float TOP = y;
    float NEAR = -1.0f;
    float FAR = 1.0f;

    float projectionMatrix[4][4] = {
        {2.0f / (RIGHT - LEFT), 0.0f, 0.0f, -1.0f * (RIGHT + LEFT) / (RIGHT - LEFT)},
        {0.0f, 2.0f / (TOP - BOTTOM), 0.0f, -1.0f * (TOP + BOTTOM) / (TOP - BOTTOM)},
        {0.0f, 0.0f, -2.0f / (FAR - NEAR), -1.0f * ((FAR + NEAR) / (FAR - NEAR))},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };

    this->shader.SetUniformMat4f("u_P", projectionMatrix);
}

ActorView::~ActorView() {

}

void ActorView::SetPosition(float position[3], float angle) {
    this->shader.Bind();
    float translationMatrix[4][4] = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };
    // apply translation to view matrix
    for (int i = 0; i < 3; i++) {
        translationMatrix[i][3] += position[i];
    }

    float rotationMatrix[4][4] = {
        {cosf(angle), sinf(angle), 0.0f, 0.0f},
        {-sinf(angle), cosf(angle), 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };

    this->shader.SetUniformMat4f("u_T", translationMatrix);
    this->shader.SetUniformMat4f("u_R", rotationMatrix);
    this->shader.Unbind();
}

void ActorView::setHidden(bool isHidden) {
    this->isHidden = isHidden;
}