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

struct ListNode
{
    glm::vec4 color;
    GLfloat depth;
    GLuint next;
};

void RenderLoop()
{
    GLenum err;
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
    glGenRenderbuffers(1, &multisampleDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, multisampleDepth);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, WindowConfig::SCR_WIDTH, WindowConfig::SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, multisampleDepth);
    // - Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // OIT buffers
    const int COUNTER_BUFFER = 0;
    const int LINKED_LIST_BUFFER = 1;
    GLuint NUM_NODES = 16 * WindowConfig::SCR_WIDTH * WindowConfig::SCR_HEIGHT;
    GLint SIZE_NODE = 5 * sizeof(GLfloat) + sizeof(GLuint); // The size of a linked list node
    // Our atomic counter
    GLuint buffers[2];
    glGenBuffers(2, buffers);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, buffers[COUNTER_BUFFER]);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);
    // The buffer of linked lists
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers[LINKED_LIST_BUFFER]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_NODES * SIZE_NODE, NULL, GL_DYNAMIC_DRAW);

    // The buffer for the head pointers, as an image texture
    GLuint headPtrTex;
    glGenTextures(1, &headPtrTex);
    glBindTexture(GL_TEXTURE_2D, headPtrTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, WindowConfig::SCR_WIDTH, WindowConfig::SCR_HEIGHT);

    vector<GLuint> headPtrClear(WindowConfig::SCR_WIDTH * WindowConfig::SCR_HEIGHT, 0xffffffff);
    // GLuint clearBuf;
    // glGenBuffers(1, &clearBuf);
    // glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf);
    // glBufferData(GL_PIXEL_UNPACK_BUFFER, headPtrClear.size() * sizeof(GLuint), &headPtrClear[0], GL_STREAM_DRAW);

    mat4 view, pre_view;
    mat4 projection = glm::perspective(WindowConfig::mainCamera->Zoom,
                                       (float)WindowConfig::SCR_WIDTH / (float)WindowConfig::SCR_HEIGHT,
                                       NEAR, FAR);
    Model glTF_model(glTF_model_path);
    Shader opaqueShader("../shaders/skin/opaque.vert", "../shaders/skin/opaque.frag");
    opaqueShader.Use();
    opaqueShader.setMat4("projection", projection);

    Shader transparentShader("../shaders/skin/transparent.vert", "../shaders/skin/transparent.frag");
    transparentShader.Use();
    transparentShader.setInt("MAX_NODES", NUM_NODES);
    transparentShader.setMat4("projection", projection);

    Shader postProcessingShader("../shaders/skin/postprocessing.vert", "../shaders/skin/postprocessing.frag");
    postProcessingShader.Use();
    postProcessingShader.setInt("MAX_NODES", NUM_NODES);
    postProcessingShader.setInt("colorTextureMS", 0);

    while ((err = glGetError()) != GL_NO_ERROR)
    {
        cout << "Before Start" << hex << showbase << err << endl;
    }
    static float deltaTime(0.0f); //        float attenuation = 1.0 / (distance * distance);
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
        opaqueShader.Use();
        opaqueShader.setMat4("view", view);
        opaqueShader.setMat4("pre_view", pre_view);
        opaqueShader.setVec3("CameraPos", WindowConfig::mainCamera->Position);
        glTF_model.Update(currentFrame);
        glTF_model.Render(opaqueShader);
        // glTF_model.RenderTransparent(opaqueShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // // Clear OIT buffers
        // glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf);
        glBindTexture(GL_TEXTURE_2D, headPtrTex);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WindowConfig::SCR_WIDTH, WindowConfig::SCR_HEIGHT, GL_RED_INTEGER, GL_UNSIGNED_INT, headPtrClear.data());
        GLuint zero = 0;
        glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, buffers[COUNTER_BUFFER]);
        glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &zero);
        // Blit
        glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampledFBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, WindowConfig::SCR_WIDTH, WindowConfig::SCR_HEIGHT, 
                          0, 0, WindowConfig::SCR_WIDTH,  WindowConfig::SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        // Render Transparent
        glClear(GL_COLOR_BUFFER_BIT);
        glDepthMask(GL_FALSE);
        transparentShader.Use();
        transparentShader.setMat4("view", view);
        transparentShader.setMat4("pre_view", pre_view);
        transparentShader.setVec3("CameraPos", WindowConfig::mainCamera->Position);
        glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, buffers[COUNTER_BUFFER]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffers[LINKED_LIST_BUFFER]);
        glBindImageTexture(0, headPtrTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
        glTF_model.RenderTransparent(transparentShader);
        glDepthMask(GL_TRUE);
        // Render screen
        glClear(GL_DEPTH_BUFFER_BIT);
        postProcessingShader.Use();
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffers[LINKED_LIST_BUFFER]);
        glBindImageTexture(0, headPtrTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisampleTex);
        RenderQuad();

        // Swap the screen buffers, Check and call events
        glfwSwapBuffers(WindowConfig::window);
    }
    // Release
    glTF_model.CleanupModel();
    opaqueShader.Release();
    postProcessingShader.Release();
    glDeleteTextures(1, &multisampleTex);
    glDeleteTextures(1, &multisampleDepth);
    glDeleteFramebuffers(1, &multisampledFBO);
    glDeleteBuffers(2, buffers);
    glDeleteTextures(1, &headPtrTex);
}
