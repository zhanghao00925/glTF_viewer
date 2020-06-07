#pragma once

#include "camera.h"
#include "controller.h"
#include "stb_image_write.h"
#include "stb_image.h"

class WindowConfig
{
public:
    static bool InitWindow();

    static int SCR_WIDTH;
    static int SCR_HEIGHT;
    static bool Visible;

    static GLFWwindow *window;
    static Camera *mainCamera;

    static void SetWindowsProperty(int width, int height, bool bVisible)
    {
        SCR_WIDTH = width;
        SCR_HEIGHT = height;
        Visible = bVisible;

        InitWindow();
    }

    class Garbo
    {
    public:
        ~Garbo()
        {
            delete mainCamera;
            glfwDestroyWindow(window);
            glfwTerminate();
        }
    };
    static Garbo garbo;
};