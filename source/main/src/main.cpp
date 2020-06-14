#include "render.h"
#include "argparse.hpp"

void RenderLoop();

string glTF_model_path;

int main(int argc, char **argv)
{
    argparse::ArgumentParser program("glTF_viewer");

    program.add_argument("glTF_model_path")
            .help("glTF model path.");

    program.add_argument("-x", "--width")
        .default_value(std::string("1920"))
        .help("specify the width.");

    program.add_argument("-y", "--height")
        .default_value(std::string("1080"))
        .help("specify the height.");

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error &err)
    {
        std::cout << err.what() << std::endl;
        std::cout << program;
        exit(0);
    }
    int width = stoi(program.get<std::string>("-x"));
    int height = stoi(program.get<std::string>("-y"));
    glTF_model_path = program.get<std::string>("glTF_model_path");

    WindowConfig::SetWindowsProperty(width, height, true);
    RenderLoop();
    
    return 0;
}

void RenderLoop()
{
    GLuint multisampledFBO;
    glGenFramebuffers(1, &multisampledFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, multisampledFBO);
    GLuint multisampleTex;
    glGenTextures(1, &multisampleTex);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisampleTex);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA32F, WindowConfig::SCR_WIDTH, WindowConfig::SCR_HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multisampleTex, 0);
    GLuint multisampleDepth;
    glGenTextures(1, &multisampleDepth);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisampleDepth);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH_COMPONENT32F, WindowConfig::SCR_WIDTH, WindowConfig::SCR_HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, multisampleDepth, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    mat4 view, pre_view;
    mat4 projection = glm::perspective(WindowConfig::mainCamera->Zoom,
                                            (float)WindowConfig::SCR_WIDTH / (float)WindowConfig::SCR_HEIGHT,
                                            NEAR, FAR);
    Model glTF_model(glTF_model_path);
    Shader animationShader("../shaders/skin/model.vert", "../shaders/skin/model.frag");
    animationShader.Use();
    animationShader.setMat4("projection", projection);

    Shader postProcessingShader("../shaders/skin/postprocessing.vert", "../shaders/skin/postprocessing.frag");
    postProcessingShader.Use();
    postProcessingShader.setInt("colorTextureMS", 0);

    GLenum err; while((err = glGetError()) != GL_NO_ERROR) {cout << "After" << hex << showbase << err << endl;}
    static float deltaTime(0.0f);//        float attenuation = 1.0 / (distance * distance);
    static float lastFrame(0.0f);
    while (!glfwWindowShouldClose(WindowConfig::window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Movement System
        glfwPollEvents();
        Controller::Movement(deltaTime);
        pre_view = view;
        view = WindowConfig::mainCamera->GetViewMatrix();
        // Render model
        glBindFramebuffer(GL_FRAMEBUFFER, multisampledFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        animationShader.Use();
        animationShader.setMat4("view", view);
        animationShader.setMat4("pre_view", pre_view);
        animationShader.setVec3("CameraPos", WindowConfig::mainCamera->Position);
        glTF_model.Update(currentFrame);
        glTF_model.Render(animationShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // Render screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        postProcessingShader.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisampleTex);
        RenderQuad();

        // Swap the screen buffers, Check and call events
        glfwSwapBuffers(WindowConfig::window);
    }
    // Release  
    glTF_model.CleanupModel();
    animationShader.Release();
    postProcessingShader.Release();
    glDeleteTextures(1, &multisampleTex);
    glDeleteTextures(1, &multisampleDepth);
    glDeleteFramebuffers(1, &multisampledFBO);
}
