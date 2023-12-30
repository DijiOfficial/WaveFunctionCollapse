#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GameEngine.h"

//-----------------------------------------------------
// Slider Class									
//-----------------------------------------------------
class Slider final
{
public:
	Slider(int x, int y, int fps);				// Constructor
	~Slider() = default;		// Nothing to destroy
		
	// Disabling copy/move constructors and assignment operators   
	Slider(const Slider& other)					= delete;
	Slider(Slider&& other) noexcept				= delete;
	Slider& operator=(const Slider& other)		= delete;
	Slider& operator=(Slider&& other) noexcept	= delete;

	//-------------------------------------------------
	// Own methods								
	//-------------------------------------------------
	void Paint();
	//void ChangeFPS(int delta);
	void Update(int x, int y);
	//void MouseMove(int x, int y);
	//void UpdateButton();
	void SetBounds(int x, int y);
	void SetClickedFalse() { m_Clicked = false; };
	int GetFPS() { return m_FPS; };
	//-------------------------------------------------
	// Constants								
	//-------------------------------------------------
	static const int WIDTH			= 10;
	static const int HEIGHT			= 25;
	static const int SLIDERWIDTH	= 100;
	static const int BUTTON_WIDTH	= 25;

	static const int MAXFPS			= 60;

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------

	int m_X{}, m_Y{}, m_XButton{};
	bool m_Clicked{};
	int m_XClick{};
	
	int m_FPS{};
};

 
