﻿#pragma once

#include <cstdio>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#include <pugixml/pugixml.hpp>
#include <map>
#include <string>
#include <vector>

class FModManager
{
public:
	struct ChannelGroup {
		FMOD::ChannelGroup* group_ptr;
		float current_pan;
		float dsp_pitch;
		float echo;

		ChannelGroup() : group_ptr(nullptr), current_pan(0.0f), dsp_pitch(1.0f) {}
	};

	struct MySound {
		std::string category;
		std::string name;
		std::string path;
		//FMOD_DEFAULT = 0
		//FMOD_LOOP_NORMAL = 2
		int mode;
	};

protected:
	FMOD_RESULT last_result_;
	FMOD::System* system_;

	pugi::xml_document soundListDoc;
	std::vector<MySound> mSoundList;

	std::map<std::string, ChannelGroup*> mChannelGroups;	
	std::map<std::string, FMOD::Sound*> mSounds;
	std::map<std::string, FMOD::DSP*> mDSPs;

	bool is_okay(const bool show_error = true) const;

public:
	//lifecycle
	FModManager();
	bool Initialize(const int number_of_channels, const int system_flags);
	void Shutdown();
	int LoadSounds();

	//channel groups
	bool create_channel_group(const std::string& name);
	bool find_channel_group(const std::string& name, ChannelGroup** channel_group);
	void remove_channel_group(const std::string& name);
	bool set_channel_group_parent(const std::string& child_name, const std::string& parent_name);
	bool get_channel_group_volume(const std::string& name, float* volume);
	bool set_channel_group_volume(const std::string& name, float volume);

	bool get_channel_group_pan(const std::string& name, float* pan);
	bool set_channel_group_pan(const std::string& name, float pan);

	bool get_channel_group_enabled(const std::string& name, bool* enabled);
	bool set_channel_group_enabled(const std::string& name, bool enabled);
	bool add_dsp_effect(const std::string& channel_group_name, const std::string& effect_name);
	bool remove_dsp_effect(const std::string& channel_group_name, const std::string& effect_name);

	//sounds
	bool create_sound(const std::string& name, const std::string& path, FMOD_MODE mode);
	bool play_sound(const std::string& sound_name, const std::string& channel_group_name);

	//dsp
	bool create_dsp(const std::string& name, FMOD_DSP_TYPE dsp_type, const float value);
	bool get_dsp(const std::string& name, FMOD::DSP** dsp);
};
