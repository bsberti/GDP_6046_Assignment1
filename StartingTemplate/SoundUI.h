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
	bool DisplayChannelEcho(std::string channelName);


public:
	bool echo_enabled;

	SoundUI(FModManager* f_manager) : fmod_manager_(f_manager) {}
	void render();
};
