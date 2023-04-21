#ifndef PA4_ELLIPSOID_H
#define PA4_ELLIPSOID_H

#include "Shape.h"

class Ellipsoid : public Shape
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
    // Constructors
    Ellipsoid()
    {
        this->center = {DEFAULT_CENTER_X, DEFAULT_CENTER_Y, DEFAULT_CENTER_Z};
        this->size = DEFAULT_SIZE;
        this->color = {DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B};
        this->oldColor = this->color;
        this->xAxisLength = 1.0f;
        this->yAxisLength = 1.0f;
        this->zAxisLength = 1.0f;
        this->initShape(this->xAxisLength, this->yAxisLength, this->zAxisLength);
    }
    Ellipsoid(glm::vec3 center, GLfloat size)
    {
        this->center = {DEFAULT_CENTER_X, DEFAULT_CENTER_Y, DEFAULT_CENTER_Z};
        this->size = DEFAULT_SIZE;
        this->color = {DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B};
        this->oldColor = this->color;
        this->xAxisLength = 1.0f;
        this->yAxisLength = 1.0f;
        this->zAxisLength = 1.0f;
        this->initShape(this->xAxisLength, this->yAxisLength, this->zAxisLength);

        this->translate(center);
        this->scale(size - 1);
    }
    Ellipsoid(glm::vec3 center, GLfloat size, glm::vec3 color)
    {
        this->center = {DEFAULT_CENTER_X, DEFAULT_CENTER_Y, DEFAULT_CENTER_Z};
        this->size = DEFAULT_SIZE;
        this->color = {DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B};
        this->oldColor = this->color;
        this->xAxisLength = 1.0f;
        this->yAxisLength = 1.0f;
        this->zAxisLength = 1.0f;
        this->initShape(this->xAxisLength, this->yAxisLength, this->zAxisLength);

        this->translate(center);
        this->scale(size - 1);
        this->setColor(color);
    }
    Ellipsoid(glm::vec3 center, GLfloat size, glm::vec3 color, glm::vec3 axesLength)
    {
        this->center = {DEFAULT_CENTER_X, DEFAULT_CENTER_Y, DEFAULT_CENTER_Z};
        this->size = DEFAULT_SIZE;
        this->color = {DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B};
        this->oldColor = this->color;
        this->xAxisLength = axesLength.x;
        this->yAxisLength = axesLength.y;
        this->zAxisLength = axesLength.z;
        this->initShape(this->xAxisLength, this->yAxisLength, this->zAxisLength);

        this->translate(center);
        this->scale(size - 1);
        this->setColor(color);
        this->setAxesLength(axesLength);
    }

    // getters
    glm::vec3 getCenter() const { return this->center; }
    GLfloat getSize() const { return this->size; }
    glm::vec3 getColor() const { return this->color; }
    glm::vec3 getAxesLength() const { return glm::vec3(this->xAxisLength, this->yAxisLength, this->zAxisLength); }

    virtual GLint getNumVertices() const override { return this->vertices.size() * 3; }
    virtual const glm::vec3 * getVertexData() const override
    {
        glm::vec3 * vertices = new glm::vec3[this->vertices.size()];
        for (int i = 0; i < this->vertices.size(); i++)
        {
            vertices[i] = this->vertices[i];
        }
        return vertices;
    }
    virtual const glm::vec3 * getNormalData() const override
    {
        glm::vec3 * normals = new glm::vec3[this->vertices.size()];
        for (int i = 0; i < this->vertices.size(); i++)
        {
            normals[i] = glm::normalize(this->vertices[i]);
        }
        return normals;
    }

    // setters
    void setCenter(glm::vec3 center) { this->translate(center - this->center); }
    void setSize(GLfloat size) { this->scale(size / this->size); }
    void setColor(glm::vec3 color) { this->oldColor = this->color; this->color = color; }
    void setAxesLength(glm::vec3 axesLength) { this->updateEllipsoidAxesLength(axesLength); }

    // methods
    void translate(glm::vec3 translation)
    {
        this->center += translation;
        this->updateEllipsoidLocation(translation);
    }

    void scale(GLfloat scaleFactor)
    {
        this->size *= scaleFactor;
        this->updateEllipsoidSize(scaleFactor);
    }

    void rotate(GLfloat angle, glm::vec3 axis)
    {
        this->updateEllipsoidOrientation(angle, axis);
    }

    void render(GLuint ellArray, GLuint ellBuffer, uint shaderID) const
    {
        initializeRender(&ellArray, &ellBuffer);

        // set the color
        GLint colorLoc = glGetUniformLocation(shaderID, "ellColor");
        glUniform3f(colorLoc, this->color.x, this->color.y, this->color.z);

        // draw the ellipsoid
        glDrawArrays(GL_TRIANGLES, 0, this->getNumVertices());

        // unbind
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void render(GLuint ellArray, GLuint ellBuffer, uint shaderID, uint options) const
    {
        initializeRender(&ellArray, &ellBuffer);

        // set the color
        GLint colorLoc = glGetUniformLocation(shaderID, "ellColor");
        glUniform3f(colorLoc, color.x, color.y, color.z);

        if (options & Options::WIREFRAME)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_LINE_STRIP, 0, this->getNumVertices());
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, this->getNumVertices());
        }
        // unbind
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

private:
    // the default ellipsoid is centered at the origin and has a side length of 1
    static constexpr GLfloat DEFAULT_CENTER_X = 0.0f;
    static constexpr GLfloat DEFAULT_CENTER_Y = 0.0f;
    static constexpr GLfloat DEFAULT_CENTER_Z = 0.0f;
    static constexpr GLfloat DEFAULT_SIZE = 1.0f;
    // the deffault color is white
    static constexpr GLfloat DEFAULT_COLOR_R = 1.0f;
    static constexpr GLfloat DEFAULT_COLOR_G = 1.0f;
    static constexpr GLfloat DEFAULT_COLOR_B = 1.0f;

    static constexpr GLint INIT_NUM_FACETS = 20; // the initial number of faces of the ellipsoid
    static constexpr GLint INIT_NUM_VERTICES = INIT_NUM_FACETS * 3; // the initial number of vertices of the ellipsoid
    const float PI = 3.14159265358979323846f;

    // copy over the vertices and normals from the icosahedron
    std::vector<std::array<GLfloat, 3>> vertices =
    {
        {X,   0.0,  Z}, {0.0, Z,    X}, {-X,  0.0,  Z},
        {0.0, Z,    X}, {-Z,  X,  0.0}, {-X,  0.0,  Z},
        {0.0, Z,    X}, {0.0, Z,   -X}, {-Z,  X,  0.0},
        {Z,   X,  0.0}, {0.0, Z,   -X}, {0.0, Z,    X},
        {X,   0.0,  Z}, {Z,   X,  0.0}, {0.0, Z,    X},
        {X,   0.0,  Z}, {Z,   -X, 0.0}, {Z,   X,  0.0},
        {Z,   -X, 0.0}, {X,   0.0, -Z}, {Z,   X,  0.0},
        {Z,   X,  0.0}, {X,   0.0, -Z}, {0.0, Z,   -X},
        {X,   0.0, -Z}, {-X,  0.0, -Z}, {0.0, Z,   -X},
        {X,   0.0, -Z}, {0.0, -Z,  -X}, {-X,  0.0, -Z},
        {X,   0.0, -Z}, {Z,   -X, 0.0}, {0.0, -Z,  -X},
        {Z,   -X, 0.0}, {0.0, -Z,   X}, {0.0, -Z,  -X},
        {0.0, -Z,   X}, {-Z,  -X, 0.0}, {0.0, -Z,  -X},
        {0.0, -Z,   X}, {-X,  0.0,  Z}, {-Z,  -X, 0.0},
        {0.0, -Z,   X}, {X,   0.0,  Z}, {-X,  0.0,  Z},
        {Z,   -X, 0.0}, {X,   0.0,  Z}, {0.0, -Z,   X},
        {-Z,  -X, 0.0}, {-X,  0.0,  Z}, {-Z,  X,  0.0},
        {-X,  0.0, -Z}, {-Z,  -X, 0.0}, {-Z,  X,  0.0},
        {0.0, Z,   -X}, {-X,  0.0, -Z}, {-Z,  X,  0.0},
        {-Z,  -X, 0.0}, {-X,  0.0, -Z}, {0.0, -Z,  -X}
    };

    glm::vec3 center;
    GLfloat size;
    glm::vec3 color;
    glm::vec3 oldColor;

    GLfloat xAxisLength;
    GLfloat yAxisLength;
    GLfloat zAxisLength;

    // intialize the vertices by scaling the icosahedron
    void initShape(GLfloat xLength, GLfloat yLength, GLfloat zLength)
    {
        for (auto &vertex : vertices)
        {
            vertex[0] *= xLength;
            vertex[1] *= yLength;
            vertex[2] *= zLength;
        }
    }

    void updateEllipsoidLocation(glm::vec3 translation)
    {
        for (auto &vertex : vertices)
        {
            vertex[0] += translation.x;
            vertex[1] += translation.y;
            vertex[2] += translation.z;
        }
    }

    void updateEllipsoidSize(GLfloat size)
    {
        auto tmp = this->getCenter();
        this->translate(-tmp);
        for (auto &vertex : vertices)
        {
            vertex[0] += vertex[0] * size;
            vertex[1] += vertex[1] * size;
            vertex[2] += vertex[2] * size;
        }
        this->translate(tmp);
    }

    void rotateX(GLfloat angle)
    {
        float angle_rad = glm::radians(angle);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix[1][1] = cos(angle_rad);
        rotationMatrix[1][2] = -sin(angle_rad);
        rotationMatrix[2][1] = sin(angle_rad);
        rotationMatrix[2][2] = cos(angle_rad);

        for (auto &vertex : vertices)
        {
            glm::vec4 vertex4 = glm::vec4(vertex[0], vertex[1], vertex[2], 1.0f);
            glm::vec4 rotatedVertex = rotationMatrix * vertex4;
            vertex[0] = rotatedVertex.x;
            vertex[1] = rotatedVertex.y;
            vertex[2] = rotatedVertex.z;
        }
    }

    void rotateY(GLfloat angle)
    {
        float angle_rad = glm::radians(angle);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix[0][0] = cos(angle_rad);
        rotationMatrix[0][2] = sin(angle_rad);
        rotationMatrix[2][0] = -sin(angle_rad);
        rotationMatrix[2][2] = cos(angle_rad);

        for (auto &vertex : vertices)
        {
            glm::vec4 vertex4 = glm::vec4(vertex[0], vertex[1], vertex[2], 1.0f);
            glm::vec4 rotatedVertex = rotationMatrix * vertex4;
            vertex[0] = rotatedVertex.x;
            vertex[1] = rotatedVertex.y;
            vertex[2] = rotatedVertex.z;
        }
    }

    void rotateZ(GLfloat angle)
    {
        float angle_rad = glm::radians(angle);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix[0][0] = cos(angle_rad);
        rotationMatrix[0][1] = -sin(angle_rad);
        rotationMatrix[1][0] = sin(angle_rad);
        rotationMatrix[1][1] = cos(angle_rad);

        for (auto &vertex : vertices)
        {
            glm::vec4 vertex4 = glm::vec4(vertex[0], vertex[1], vertex[2], 1.0f);
            glm::vec4 rotatedVertex = rotationMatrix * vertex4;
            vertex[0] = rotatedVertex.x;
            vertex[1] = rotatedVertex.y;
            vertex[2] = rotatedVertex.z;
        }
    }

    void updateEllipsoidOrientation(GLfloat angle, glm::vec3 axes)
    {
        auto tmp = this->getCenter();
        this->translate(-tmp);
        if (axes[0] != 0) { rotateX(angle); }
        if (axes[1] != 0) { rotateY(angle); }
        if (axes[2] != 0) { rotateZ(angle); }
        this->translate(tmp);
    }

    void updateEllipsoidAxes(glm::vec3 axes)
    {
        this->xAxisLength = axes[0];
        this->yAxisLength = axes[1];
        this->zAxisLength = axes[2];
        this->scaleAxes(axes);
    }

    void scaleAxes(glm::vec3 axes)
    {
        auto tmp = this->getCenter();
        this->translate(-tmp);
        for (auto &vertex : vertices)
        {
            vertex[0] *= axes[0];
            vertex[1] *= axes[1];
            vertex[2] *= axes[2];
        }
        this->translate(tmp);
    }

    void initializeRender(GLuint * ellArray, GLuint * ellBuffer)
    {
        glGenVertexArrays(1, ellArray);
        glGenBuffers(1, ellBuffer);

        glBindVertexArray(*ellArray);
        glBindBuffer(GL_ARRAY_BUFFER, *ellBuffer);

        const glm::vec3 * verticesData = this->getVertexData();
        glBufferData(GL_ARRAY_BUFFER, INIT_NUM_VERTICES * sizeof(glm::vec3), verticesData, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

    }
}

#endif // PA4_ELLIPSOID_H