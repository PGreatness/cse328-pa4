#ifndef PA4_DODECAHEDRON_H
#define PA4_DODECAHEDRON_H

#include "shape/Shape.h"

class Dodecahedron : public Shape
{
private:
        struct Options
        {
                static const uint DEFAULT = 0;
                static const uint SOLID = 0;
                static const uint WIREFRAME = 1;
                static const uint FLAT = 2;
                static const uint SMOOTH = 4;
        };

        static const Options options;

public:
        Dodecahedron()
        {
                initializeVertex();
                this->center = glm::vec3(DEFAULT_CENTER_X, DEFAULT_CENTER_Y, DEFAULT_CENTER_Z);
                this->size = DEFAULT_SIZE;
                this->color = glm::vec3(DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B);
                this->oldColor = this->color;
        }
        Dodecahedron(glm::vec3 center, GLfloat size) {
                initializeVertex();
                this->center = glm::vec3(DEFAULT_CENTER_X, DEFAULT_CENTER_Y, DEFAULT_CENTER_Z);
                this->size = DEFAULT_SIZE;
                this->color = glm::vec3(DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B);
                this->oldColor = this->color;
        }
        Dodecahedron(glm::vec3 center, GLfloat size, glm::vec3 color) {
                initializeVertex();
                this->center = glm::vec3(DEFAULT_CENTER_X, DEFAULT_CENTER_Y, DEFAULT_CENTER_Z);
                this->size = DEFAULT_SIZE;
                this->color = glm::vec3(DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B);
                this->oldColor = this->color;
        }

        // getters
        glm::vec3 getCenter() const { return center; }
        GLfloat getSize() const { return size; }
        glm::vec3 getColor() const { return color; }
        glm::vec3 getOldColor() const { return oldColor; }

        virtual GLint getNumVertices() const override {
                return this->INIT_NUM_VERTICES;
        }

        virtual const glm::vec3 * getVertexData() const override {
                glm::vec3 *vertexData = new glm::vec3[getNumVertices()];
                for (int i = 0; i < getNumVertices(); i++) {
                        glm::vec3 vertex = glm::vec3(this->vertex[i][0],
                                                    this->vertex[i][1],
                                                    this->vertex[i][2]);
                        vertexData[i] = vertex;
                }
                return vertexData;
        }

        virtual const glm::vec3 * getNormalData() const override {
                glm::vec3 *normalData = new glm::vec3[getNumVertices()];
                for (int i = 0; i < getNumVertices(); i+=3) {
                        glm::vec3 v1 = glm::vec3(this->vertex[i][0],
                                                    this->vertex[i][1],
                                                    this->vertex[i][2]);
                        glm::vec3 v2 = glm::vec3(this->vertex[i+1][0],
                                                    this->vertex[i+1][1],
                                                    this->vertex[i+1][2]);
                        glm::vec3 v3 = glm::vec3(this->vertex[i+2][0],
                                                    this->vertex[i+2][1],
                                                    this->vertex[i+2][2]);
                        glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
                        normalData[i] = normal;
                }
                return normalData;
        }

        // setters
        void setCenter(glm::vec3 center) { translate(center - this->center); }
        void setSize(GLfloat size) { scale(size / this->size); }
        void setColor(glm::vec3 color) { this->oldColor = this->color; this->color = color; }

        // methods
        void translate(glm::vec3 translation) {
                updateDodecahedronLocation(translation);
                this->center += translation;
        }

        void rotate(float angle, glm::vec3 axis) {
                updateDodecahedronOrientation(angle, axis);
        }

        void scale(GLfloat scale) {
                updateDodecahedronSize(scale);
                this->size *= scale;
        }

        void render(GLuint dodecaArray, GLuint dodecaBuffer, GLuint shaderID) const {
                initializeRender(&dodecaArray, &dodecaBuffer);

                GLuint colorLocation = glGetUniformLocation(shaderID, "dodecaColor");
                glUniform3f(colorLocation, this->color[0], this->color[1], this->color[2]);

                glDrawArrays(GL_TRIANGLES, 0, this->getNumVertices());

                glBindVertexArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void render(GLuint dodecaArray, GLuint dodecaBuffer, GLuint shaderID, uint options) const {
                initializeRender(&dodecaArray, &dodecaBuffer);

                GLuint colorLocation = glGetUniformLocation(shaderID, "dodecaColor");
                glUniform3f(colorLocation, this->color[0], this->color[1], this->color[2]);

                if (options & Options::WIREFRAME) {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        glDrawArrays(GL_LINE_STRIP, 0, this->getNumVertices());
                } else {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        glDrawArrays(GL_TRIANGLES, 0, this->getNumVertices());
                }

                glBindVertexArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
private:
        static constexpr GLfloat DEFAULT_CENTER_X = 0.0f;
        static constexpr GLfloat DEFAULT_CENTER_Y = 0.0f;
        static constexpr GLfloat DEFAULT_CENTER_Z = 0.0f;
        static constexpr GLfloat DEFAULT_SIZE = 1.0f;
        // the deffault color is white
        static constexpr GLfloat DEFAULT_COLOR_R = 1.0f;
        static constexpr GLfloat DEFAULT_COLOR_G = 1.0f;
        static constexpr GLfloat DEFAULT_COLOR_B = 1.0f;

        static constexpr double M_PHI = 1.618033988749894902525738871191;
        static constexpr double M_1_PHI = 1.0 / M_PHI;

        static constexpr GLint INIT_NUM_FACETS = 12 * 3;
        static constexpr GLint INIT_NUM_VERTICES = INIT_NUM_FACETS * 3;

        static constexpr std::array<glm::vec3, 8> A{
            glm::vec3{1, 1, 1},
            glm::vec3{1, -1, 1},
            glm::vec3{-1, -1, 1},
            glm::vec3{-1, 1, 1},
            glm::vec3{1, 1, -1},
            glm::vec3{1, -1, -1},
            glm::vec3{-1, -1, -1},
            glm::vec3{-1, 1, -1}};

        static constexpr std::array<glm::vec3, 4> B{
            glm::vec3{0, M_PHI, M_1_PHI},
            glm::vec3{0, M_PHI, -M_1_PHI},
            glm::vec3{0, -M_PHI, -M_1_PHI},
            glm::vec3{0, -M_PHI, M_1_PHI},
        };

        static constexpr std::array<glm::vec3, 4> C{
            glm::vec3{M_1_PHI, 0, M_PHI},
            glm::vec3{M_1_PHI, 0, -M_PHI},
            glm::vec3{-M_1_PHI, 0, -M_PHI},
            glm::vec3{-M_1_PHI, 0, M_PHI},
        };

        static constexpr std::array<glm::vec3, 4> D{
            glm::vec3{M_PHI, M_1_PHI, 0},
            glm::vec3{M_PHI, -M_1_PHI, 0},
            glm::vec3{-M_PHI, -M_1_PHI, 0},
            glm::vec3{-M_PHI, M_1_PHI, 0},
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
        std::vector<glm::vec3> vertex; // vertices organized into triangular facets

        glm::vec3 center;
        GLfloat size;
        glm::vec3 color;
        glm::vec3 oldColor;

        // updates the vertex data when the size of the dodecahedron is changed
        void updateDodecahedronSize(float scale)
        {
                for (int i = 0; i < INIT_NUM_VERTICES; i++)
                {
                    vertex[i][0] *= scale;
                    vertex[i][1] *= scale;
                    vertex[i][2] *= scale;
                }
        }

        // updates the vertex data when the location of the dodecahedron is changed
        void updateDodecahedronLocation(glm::vec3 translation)
        {
                for (int i = 0; i < INIT_NUM_VERTICES; i++)
                {
                    vertex[i][0] += translation[0];
                    vertex[i][1] += translation[1];
                    vertex[i][2] += translation[2];
                }
        }

        void rotateX(float angle) {
            float angle_rad = glm::radians(angle);

            glm::mat4 rotationMatrix = glm::mat4(1.0f);
            rotationMatrix[1][1] = cos(angle_rad);
            rotationMatrix[1][2] = -sin(angle_rad);
            rotationMatrix[2][1] = sin(angle_rad);
            rotationMatrix[2][2] = cos(angle_rad);

            for (int i = 0; i < INIT_NUM_VERTICES; i++)
            {
                glm::vec4 vertex = glm::vec4(this->vertex[i][0],
                                            this->vertex[i][1],
                                            this->vertex[i][2],
                                            1.0f);
                vertex = rotationMatrix * vertex;
                this->vertex[i][0] = vertex[0];
                this->vertex[i][1] = vertex[1];
                this->vertex[i][2] = vertex[2];
            }
        }

        void rotateY(float angle) {
            float angle_rad = glm::radians(angle);

            glm::mat4 rotationMatrix = glm::mat4(1.0f);
            rotationMatrix[0][0] = cos(angle_rad);
            rotationMatrix[0][2] = sin(angle_rad);
            rotationMatrix[2][0] = -sin(angle_rad);
            rotationMatrix[2][2] = cos(angle_rad);

            for (int i = 0; i < INIT_NUM_VERTICES; i++)
            {
                glm::vec4 vertex = glm::vec4(this->vertex[i][0],
                                            this->vertex[i][1],
                                            this->vertex[i][2],
                                            1.0f);
                vertex = rotationMatrix * vertex;
                this->vertex[i][0] = vertex[0];
                this->vertex[i][1] = vertex[1];
                this->vertex[i][2] = vertex[2];
            }
        }

        void rotateZ(float angle) {
            float angle_rad = glm::radians(angle);

            glm::mat4 rotationMatrix = glm::mat4(1.0f);
            rotationMatrix[0][0] = cos(angle_rad);
            rotationMatrix[0][1] = -sin(angle_rad);
            rotationMatrix[1][0] = sin(angle_rad);
            rotationMatrix[1][1] = cos(angle_rad);

            for (int i = 0; i < INIT_NUM_VERTICES; i++)
            {
                glm::vec4 vertex = glm::vec4(this->vertex[i][0],
                                            this->vertex[i][1],
                                            this->vertex[i][2],
                                            1.0f);
                vertex = rotationMatrix * vertex;
                this->vertex[i][0] = vertex[0];
                this->vertex[i][1] = vertex[1];
                this->vertex[i][2] = vertex[2];
            }
        }

        void updateDodecahedronOrientation(float angle, glm::vec3 axis) {
            if (axis[0] != 0) {
                rotateX(angle);
            }
            if (axis[1] != 0) {
                rotateY(angle);
            }
            if (axis[2] != 0) {
                rotateZ(angle);
            }
        }

        void initializeRender(GLuint * dodecaArray, GLuint * dodecaBuffer) const {
            glGenVertexArrays(1, dodecaArray);
            glBindVertexArray(*dodecaArray);

            glGenBuffers(1, dodecaBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, *dodecaBuffer);

            const glm::vec3 *dataStart = getVertexData();
            glBufferData(GL_ARRAY_BUFFER, getNumVertices() * sizeof(glm::vec3), dataStart, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        }
};

#endif // PA4_DODECAHEDRON_H
