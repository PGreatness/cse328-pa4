#include <memory>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shape/Cube.h"
#include "shape/Dodecahedron.h"
#include "shape/Icosahedron.h"
#include "shape/Tetrahedron.h"
#include "shape/Octahedron.h"
#include "util/Camera.h"
#include "util/Shader.h"


void cursorPosCallback(GLFWwindow * window, double xpos, double ypos);

void framebufferSizeCallback(GLFWwindow * window, int width, int height);

void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);

void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods);

void scrollCallback(GLFWwindow * window, double xoffset, double yoffset);

void perFrameKeyInput(GLFWwindow * window);

void perFrameTimeLogic();

void initializeContext();

void initializeCubes();

namespace Colors
{
    const glm::vec3 WIREFRAME = glm::vec3(1.0f, 1.0f, 1.0f);
    const glm::vec3 FLAT = glm::vec3(1.0f, 0.5f, 0.31f);
    const glm::vec3 SMOOTH = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 currentColor = WIREFRAME;
} // namespace Colors


namespace Context
{

// screen size
constexpr unsigned int kWindowWidth = 1024U;
constexpr unsigned int kWindowHeight = 1024U;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

// axis visibility
bool axesVisible = true;

// lighting
glm::vec3 lightPos(10.0f, -10.0f, 10.0f);

// mouse
bool mousePressed = false;
bool firstMouse = true;
float lastX = kWindowWidth / 2.0f;
float lastY = kWindowHeight / 2.0f;

glm::vec3 mouseLocalPos;

// timing
float deltaTime = 0.0f;     // time between current frame and last frame
float lastFrame = 0.0f;

bool modificationKeyPressed = false;
bool rotationKeyPressed = false;
bool scaleKeyPressed = false;
bool shearKeyPressed = false;

std::shared_ptr<Shader> axisShader;       // shader for x, y, z axis

// TODO: Add other context configurations
std::shared_ptr<Shader> cubeShader;       // shader for cubes
Cube cube(glm::vec3(0.0f,0.0f,0.0f), 1.0f, Colors::currentColor); // default cube object

std::shared_ptr<Shader> tetrahedronShader;       // shader for tetrahedron
Tetrahedron tetrahedron(glm::vec3(-2.0f,-0.0f,-0.0f), 1.0f, Colors::currentColor); // default tetrahedron object

std::shared_ptr<Shader> octahedronShader;       // shader for octahedron
Octahedron octahedron(glm::vec3(2.0f,0.0f,0.0f), 1.0f, Colors::currentColor); // default octahedron object

std::shared_ptr<Shader> dodecahedronShader;       // shader for dodecahedron
Dodecahedron dodecahedron(glm::vec3(0.0f,0.0f,0.0f), 0.5f, Colors::currentColor); // default dodecahedron object

struct cubeOptions
{
    static const uint DEFAULT = 0;
    static const uint SOLID = 0;
    static const uint WIREFRAME = 1;
    static const uint FLAT = 2;
    static const uint SMOOTH = 4;
}; // cube options

uint options = 0;

void setOptions(uint newOptions)
{
    options = newOptions;
}

Shape * selectedShape = nullptr;

}  // namespace Context


namespace Primitive
{

GLuint axisVertexArray;
GLuint axisVertexBuffer;

// TODO: Add other OpenGL objects for other primitives
GLuint cubeVertexArray;
GLuint cubeVertexBuffer;

GLuint tetrahedronVertexArray;
GLuint tetrahedronVertexBuffer;

GLuint octahedronVertexArray;
GLuint octahedronVertexBuffer;

GLuint dodecahedronVertexArray;
GLuint dodecahedronVertexBuffer;



}  // namespace Primitive


namespace Context
{

// display function for x, y, z axis
void displayAxis()
{
    Context::axisShader->use();
    glm::mat4 projection = glm::perspective(glm::radians(Context::camera.zoom),
                                            static_cast<GLfloat>(Context::kWindowWidth) /
                                            static_cast<GLfloat>(Context::kWindowHeight),
                                            0.01f,
                                            100.0f);
    Context::axisShader->setMat4("projection", projection);
    glm::mat4 view = Context::camera.getViewMatrix();
    Context::axisShader->setMat4("view", view);
    Context::axisShader->setMat4("model", glm::mat4(1.0f));

    glBindVertexArray(Primitive::axisVertexArray);
    glDrawArrays(GL_LINES, 0, 6);  // 3 axis to show (x, y, z), each has 2 vertices
    glBindVertexArray(0);
}

void displayCube()
{
    Context::cubeShader->use();

    // set lighting uniforms
    Context::cubeShader->setVec3("lightPos", Context::lightPos);
    Context::cubeShader->setVec3("viewPos", Context::camera.position);
    Context::cubeShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

    // set options
    Context::cubeShader->setInt("options", options);

    glm::mat4 projection = glm::perspective(glm::radians(Context::camera.zoom),
                                            static_cast<GLfloat>(Context::kWindowWidth) /
                                            static_cast<GLfloat>(Context::kWindowHeight),
                                            0.01f,
                                            100.0f);
    Context::cubeShader->setMat4("projection", projection);
    glm::mat4 view = Context::camera.getViewMatrix();
    Context::cubeShader->setMat4("view", view);
    Context::cubeShader->setMat4("model", glm::mat4(1.0f));

    cube.render(Primitive::cubeVertexArray,
                Primitive::cubeVertexBuffer,
                Context::cubeShader->getShaderProgramHandle(),
                options);
}

void displayTetrahedron()
{
    Context::tetrahedronShader->use();

    // set lighting uniforms
    Context::tetrahedronShader->setVec3("lightPos", Context::lightPos);
    Context::tetrahedronShader->setVec3("viewPos", Context::camera.position);
    Context::tetrahedronShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

    // set options
    Context::tetrahedronShader->setInt("options", options);

    glm::mat4 projection = glm::perspective(glm::radians(Context::camera.zoom),
                                            static_cast<GLfloat>(Context::kWindowWidth) /
                                            static_cast<GLfloat>(Context::kWindowHeight),
                                            0.01f,
                                            100.0f);
    Context::tetrahedronShader->setMat4("projection", projection);
    glm::mat4 view = Context::camera.getViewMatrix();
    Context::tetrahedronShader->setMat4("view", view);
    Context::tetrahedronShader->setMat4("model", glm::mat4(1.0f));

    tetrahedron.render(Primitive::tetrahedronVertexArray,
                        Primitive::tetrahedronVertexBuffer,
                        Context::tetrahedronShader->getShaderProgramHandle(),
                        options);
}

void displayOctahedron()
{
    Context::octahedronShader->use();

    // set lighting uniforms
    Context::octahedronShader->setVec3("lightPos", Context::lightPos);
    Context::octahedronShader->setVec3("viewPos", Context::camera.position);
    Context::octahedronShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

    // set options
    Context::octahedronShader->setInt("options", options);

    glm::mat4 projection = glm::perspective(glm::radians(Context::camera.zoom),
                                            static_cast<GLfloat>(Context::kWindowWidth) /
                                            static_cast<GLfloat>(Context::kWindowHeight),
                                            0.01f,
                                            100.0f);
    Context::octahedronShader->setMat4("projection", projection);
    glm::mat4 view = Context::camera.getViewMatrix();
    Context::octahedronShader->setMat4("view", view);
    Context::octahedronShader->setMat4("model", glm::mat4(1.0f));

    octahedron.render(Primitive::octahedronVertexArray,
                        Primitive::octahedronVertexBuffer,
                        Context::octahedronShader->getShaderProgramHandle(),
                        options);
}

void displayDodecahedron()
{
    Context::dodecahedronShader->use();

    // set lighting uniforms
    Context::dodecahedronShader->setVec3("lightPos", Context::lightPos);
    Context::dodecahedronShader->setVec3("viewPos", Context::camera.position);
    Context::dodecahedronShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

    // set options
    Context::dodecahedronShader->setInt("options", options);

    glm::mat4 projection = glm::perspective(glm::radians(Context::camera.zoom),
                                            static_cast<GLfloat>(Context::kWindowWidth) /
                                            static_cast<GLfloat>(Context::kWindowHeight),
                                            0.01f,
                                            100.0f);
    Context::dodecahedronShader->setMat4("projection", projection);
    glm::mat4 view = Context::camera.getViewMatrix();
    Context::dodecahedronShader->setMat4("view", view);
    Context::dodecahedronShader->setMat4("model", glm::mat4(1.0f));

    dodecahedron.render(Primitive::dodecahedronVertexArray,
                        Primitive::dodecahedronVertexBuffer,
                        Context::dodecahedronShader->getShaderProgramHandle(),
                        options);
}
// TODO: Add display functions for other primitives

}  // namespace Context


int main()
{
    // initialize OpenGL
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Special care will be need to be taken for normal vectors if you want to support viewport scaling;
    // just forbid it for convenience.
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow * window = glfwCreateWindow(Context::kWindowWidth,
                                           Context::kWindowHeight,
                                           "PA4",
                                           nullptr,
                                           nullptr);

    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwTerminate();
        throw std::runtime_error("failed to initialize GLAD");
    }

    // initial the global program context
    initializeContext();

    // TODO: Add more contents to fit your own ideas

    Context::cubeShader = std::make_shared<Shader>("src/shader/Cube/vert.glsl",
                                                    "src/shader/Cube/frag.glsl");

    Context::tetrahedronShader = std::make_shared<Shader>("src/shader/Tetrahedron/vert.glsl",
                                                            "src/shader/Tetrahedron/frag.glsl");

    Context::octahedronShader = std::make_shared<Shader>("src/shader/Octahedron/vert.glsl",
                                                            "src/shader/Octahedron/frag.glsl");

    Context::dodecahedronShader = std::make_shared<Shader>("src/shader/Dodecahedron/vert.glsl",
                                                            "src/shader/Dodecahedron/frag.glsl");

    // render loop
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, Context::kWindowWidth, Context::kWindowHeight);

    while (!glfwWindowShouldClose(window))
    {
        perFrameKeyInput(window);
        perFrameTimeLogic();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO: Display or hide axis based on keyboard functions
        if (Context::axesVisible) { Context::displayAxis(); }

        // TODO: Render

        // Context::displayCube();

        Context::displayTetrahedron();

        Context::displayOctahedron();

        Context::displayDodecahedron();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


void cursorPosCallback(GLFWwindow * window, double xpos, double ypos)
{
    // TODO: Modify this function to fit your own ideas
    
    if (Context::firstMouse)
    {
        Context::lastX = static_cast<float>(xpos);
        Context::lastY = static_cast<float>(ypos);
        Context::firstMouse = false;
    }

    // reversed since y-coordinates go from bottom to top
    auto xoffset = static_cast<float>(xpos - Context::lastX);
    auto yoffset = static_cast<float>(Context::lastY - ypos);

    Context::lastX = static_cast<float>(xpos);
    Context::lastY = static_cast<float>(ypos);

    if (Context::mousePressed)
    {
        Context::camera.processMouseMovement(xoffset, yoffset);
    }

    // make the mouse coordinates from the window coordinates
    // into the coordinates of the axis
    glm::mat4 projection = glm::perspective(glm::radians(Context::camera.zoom),
                                            static_cast<GLfloat>(Context::kWindowWidth) /
                                            static_cast<GLfloat>(Context::kWindowHeight),
                                            0.01f,
                                            100.0f);
    glm::mat4 view = Context::camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = projection * view * model;
    glm::vec4 localPos = glm::inverse(mvp) * glm::vec4(xpos, Context::kWindowHeight - ypos, 0.0f, 1.0f);
    glm::vec4 localOffset = glm::inverse(mvp) * glm::vec4(Context::kWindowWidth, Context::kWindowHeight, 0.0f, 1.0f);
    localOffset = (localOffset / 2.0f) / localOffset.w;
    Context::mouseLocalPos = (glm::vec3(localPos) / localPos.w) - (glm::vec3(localOffset));
}


void framebufferSizeCallback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    // TODO: Modify this function to fit your own ideas
    // set the color of the shapes
    Context::cube.setColor(Colors::currentColor);
    Context::tetrahedron.setColor(Colors::currentColor);
    Context::octahedron.setColor(Colors::currentColor);
    Context::dodecahedron.setColor(Colors::currentColor);

    // check if key pressed is 1
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        Colors::currentColor = Colors::WIREFRAME;
        Context::setOptions(Context::cubeOptions::WIREFRAME);
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        Colors::currentColor = Colors::FLAT;
        Context::setOptions(Context::cubeOptions::FLAT);
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        Colors::currentColor = Colors::SMOOTH;
        Context::setOptions(Context::cubeOptions::SMOOTH);
    }
    // check if holding the T key
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        Context::modificationKeyPressed = true;
    }
    if (key == GLFW_KEY_T && action == GLFW_RELEASE)
    {
        Context::modificationKeyPressed = false;
    }

    // check if holding the R key
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        Context::rotationKeyPressed = true;
    }
    if (key == GLFW_KEY_R && action == GLFW_RELEASE)
    {
        Context::rotationKeyPressed = false;
    }

    // check if holding the Y key
    if (key == GLFW_KEY_Y && action == GLFW_PRESS)
    {
        Context::scaleKeyPressed = true;
    }
    if (key == GLFW_KEY_Y && action == GLFW_RELEASE)
    {
        Context::scaleKeyPressed = false;
    }

    // check if holding the U key
    if (key == GLFW_KEY_U && action == GLFW_PRESS)
    {
        Context::shearKeyPressed = true;
    }
    if (key == GLFW_KEY_U && action == GLFW_RELEASE)
    {
        Context::shearKeyPressed = false;
    }

    // axis visibility
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        Context::axesVisible = !Context::axesVisible;
    }

}


void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        Context::mousePressed = true;
        // testing
        if (Context::cube.isMouseOver(Context::mouseLocalPos))
        {
            Context::cube.setColor(Colors::WIREFRAME);
        }
        else
        {
            Context::cube.setColor(Colors::currentColor);
        }
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        Context::mousePressed = false;
    }
}


void scrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
    bool specialKeyPressed = false;
    if (Context::rotationKeyPressed) {
        specialKeyPressed = true;
        // rotate the shapes clockwise given degree and axes of rotation based on the
        // camera's up vector
        // normalize the up vector
        glm::vec3 up = glm::normalize(Context::camera.up);
        // rotate the shapes
        Context::cube.rotate(static_cast<float>(yoffset), up);
        Context::tetrahedron.rotate(static_cast<float>(yoffset), up);
        Context::octahedron.rotate(static_cast<float>(yoffset), up);
        Context::dodecahedron.rotate(static_cast<float>(yoffset), up);
    }

    if (Context::scaleKeyPressed) {
        specialKeyPressed = true;
        // scale the shapes given the scroll offset
        Context::cube.scale(static_cast<float>(yoffset) * 0.01f);
        Context::tetrahedron.scale(static_cast<float>(yoffset) * 0.01f);
        Context::octahedron.scale(static_cast<float>(yoffset) * 0.01f);
        Context::dodecahedron.scale(static_cast<float>(yoffset) * 0.01f);
    }
    if (specialKeyPressed) { return; }
    Context::camera.processMouseScroll(static_cast<float>(yoffset));
}


void perFrameKeyInput(GLFWwindow * window)
{
    bool specialKeyPressed = false;
    // move the shapes depending on the key pressed so it can be done smoothly
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && Context::modificationKeyPressed)
    {
        specialKeyPressed = true;
        float displacement = Context::camera.movementSpeed * static_cast<float>(Context::deltaTime);
        // normalize the front vector
        glm::vec3 front = glm::normalize(Context::camera.front);
        // find the vector perpendicular to the front vector heading towards the left
        glm::vec3 left = glm::normalize(glm::cross(front, Context::camera.up));
        // move the shapes to the left
        Context::cube.translate(-left * displacement);
        Context::tetrahedron.translate(-left * displacement);
        Context::octahedron.translate(-left * displacement);
        Context::dodecahedron.translate(-left * displacement);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && Context::modificationKeyPressed)
    {
        specialKeyPressed = true;
        float displacement = Context::camera.movementSpeed * static_cast<float>(Context::deltaTime);
        // normalize the front vector
        glm::vec3 front = glm::normalize(Context::camera.front);
        // find the vector perpendicular to the front vector heading towards the left
        glm::vec3 left = glm::normalize(glm::cross(front, Context::camera.up));
        // move the shapes to the left
        Context::cube.translate(left * displacement);
        Context::tetrahedron.translate(left * displacement);
        Context::octahedron.translate(left * displacement);
        Context::dodecahedron.translate(left * displacement);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && Context::modificationKeyPressed)
    {
        specialKeyPressed = true;
        float displacement = Context::camera.movementSpeed * static_cast<float>(Context::deltaTime);
        // normalize the front vector
        glm::vec3 front = glm::normalize(Context::camera.front);
        // move the shapes closer to the camera
        Context::cube.translate(front * displacement);
        Context::tetrahedron.translate(front * displacement);
        Context::octahedron.translate(front * displacement);
        Context::dodecahedron.translate(front * displacement);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && Context::modificationKeyPressed)
    {
        specialKeyPressed = true;
        float displacement = Context::camera.movementSpeed * static_cast<float>(Context::deltaTime);
        // normalize the front vector
        glm::vec3 front = glm::normalize(Context::camera.front);
        // move the shapes away from the camera
        Context::cube.translate(-front * displacement);
        Context::tetrahedron.translate(-front * displacement);
        Context::octahedron.translate(-front * displacement);
        Context::dodecahedron.translate(-front * displacement);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && Context::modificationKeyPressed)
    {
        specialKeyPressed = true;
        float displacement = Context::camera.movementSpeed * static_cast<float>(Context::deltaTime);
        // move the shapes up
        Context::cube.translate(glm::vec3(0.0f, displacement, 0.0f));
        Context::tetrahedron.translate(glm::vec3(0.0f, displacement, 0.0f));
        Context::octahedron.translate(glm::vec3(0.0f, displacement, 0.0f));
        Context::dodecahedron.translate(glm::vec3(0.0f, displacement, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && Context::modificationKeyPressed)
    {
        specialKeyPressed = true;
        float displacement = Context::camera.movementSpeed * static_cast<float>(Context::deltaTime);
        // move the shapes down
        Context::cube.translate(glm::vec3(0.0f, -displacement, 0.0f));
        Context::tetrahedron.translate(glm::vec3(0.0f, -displacement, 0.0f));
        Context::octahedron.translate(glm::vec3(0.0f, -displacement, 0.0f));
        Context::dodecahedron.translate(glm::vec3(0.0f, -displacement, 0.0f));
    }
    if (specialKeyPressed) { return; }
    // camera control
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Context::camera.processKeyboard(Camera::kLeft, Context::deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Context::camera.processKeyboard(Camera::kRight, Context::deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Context::camera.processKeyboard(Camera::kBackWard, Context::deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Context::camera.processKeyboard(Camera::kForward, Context::deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        Context::camera.processKeyboard(Camera::kUp, Context::deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        Context::camera.processKeyboard(Camera::kDown, Context::deltaTime);
    }
}


void perFrameTimeLogic()
{
    auto currentFrame = static_cast<float>(glfwGetTime());
    Context::deltaTime = currentFrame - Context::lastFrame;
    Context::lastFrame = currentFrame;
}


void initializeContext()
{
    // TODO: Modify this function to fit your own ideas
    
    Context::axisShader = std::make_shared<Shader>("src/shader/axis.vert.glsl",
                                                   "src/shader/axis.frag.glsl");

    // initialize axis
    glGenVertexArrays(1, &Primitive::axisVertexArray);
    glBindVertexArray(Primitive::axisVertexArray);
    std::vector<glm::vec3> axisVertexData
            {
                    // x+ axis in red
                    {0, 0, 0}, {1, 0, 0},
                    {10, 0, 0}, {1, 0, 0},
                    // y+ axis in green
                    {0, 0, 0}, {0, 1, 0},
                    {0, 10, 0}, {0, 1, 0},
                    // z+ axis in green
                    {0, 0, 0}, {0, 0, 1},
                    {0, 0, 10}, {0, 0, 1},
            };
    glGenBuffers(1, &Primitive::axisVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, Primitive::axisVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(axisVertexData.size() * sizeof(glm::vec3)),
                 axisVertexData.data(),
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), reinterpret_cast<void *>(sizeof(glm::vec3)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
