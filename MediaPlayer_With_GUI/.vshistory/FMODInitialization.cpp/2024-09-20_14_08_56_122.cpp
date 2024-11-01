#include "FMODInitialization.h"

FMODInitialization::FMODInitialization() {
	FMOD::System* fmodSystem = nullptr;
	FMOD_RESULT result;
	result = FMOD::System_Create(&fmodSystem);
	if (result != FMOD_OK) {
		cout << "System creation error! " << FMOD_ErrorString(result) << endl;
	}

	result = fmodSystem->init(512, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK) {
		cout << "FMOD initialization error! " << FMOD_ErrorString(result) << endl;
	}
}

FMODInitialization::~FMODInitialization() {

}
