#include "BaseDataView.h"

#include "gl/VertexBufferLayout.h"

#include <iostream>

BaseDataView::BaseDataView(unsigned int indices[], float positions[], unsigned int vertexCount, unsigned int positionsCount)
    : va(), ib(indices, vertexCount), vb(positions, positionsCount * sizeof(float)) {

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);

    this->va.AddBuffer(this->vb, layout);

    this->va.Unbind();
    this->vb.Unbind();
    this->ib.Unbind();
}

BaseDataView::~BaseDataView() {

}