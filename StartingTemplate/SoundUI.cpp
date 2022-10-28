#include "SoundUI.h"
#include "imgui/imgui.h"

#include <string>

SoundUI::SoundUI(FModManager* f_manager) {
	fmod_manager_ = f_manager;

	masterVolume = 0.f;
	musicVolume = 0.f;
	fxVolume = 0.f;
}

bool SoundUI::DisplayChannelVolume(std::string channelName)
{
	FModManager::ChannelGroup* channel_group;
	if (!fmod_manager_->find_channel_group(channelName, &channel_group)) {
		return false;
	}

	float currentVolume;
	if (!fmod_manager_->get_channel_group_volume(channelName, &currentVolume)) {
		return false;
	}

	currentVolume *= 100;
	ImGui::SliderFloat((channelName + " Volume").c_str(), &currentVolume, 0.0f, 100.0f, "%.0f");
	currentVolume /= 100;

	if (!fmod_manager_->set_channel_group_volume(channelName, currentVolume)) {
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

	ImGui::SliderFloat("Pan", &channel_group->current_pan, -1.0f, 1.0f, " % .2f");

	if (!fmod_manager_->set_channel_group_pan(channelName, channel_group->current_pan)) {
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
	ImGui::SliderFloat((channelName + " Pitch").c_str(), &current_pitch, 0.5f, 2.0f, "%.2f");
	channel_group->group_ptr->setPitch(current_pitch);

	return true;
}

bool SoundUI::DisplayChannelPitchDSP(std::string channelName)
{
	FModManager::ChannelGroup* channel_group;
	if (!fmod_manager_->find_channel_group(channelName, &channel_group)) {
		return false;
	}

	ImGui::SliderFloat("Pitch (using dsp)", &channel_group->dsp_pitch, 0.5f, 2.0f, "%.2f");
	FMOD::DSP* dsp;
	if (!fmod_manager_->get_dsp("dsp_pitch", &dsp)) {
		return false;
	}
	dsp->setParameterFloat(0, channel_group->dsp_pitch);
	return true;
}

bool SoundUI::DisplayChannelEcho(std::string channelName) {
	FModManager::ChannelGroup* channel_group;
	if (!fmod_manager_->find_channel_group(channelName, &channel_group)) {
		return false;
	}

	ImGui::SliderFloat((channelName + " echo").c_str(), &channel_group->echo, 10.f, 5000.0f, "%.2f");
	FMOD::DSP* dsp;
	if (!fmod_manager_->get_dsp("echo", &dsp)) {
		return false;
	}
	dsp->setParameterFloat(0, channel_group->echo);

	ImGui::SameLine();
	ImGui::Checkbox(("##" + channelName + "_echo").c_str(), &channel_group->bEcho);

	if (channel_group->bEcho && !channel_group->echo_enabled) {
		channel_group->echo_enabled = true;
		if (!fmod_manager_->add_dsp_effect(channelName, "echo")) {
			return false;
		}
	}
	else if (!channel_group->bEcho && channel_group->echo_enabled) {
		channel_group->echo_enabled = false;
		if (!fmod_manager_->remove_dsp_effect(channelName, "echo")) {
			return false;
		}
	}

	return true;
}

bool SoundUI::DisplayChannelFader(std::string channelName) {
	FModManager::ChannelGroup* channel_group;
	if (!fmod_manager_->find_channel_group(channelName, &channel_group)) {
		return false;
	}

	ImGui::SliderFloat((channelName + " fader").c_str(), &channel_group->fader, 0.5f, 2.0f, "%.2f");
	FMOD::DSP* dsp;
	if (!fmod_manager_->get_dsp("fader", &dsp)) {
		return false;
	}
	dsp->setParameterFloat(0, channel_group->fader);

	ImGui::SameLine();
	ImGui::Checkbox(("##" + channelName + "_fader").c_str(), &channel_group->bFader);

	if (channel_group->bFader && !channel_group->fader_enabled) {
		channel_group->fader_enabled = true;
		if (!fmod_manager_->add_dsp_effect(channelName, "fader")) {
			return false;
		}
	}
	else if (!channel_group->bFader && channel_group->fader_enabled) {
		channel_group->fader_enabled = false;
		if (!fmod_manager_->remove_dsp_effect(channelName, "fader")) {
			return false;
		}
	}

	return true;
}

void SoundUI::render() {
	//setup ui structure
	ImGui::Begin("Game Audio Settings");
	ImGui::BulletText("Master Channel");

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
	
	if (!DisplayChannelEcho("master")) {
		// Something went wrong, what now?
	}

	if (!DisplayChannelFader("master")) {
		// Something went wrong, what now?
	}	

	ImGui::BulletText("Music Channel");
	if (!DisplayChannelVolume("music")) {
		// Something went wrong, what now?
	}

	unsigned int position = fmod_manager_->getSoundPosition("piano-bg", "master");
	std::string minuto = std::to_string(position / 1000 / 60);
	std::string segundo = std::to_string(position / 1000 % 60);

	ImGui::Text(("Time Elapsed: " + minuto + ":" + segundo).c_str());

	if (!DisplayChannelPitch("music")) {
		// Something went wrong, what now?
	}

	if (!DisplayChannelEcho("music")) {
		// Something went wrong, what now?
	}

	if (!DisplayChannelFader("music")) {
		// Something went wrong, what now?
	}

	ImGui::BulletText("Sound Effect Channel");
	if (!DisplayChannelVolume("fx")) {
		// Something went wrong, what now?
	}

	if (!DisplayChannelPitch("fx")) {
		// Something went wrong, what now?
	}

	if (!DisplayChannelEcho("fx")) {
		// Something went wrong, what now?
	}

	if (!DisplayChannelFader("fx")) {
		// Something went wrong, what now?
	}

	ImGui::End();
	
}
