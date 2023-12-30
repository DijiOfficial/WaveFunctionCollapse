//---------------------------
// Includes
//---------------------------
#include "Slider.h"

//---------------------------
// Defines
//---------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())

//---------------------------
// Constructor & Destructor
//---------------------------
Slider::Slider(int x, int y, int fps) 
	: m_X(x)
	, m_Y(y)
	, m_XButton(0)
	, m_FPS(fps)
{
	m_XButton = m_X + SLIDERWIDTH * m_FPS / MAXFPS;
}

//---------------------------
// Public methods
//---------------------------


void Slider::Paint()
{
	GAME_ENGINE->SetColor(RGB(255, 255, 255));
	GAME_ENGINE->FillRect(m_X, m_Y, SLIDERWIDTH, 5);
	GAME_ENGINE->SetColor(RGB(128, 128, 128));
	GAME_ENGINE->FillRect(m_XButton, m_Y - HEIGHT / 2.f + 3, WIDTH, HEIGHT);
}

void Slider::Update(int x, int y)
{
	int yButton = m_Y - HEIGHT / 2.f + 3;
	//if (x > m_XButton && x < m_XButton + WIDTH && y > yButton && y < yButton + HEIGHT)
	if (x > m_X && x < m_X + SLIDERWIDTH && y > yButton && y < yButton + HEIGHT)
	{
		m_FPS = static_cast<int>((x - m_X) * MAXFPS / SLIDERWIDTH);
		//int middleButton = m_XButton + WIDTH / 2;
		//m_FPS = (SLIDERWIDTH - middleButton) * MAXFPS / SLIDERWIDTH;
		m_FPS = min(max(m_FPS, 1), MAXFPS);

		m_XButton = m_X + SLIDERWIDTH * m_FPS / MAXFPS;
	}
	//if (x > m_XButton && x < m_XButton + WIDTH && y > yButton && y < yButton + HEIGHT and not m_Clicked)
	//{
	//	OutputDebugString(L"Clicked\n");
	//	m_Clicked = true;
	//	m_XClick = x;
	//}
	//else if (m_Clicked)
	//{
	//	int deltaX = x - m_XClick;
	//	int middleButton = m_XButton + WIDTH / 2;

	//	//m_FPS = (SLIDERWIDTH - middleButton) * MAXFPS / SLIDERWIDTH;
	//	m_FPS = static_cast<int>((x - m_X) / static_cast<double>(SLIDERWIDTH) * MAXFPS);
	//	m_FPS = min(max(m_FPS, MAXFPS), 0);

	//	m_XButton = m_X + SLIDERWIDTH - SLIDERWIDTH * m_FPS / MAXFPS;
	//}
}

void Slider::SetBounds(int x, int y)
{
	m_X = x;
	m_Y = y;
	m_XButton = m_X + SLIDERWIDTH * m_FPS / MAXFPS;
}
