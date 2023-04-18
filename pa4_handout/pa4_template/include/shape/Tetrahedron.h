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

        translate(baseCenter);
        scale(size);
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

    Tetrahedron(glm::vec3 baseCenter, GLfloat size, glm::vec3 color) {
        std::cout<<"1"<<std::endl;
        this->baseCenter = glm::vec3(DEFAULT_BASE_CENTER_X, DEFAULT_BASE_CENTER_Y, DEFAULT_BASE_CENTER_Z);
        std::cout<<"2"<<std::endl;
        this->size = DEFAULT_SIZE;
        std::cout<<"3"<<std::endl;

        std::cout << "baseCenter: " << baseCenter.x << ", " << baseCenter.y << ", " << baseCenter.z << std::endl;
        translate(baseCenter);
        std::cout<<"4"<<std::endl;
        scale(size);
        std::cout<<"5"<<std::endl;
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
    }

    virtual GLint getNumVertices() const override {
        return NUM_VERTICES;
    }

    virtual const glm::vec3 * getVertexData() const override {
        return vertex.data();
    }

    virtual const glm::vec3 * getNormalData() const override {
        glm::vec3 *normalData = new glm::vec3[NUM_VERTICES];
        for (int i = 0; i < NUM_VERTICES; i++) {
            normalData[i] = glm::normalize(vertex[i]);
        }
        return normalData;
    }

    // getters
    glm::vec3 getBaseCenter() const { return baseCenter; }
    GLfloat getSize() const { return size; }
    glm::vec3 getColor() const { return color; }
    glm::vec3 getOldColor() const { return oldColor; }

    // setters
    void setBaseCenter(glm::vec3 baseCenter) { this->translate(baseCenter - this->baseCenter); }
    void setSize(GLfloat size) { this->scale(size / this->size); }
    void setColor(glm::vec3 color) { this->oldColor = this->color; this->color = color; }

    // scale the tetrahedron
    void scale(float scaleFactor) {
        this->size = scaleFactor;
        // size changed, so we need to update the vertex data
        updateTetrahedronSize(scaleFactor);
    }

    void translate(glm::vec3 translation) {
        std::cout<<"a"<<std::endl;
        this->baseCenter = translation;
        std::cout<<"b"<<std::endl;
        // baseCenter changed, so we need to update the vertex data
        updateTetrahedronLocation(translation);
        std::cout<<"c"<<std::endl;
    }

    void rotate(float angle, glm::vec3 axis) {
        updateTetrahedronOrientation(angle, axis);
    }

    void render(GLuint tetArray, GLuint tetBuffer, GLuint shaderID) const {
        initializeRender(&tetArray, &tetBuffer);

        // set the color
        GLuint colorLocation = glGetUniformLocation(shaderID, "tetraColor");

        glUniform3f(colorLocation, color[0], color[1], color[2]);

        // draw the tetrahedron
        glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);

        // unbind the buffer and array
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void render(GLuint tetArray, GLuint tetBuffer, GLuint shaderID, uint options) const {
        initializeRender(&tetArray, &tetBuffer);

        // set the color
        GLuint colorLocation = glGetUniformLocation(shaderID, "tetraColor");

        glUniform3f(colorLocation, color[0], color[1], color[2]);

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
        for (int i = 0; i < NUM_VERTICES; i++) {
            vertex[i] *= scaleFactor;
        }
    }

    void updateTetrahedronLocation(glm::vec3 translation) {
        std::cout<<"d"<<std::endl;
        std::cout<<translation[0]<<std::endl;
        std::cout<<translation[1]<<std::endl;
        std::cout<<translation[2]<<std::endl;
        for (int i = 0; i < NUM_VERTICES; i++) {
            vertex[i][0] = translation[0];
            vertex[i][1] = translation[1];
            vertex[i][2] = translation[2];
        }
        std::cout<<"e"<<std::endl;
    }

    void updateTetrahedronOrientation(float angle, glm::vec3 axis) {
        if (axis[0] != 0) { rotateX(angle); }
        if (axis[1] != 0) { rotateY(angle); }
        if (axis[2] != 0) { rotateZ(angle); }
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

    void initializeRender(GLuint * tetArray, GLuint * tetBuffer) const {
        glGenVertexArrays(1, tetArray);
        glBindVertexArray(*tetArray);

        glGenBuffers(1, tetBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, *tetBuffer);
        glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * sizeof(glm::vec3), getVertexData(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    }
};


#endif  // PA4_TETRAHEDRON_H
