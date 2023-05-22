#include "ModuleWinLose.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "Puntuation.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"
#include "Globals.h"
#include "ModuleFadeToBlack.h"
#include <iostream>

using namespace std;


WinLose::WinLose(bool startEnabled) : Module(startEnabled)
{
	for (int fila = 0; fila <= 1; fila++) {
		for (int columna = 0; columna <= 3; columna++) {
			int frameX = columna * SCREEN_WIDTH;
			int frameY = fila * SCREEN_HEIGHT;
			WinAnimation.PushBack({ frameX, frameY, SCREEN_WIDTH, SCREEN_HEIGHT });
		}
	}
	WinAnimation.speed = 0.02f;
	WAnimationPath.PushBack({ 0.0f, 0.0f }, 200, &WinAnimation);
	
	for (int fila = 0; fila <= 1; fila++) {
		for (int columna = 0; columna <= 3; columna++) 
		{
			int frameX = columna * SCREEN_WIDTH;
			int frameY = fila * SCREEN_HEIGHT;
			LoseAnimation.PushBack({ frameX, frameY, SCREEN_WIDTH, SCREEN_HEIGHT });
		}
	}
	LoseAnimation.speed = 0.01f;
	LAnimationPath.PushBack({ 0.0f, 0.0f }, 200, &LoseAnimation);

}

WinLose::~WinLose()
{

}

// Load assets
bool WinLose::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	WinTexture = App->textures->Load("Assets/Sprites/SpritesWin.png");
	LoseTexture = App->textures->Load("Assets/Sprites/SpritesLose.png");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;

}

Update_Status WinLose::Update()
{

	if (App->input->keys[SDL_SCANCODE_F3] == KEY_DOWN)
	{
		gameFinish = true;
	}
	if (gameFinish && App->puntuation->score < 1000)
	{
		AAnimationPath = LAnimationPath;
		ActiveTexture = LoseTexture;
		gameFinish = false;
		App->audio->PlayMusic("Assets/Music/Lose.ogg", 1.0f);
		App->fade->FadeToBlack((Module*)App->sceneLevel_1, (Module*)App->lose_screen, 90);
	}
	if (gameFinish && App->puntuation->score >= 1000)
	{
		AAnimationPath = WAnimationPath;
		ActiveTexture = WinTexture;
		gameFinish = false;
		App->audio->PlayMusic("Assets/Music/Win.ogg", 1.0f);
		App->fade->FadeToBlack((Module*)App->sceneLevel_1, (Module*)App->intro, 90);
	}
	
	return Update_Status::UPDATE_CONTINUE;
}

Update_Status WinLose::PostUpdate()
{
	if (ActiveTexture != nullptr)
	{
		AAnimationPath.Update();
		App->render->Blit(ActiveTexture, 0, 0, &(AAnimationPath.GetCurrentAnimation()->GetCurrentFrame()), 1.0f);
	}

	return Update_Status::UPDATE_CONTINUE;
}
bool WinLose::CleanUp()
{
	if (WinTexture != nullptr)
	{
		SDL_DestroyTexture(WinTexture);
		WinTexture = nullptr;
	}
	if (LoseTexture != nullptr)
	{
		SDL_DestroyTexture(LoseTexture);
		LoseTexture = nullptr;
	}
	if (ActiveTexture != nullptr)
	{
		SDL_DestroyTexture(ActiveTexture);
		ActiveTexture = nullptr;
	}
	return true;
}