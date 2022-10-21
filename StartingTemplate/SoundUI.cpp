#include "SoundUI.h"
#include "imgui/imgui.h"

void SoundUI::render()
{
	//setup ui structure
	ImGui::Begin("Audio Settings");

	{
		FModManager::ChannelGroup* channel_group;
		if (!fmod_manager_->find_channel_group("master", &channel_group))
		{
			//cannot find channel group
			//do something...
		}

		float current_volume;
		if (!fmod_manager_->get_channel_group_volume("master", &current_volume)) {
			//failed
		}
		current_volume *= 100;
		ImGui::SliderFloat("master volume", &current_volume, 0.0f, 100.0f, "%.0f");
		current_volume /= 100;

		// WARNING: volume range (0.0-1.0) //dont go to high above 1 (you can hurt your hardware)
		if (!fmod_manager_->set_channel_group_volume("master", current_volume)) {
			//failed
		}

		bool volume_enabled;
		if (!fmod_manager_->get_channel_group_enabled("master", &volume_enabled)) {
			//failed
		}

		ImGui::SameLine();
		ImGui::Checkbox("##master_volume", &volume_enabled);

		if (!fmod_manager_->set_channel_group_enabled("master", volume_enabled)) {
			//failed
		}
	}
	
	{
		FModManager::ChannelGroup* channel_group;
		if (!fmod_manager_->find_channel_group("music", &channel_group))
		{
			//cannot find channel group
			//do something...
		}

		float current_volume;
		if (!fmod_manager_->get_channel_group_volume("music", &current_volume)) {
			//failed
		}
		current_volume *= 100;
		ImGui::SliderFloat("music volume", &current_volume, 0.0f, 100.0f, "%.0f");
		current_volume /= 100;

		// WARNING: volume range (0.0-1.0) //dont go to high above 1 (you can hurt your hardware)
		if (!fmod_manager_->set_channel_group_volume("music", current_volume)) {
			//failed
		}

		bool volume_enabled;
		if (!fmod_manager_->get_channel_group_enabled("music", &volume_enabled)) {
			//failed
		}

		ImGui::SameLine();
		ImGui::Checkbox("##music_volume", &volume_enabled);

		if (!fmod_manager_->set_channel_group_enabled("music", volume_enabled)) {
			//failed
		}
	}

	{
		FModManager::ChannelGroup* channel_group;
		if (!fmod_manager_->find_channel_group("fx", &channel_group))
		{
			//cannot find channel group
			//do something...
		}

		float current_volume;
		if (!fmod_manager_->get_channel_group_volume("fx", &current_volume)) {
			//failed
		}
		current_volume *= 100;
		ImGui::SliderFloat("fx volume", &current_volume, 0.0f, 100.0f, "%.0f");
		current_volume /= 100;

		// WARNING: volume range (0.0-1.0) //dont go to high above 1 (you can hurt your hardware)
		if (!fmod_manager_->set_channel_group_volume("fx", current_volume)) {
			//failed
		}

		bool volume_enabled;
		if (!fmod_manager_->get_channel_group_enabled("fx", &volume_enabled)) {
			//failed
		}

		ImGui::SameLine();
		ImGui::Checkbox("##fx_volume", &volume_enabled);

		if (!fmod_manager_->set_channel_group_enabled("fx", volume_enabled)) {
			//failed
		}
	}

	{
		FModManager::ChannelGroup* channel_group;
		if (!fmod_manager_->find_channel_group("fx", &channel_group))
		{
			//cannot find channel group
			//do something...
		}

		ImGui::SliderFloat("master pan", &channel_group->current_pan, -1.0f, 1.0f, "%.2f");

		if (!fmod_manager_->set_channel_group_pan("master", channel_group->current_pan))
		{
			//error
			//do something?
		}
	}

	{
		FModManager::ChannelGroup* channel_group;
		if (!fmod_manager_->find_channel_group("master", &channel_group))
		{
			//cannot find channel group
			//do something...
		}

		float current_pitch;
		channel_group->group_ptr->getPitch(&current_pitch);
		ImGui::SliderFloat("master pitch (no dsp)", &current_pitch, 0.5f, 2.0f, "%.2f");
		channel_group->group_ptr->setPitch(current_pitch);
	}

	{
		FModManager::ChannelGroup* channel_group;
		if (!fmod_manager_->find_channel_group("master", &channel_group))
		{
			//cannot find channel group
			//do something...
		}

		ImGui::SliderFloat("master pitch (using dsp)", &channel_group->dsp_pitch, 0.5f, 2.0f, "%.2f");
		FMOD::DSP* dsp;
		if(!fmod_manager_->get_dsp("dsp_pitch", &dsp))
		{
			//dsp not found...
		}
		dsp->setParameterFloat(0, channel_group->dsp_pitch);
	}

	ImGui::End();
	
}
