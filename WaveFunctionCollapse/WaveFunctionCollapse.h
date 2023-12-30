//-----------------------------------------------------------------
// Main Game  File
// C++ Header - WaveFunctionCollapse.h - version v7_02			
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------

#include "Resource.h"	
#include "GameEngine.h"
#include "AbstractGame.h"

class Tiles;
struct UI
{
	const int padding{ 10 };
	int viewWidth{ 0 };
	int viewHeight{ 0 };
	
	const int buttonWidth{ 100 };
	const int buttonHeight{ 25 };

	int alignRight{ 0 };
	int alignBottom{ 0 };
	const int alignLeft{ padding };
	const int alignTop{ padding };
	const int translateY{ -2 * padding - buttonHeight };

public:
	void SetViewWidth(int width) { viewWidth = width; alignRight = { viewWidth - 2 * padding - buttonWidth }; };
	void SetViewHeight(int height) { viewHeight = height; alignBottom = { viewHeight - padding }; };

};
//-----------------------------------------------------------------
// WaveFunctionCollapse Class																
//-----------------------------------------------------------------
class WaveFunctionCollapse : public AbstractGame, public Callable
{
public:
	//---------------------------
	// Constructor(s) and Destructor
	//---------------------------
	WaveFunctionCollapse();

	virtual ~WaveFunctionCollapse() override;

	//---------------------------
	// Disabling copy/move constructors and assignment operators   
	//---------------------------
	WaveFunctionCollapse(const WaveFunctionCollapse& other) = delete;
	WaveFunctionCollapse(WaveFunctionCollapse&& other) noexcept = delete;
	WaveFunctionCollapse& operator=(const WaveFunctionCollapse& other) = delete;
	WaveFunctionCollapse& operator=(WaveFunctionCollapse&& other) noexcept = delete;

	//---------------------------
	// General Methods
	//---------------------------
	void Initialize(HINSTANCE hInstance) override;
	void Start() override { Start(false); };
	void Start(bool reset);
	void End() override;
	void Paint(RECT rect) override;
	void Tick() override;
	void MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam) override;
	void MouseWheelAction(int x, int y, int distance, WPARAM wParam) override;
	void MouseMove(int x, int y, WPARAM wParam) override;
	void CheckKeyboard() override;
	void KeyPressed(TCHAR cKey) override;

	void CallAction(Caller* callerPtr) override;
	bool CollapseFunctionAlgorithm(int x = -1, int y = -1, bool isClicked = false);
	std::vector<Tiles*> GetLowestEntropyTiles();
	int GetLowestEntropy() const;

private:
	// -------------------------
	// Datamembers
	// -------------------------
	int m_WorldWidth = 60;
	int m_WorldHeight = 34;
	constexpr static int TILE_SIZE = 16;

	std::vector<Tiles*> m_TilesPtrVec{};
	Bitmap* m_BmpTileTexturePtr{};

	bool m_WFCIsOver{ false };
	bool m_WFCIsRunning{ false };
	bool m_AllowTileClick{ false };

	//Resize button
	//dropdown menu to edit weights
	//speed slider?
	//dropdown menu to select tileset you want to place or after click spawn option with dropdown menu?
	TextBox* m_TxtInputPtr{};
	Button* m_BtnClickPtr{};
	Button* m_BtnAllowTileClickPtr{};
	Button* m_BtnCompletePtr{};
	Button* m_BtnResetPtr{};
	TextBox* m_TxtViewWidthPtr{};
	TextBox* m_TxtViewHeightPtr{};

	void CreateUI();
	void Reset();
	UI m_UI{};
};
