#pragma once

namespace TriangleShape {
    std::vector<float> positions = {
        -10.0f, -10.0f, 0.0f, 0.0f,
        10.0f, -10.0f, 1.0f, 0.0f,
        0.0f, 20.0f, 0.5f, 1.0f
    };

    std::vector<unsigned int> indices = {
        0, 1, 2
    };
}

namespace QuadShape {
    std::vector<float> positions = {
        -15.0f, -15.0f, 0.0f, 0.0f,
        15.0f, -15.0f, 1.0f, 0.0f,
        15.0f, 15.0f, 1.0f, 1.0f,
        -15.0f, 15.0f, 0.0f, 1.0f
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0
    };
}

namespace Quad2Shape {
    std::vector<float> positions = {
        -2.0f, -2.0f, 0.0f, 0.0f,
        2.0f, -2.0f, 1.0f, 0.0f,
        2.0f, 2.0f, 1.0f, 1.0f,
        -2.0f, 2.0f, 0.0f, 1.0f
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0
    };
}