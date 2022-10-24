#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SoundUI.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "CardGame.h"

GLFWwindow* window;

#include "FModManager.h"

constexpr int max_channels = 255;

//initialize our sound manager
FModManager fmod_manager;

CardGame g_CardGame;

void key_callback(GLFWwindow* window, const int key, int scancode, const int action, const int mods)
{
	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		fmod_manager.play_sound("card-shuffling", "fx");
		g_CardGame.Shuffle();
	}

	if(key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		fmod_manager.add_dsp_effect("master", "echo");
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		fmod_manager.remove_dsp_effect("master", "echo");
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		fmod_manager.add_dsp_effect("master", "dsp_pitch");
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		// Player is beting higher card
		if (g_CardGame.HigherGuess()) {
			//Player win!
			fmod_manager.play_sound("victory", "fx");
		}
	}
	
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		// Player is beting lower card
		if (g_CardGame.LowerGuess()) {
			//Player win!
			fmod_manager.play_sound("victory", "fx");
		}
	}
}

int main(int argc, char* argv[])
{
	//initialize glfw/glad
	glfwInit();
	window = glfwCreateWindow(800, 600, "INFO-6064", nullptr, nullptr);

	if(!window)
		return 1;
	
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		return 2;
	

	glfwSetKeyCallback(window, key_callback);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	//initialize imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	//platform / render bindings
	if (!ImGui_ImplGlfw_InitForOpenGL(window, true) || !ImGui_ImplOpenGL3_Init("#version 460"))
		return 3;

	//imgui style (dark mode for the win)
	ImGui::StyleColorsDark();

	//initialize fmod with max channels
	if (!fmod_manager.Initialize(max_channels, FMOD_INIT_NORMAL))
		return -1;

	//create channel groups
	if (
		!fmod_manager.create_channel_group("master") ||
		!fmod_manager.create_channel_group("music") ||
		!fmod_manager.create_channel_group("fx")
		)
	{
		return -2;
	}

	//set the volume to 30% (on load)
	if(!fmod_manager.set_channel_group_volume("master", 0.3f))
		return -3;

	//set parents for channel groups
	if(!fmod_manager.set_channel_group_parent("music", "master") || ! fmod_manager.set_channel_group_parent("fx", "master"))
		return -4;

	// Load XML soundList and create sounds
	if (fmod_manager.LoadSounds() != 0) {
		return -5;
	}
	
	//play our bg sound
	if(!fmod_manager.play_sound("bg", "music"))
		return -6;

	//dsp effects
	if(!fmod_manager.create_dsp("echo", FMOD_DSP_TYPE_ECHO, 500.0f) || !fmod_manager.create_dsp("dsp_pitch", FMOD_DSP_TYPE_PITCHSHIFT, 1.0f))
		return -7;

	//create sound ui
	SoundUI sound_ui(&fmod_manager);

	g_CardGame.Initialize(1, 50);

	//game loop
	while(!glfwWindowShouldClose(window)) {
		//poll for user events
		glfwPollEvents();

		//clear the back buffer
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		sound_ui.render();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//present to the user
		glfwSwapBuffers(window);
	}

	fmod_manager.Shutdown();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}
