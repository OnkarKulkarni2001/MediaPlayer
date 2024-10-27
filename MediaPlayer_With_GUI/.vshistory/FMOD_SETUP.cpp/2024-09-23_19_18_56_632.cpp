#include "FMODInitialization.h"
#include "AudioUtils.hpp"
#include "fmod.h"
#include <iostream>
#include "cMediaPlayer.h"
#include "cAudioManager.h"
#include <conio.h>

using namespace std;

int main()
{
	cAudioManager audioManager;
	cMediaPlayer mediaPlayer;
	int key = 0;
	mediaPlayer.RunMediaPlayer();
	while (_kbhit()) {
		if(audioManager.bIsPlaying) {
			key = _getch();
			if (key == 27) {
				mediaPlayer.StopMediaPlayer();
			}
		}
	}

	return 0;
}