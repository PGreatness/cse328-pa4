#ifndef PA4_CUBE_H
#define PA4_CUBE_H

#include "shape/Shape.h"


class Cube : public Shape
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

    // default constructor
    Cube()
    {
        // create a default cube
        this->center = {DEFAULT_CENTER_X, DEFAULT_CENTER_Y, DEFAULT_CENTER_Z};
        this->size = DEFAULT_SIZE;
        this->color = {DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B};
        this->oldColor = this->color;
    }

    Cube(glm::vec3 center, float size)
    {
        // user defined center and size
        this->center = center;
        this->size = size;

        this->translate(center);
        this->scale(size);
        this->color = {DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B};
        this->oldColor = this->color;
    }

    Cube(glm::vec3 center, float size, glm::vec3 color)
    {
        // user defined center, size, and color
        this->center = center;
        this->size = size;

        this->translate(center);
        this->scale(size);
        this->color = color;
        this->oldColor = {DEFAULT_COLOR_R, DEFAULT_COLOR_G, DEFAULT_COLOR_B};
    }

    virtual GLint getNumVertices() const override {
        return NUM_VERTICES;
    }

    virtual const glm::vec3 * getVertexData() const override {
        glm::vec3 *vertexData = new glm::vec3[NUM_VERTICES];
        for (int i = 0; i < NUM_VERTICES; i++) {
            vertexData[i] = glm::vec3(this->vertexData[i][0],
                                        this->vertexData[i][1],
                                        this->vertexData[i][2]);
        }
        return vertexData;
    }

    virtual const glm::vec3 * getNormalData() const override {
        // normalize the vertex data
        glm::vec3 *normalData = new glm::vec3[NUM_VERTICES];
        for (int i = 0; i < NUM_VERTICES; i += 3) {
            glm::vec3 v1 = glm::vec3(this->vertexData[i][0],
                                        this->vertexData[i][1],
                                        this->vertexData[i][2]);
            glm::vec3 v2 = glm::vec3(this->vertexData[i + 1][0],
                                        this->vertexData[i + 1][1],
                                        this->vertexData[i + 1][2]);
            glm::vec3 v3 = glm::vec3(this->vertexData[i + 2][0],
                                        this->vertexData[i + 2][1],
                                        this->vertexData[i + 2][2]);
            glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
            normalData[i] = normal;
        }
        return normalData;
    }

    // getters
    glm::vec3 getCenter() const {
        return center;
    }

    float getSize() const {
        return size;
    }

    glm::vec3 getColor() const {
        return color;
    }

    glm::vec3 getOldColor() const {
        return oldColor;
    }

    // setters
    void setCenter(glm::vec3 center) {
        this->translate(center - this->center);
    }

    void setSize(float size) {
        this->scale(size / this->size);
    }

    void setColor(glm::vec3 color) {
        this->oldColor = this->color;
        this->color = color;
    }

    // scale the cube
    void scale(float scaleFactor) {
        this->size *= scaleFactor;
        // size changed, so we need to update the vertex data
        updateCubeSize(scaleFactor);
    }

    // translate the cube
    void translate(glm::vec3 translation) {
        this->center += translation;
        // center changed, so we need to update the vertex data
        updateCubeLocation(translation);
    }

    // rotate the cube. angle is in degrees, axis is the rotation axis
    // given as a vector. non-zero components of the axis vector
    // indicate the axes of rotation.
    void rotate(float angle, glm::vec3 axis) {
        updateCubeOrientation(angle, axis);
    }


    // render the cube
    void render(GLuint cubeArray, GLuint cubeBuffer, GLuint shaderID) const {
        // initialize the cube renders
        initializeRender(&cubeArray, &cubeBuffer);

        // give the color to the shader
        GLuint colorLocation = glGetUniformLocation(shaderID, "cubeColor");

        // set the color
        glUniform3f(colorLocation, this->color[0], this->color[1], this->color[2]);

        // draw the cube
        glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);

        // unbind the cube array and buffer
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void render(GLuint cubeArray, GLuint cubeBuffer, GLuint shaderID, uint options) {
        // initialize the cube renders
        initializeRender(&cubeArray, &cubeBuffer);

        // give the color to the shader
        GLuint colorLocation = glGetUniformLocation(shaderID, "cubeColor");

        // set the color
        glUniform3f(colorLocation, this->color[0], this->color[1], this->color[2]);

        // draw the cube
        if (options & Options::WIREFRAME) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_LINE_STRIP, 0, NUM_VERTICES);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
        }

        // unbind the cube array and buffer
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // returns true if the mouse is over the cube
    bool isMouseOver(glm::vec3 mousePos) {
        float mouseX = mousePos[0];
        float mouseY = mousePos[1];
        if (mouseX >= this->center[0] - this->size / 2 &&
            mouseX <= this->center[0] + this->size / 2 &&
            mouseY >= this->center[1] - this->size / 2 &&
            mouseY <= this->center[1] + this->size / 2) {
                // std::cout << "Mouse is over cube" << std::endl;
                // std::cout << "mouse: " << mouseX << ", " << mouseY << std::endl;
                return true;
        }
        // std::cout << "Mouse is not over cube" << std::endl;
        // std::cout << "center: " << this->center[0] << ", " << this->center[1] << std::endl;
        // std::cout << "mouse: " << mouseX << ", " << mouseY << std::endl;
        return false;
    }


private:

    // the default cube is centered at the origin and has a side length of 1
    static constexpr GLfloat DEFAULT_CENTER_X = 0.0f;
    static constexpr GLfloat DEFAULT_CENTER_Y = 0.0f;
    static constexpr GLfloat DEFAULT_CENTER_Z = 0.0f;
    static constexpr GLfloat DEFAULT_SIZE = 1.0f;
    // the deffault color is white
    static constexpr GLfloat DEFAULT_COLOR_R = 1.0f;
    static constexpr GLfloat DEFAULT_COLOR_G = 1.0f;
    static constexpr GLfloat DEFAULT_COLOR_B = 1.0f;

    // user-defined center, size, and color
    glm::vec3 center;
    GLfloat size;
    glm::vec3 color;
    glm::vec3 oldColor;

    static constexpr GLint NUM_FACETS = 6 * 2;  // 6 square faces, each composed of 2 triangular facets
    static constexpr GLint NUM_VERTICES = NUM_FACETS * 3;

    static constexpr GLfloat RADIUS = 0.5773502691896258f;

    // Each line represents a 3D vertex,
    // Each 3 lines represents a triangle in 3D space,
    // Each 2x3 lines represents a quad facet (composed of 2 triangles) of this cube.
    GLfloat vertexData[NUM_VERTICES][3] =
            {
                    // front
                    -0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f,  0.5f, -0.5f,
                    0.5f,  0.5f, -0.5f,
                    -0.5f,  0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,

                    // back
                    -0.5f, -0.5f,  0.5f,
                    0.5f, -0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,
                    -0.5f,  0.5f,  0.5f,
                    -0.5f, -0.5f,  0.5f,

                    // left
                    -0.5f,  0.5f,  0.5f,
                    -0.5f,  0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f,  0.5f,
                    -0.5f,  0.5f,  0.5f,

                    // right
                    0.5f,  0.5f,  0.5f,
                    0.5f,  0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,

                    // bottom
                    -0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f,  0.5f,
                    0.5f, -0.5f,  0.5f,
                    -0.5f, -0.5f,  0.5f,
                    -0.5f, -0.5f, -0.5f,

                    // top
                    -0.5f,  0.5f, -0.5f,
                    0.5f,  0.5f, -0.5f,
                    0.5f,  0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,
                    -0.5f,  0.5f,  0.5f,
                    -0.5f,  0.5f, -0.5f,
            };


    // updates the vertex data when the size of the cube changes
    void updateCubeSize(float scale) {
        // update the vertex data
        for (int i = 0; i < NUM_VERTICES; i++) {
            vertexData[i][0] *= scale;
            vertexData[i][1] *= scale;
            vertexData[i][2] *= scale;
        }
    }

    // updates the vertex data when the center of the cube changes
    void updateCubeLocation(glm::vec3 translation) {
        // update the vertex data
        for (int i = 0; i < NUM_VERTICES; i++) {
            vertexData[i][0] += translation[0];
            vertexData[i][1] += translation[1];
            vertexData[i][2] += translation[2];
        }
    }

    void rotateX(float angle) {
        // Convert angle to radians
        float angle_rad = glm::radians(angle);

        // Create rotation matrix for x-axis
        glm::mat4 rotation_mat = glm::mat4(1.0f);
        rotation_mat[1][1] = cos(angle_rad);
        rotation_mat[1][2] = -sin(angle_rad);
        rotation_mat[2][1] = sin(angle_rad);
        rotation_mat[2][2] = cos(angle_rad);

        // Multiply rotation matrix with each vertex of the cube
        for (int i = 0; i < NUM_VERTICES; i++)
        {
            glm::vec4 vertex = glm::vec4(vertexData[i][0], vertexData[i][1], vertexData[i][2], 1.0f);
            vertex = rotation_mat * vertex;
            vertexData[i][0] = vertex.x;
            vertexData[i][1] = vertex.y;
            vertexData[i][2] = vertex.z;
        }
    }

    // rotate the cube around the y-axis given an angle in degrees
    void rotateY(float angle) {
        // convert the angle to radians
        angle = glm::radians(angle);

        // calculate the sine and cosine of the angle
        float c = cos(angle);
        float s = sin(angle);

        // create the rotation matrix
        glm::mat4 rotation = {
            c, 0, s, 0,
            0, 1, 0, 0,
            -s, 0, c, 0,
            0, 0, 0, 1
        };

        // translate to the origin
        glm::mat4 translation1 = glm::translate(glm::mat4(1.0f), -center);

        // translate back to the original position
        glm::mat4 translation2 = glm::translate(glm::mat4(1.0f), center);

        // apply the transformations to each vertex
        for (int i = 0; i < NUM_VERTICES; i++) {
            glm::vec4 vertex = glm::vec4(vertexData[i][0], vertexData[i][1], vertexData[i][2], 1.0f);
            vertex = translation1 * vertex;
            vertex = rotation * vertex;
            vertex = translation2 * vertex;
            vertexData[i][0] = vertex.x;
            vertexData[i][1] = vertex.y;
            vertexData[i][2] = vertex.z;
        }
    }

    void rotateZ(float angle) {
        // Convert angle to radians
        float angle_rad = glm::radians(angle);

        // Create rotation matrix for z-axis
        glm::mat4 rotation_mat = glm::mat4(1.0f);
        rotation_mat[0][0] = cos(angle_rad);
        rotation_mat[0][1] = sin(angle_rad);
        rotation_mat[1][0] = -sin(angle_rad);
        rotation_mat[1][1] = cos(angle_rad);

        // Multiply rotation matrix with each vertex of the cube
        for (int i = 0; i < NUM_VERTICES; i++)
        {
            glm::vec4 vertex = glm::vec4(vertexData[i][0], vertexData[i][1], vertexData[i][2], 1.0f);
            vertex = rotation_mat * vertex;
            vertexData[i][0] = vertex.x;
            vertexData[i][1] = vertex.y;
            vertexData[i][2] = vertex.z;
        }
    }

    void updateCubeOrientation(float angle, glm::vec3 axes) {
        auto tmp = this->getCenter();
        this->translate(-tmp);
        if (axes[0] != 0) {
            rotateX(angle);
        }
        if (axes[1] != 0) {
            rotateY(angle);
        }
        if (axes[2] != 0) {
            rotateZ(angle);
        }
        this->translate(tmp);
    }

    // initialize the render data for the cube
    // use 0 for no lighting, 1 for ambient lighting, 2 for diffuse lighting, 3 for specular lighting
    void initializeRender(GLuint * cubeArray, GLuint * cubeBuffer) const
    {
        // generate the array and buffer objects
        glGenVertexArrays(1, cubeArray);
        glGenBuffers(1, cubeBuffer);

        // bind the array and buffer objects
        glBindVertexArray(*cubeArray);
        glBindBuffer(GL_ARRAY_BUFFER, *cubeBuffer);

        // get the vertex data
        const glm::vec3 *dataStart = getVertexData();
        // const glm::vec3 *tetraStart = getTetraData();

        // copy the vertex data to the buffer object
        glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * sizeof(glm::vec3), dataStart, GL_STATIC_DRAW);

        // enable the vertex attribute array for position
        glEnableVertexAttribArray(0);

        // specify the layout of the vertex data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    }
};


#endif  // PA4_CUBE_H
