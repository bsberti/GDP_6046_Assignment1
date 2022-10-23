#include "SoundUI.h"
#include "imgui/imgui.h"

#include <string>

bool SoundUI::DisplayChannelVolume(std::string channelName)
{
	FModManager::ChannelGroup* channel_group;
	if (!fmod_manager_->find_channel_group(channelName, &channel_group)) {
		return false;
	}

	float current_volume;
	if (!fmod_manager_->get_channel_group_volume(channelName, &current_volume)) {
		return false;
	}
	current_volume *= 100;
	ImGui::SliderFloat((channelName + " volume").c_str(), &current_volume, 0.0f, 100.0f, "%.0f");
	current_volume /= 100;

	// WARNING: volume range (0.0-1.0) //dont go to high above 1 (you can hurt your hardware)
	if (!fmod_manager_->set_channel_group_volume(channelName, current_volume)) {
		return false;
	}

	bool volume_enabled;
	if (!fmod_manager_->get_channel_group_enabled(channelName, &volume_enabled)) {
		return false;
	}

	ImGui::SameLine();
	ImGui::Checkbox(("##" + channelName + "_volume").c_str(), &volume_enabled);

	if (!fmod_manager_->set_channel_group_enabled(channelName, volume_enabled)) {
		return false;
	}

	return true;
}

bool SoundUI::DisplayChannelPan(std::string channelName) {
	FModManager::ChannelGroup* channel_group;
	if (!fmod_manager_->find_channel_group(channelName, &channel_group)) {
		return false;
	}

	ImGui::SliderFloat((channelName + " pan").c_str(), &channel_group->current_pan, -1.0f, 1.0f, "%.2f");

	if (!fmod_manager_->set_channel_group_pan(channelName, channel_group->current_pan)) 	{
		return false;
	}

	return true;
}

bool SoundUI::DisplayChannelPitch(std::string channelName)
{
	FModManager::ChannelGroup* channel_group;
	if (!fmod_manager_->find_channel_group(channelName, &channel_group)) {
		return false;
	}

	float current_pitch;
	channel_group->group_ptr->getPitch(&current_pitch);
	ImGui::SliderFloat((channelName + " pitch (no dsp)").c_str(), &current_pitch, 0.5f, 2.0f, "%.2f");
	channel_group->group_ptr->setPitch(current_pitch);

	return true;
}

bool SoundUI::DisplayChannelPitchDSP(std::string channelName)
{
	FModManager::ChannelGroup* channel_group;
	if (!fmod_manager_->find_channel_group(channelName, &channel_group)) {
		return false;
	}

	ImGui::SliderFloat((channelName + " pitch (using dsp)").c_str(), &channel_group->dsp_pitch, 0.5f, 2.0f, "%.2f");
	FMOD::DSP* dsp;
	if (!fmod_manager_->get_dsp("dsp_pitch", &dsp)) {
		return false;
	}
	dsp->setParameterFloat(0, channel_group->dsp_pitch);
	return true;
}

void SoundUI::render()
{
	//setup ui structure
	ImGui::Begin("Game Audio Settings");
	
	if (!DisplayChannelVolume("master")) {
		// Something went wrong, what now?
	}

	if (!DisplayChannelPan("master")) {
		// Something went wrong, what now?
	}

	if (!DisplayChannelPitch("master")) {
		// Something went wrong, what now?
	}

	if (!DisplayChannelPitchDSP("master")) {
		// Something went wrong, what now?
	}

	if (!DisplayChannelVolume("music")) {
		// Something went wrong, what now?
	}

	if (!DisplayChannelVolume("fx")) {
		// Something went wrong, what now?
	}

	ImGui::End();
	
}
