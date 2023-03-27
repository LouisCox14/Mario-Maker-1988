#include "Titlescreen.h"
#include <windows.h>
#include <Commdlg.h>

Titlescreen::Titlescreen(SDL_Renderer* renderer, GameScreenManager* _screenManager) : GameScreen(renderer)
{
	screenManager = _screenManager;

	background = new Texture2D(m_renderer, 2.0f);
	background->LoadFromFile("UI/Titlescreen.png");

	Animation introAnim = Animation(std::vector<std::string>{}, 0.075f, false);
	for (int i = 1; i < 24; i++)
	{
		introAnim.framePaths.push_back("Titlescreen Open " + std::to_string(i) + ".png");
	}

	backgroundAnimator = new Animator(*background, std::map<std::string, Animation>{
		{"Idle", Animation(std::vector<std::string>{"Titlescreen.png", "Titlescreen 1.png", "Titlescreen 2.png", "Titlescreen 3.png", "Titlescreen 2.png",
			"Titlescreen 1.png", "Titlescreen.png", "Titlescreen 4.png", "Titlescreen 5.png", "Titlescreen 6.png", "Titlescreen 5.png",
		"Titlescreen 4.png", "Titlescreen.png"}, 0.2f, true)},
		{"Intro", introAnim}
	}, std::string("Idle"), std::string("UI/Titlescreen/"));

	backgroundAnimator->SetAnimation("Intro");
}

Titlescreen::~Titlescreen()
{
	delete background;
	background = nullptr;
}

void Titlescreen::Render()
{
	background->Render(Vector2D(0, 0), SDL_FLIP_NONE);
}

void Titlescreen::Update(float deltaTime, SDL_Event e)
{
	backgroundAnimator->Update(deltaTime);

	// Handle the events
	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_e:
			std::string targetLevel = GetFileInput();

			if (targetLevel != "")
				screenManager->ChangeScreen(SCREEN_LEVEL1, targetLevel);
			break;
		}
	}
}

std::string Titlescreen::GetFileInput()
{
	OPENFILENAMEA ofn;
	char szFile[100];

	// open a file name
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = ".Json\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	GetOpenFileNameA(&ofn);
	return std::string(szFile);
}
