#include "ActorDataView.h"

#include "gl/VertexBufferLayout.h"

#include <iostream>

ActorDataView::ActorDataView(unsigned int indices[], float positions[], unsigned int vertexCount, unsigned int positionsCount, std::string& texturePath)
    : va(), ib(indices, vertexCount), vb(positions, positionsCount * sizeof(float)), texture(texturePath) {

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);

    this->va.AddBuffer(this->vb, layout);

    this->texture.Bind();

    this->va.Unbind();
    this->vb.Unbind();
    this->ib.Unbind();
}

ActorDataView::~ActorDataView() {

}