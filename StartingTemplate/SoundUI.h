#pragma once
#include "FModManager.h"

class SoundUI
{
protected:
	FModManager* fmod_manager_;

public:
	SoundUI(FModManager* f_manager) : fmod_manager_(f_manager) {}
	void render();
};
