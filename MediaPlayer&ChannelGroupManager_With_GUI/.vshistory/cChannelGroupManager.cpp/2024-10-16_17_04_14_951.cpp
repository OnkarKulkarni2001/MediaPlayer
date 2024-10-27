#include "cChannelGroupManager.h"
#include "cMediaPlayer.h"
#include "FMODInitialization.h"
#include <fstream>
#include <vector>
#include <Windows.h>
#include <conio.h>

FMODInitialization initializationObject;
bool m_Initialized;
FMOD_RESULT result;
FMOD::System* fmodSystem = initializationObject.Initialized(result, m_Initialized);
FMOD::Sound* sound;
std::string folderPath;  // Storing folder path from user input

char channel_name[128] = "";  // Buffer for inputting the channel name
char createMessage[256] = ""; // Buffer to store success or error messages

void cChannelGroupManager::CreateChannelGroup() {
    if (!m_Initialized) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "System not initialized!");
        return;
    }

    ImGui::InputText("Channel Name", channel_name, IM_ARRAYSIZE(channel_name));

    if (ImGui::Button("Create Channel Group")) {
        if (strlen(channel_name) > 0) {
            FMOD_RESULT result = fmodSystem->createChannelGroup(channel_name, &m_ChannelGroup);

            if (result != FMOD_OK) {
                snprintf(createMessage, IM_ARRAYSIZE(createMessage), "Error: %s", FMOD_ErrorString(result));
            }
            else {
                snprintf(createMessage, IM_ARRAYSIZE(createMessage), "Channel group '%s' has been created!", channel_name);
            }
        }
        else {
            snprintf(createMessage, IM_ARRAYSIZE(createMessage), "Please enter a valid channel name.");
        }
    }

    if (strlen(createMessage) > 0) {
        ImGui::Text("%s", createMessage);
    }
}

void cChannelGroupManager::SetChannelGroupDSP() {
    if (!m_Initialized) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "System not initialized!");
        return;
    }

    if (ImGui::Button("Set Distortion DSP")) {
        FMOD_RESULT result;
        FMOD::DSP* distortionDSP;

        result = fmodSystem->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &distortionDSP);

        if (result != FMOD_OK) {
            snprintf(dspMessage, IM_ARRAYSIZE(dspMessage), "Error: %s", FMOD_ErrorString(result));
        }
        else {
            result = distortionDSP->setParameterFloat(FMOD_DSP_DISTORTION_LEVEL, 0.0f);

            if (result != FMOD_OK) {
                snprintf(dspMessage, IM_ARRAYSIZE(dspMessage), "Error: %s", FMOD_ErrorString(result));
            }
            else {
                m_ChannelGroup->addDSP(0, distortionDSP);
                snprintf(dspMessage, IM_ARRAYSIZE(dspMessage), "Distortion DSP has been set!");
            }
        }
    }

    if (strlen(dspMessage) > 0) {
        ImGui::Text("%s", dspMessage);
    }
}

void cChannelGroupManager::PrintChannelGroupInfo() {
    char name[256];
    m_ChannelGroup->getName(name, sizeof(name));

    float volume = 0.0f;
    m_ChannelGroup->getVolume(&volume);

    float pitch = 0.0f;
    m_ChannelGroup->getPitch(&pitch);

    std::cout << name << " channel group has a volume of " << volume << std::endl;
    std::cout << name << " channel group has a pitch of " << pitch << std::endl;
}


FMOD::ChannelGroup* cChannelGroupManager::getChannelGroup()
{
    return m_ChannelGroup;
}

void cChannelGroupManager::loadAssets() {
    if (!m_Initialized) {
        ImGui::Text("FMOD not initialized");
        return;
    }
    ImGui::Text("Enter the folder path containing the songs:");

    static char folderPathInput[256] = "";  // Buffer for storing input folder path
    ImGui::InputText("Folder Path", folderPathInput, sizeof(folderPathInput));  // Taking Input for folder path

    ImGui::Text("Please wait for songs to load if your directory is bigger than usual...");
    // Button to trigger loading of assets
    if (ImGui::Button("Load Songs")) {

        std::string folderPath = std::string(folderPathInput);  // Converting input to std::string

        std::wstring wideFolderPath(folderPath.begin(), folderPath.end());
        std::wstring searchPath = wideFolderPath + L"\\*";  // Search all files

        WIN32_FIND_DATA findFileData;
        HANDLE hFind = FindFirstFile(searchPath.c_str(), &findFileData);

        if (hFind == INVALID_HANDLE_VALUE) {
            ImGui::Text("Could not open directory: %s", folderPath.c_str());
            ImGui::End(); 
            return;
        }

        FMOD_RESULT result;

        do {
            // Check if the found entry is a file
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::wstring filePath = wideFolderPath + L"\\" + findFileData.cFileName;  // Converting to full file path

                FMOD::Sound* newSound;
                result = fmodSystem->createSound(std::string(filePath.begin(), filePath.end()).c_str(), FMOD_DEFAULT, 0, &newSound);

                if (result != FMOD_OK) {
                    ImGui::Text("Error: Sound creation unsuccessful for file %s", std::string(filePath.begin(), filePath.end()).c_str());
                }
                else {
                    ImGui::Text("Sound %s loaded successfully!", std::string(filePath.begin(), filePath.end()).c_str());
                    sounds.push_back(newSound);  // Storing the new sound
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);  // Closing the find handle
    }
}

void cChannelGroupManager::unloadAssets() {
    
    bool bWindowShouldShow = true;
    if (!m_Initialized) {
        ImGui::Text("FMOD not initialized.");
        return;
    }

    if (result != FMOD_OK) {
        FMODCheckError(result);
        return;
    }

    if (sounds.empty()) {
        ImGui::Text("No sounds are loaded.");
        return;
    }


    static int choice = 0;
    ImGui::Text("Do you want to:");
    if (ImGui::RadioButton("Unload a specific song", choice == 1)) {
        choice = 1;
    }
    if (ImGui::RadioButton("Unload all songs", choice == 2)) {
        choice = 2;
    }

    if (choice == 1) {
        static int index = 0;
        ImGui::InputInt("Enter the song index", &index);

        // Ensuring the index is within bounds
        if (index >= 1 && index <= sounds.size()) {
            if (ImGui::Button("Unload Song")) {
                // Converting user input to zero-based index
                int zeroBasedIndex = index - 1;

                if (zeroBasedIndex >= 0 && zeroBasedIndex < sounds.size()) {
                    sounds[zeroBasedIndex]->release(); // Releasing the sound resource
                    sounds.erase(sounds.begin() + zeroBasedIndex); // Removing from the vector
                    ImGui::Text("Song at index %d unloaded successfully.", index);
                    bWindowShouldShow = false;
                }
            }
        }
        else {
            ImGui::Text("Invalid song index!");
        }
    }
    else if (choice == 2) {
        if (ImGui::Button("Unload All Songs")) {
            // Unloading all songs
            for (auto& sound : sounds) {
                sound->release(); // Releasing each sound
            }
            sounds.clear(); // Clearing the vector
            ImGui::Text("All songs unloaded successfully.");
            bWindowShouldShow = false;
        }
    }

    if (result != FMOD_OK) {
        FMODCheckError(result);
        return;
    }
}

void cChannelGroupManager::play(int index) {
    if (!m_Initialized) {
        cout << "FMOD not initialized" << endl;
        return;
    }

    if (result != FMOD_OK) {
        FMODCheckError(result);
        return;
    }
    channel->isPlaying(&bIsPlaying);
    if (bIsPlaying) {
        stop();
        play(index);
    }
    else {
        result = channel->isPlaying(&bIsPlaying);
        int userInput = index - 1;
        result = fmodSystem->playSound(sounds[userInput], nullptr, false, &channel);    // Playing sound
        do {
            fmodSystem->update();
        } while (bIsPlaying);
    }
}

void cChannelGroupManager::pause() {
    if (!m_Initialized) {
        cout << "FMOD not initialized" << endl;
        return;
    }

    if (result != FMOD_OK) {
        FMODCheckError(result);
        return;
    }

    if (channel) {
        channel->isPlaying(&bIsPlaying);
        if (bIsPlaying) {
            channel->getPaused(&bIsPaused);
            if (!bIsPaused) {
                channel->setPaused(true);   // Pausing sound
            }
        }
        else {
            cout << "No sound is playing to pause." << endl;
        }
    }

    if (result != FMOD_OK) {
        FMODCheckError(result);
        return;
    }
}

void cChannelGroupManager::resume() {
    if (!m_Initialized) {
        cout << "FMOD not initialized" << endl;
        return;
    }

    if (result != FMOD_OK) {
        FMODCheckError(result);
        return;
    }

    if (channel) {
        channel->isPlaying(&bIsPlaying);
        if (bIsPlaying) {
            channel->getPaused(&bIsPaused);
            if (bIsPaused) {
                channel->setPaused(false);      // Resuming sound
            }
        }
        else {
            cout << "No sound is paused to play." << endl;
        }
    }

    if (result != FMOD_OK) {
        FMODCheckError(result);
        return;
    }
}

void cChannelGroupManager::stop() {
    if (!m_Initialized) {
        cout << "FMOD not initialized" << endl;
        return;
    }

    if (result != FMOD_OK) {
        FMODCheckError(result);
        return;
    }
    channel->isPlaying(&bIsPlaying);
    if (bIsPlaying) {
        channel->stop();        // Stopping sound
    }
}

void cChannelGroupManager::setPitch(float pitchValue) {
    if (!m_Initialized) {
        cout << "FMOD not initialized" << endl;
        return;
    }

    if (result != FMOD_OK) {
        FMODCheckError(result);
        return;
    }


    if (channel) {
        channel->isPlaying(&bIsPlaying);
        if (bIsPlaying) {
            channel->setPitch(pitchValue);      // Setting pitch
        }
        else {
            cout << "No sound is currently playing to set pitch." << endl;
        }
    }
    else {
        cout << "No active channel found to set sound pitch." << endl;
    }

    if (result != FMOD_OK) {
        FMODCheckError(result);
        return;
    }
}

void cChannelGroupManager::setVolume(float volumeValue) {
    if (!m_Initialized) {
        cout << "FMOD not initialized" << endl;
        return;
    }

    if (result != FMOD_OK) {
        FMODCheckError(result);
        return;
    }

    if (channel) {
        channel->isPlaying(&bIsPlaying);
        if (bIsPlaying) {
            channel->setVolume(volumeValue);        // Setting Volume
        }
        else {
            cout << "No sound is currently playing to set volume." << endl;
        }
    }
    else {
        cout << "No active channel found to set sound volume." << endl;
    }

    if (result != FMOD_OK) {
        FMODCheckError(result);
        return;
    }
}

void cChannelGroupManager::setPan(float panValue) {
    if (!m_Initialized) {
        cout << "FMOD not initialized" << endl;
        return;
    }

    if (result != FMOD_OK) {
        FMODCheckError(result);
        return;
    }

    if (channel) {
        channel->isPlaying(&bIsPlaying);
        if (bIsPlaying) {
            channel->setPan(panValue);      // Setting pan
        }
        else {
            cout << "No sound is currently playing to set pan." << endl;
        }
    }
    else {
        cout << "No active channel found to set sound pan." << endl;
    }

    if (result != FMOD_OK) {
        FMODCheckError(result);
        return;
    }
}