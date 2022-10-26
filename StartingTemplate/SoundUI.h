#pragma once
#include "FModManager.h"
#include <vector>

class SoundUI
{
protected:
	FModManager* fmod_manager_;

	bool DisplayChannelVolume(std::string channelName);
	bool DisplayChannelPan(std::string channelName);
	bool DisplayChannelPitch(std::string channelName);
	bool DisplayChannelPitchDSP(std::string channelName);
	bool DisplayChannelEcho(std::string channelName);
	bool DisplayChannelFader(std::string channelName);

public:

	float masterVolume;
	float musicVolume;
	float fxVolume;

	SoundUI(FModManager* f_manager);
	void render();
};
