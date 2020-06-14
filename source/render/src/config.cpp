#include "config.h"

int WindowConfig::SCR_WIDTH = 1280;
int WindowConfig::SCR_HEIGHT = 1080;
bool WindowConfig::Visible = true;

GLFWwindow *WindowConfig::window = nullptr;
Camera *WindowConfig::mainCamera = nullptr;

bool WindowConfig::InitWindow()
{
    // Init GLFW
    if (!glfwInit())
    {
        const char *description;
        int code = glfwGetError(&description);
        if (description)
        {
            cout << "Error code : " << code << ", description : " << description << endl;
        }
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // glfwWindowHint(GLFW_SAMPLES, 4);
    if (!WindowConfig::Visible)
    {
        //            cout << "WindowsConfig::visible : " << WindowConfig::visible << endl;
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    }
    window = glfwCreateWindow(WindowConfig::SCR_WIDTH, WindowConfig::SCR_HEIGHT, "glTF Viewer", nullptr, nullptr); // Windowed
    if (window == nullptr)
    {
        const char *description;
        int code = glfwGetError(&description);
        if (description)
        {
            cout << "Error code : " << code << ", description : " << description << endl;
        }
        return false;
    }
    glfwMakeContextCurrent(window);
    // Set the required callback functions
    glfwSetKeyCallback(window, Controller::KeyCallback);
    glfwSetCursorPosCallback(window, Controller::MouseCallback);
    // Options
    //        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Initialize GLAD to setup the OpenGL Function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    // Define the viewport dimensions
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_MULTISAMPLE);
//     glEnable(GL_CULL_FACE);
    // Setup camera
    mainCamera = new Camera(vec3(0, 0, 10));
    Controller::BindCamera(mainCamera);
    // stbi flip
    stbi_flip_vertically_on_write(true);
    //        stbi_set_flip_vertically_on_load(true);

#ifdef DEBUG
    // Print out some info about the graphics drivers
    std::cout << "--------------------------------------" << endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
#endif

    return true;
}