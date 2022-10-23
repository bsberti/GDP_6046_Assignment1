#pragma once
#include "FModManager.h"

class SoundUI
{
protected:
	FModManager* fmod_manager_;

	bool DisplayChannelVolume(std::string channelName);
	bool DisplayChannelPan(std::string channelName);
	bool DisplayChannelPitch(std::string channelName);
	bool DisplayChannelPitchDSP(std::string channelName);

public:
	SoundUI(FModManager* f_manager) : fmod_manager_(f_manager) {}
	void render();
};
