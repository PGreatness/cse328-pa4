#ifndef PA4_OCTAHEDRON_H
#define PA4_OCTAHEDRON_H

#include "shape/Shape.h"


class Octahedron : public Shape
{
public:
    Octahedron()
    {
        vertex.reserve(NUM_VERTICES);

        vertex.emplace_back(V3); vertex.emplace_back(V5); vertex.emplace_back(V1);
        vertex.emplace_back(V3); vertex.emplace_back(V2); vertex.emplace_back(V5);
        vertex.emplace_back(V2); vertex.emplace_back(V4); vertex.emplace_back(V5);
        vertex.emplace_back(V5); vertex.emplace_back(V4); vertex.emplace_back(V1);
        vertex.emplace_back(V3); vertex.emplace_back(V1); vertex.emplace_back(V6);
        vertex.emplace_back(V3); vertex.emplace_back(V6); vertex.emplace_back(V2);
        vertex.emplace_back(V6); vertex.emplace_back(V4); vertex.emplace_back(V2);
        vertex.emplace_back(V6); vertex.emplace_back(V1); vertex.emplace_back(V4);

        // TODO
    }

private:
    // 8 triangular facets
    static constexpr GLint NUM_FACETS = 8;
    static constexpr GLint NUM_VERTICES = NUM_FACETS * 3;

    static constexpr glm::vec3 V1 { 1,  0,  0};
    static constexpr glm::vec3 V2 {-1, 0,  0};
    static constexpr glm::vec3 V3 {0,  1, 0};
    static constexpr glm::vec3 V4 { 0, -1, 0};
    static constexpr glm::vec3 V5 {0,  0, 1};
    static constexpr glm::vec3 V6 { 0, 0, -1};

    // vertices organized into triangular facets
    std::vector<glm::vec3> vertex;
};


#endif  // PA4_OCTAHEDRON_H
