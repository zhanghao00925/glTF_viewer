#include "render.h"
#include "argparse.hpp"

// Main
void RenderLoop();

string output_folder;
int main(int argc, char **argv)
{
    argparse::ArgumentParser program("glTF_viewer");

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

    WindowConfig::SetWindowsProperty(width, height, true);
    RenderLoop();
    
    return 0;
}

void RenderLoop()
{
    const int maxMotionBlurRadius = 20;
    // Init captureFBO for cube map
    unsigned int captureFBO;
    unsigned int captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1920/maxMotionBlurRadius, 1080/maxMotionBlurRadius );
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    unsigned int tileMaxTexture;
    glGenTextures(1, &tileMaxTexture);

    // pre-allocate enough memory for the LUT texture.
    glBindTexture(GL_TEXTURE_2D, tileMaxTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 1920/maxMotionBlurRadius, 1080/maxMotionBlurRadius, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    unsigned int neighborMaxTexture;
    glGenTextures(1, &neighborMaxTexture);

    // pre-allocate enough memory for the LUT texture.
    glBindTexture(GL_TEXTURE_2D, neighborMaxTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 1920/maxMotionBlurRadius, 1080/maxMotionBlurRadius, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // pre-calculate BRDF intergration

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1920/maxMotionBlurRadius, 1080/maxMotionBlurRadius);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tileMaxTexture, 0);



    glm::mat4 projection = glm::perspective(WindowConfig::mainCamera->Zoom,
                                            (float)WindowConfig::SCR_WIDTH / (float)WindowConfig::SCR_HEIGHT,
                                            NEAR, FAR);

   GLTFModel girlModel("../assets/sci-fi_girl/scene.gltf");
    // GLTFModel girlModel("../assets/buster_drone/scene.gltf");
    Shader animationShader("../shaders/skin/model.vert", "../shaders/skin/model.frag");
    animationShader.Use();
    animationShader.setMat4("projection", projection);

    GBuffer gBuffer;
    Shader gBufferShader("../shaders/skin/gBuffer.vert", "../shaders/skin/gBuffer.frag");
    gBufferShader.Use();
    gBufferShader.setMat4("projection", projection);
    // Screen
    Shader screenShader("../shaders/skin/screen.vert", "../shaders/skin/screen.frag");
    screenShader.Use();
    screenShader.setInt("gPosition", 0);
    screenShader.setInt("gNormal", 1);
    screenShader.setInt("gDepthMap", 2);

    Shader tileMaxShader("../shaders/skin/tileMax.vert", "../shaders/skin/tileMax.frag");
    tileMaxShader.Use();
    tileMaxShader.setInt("gPosition", 0);
    tileMaxShader.setInt("maxMotionBlurRadius", maxMotionBlurRadius);

    Shader neighborMaxShader("../shaders/skin/neighborMax.vert", "../shaders/skin/neighborMax.frag");
    neighborMaxShader.Use();
    neighborMaxShader.setInt("tileMax", 0);

    Shader gatherShader("../shaders/skin/gather.vert", "../shaders/skin/gather.frag");
    gatherShader.Use();
    gatherShader.setInt("gPosition", 0);
    gatherShader.setInt("gNormal", 1);
    gatherShader.setInt("neighborMax", 2);

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
         cout << "After" << hex << showbase << err << endl;
    }
    mat4 view, pre_view;
    static float deltaTime(0.0f);
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
        // Render System
        gBuffer.Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now
        gBufferShader.Use();
        gBufferShader.setMat4("view", view);
        gBufferShader.setMat4("pre_view", pre_view);
        girlModel.Update(gBufferShader, currentFrame);
        gBuffer.Unbind();


        // TileMax
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tileMaxTexture, 0);
        glViewport(0, 0, 1920 / maxMotionBlurRadius, 1080 / maxMotionBlurRadius);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        tileMaxShader.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gPosition);
        RenderQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glActiveTexture(GL_TEXTURE0);

        // neighborMax
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, neighborMaxTexture, 0);
        glViewport(0, 0, 1920 / maxMotionBlurRadius, 1080 / maxMotionBlurRadius);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        neighborMaxShader.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tileMaxTexture);
        RenderQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        glViewport(0, 0, 1920, 1080);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now
        // Render pass
        gatherShader.Use();
        // Bind Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, neighborMaxTexture);
        RenderQuad();
        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
        
        // Swap the screen buffers, Check and call events
        glfwSwapBuffers(WindowConfig::window);
    }
    // Release  
    girlModel.CleanupModel();
    animationShader.Release();
    screenShader.Release();
    gBufferShader.Release();
    gBuffer.Release();
    glDeleteFramebuffers(1, &captureFBO);
    glDeleteRenderbuffers(1, &captureRBO);
    glDeleteTextures(1, &tileMaxTexture);
}
