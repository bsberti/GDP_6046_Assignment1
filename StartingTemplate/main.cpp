#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "SoundUI.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "CardGame.h"

GLFWwindow* window;

#include "FModManager.h"

constexpr int max_channels = 255;

//initialize our sound manager
FModManager fmod_manager;
bool compressed;
CardGame g_CardGame;

void key_callback(GLFWwindow* window, const int key, int scancode, const int action, const int mods)
{
	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		if (!g_CardGame.playerGuessing) {
			g_CardGame.Shuffle();
			fmod_manager.play_sound("card-shuffling", "fx");
		}
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		if (g_CardGame.playerGuessing) {
			fmod_manager.play_sound("whistle-up", "fx");
			Sleep(2000);
			// Player is beting higher card
			int result = g_CardGame.HigherGuess();
			if (result == 0) {
				//Player win!
				fmod_manager.play_sound("victory", "fx");
			}
			else if (result == 1) {
				fmod_manager.play_sound("wrong", "fx");
			}
			else if (result == 2) {
				fmod_manager.play_sound("game-over", "fx");
			}
		}
	}
	
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		if (g_CardGame.playerGuessing) {
			fmod_manager.play_sound("whistle-down", "fx");
			Sleep(2000);
			// Player is beting lower card
			int result = g_CardGame.LowerGuess();
			if (result == 0) {
				//Player win!
				fmod_manager.play_sound("victory", "fx");
			}
			else if (result == 1) {
				fmod_manager.play_sound("wrong", "fx");
			}
			else if (result == 2) {
				fmod_manager.play_sound("game-over", "fx");
			}
		}
	}
}

int main(int argc, char* argv[]) {
	compressed = true;
	if (argc > 1) {
		if (argv[1] != NULL) {
			printf("Argument received");
			sscanf_s(argv[1], "%b", &compressed);
			printf("compressed = %b\n", compressed);
		}
	}

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

	//set parents for channel groups
	if(!fmod_manager.set_channel_group_parent("music", "master") || ! fmod_manager.set_channel_group_parent("fx", "master"))
		return -4;

	// Load XML soundList and create sounds
	if (fmod_manager.LoadSounds() != 0) {
		return -5;
	}
	
	//play our bg sound
	if(!fmod_manager.play_sound("piano-bg", "music"))
		return -6;

	//dsp effects
	if(!fmod_manager.create_dsp("echo", FMOD_DSP_TYPE_ECHO, 500.0f) || 
		!fmod_manager.create_dsp("fader", FMOD_DSP_TYPE_FADER, 0.f) || 
		!fmod_manager.create_dsp("dsp_pitch", FMOD_DSP_TYPE_PITCHSHIFT, 1.0f))
		return -7;



	//create sound ui
	SoundUI sound_ui(&fmod_manager);

	g_CardGame.Initialize(1, 10);

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
