﻿#include "cMediaPlayer.h"
#include "cAudioManager.h"
#include <conio.h>
#include <iostream>


using namespace std;

cAudioManager audioManager;

void cMediaPlayer::StartMediaPlayer(GLFWwindow* window) {
    // Create a window for the media player
    ImGui::Begin("Media Player");

    ImGui::Text("Media Player Started");
    ImGui::Text("Welcome to the Media Player developed by Onkar Kulkarni!");
    ImGui::Text("You need to load songs first to listen them. I'll do that for you but you just need to specify the folder name.");
    audioManager.loadAssets();
    ImGui::Text("Enjoy listening!");
    ImGui::End();

    // Render ImGui
    ImGui::Render();
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void cMediaPlayer::RunMediaPlayer(GLFWwindow* window) {
    
    /*cout << "Commands:\n0) P: Play Song\n1) V: Set Volume\n2) I: Set Pitch\n3) A: Set Pan\n4) Spacebar: Pause Song\n5) R: Resume Song\n6) S: Stop Song\n7) Esc: Quit Media Player\n";
    bool bIsRunning = true;
    do {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'P' || key == 'p' ) {
                int indexOfSong = 1;
                ListSongs();
                cout << "Enter number of song you wanna play: " << endl;
                cin >> indexOfSong;
                if (indexOfSong >= 0 && indexOfSong < audioManager.sounds.size()) {
                    audioManager.channel->isPlaying(&audioManager.bIsPlaying);
                    PlayAudio(indexOfSong);
                }
                else {
                    cout << "Enter valid choice!" << endl;
                    cin >> indexOfSong;
                    audioManager.channel->isPlaying(&audioManager.bIsPlaying);
                    PlayAudio(indexOfSong);
                }
            }
            if (key == 'V' || key == 'v') {
                AdjustVolume();
            }
            if (key == 'I' || key == 'i') {
                AdjustPitch();
            }
            if (key == 'A' || key == 'a') {
                AdjustPan();
            }
            if (key == 32) {
                PauseAudio();
            }
            if (key == 'R' || key == 'r') {
                audioManager.resume();
            }
            if (key == 'S' || key == 's') {
                StopAudio();
            }
            if (key == 27) {
                StopMediaPlayer();
                bIsRunning = false;
            }
        } 
    } while (bIsRunning);*/


    bool bIsRunning = true;

    // Main loop for ImGui rendering
    while (bIsRunning) {
        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create a window for the media player
        ImGui::Begin("Media Player");
        // Variable to store the index of the selected song
        static int selectedSongIndex = -1;

        if (ImGui::Button("Load Songs")) {
            audioManager.loadAssets();
        }

        // List Songs using ImGui ListBox
        if (!audioManager.sounds.empty()) {
            ImGui::Text("Songs available:");
            if (ImGui::BeginListBox("##SongListBox", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing()))) {
                for (int i = 0; i < audioManager.sounds.size(); i++) {
                    char name[256];
                    if (audioManager.sounds[i]) {
                        FMOD_RESULT result = audioManager.sounds[i]->getName(name, sizeof(name));
                        if (result == FMOD_OK) {
                            // Make each song selectable
                            bool isSelected = (selectedSongIndex == i+1); // Check if this is the selected song
                            if (ImGui::Selectable(name, isSelected)) {
                                selectedSongIndex = i; // Mark the clicked song as selected
                                PlayAudio(selectedSongIndex); // Play the selected song
                            }
                            if (isSelected) {
                                ImGui::SetItemDefaultFocus(); // Ensure the selected item remains in focus
                            }
                        }
                    }
                }
                ImGui::EndListBox();
            }
        }
        else {
            ImGui::Text("No songs loaded. Please load songs first.");
        }
        // Volume Control
        static float volume = 1.0f; // Default volume
        ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);
        if (ImGui::Button("Set Volume")) {
            audioManager.setVolume(volume);
        }

        // Pitch Control
        static float pitch = 1.0f; // Default pitch
        ImGui::SliderFloat("Pitch", &pitch, 0.5f, 2.0f);
        if (ImGui::Button("Set Pitch")) {
            audioManager.setPitch(pitch);
        }

        // Pan Control
        static float pan = 0.0f; // Default pan
        ImGui::SliderFloat("Pan", &pan, -1.0f, 1.0f);
        if (ImGui::Button("Set Pan")) {
            audioManager.setPan(pan);
        }

        // Pause, Resume, and Stop Controls
        if (ImGui::Button("Pause")) {
            PauseAudio();
        }
        if (ImGui::Button("Resume")) {
            audioManager.resume();
        }
        if (ImGui::Button("Stop")) {
            StopAudio();
        }

        // Quit
        if (ImGui::Button("Quit")) {
            StopMediaPlayer(window);
            bIsRunning = false;
        }

        ImGui::End();

        // Render ImGui
        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void cMediaPlayer::StopMediaPlayer(GLFWwindow* window) {
    /*string decision;
    cout << "You want to quit? Are you sure? YES = Y, NO = N " << endl;
    cin >> decision;
    if (decision == "Y" || decision == "y") {
        StopAudio();
        cout << "\n----------------------------------------------Unloading Songs!--------------------------------------------" << endl;
        audioManager.unloadAssets();
        ListSongs();
        cout << "\n----------------------------------------------BYE for now!--------------------------------------------" << endl;
    }
    else if (decision == "N" || decision == "n") {
        cout << "Continue listening..." << endl;
        RunMediaPlayer();
    }
    else {
        cout << "Enter valid choice!" << endl;
        StopMediaPlayer();
    }*/


    string decision;
    cout << "You want to quit? Are you sure? YES = Y, NO = N " << endl;
    cin >> decision;
    if (decision == "Y" || decision == "y") {
        StopAudio();
        cout << "\n----------------------------------------------Unloading Songs!--------------------------------------------" << endl;
        audioManager.unloadAssets();
        ListSongs();
        cout << "\n----------------------------------------------BYE for now!--------------------------------------------" << endl;
    }
    else if (decision == "N" || decision == "n") {
        cout << "Continue listening..." << endl;
        RunMediaPlayer(window);
    }
    else {
        cout << "Enter valid choice!" << endl;
        StopMediaPlayer(window);
    }
}

void cMediaPlayer::ListSongs() {
    char name[256];
    cout << "\n----------------------------------------------List of Songs!--------------------------------------------" << endl;
    for (int index = 0; index < audioManager.sounds.size(); index++) {
        if (audioManager.sounds[index]) {
            // Get the name of the sound
            FMOD_RESULT result = audioManager.sounds[index]->getName(name, sizeof(name));

            if (result == FMOD_OK) {
                // Print the name of the sound
                std::cout << "Song " << index + 1 << ": " << name << std::endl;
            }
            else {
                // Handle errors (e.g., if sound name could not be retrieved)
                std::cout << "Error retrieving the name of sound " << index + 1 << std::endl;
            }
        }
        else {
            std::cout << "Sound " << index + 1 << " is null." << std::endl;
        }
    }
}

void cMediaPlayer::PlayAudio(int index) {
	audioManager.play(index);
}

void cMediaPlayer::PauseAudio() {
	audioManager.pause();
}

void cMediaPlayer::StopAudio() {
	audioManager.stop();
    ListSongs();
}

void cMediaPlayer::AdjustPitch() {
    float pitch = 0.0;
	audioManager.setPitch(pitch);
}

void cMediaPlayer::AdjustVolume() {
    float volume = 0.0;
	audioManager.setVolume(volume);
}

void cMediaPlayer::AdjustPan() {
    float pan = 0.0;
	audioManager.setPan(pan);
}