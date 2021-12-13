#include "ActorView.h"

#include "gl/VertexBufferLayout.h"

#include <iostream>


float LEFT = 0.0f;
float RIGHT = 960.0f;
float BOTTOM = 0.0f;
float TOP = 540.0f;
float NEAR = -1.0f;
float FAR = 1.0f;

float ActorView::projectionMatrix[4][4] = {
    {2.0f / (RIGHT - LEFT), 0.0f, 0.0f, -1.0f * (RIGHT + LEFT) / (RIGHT - LEFT)},
    {0.0f, 2.0f / (TOP - BOTTOM), 0.0f, -1.0f * (TOP + BOTTOM) / (TOP - BOTTOM)},
    {0.0f, 0.0f, -2.0f / (FAR - NEAR), -1.0f * ((FAR + NEAR) / (FAR - NEAR))},
    {0.0f, 0.0f, 0.0f, 1.0f}
};

ActorView::ActorView(unsigned int indices[], float positions[], unsigned int vertexCount, unsigned int positionsCount, std::string& shaderPath, std::string& texturePath, unsigned int id)
    : va(), ib(indices, vertexCount), vb(positions, positionsCount * sizeof(float)), shader(shaderPath), texture(texturePath), id(id) {

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);

    this->va.AddBuffer(this->vb, layout);

    this->shader.Bind();
    this->shader.SetUniformMat4f("u_P", this->projectionMatrix);

    float identityMatrix[4][4] = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };

    this->shader.SetUniformMat4f("u_T", identityMatrix);
    this->shader.SetUniformMat4f("u_R", identityMatrix);

    this->texture.Bind();
    this->shader.SetUniform1i("u_Texture", 0);

    this->va.Unbind();
    this->vb.Unbind();
    this->ib.Unbind();
    this->shader.Unbind();
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