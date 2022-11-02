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
#include <iostream>
#include "CardGame.h"

GLFWwindow* window;

#include "FModManager.h"

constexpr int max_channels = 255;

//initialize our sound manager
FModManager fmod_manager;
CardGame g_CardGame;

void key_callback(GLFWwindow* window, const int key, int scancode, const int action, const int mods)
{
	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		if (!g_CardGame.playerGuessing) {
			g_CardGame.Shuffle();
			fmod_manager.play_sound("card-shuffling", "fx");
		}
	}

	if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		g_CardGame.StartNewGame(1, 10);
	}

	if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		if (!g_CardGame.playerGuessing) {
			g_CardGame.showGameInfo();
		}
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		g_CardGame.SaveGame();
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		if (g_CardGame.playerGuessing) {
			fmod_manager.play_sound("whistle-up", "fx");
			std::cout << "Player guess: Higher!\n";
			Sleep(500);
			std::cout << ". ";
			Sleep(500);
			std::cout << ". ";
			Sleep(500);
			std::cout << ". ";
			Sleep(500);
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
	
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		if (g_CardGame.playerGuessing) {
			fmod_manager.play_sound("whistle-down", "fx");
			std::cout << "Player guess: Lower!\n";
			Sleep(500);
			std::cout << ". ";
			Sleep(500);
			std::cout << ". ";
			Sleep(500);
			std::cout << ". ";
			Sleep(500);
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

int InitializeProject() {
	std::cout << "Please select game language:\n";
	std::cout << "1 - English\n";
	std::cout << "2 - Portuguese\n";
	std::cout << "3 - Spanish\n";
	std::cout << "4 - Deutsch\n";
	std::cout << "5 - French\n";

	int playerChoice;
	std::cin >> playerChoice;

	switch (playerChoice) {
	case 1:
		g_CardGame.choosenLanguage = "English";
		break;
	case 2:
		g_CardGame.choosenLanguage = "Portuguese";
		break;
	case 3:
		g_CardGame.choosenLanguage = "Spanish";
		break;
	case 4:
		g_CardGame.choosenLanguage = "Deutsch";
		break;
	case 5:
		g_CardGame.choosenLanguage = "French";
		break;
	default:
		break;
	}

	std::cout << "Please select audio:\n";
	std::cout << "1 - Compact (mp3)\n";
	std::cout << "2 - Uncompact (wav)\n";

	std::cin >> fmod_manager.choosenAudio;

	// Load XML soundList and create sounds
	if (fmod_manager.LoadSounds() != 0) {
		return -5;
	}

	system("CLS");
	g_CardGame.loadGameLanguage();

	return 0;
}

int main(int argc, char* argv[]) {
	//initialize fmod with max channels
	if (!fmod_manager.Initialize(max_channels, FMOD_INIT_NORMAL))
		return -1;

	if (InitializeProject() != 0)
		return -5;

	// Create sound ui
	SoundUI sound_ui(&fmod_manager);

	// Initialize Game
	g_CardGame.Initialize(1, 10);

	// Initialize glfw/glad
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
	
	//play our bg sound
	if(!fmod_manager.play_sound("piano-bg", "music"))
		return -6;

	//dsp effects
	if(!fmod_manager.create_dsp("echo", FMOD_DSP_TYPE_ECHO, 500.0f) || 
		!fmod_manager.create_dsp("fader", FMOD_DSP_TYPE_FADER, 0.f) || 
		!fmod_manager.create_dsp("dsp_pitch", FMOD_DSP_TYPE_PITCHSHIFT, 1.0f))
		return -7;


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
