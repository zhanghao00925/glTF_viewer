//
// Created by hao on 5/16/17.
//

#pragma once

#include "render_core.h"

class Camera;

class Controller {
public:
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void Movement(double deltaTime);
    static void BindCamera(Camera *camera);

    static bool bRecompile;
    static bool bSave;
private:
    static Camera *mainCamera;
    static bool firstMouse;
    static bool keys[1024];
    static double lastX, lastY;
};

