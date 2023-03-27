#include "ButtonUI.h"
#include "Texture2D.h"

ButtonUI::ButtonUI(SDL_Renderer* renderer, GameScreen* attatchedScreen, Vector2D startPosition, std::string imagePath, float scale)
{
	m_renderer = renderer;

	// Load texture
	m_texture = new Texture2D(m_renderer, scale);

	if (!m_texture->LoadFromFile(imagePath))
	{
		std::cout << "Failed to load UI texture! " << imagePath << " not found." << std::endl;
	}

	position = startPosition;

	m_attatchedScreen = attatchedScreen;
}

ButtonUI::~ButtonUI()
{
	m_renderer = nullptr;
	m_attatchedScreen = nullptr;

	delete m_texture;
	m_texture = nullptr;
}

void ButtonUI::Update(float deltaTime, SDL_Event e)
{
	if (e.key.repeat == 0)
	{
		switch (e.type)
		{

		case SDL_MOUSEBUTTONDOWN:
			switch (e.button.button)
			{
				case SDL_BUTTON_LEFT:
					if (ClickInBounds(Vector2D((float)e.button.x, (float)e.button.y)))
					{
						m_attatchedScreen->ButtonClicked(this, true);
					}
					break;
				case SDL_BUTTON_RIGHT:
					if (ClickInBounds(Vector2D((float)e.button.x, (float)e.button.y)))
					{
						m_attatchedScreen->ButtonClicked(this, false);
					}
					break;
			}
			break;
		}
	}
}

void ButtonUI::Render()
{
	m_texture->Render(position, SDL_FLIP_NONE);
}

bool ButtonUI::ClickInBounds(Vector2D clickPos)
{
	if (clickPos.x > position.x && clickPos.y > position.y && clickPos.x < position.x + m_texture->GetWidth() && clickPos.y < position.y + m_texture->GetHeight())
	{
		return true;
	}

	return false;
}