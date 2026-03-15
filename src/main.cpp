#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "shaderLoader.h"
#include "TextureLoader.h"
#include "Model.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool gSoundPlaying = false;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        return; // ImGui is handling the mouse; don't move the camera
    }
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        return; // ImGui is handling the mouse; don't move the camera
    }
    // Forward to ImGui
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    ImGui_ImplGlfw_CursorPosCallback(window, xposIn, yposIn);
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        return; // ImGui is handling the mouse; don't move the camera
    }
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow* window)
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        return; // ImGui is handling the mouse; don't move the camera
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS )
    {
        if (gSoundPlaying == false)
        {
            PlaySound(TEXT("../assets/ba_shooting_star.wav"), NULL, SND_FILENAME | SND_ASYNC);
			printf("playing song\n");   
            gSoundPlaying = true;
			Sleep(100); // Add a small delay to prevent rapid toggling
        }
        else if(gSoundPlaying == true) {
            PlaySound(NULL, NULL, 0);
            gSoundPlaying = false;
			printf("stopping song\n");
            Sleep(100);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

enum Songs {
    Constant_Moderato,
    Shooting_Star
};

std::string c_m = "\"../assets/music/ba_constant_moderato.wav\"";
std::string s_s = "\"../assets/music/ba_shooting_star.wav\"";
static int item_current = 0; // The index of the selected item

// 1. Audio Data
const char* songNames[] = { "Constant Moderato", "Shooting Star"};
const char* songPaths[] = { "../assets/music/ba_constant_moderato.wav", "../assets/music/ba_shooting_star.wav"};
static int currentSongIdx = 0;
bool isPaused = false;

void draw_music_choice() {
    ImGui::Begin("Music Player");

    // Dropdown to select the song
    ImGui::Combo("Select Song", &currentSongIdx, songNames, IM_ARRAYSIZE(songNames));

    ImGui::Spacing();

    // Play Button
    if (ImGui::Button("Play")) {
        // We "close" any previous alias to ensure we can load a new one
        mciSendString(TEXT("close bgm"), NULL, 0, NULL);

        // Open the currently selected song path
        std::string cmd = "open \"" + std::string(songPaths[currentSongIdx]) + "\" alias bgm";
        mciSendString(cmd.c_str(), NULL, 0, NULL);
        mciSendString(TEXT("play bgm"), NULL, 0, NULL);

        isPaused = false;
    }

    ImGui::SameLine(); // Put buttons on the same row

    // Pause/Resume Toggle
    if (ImGui::Button(isPaused ? "Resume" : "Pause")) {
        if (!isPaused) {
            mciSendString(TEXT("pause bgm"), NULL, 0, NULL);
        }
        else {
            mciSendString(TEXT("resume bgm"), NULL, 0, NULL);
        }
        isPaused = !isPaused;
    }

    ImGui::SameLine();

    // Stop Button
    if (ImGui::Button("Stop")) {
        mciSendString(TEXT("stop bgm"), NULL, 0, NULL);
        mciSendString(TEXT("close bgm"), NULL, 0, NULL);
        isPaused = false;
    }

    ImGui::End();
}

int main(void)
{
    GLFWwindow* window;
    int windowWidth = SCR_WIDTH;
    int windowHeight = SCR_HEIGHT;


    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };


    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
    glm::vec3 result = lightColor * toyColor; // = (1.0f, 0.5f, 0.31f);


    if (!glfwInit())
    {
        printf("Crash at glfw init\n");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
    window = glfwCreateWindow(windowWidth * main_scale, windowHeight * main_scale, "if you read this, you are gae", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        printf("Crash at window\n");
        return -1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // Calculate centered position
    
	printf("Screen Width: %d, Height: %d\n", mode->width, mode->height);

    int xpos = (mode->width - windowWidth) / 2;
    int ypos = (mode->height - windowHeight) / 2;

    // Set position
    glfwSetWindowPos(window, xpos, ypos);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    glViewport(0, 0, windowWidth, windowHeight);
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

	shaderLoader modelShader("../shaders/model.vertex.glsl", "../shaders/model.fragment.glsl");
	modelShader.createShaders(BOTH_FROM_FILE);

    Model ourModel("../assets/room.glb");

    bool show_demo_window = true;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        draw_music_choice();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ImGuiIO& io = ImGui::GetIO();

        // 2. Only update "Game Logic" if ImGui is NOT being used
        if (!io.WantCaptureMouse && !io.WantCaptureKeyboard)
        {
            processInput(window);
            // Any other game logic (like animations or physics) goes here
        }
        //processInput(window);
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        glClearColor(.2f, .3f, .3f, 1.f);
        //glClearColor(.0f, .0f, .0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        modelShader.useProgram();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        modelShader.setMat4("model", model);
        ourModel.Draw(modelShader);


        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}