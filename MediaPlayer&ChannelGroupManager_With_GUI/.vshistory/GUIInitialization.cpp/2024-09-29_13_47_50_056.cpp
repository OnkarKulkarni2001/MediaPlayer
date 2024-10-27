#include <glad/glad.h> 
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "cMediaPlayer.h"
#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
#include <iostream>

cMediaPlayer mediaPlayer;

void SetupImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
}

void MainLoop(GLFWwindow* window) {
    bool loadSongs = false;    // Track whether songs have been loaded
    bool runPlayer = false;    // Track whether to run the media player
    bool quitPlayer = false;   // Track whether the player should quit

    // Main application loop
    while (!glfwWindowShouldClose(window)) {

        // Media Player Start
        if (!loadSongs && !runPlayer && !quitPlayer) {
            // Call StartMediaPlayer to show initial screen
            mediaPlayer.StartMediaPlayer(window);

            if (ImGui::Button("Load Songs")) {
                loadSongs = true;  // Set flag to load songs
            }

        }

        // Media Player Load Songs
        if (loadSongs && !runPlayer && !quitPlayer) {
            // Load songs via audio manager
            mediaPlayer.audioManager.loadAssets();

            ImGui::Text("Songs loaded successfully!");

            if (ImGui::Button("Run Media Player")) {
                runPlayer = true;  // Switch to running the media player
            }

        }

        // Media Player Running State
        if (runPlayer && !quitPlayer) {
            // Call RunMediaPlayer function for ImGui interface
            mediaPlayer.RunMediaPlayer(window);
        }

        // Quit the Media Player
        if (ImGui::Button("Quit Media Player")) {
            quitPlayer = true;  // Set flag to quit
        }

        if (quitPlayer) {
            mediaPlayer.StopMediaPlayer(window);
            break;  // Exit the loop and clean up
        }

    }
}

void CleanupImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a GLFW window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui + GLFW Example", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Setup ImGui context
    SetupImGui();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330"); // Specify your OpenGL version

    // Main application loop
    MainLoop(window);

    // Cleanup
    CleanupImGui();

    // Cleanup GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}