#include "ActorDataView.h"

#include "gl/VertexBufferLayout.h"

#include <iostream>

ActorDataView::ActorDataView(unsigned int indices[], float positions[], unsigned int vertexCount, unsigned int positionsCount, std::string& texturePath)
    : BaseDataView(indices, positions, vertexCount, positionsCount), texture(texturePath) {
    this->texture.Bind();
}

ActorDataView::~ActorDataView() {

}