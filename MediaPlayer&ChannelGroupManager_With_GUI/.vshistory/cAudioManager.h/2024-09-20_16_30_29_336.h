#pragma once
class cAudioManager {
public:
	void loadAssets();
	void unloadAssets();

	void play();
	void pause();
	void stop();
	void setPitch();
	void setVolume();
	void setPan();
};