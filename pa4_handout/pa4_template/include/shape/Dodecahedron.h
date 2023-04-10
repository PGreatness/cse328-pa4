#ifndef PA4_DODECAHEDRON_H
#define PA4_DODECAHEDRON_H

#include "shape/Shape.h"


class Dodecahedron : public Shape
{
public:
    Dodecahedron()
    {
        initializeVertex();

        // TODO
    }

private:
    static constexpr double M_PHI = 1.618033988749894902525738871191;
    static constexpr double M_1_PHI = 1.0 / M_PHI;

    static constexpr GLint INIT_NUM_FACETS = 12 * 3;
    static constexpr GLint INIT_NUM_VERTICES = INIT_NUM_FACETS * 3;

    static constexpr std::array<glm::vec3, 8> A
            {
                    glm::vec3 { 1,  1,  1},
                    glm::vec3 { 1, -1,  1},
                    glm::vec3 {-1, -1,  1},
                    glm::vec3 {-1,  1,  1},
                    glm::vec3 { 1,  1, -1},
                    glm::vec3 { 1, -1, -1},
                    glm::vec3 {-1, -1, -1},
                    glm::vec3 {-1,  1, -1}
            };

    static constexpr std::array<glm::vec3, 4> B
            {
                    glm::vec3 {0,  M_PHI,  M_1_PHI},
                    glm::vec3 {0,  M_PHI, -M_1_PHI},
                    glm::vec3 {0, -M_PHI, -M_1_PHI},
                    glm::vec3 {0, -M_PHI,  M_1_PHI},
            };

    static constexpr std::array<glm::vec3, 4> C
            {
                    glm::vec3 { M_1_PHI, 0,  M_PHI},
                    glm::vec3 { M_1_PHI, 0, -M_PHI},
                    glm::vec3 {-M_1_PHI, 0, -M_PHI},
                    glm::vec3 {-M_1_PHI, 0,  M_PHI},
            };

    static constexpr std::array<glm::vec3, 4> D
            {
                    glm::vec3 { M_PHI,  M_1_PHI, 0},
                    glm::vec3 { M_PHI, -M_1_PHI, 0},
                    glm::vec3 {-M_PHI, -M_1_PHI, 0},
                    glm::vec3 {-M_PHI,  M_1_PHI, 0},
            };

    void initializeVertex()
    {
        // 12 pentagon faces, each composed of 3 triangular facets.
        vertex.reserve(INIT_NUM_VERTICES);

        vertex.emplace_back(A[0]); vertex.emplace_back(C[0]); vertex.emplace_back(A[1]);
        vertex.emplace_back(A[0]); vertex.emplace_back(A[1]); vertex.emplace_back(D[1]);
        vertex.emplace_back(A[0]); vertex.emplace_back(D[1]); vertex.emplace_back(D[0]);

        vertex.emplace_back(D[0]); vertex.emplace_back(D[1]); vertex.emplace_back(A[5]);
        vertex.emplace_back(D[0]); vertex.emplace_back(A[5]); vertex.emplace_back(C[1]);
        vertex.emplace_back(D[0]); vertex.emplace_back(C[1]); vertex.emplace_back(A[4]);

        vertex.emplace_back(B[2]); vertex.emplace_back(A[6]); vertex.emplace_back(C[2]);
        vertex.emplace_back(B[2]); vertex.emplace_back(C[2]); vertex.emplace_back(C[1]);
        vertex.emplace_back(B[2]); vertex.emplace_back(C[1]); vertex.emplace_back(A[5]);

        vertex.emplace_back(A[2]); vertex.emplace_back(D[2]); vertex.emplace_back(A[6]);
        vertex.emplace_back(A[2]); vertex.emplace_back(A[6]); vertex.emplace_back(B[2]);
        vertex.emplace_back(A[2]); vertex.emplace_back(B[2]); vertex.emplace_back(B[3]);

        vertex.emplace_back(C[3]); vertex.emplace_back(A[2]); vertex.emplace_back(B[3]);
        vertex.emplace_back(C[3]); vertex.emplace_back(B[3]); vertex.emplace_back(A[1]);
        vertex.emplace_back(C[3]); vertex.emplace_back(A[1]); vertex.emplace_back(C[0]);

        vertex.emplace_back(A[1]); vertex.emplace_back(B[3]); vertex.emplace_back(B[2]);
        vertex.emplace_back(A[1]); vertex.emplace_back(B[2]); vertex.emplace_back(A[5]);
        vertex.emplace_back(A[1]); vertex.emplace_back(A[5]); vertex.emplace_back(D[1]);

        vertex.emplace_back(C[0]); vertex.emplace_back(A[0]); vertex.emplace_back(B[0]);
        vertex.emplace_back(C[0]); vertex.emplace_back(B[0]); vertex.emplace_back(A[3]);
        vertex.emplace_back(C[0]); vertex.emplace_back(A[3]); vertex.emplace_back(C[3]);

        vertex.emplace_back(A[0]); vertex.emplace_back(D[0]); vertex.emplace_back(A[4]);
        vertex.emplace_back(A[0]); vertex.emplace_back(A[4]); vertex.emplace_back(B[1]);
        vertex.emplace_back(A[0]); vertex.emplace_back(B[1]); vertex.emplace_back(B[0]);

        vertex.emplace_back(B[1]); vertex.emplace_back(A[4]); vertex.emplace_back(C[1]);
        vertex.emplace_back(B[1]); vertex.emplace_back(C[1]); vertex.emplace_back(C[2]);
        vertex.emplace_back(B[1]); vertex.emplace_back(C[2]); vertex.emplace_back(A[7]);

        vertex.emplace_back(D[3]); vertex.emplace_back(A[7]); vertex.emplace_back(C[2]);
        vertex.emplace_back(D[3]); vertex.emplace_back(C[2]); vertex.emplace_back(A[6]);
        vertex.emplace_back(D[3]); vertex.emplace_back(A[6]); vertex.emplace_back(D[2]);

        vertex.emplace_back(C[3]); vertex.emplace_back(A[3]); vertex.emplace_back(D[3]);
        vertex.emplace_back(C[3]); vertex.emplace_back(D[3]); vertex.emplace_back(D[2]);
        vertex.emplace_back(C[3]); vertex.emplace_back(D[2]); vertex.emplace_back(A[2]);

        vertex.emplace_back(A[3]); vertex.emplace_back(B[0]); vertex.emplace_back(B[1]);
        vertex.emplace_back(A[3]); vertex.emplace_back(B[1]); vertex.emplace_back(A[7]);
        vertex.emplace_back(A[3]); vertex.emplace_back(A[7]); vertex.emplace_back(D[3]);
    }

    // 12 pentagon faces, each composed of 3 triangular facets.
    std::vector<glm::vec3> vertex;  // vertices organized into triangular facets
};


#endif  // PA4_DODECAHEDRON_H
