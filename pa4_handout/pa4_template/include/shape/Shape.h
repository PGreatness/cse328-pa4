#ifndef PA4_SHAPE_H
#define PA4_SHAPE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>


// An abstract class to represent generic shapes,
// just consider it a Java interface if you are new to C++ polymorphism.
// An "= 0;" says that the function is pure virtual and there will be no definition
// (just like APIs in Java interfaces).
class Shape
{
public:
    [[nodiscard]] virtual GLint getNumVertices() const = 0;

    [[nodiscard]] virtual const glm::vec3 * getVertexData() const = 0;

    [[nodiscard]] virtual const glm::vec3 * getNormalData() const = 0;

    glm::vec3 getCenter() const;
    GLfloat getSize() const;
    glm::vec3 getColor() const;
    glm::vec3 getOldColor() const;

    void translate(glm::vec3 translation);
    void scale(GLfloat scaleFactor);
    void rotate(float angle, glm::vec3 axis);

    void render(GLuint VAO, GLuint VBO, GLuint shaderID) const;
    void render(GLuint VAO, GLuint VBO, GLuint shaderID, uint options) const;
};



#endif  // PA4_SHAPE_H
