#pragma once
#include "fmod.hpp"
#include <vector>
#include <string>

class cChannelGroupManager {
public:
	FMOD::Channel* channel; 
	std::vector<FMOD::Sound*> sounds; // Vector to hold multiple sound pointers
	FMOD::ChannelGroup* m_ChannelGroup = nullptr;

	void CreateChannelGroup();
	void LoadScene();
	void SetChannelGroupDSP();
	void PrintChannelGroupInfo();
	FMOD::ChannelGroup* getChannelGroup();
	void setListenerAttributes(FMOD_VECTOR position, FMOD_VECTOR forward, FMOD_VECTOR up);
	void SetListenerPosition();

	bool bIsPlaying = false;
	bool bIsPaused = false;
	float pitchValue = 0;
	float panValue = 0;
	float volumeValue = 0;

	// Load and unload function definitions
	void loadAssets();
	void loadAssets(std::string folderPathInput);
	void unloadAssets();

	// Sound control function definitions
	void play(int index);
	void pause();
	void resume();
	void stop();
	void setPitch(float pitchValue);
	void setVolume(float volumeValue);
	void setPan(float panValue);

	void Update();
};
