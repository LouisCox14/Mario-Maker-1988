#include "Titlescreen.h"
#include <windows.h>
#include <Commdlg.h>

Titlescreen::Titlescreen(SDL_Renderer* renderer, GameScreenManager* _screenManager) : GameScreen(renderer, _screenManager)
{
	background = new Texture2D(m_renderer, 2.0f);
	background->LoadFromFile("Assets/UI/Titlescreen/Titlescreen.png");

	curtain = new Texture2D(m_renderer, 2.0f);
	curtain->LoadFromFile("Assets/UI/Titlescreen/Curtain.png");
	curtainDrawTime = 2.0f;

	title = new Texture2D(m_renderer, 2.0f);
	title->LoadFromFile("Assets/UI/Titlescreen/Title.png");
	titleFloatSpeed = 0.35f;
	titleFloatOffset = 5.0f;

	version = new Texture2D(m_renderer, 2.0f);
	version->LoadFromFile("Assets/UI/Titlescreen/Version.png");

	playButton = new ButtonUI(renderer, this, Vector2D(150, 260), std::string("Assets/UI/Titlescreen/Play Button.png"), 2.0f);
	multiplayerButton = new ButtonUI(renderer, this, Vector2D(150, 285), std::string("Assets/UI/Titlescreen/Multiplayer Button.png"), 2.0f);
	editButton = new ButtonUI(renderer, this, Vector2D(150, 310), std::string("Assets/UI/Titlescreen/Edit Button.png"), 2.0f);
	createButton = new ButtonUI(renderer, this, Vector2D(150, 335), std::string("Assets/UI/Titlescreen/Create Button.png"), 2.0f);

	audioPlayer = new AudioPlayer({"ButtonClicked"}, {"MenuMusic"});
	audioPlayer->PlayMusic("MenuMusic");
}

Titlescreen::~Titlescreen()
{
	delete background;
	background = nullptr;

	delete title;
	title = nullptr;

	delete version;
	version = nullptr;

	delete curtain;
	curtain = nullptr;

	delete playButton;
	playButton = nullptr;

	delete multiplayerButton;
	multiplayerButton = nullptr;

	delete editButton;
	editButton = nullptr;

	delete createButton;
	createButton = nullptr;
}

void Titlescreen::Render()
{
	background->Render(Vector2D(0, 0), SDL_FLIP_NONE);
	title->Render(Vector2D(SCREEN_WIDTH / 2 - title->GetWidth() / 2, 40 + sin(SDL_GetTicks() / 100 * titleFloatSpeed) * titleFloatOffset), SDL_FLIP_NONE);
	version->Render(Vector2D(SCREEN_WIDTH / 2 - version->GetWidth() / 2, 170), SDL_FLIP_NONE);

	playButton->Render();
	multiplayerButton->Render();
	editButton->Render();
	createButton->Render();

	if (curtainPosition > -SCREEN_HEIGHT)
	{
		curtain->Render(Vector2D(0, curtainPosition), SDL_FLIP_NONE);
	}
}

void Titlescreen::Update(float deltaTime, SDL_Event e)
{
	if (curtainPosition > -SCREEN_HEIGHT)
	{
		curtainPosition -= deltaTime / curtainDrawTime * SCREEN_HEIGHT;
	}

	if (!buttonClicked)
		playButton->Update(deltaTime, e);
	if (!buttonClicked)
		multiplayerButton->Update(deltaTime, e);
	if (!buttonClicked)
		editButton->Update(deltaTime, e);
	if (!buttonClicked)
		createButton->Update(deltaTime, e);

	buttonClicked = false;
}

void Titlescreen::ButtonClicked(ButtonUI* button, bool leftClick)
{
	audioPlayer->PlayClip("ButtonClicked");
	buttonClicked = true;

	if (button == playButton)
	{
		GameScreen::screenManager->multiplayer = false;
		std::string targetLevel = GetFileInput();

		if (targetLevel != "")
			GameScreen::screenManager->ChangeScreen(SCREEN_LEVEL, targetLevel);
	}
	else if (button == multiplayerButton)
	{
		GameScreen::screenManager->multiplayer = true;
		std::string targetLevel = GetFileInput();

		if (targetLevel != "")
			GameScreen::screenManager->ChangeScreen(SCREEN_LEVEL, targetLevel);
	}
	else if (button == editButton)
	{
		std::string targetLevel = GetFileInput();

		if (targetLevel != "")
			GameScreen::screenManager->ChangeScreen(SCREEN_LEVEL_CREATOR, targetLevel);
	}
	else if (button == createButton)
	{
		GameScreen::screenManager->ChangeScreen(SCREEN_LEVEL_CREATOR);
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
