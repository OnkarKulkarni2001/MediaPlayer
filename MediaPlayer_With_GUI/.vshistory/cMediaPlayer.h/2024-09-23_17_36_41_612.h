#pragma once
class cMediaPlayer {
public:
	void RunMediaPlayer();
	void StopMediaPlayer();
	void PlayAudio(int index);
	void PauseAudio();
	void StopAudio();
	void AdjustPitch();
	void AdjustVolume();
	void AdjustPan();
};

