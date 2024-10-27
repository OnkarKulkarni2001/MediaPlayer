#pragma once
class cAudioManager {
public:
	bool bIsPlaying = false;
	bool bIsPaused = false;
	float pitchValue = 0;
	float panValue = 0;
	float volumeValue = 0;
	void loadAssets();
	void unloadAssets();

	void play(int index);
	void pause();
	void resume();
	void stop();
	void setPitch();
	void setVolume();
	void setPan();
};