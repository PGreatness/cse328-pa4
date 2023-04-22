#ifndef PA4_TETRAHEDRON_H
#define PA4_TETRAHEDRON_H

#include "shape/Shape.h"


class Tetrahedron : public Shape
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
    Tetrahedron()
    {
        this->baseCenter = glm::vec3(DEFAULT_BASE_CENTER_X, DEFAULT_BASE_CENTER_Y, DEFAULT_BASE_CENTER_Z);
        this->size = DEFAULT_SIZE;
        this->color = glm::vec3(DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B);
        this->oldColor = this->color;

        vertex.reserve(NUM_VERTICES);

        vertex.emplace_back(V3);
        vertex.emplace_back(V1);
        vertex.emplace_back(V4);

        vertex.emplace_back(V3);
        vertex.emplace_back(V2);
        vertex.emplace_back(V1);

        vertex.emplace_back(V1);
        vertex.emplace_back(V2);
        vertex.emplace_back(V4);

        vertex.emplace_back(V3);
        vertex.emplace_back(V4);
        vertex.emplace_back(V2);

    }

    Tetrahedron(glm::vec3 baseCenter, GLfloat size) {
        this->baseCenter = glm::vec3(DEFAULT_BASE_CENTER_X, DEFAULT_BASE_CENTER_Y, DEFAULT_BASE_CENTER_Z);
        this->size = DEFAULT_SIZE;

        this->color = glm::vec3(DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B);
        this->oldColor = this->color;

        vertex.reserve(NUM_VERTICES);

        vertex.emplace_back(V3);
        vertex.emplace_back(V1);
        vertex.emplace_back(V4);

        vertex.emplace_back(V3);
        vertex.emplace_back(V2);
        vertex.emplace_back(V1);

        vertex.emplace_back(V1);
        vertex.emplace_back(V2);
        vertex.emplace_back(V4);

        vertex.emplace_back(V3);
        vertex.emplace_back(V4);
        vertex.emplace_back(V2);

        translate(baseCenter);
        scale(size - 1);
    }

    Tetrahedron(glm::vec3 baseCenter, GLfloat size, glm::vec3 color) {
        this->baseCenter = glm::vec3(DEFAULT_BASE_CENTER_X, DEFAULT_BASE_CENTER_Y, DEFAULT_BASE_CENTER_Z);
        this->size = DEFAULT_SIZE;

        this->color = color;
        this->oldColor = this->color;

        vertex.reserve(NUM_VERTICES);

        vertex.emplace_back(V3);
        vertex.emplace_back(V1);
        vertex.emplace_back(V4);

        vertex.emplace_back(V3);
        vertex.emplace_back(V2);
        vertex.emplace_back(V1);

        vertex.emplace_back(V1);
        vertex.emplace_back(V2);
        vertex.emplace_back(V4);

        vertex.emplace_back(V3);
        vertex.emplace_back(V4);
        vertex.emplace_back(V2);

        translate(baseCenter);
        scale(size - 1);
    }

    virtual GLint getNumVertices() const override {
        return NUM_VERTICES;
    }

    virtual const glm::vec3 * getVertexData() const override {
        glm::vec3 *vertexData = new glm::vec3[NUM_VERTICES];
        for (int i = 0; i < NUM_VERTICES; i++) {
            vertexData[i] = glm::vec3(this->vertex[i][0],
                                        this->vertex[i][1],
                                        this->vertex[i][2]);
        }
        return vertexData;
    }

    virtual const glm::vec3 * getNormalData() const override {
        glm::vec3 *normalData = new glm::vec3[NUM_VERTICES];
        for (int i = 0; i < NUM_VERTICES; i += 3)
        {
            glm::vec3 v1 = vertex[i + 1] - vertex[i];
            glm::vec3 v2 = vertex[i + 2] - vertex[i];
            glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

            normalData[i] = normal;
            normalData[i + 1] = normal;
            normalData[i + 2] = normal;
        }
        return normalData;
    }

    // getters
    glm::vec3 getCenter() const { return baseCenter; }
    GLfloat getSize() const { return size; }
    glm::vec3 getColor() const { return color; }
    glm::vec3 getOldColor() const { return oldColor; }

    // setters
    void setBaseCenter(glm::vec3 baseCenter) { this->translate(baseCenter - this->baseCenter); }
    void setSize(GLfloat size) { this->scale(size); }
    void setColor(glm::vec3 color) { this->oldColor = this->color; this->color = color; }

    // scale the tetrahedron
    void scale(float scaleFactor) {
        this->size += scaleFactor;
        // size changed, so we need to update the vertex data
        updateTetrahedronSize(scaleFactor);
    }

    void translate(glm::vec3 translation) {
        this->baseCenter += translation;
        // baseCenter changed, so we need to update the vertex data
        updateTetrahedronLocation(translation);
    }

    void rotate(float angle, glm::vec3 axis) {
        updateTetrahedronOrientation(angle, axis);
    }

    void reflect(glm::vec3 planeA, glm::vec3 planeB) {
        updateTetrahedronReflection(planeA, planeB);
    }

    void render(GLuint tetArray, GLuint tetBuffer, GLuint shaderID) const {
        GLuint tetNormals;
        initializeRender(&tetArray, &tetBuffer, &tetNormals);

        // set the color
        GLuint colorLocation = glGetUniformLocation(shaderID, "tetraColor");

        glUniform3f(colorLocation, color[0], color[1], color[2]);

        // draw the tetrahedron
        glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);

        // unbind the buffer and array
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void render(GLuint tetraArray, GLuint tetraBuffer, GLuint shaderID, uint options) const {
        GLuint tetNormals;
        initializeRender(&tetraArray, &tetraBuffer, &tetNormals);

        // set the color
        GLuint colorLocation = glGetUniformLocation(shaderID, "tetraColor");

        glUniform3f(colorLocation, color[0], color[1], color[2]);

        GLuint flatLocation = glGetUniformLocation(shaderID, "isFlat");
        if (options == Options::FLAT) {
            glUniform1i(flatLocation, 1);
        } else {
            glUniform1i(flatLocation, 0);
        }
        // draw the tetrahedron
        if (options & Options::WIREFRAME) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_LINE_STRIP, 0, NUM_VERTICES);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
        }

        // unbind the buffer and array
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


private:
    // 4 triangular facets
    static constexpr GLint NUM_FACETS = 4;
    static constexpr GLint NUM_VERTICES = NUM_FACETS * 3;

    static constexpr GLfloat RADIUS = 0.5773502691896258f;

    static constexpr glm::vec3 V1 { RADIUS,  RADIUS,  RADIUS};
    static constexpr glm::vec3 V2 {-RADIUS, -RADIUS,  RADIUS};
    static constexpr glm::vec3 V3 {-RADIUS,  RADIUS, -RADIUS};
    static constexpr glm::vec3 V4 { RADIUS, -RADIUS, -RADIUS};

    // vertices organized into triangular facets
    std::vector<glm::vec3> vertex;

    // defaults
    static constexpr GLfloat DEFAULT_BASE_CENTER_X = 0.0f;
    static constexpr GLfloat DEFAULT_BASE_CENTER_Y = 0.0f;
    static constexpr GLfloat DEFAULT_BASE_CENTER_Z = 0.0f;
    static constexpr GLfloat DEFAULT_SIZE = 1.0f;
    // default color
    static constexpr GLfloat DEFAULT_COLOR_R = 1.0f;
    static constexpr GLfloat DEFAULT_COLOR_G = 1.0f;
    static constexpr GLfloat DEFAULT_COLOR_B = 1.0f;
    // user defined center, size, and color
    glm::vec3 baseCenter;
    GLfloat size;
    glm::vec3 color;
    glm::vec3 oldColor;

    void updateTetrahedronSize(float scaleFactor) {
        auto tmp = this->getCenter();
        this->translate(-tmp);
        for (int i = 0; i < NUM_VERTICES; i++) {
            vertex[i][0] += vertex[i][0] * scaleFactor;
            vertex[i][1] += vertex[i][1] * scaleFactor;
            vertex[i][2] += vertex[i][2] * scaleFactor;
        }
        this->translate(tmp);
    }

    void updateTetrahedronLocation(glm::vec3 translation) {
        for (int i = 0; i < NUM_VERTICES; i++) {
            vertex[i][0] += translation[0];
            vertex[i][1] += translation[1];
            vertex[i][2] += translation[2];
        }
    }

    void updateTetrahedronOrientation(float angle, glm::vec3 axis) {
        auto tmp = this->getCenter();
        this->translate(-tmp);
        if (axis[0] != 0) { rotateX(angle); }
        if (axis[1] != 0) { rotateY(angle); }
        if (axis[2] != 0) { rotateZ(angle); }
        this->translate(tmp);
    }

    void rotateX(float angle) {
        float angle_rad = glm::radians(angle);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix[1][1] = cos(angle_rad);
        rotationMatrix[1][2] = -sin(angle_rad);
        rotationMatrix[2][1] = sin(angle_rad);
        rotationMatrix[2][2] = cos(angle_rad);

        for (int i = 0; i < NUM_VERTICES; i++) {
            glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(vertex[i], 1.0f);
            vertex[i] = glm::vec3(rotatedVertex);
        }
    }

    void rotateY(float angle) {
        float angle_rad = glm::radians(angle);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix[0][0] = cos(angle_rad);
        rotationMatrix[0][2] = sin(angle_rad);
        rotationMatrix[2][0] = -sin(angle_rad);
        rotationMatrix[2][2] = cos(angle_rad);

        for (int i = 0; i < NUM_VERTICES; i++) {
            glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(vertex[i], 1.0f);
            vertex[i] = glm::vec3(rotatedVertex);
        }
    }

    void rotateZ(float angle) {
        float angle_rad = glm::radians(angle);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix[0][0] = cos(angle_rad);
        rotationMatrix[0][1] = -sin(angle_rad);
        rotationMatrix[1][0] = sin(angle_rad);
        rotationMatrix[1][1] = cos(angle_rad);

        for (int i = 0; i < NUM_VERTICES; i++) {
            glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(vertex[i], 1.0f);
            vertex[i] = glm::vec3(rotatedVertex);
        }
    }

    void updateTetrahedronReflection(glm::vec3 planeA, glm::vec3 planeB)
    {
        auto tmp = this->getCenter();
        this->translate(-tmp);

        glm::vec3 planeNormal = glm::normalize(glm::cross(planeA, planeB));

        // Create reflection matrix
        glm::mat4 P = glm::outerProduct(glm::vec4(planeNormal, 0.0f), glm::vec4(planeNormal, 0.0f));
        glm::mat4 R = glm::mat4(1.0f) - 2.0f * P;

        // Apply reflection matrix to each vertex of the cube
        for (int i = 0; i < this->getNumVertices(); i++)
        {
            glm::vec4 vertices = glm::vec4(this->vertex[i][0], this->vertex[i][1], this->vertex[i][2], 1.0f);
            glm::vec4 reflectedVertices = R * vertices;
            this->vertex[i][0] = reflectedVertices[0];
            this->vertex[i][1] = reflectedVertices[1];
            this->vertex[i][2] = reflectedVertices[2];
        }
        this->translate(tmp);
    }

    void initializeRender(GLuint * tetArray, GLuint * tetBuffer, GLuint * tetNormals) const {
        glGenVertexArrays(1, tetArray);
        glBindVertexArray(*tetArray);

        glGenBuffers(1, tetBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, *tetBuffer);

        const glm::vec3 * vertexData = getVertexData();
        glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * sizeof(glm::vec3), vertexData, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

        glGenBuffers(1, tetNormals);
        glBindBuffer(GL_ARRAY_BUFFER, *tetNormals);
        const glm::vec3 * normalData = getNormalData();
        glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * sizeof(glm::vec3), normalData, GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    }
};


#endif  // PA4_TETRAHEDRON_H
