//
// Created by hao on 5/16/17.
//

#include "controller.h"
#include "camera.h"

bool Controller::bRecompile = false;
bool Controller::bSave = false;

double Controller::lastX = 0;
double Controller::lastY = 0;
bool Controller::firstMouse = true;
bool Controller::keys[1024] = {false};
Camera *Controller::mainCamera = nullptr;

void Controller::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
    if (key == GLFW_KEY_R && action == GLFW_RELEASE)
    {
        bRecompile = true;
    }
    if (key == GLFW_KEY_P && action == GLFW_RELEASE)
    {
        bSave = true;
    }
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        cout << "Log point" << endl;
    }
    if (key == GLFW_KEY_0 && action == GLFW_RELEASE)
    {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}

void Controller::MouseCallback(GLFWwindow *window, double xpos, double ypos)
{
#ifdef linux
    if (__glibc_unlikely(firstMouse))
    {
#else
    if (firstMouse)
    {
#endif
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    mainCamera->ProcessMouseMovement((GLfloat)(xpos - lastX), (GLfloat)(lastY - ypos));

    lastX = xpos;
    lastY = ypos;
}

void Controller::Movement(double deltaTime)
{
    // Camera controls
    if (keys[GLFW_KEY_W])
        mainCamera->ProcessKeyboard(CAM_FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        mainCamera->ProcessKeyboard(CAM_BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        mainCamera->ProcessKeyboard(CAM_LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        mainCamera->ProcessKeyboard(CAM_RIGHT, deltaTime);
}

void Controller::BindCamera(Camera *camera)
{
    mainCamera = camera;
}
