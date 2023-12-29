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
	void Start() override;
	void End() override;
	void Paint(RECT rect) override;
	void Tick() override;
	void MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam) override;
	void MouseWheelAction(int x, int y, int distance, WPARAM wParam) override;
	void MouseMove(int x, int y, WPARAM wParam) override;
	void CheckKeyboard() override;
	void KeyPressed(TCHAR cKey) override;
	
	void CallAction(Caller* callerPtr) override;
	bool CollapseFunctionAlgorithm();
	std::vector<Tiles*> GetLowestEntropyTiles();
	int GetLowestEntropy() const;

private:
	// -------------------------
	// Datamembers
	// -------------------------
	constexpr static int WORLD_WIDTH = 60;
	constexpr static int WORLD_HEIGHT = 34;
	constexpr static int TILE_SIZE = 16;

	std::vector<Tiles*> m_TilesPtrVec{};
	Bitmap* m_BmpTileTexturePtr{};

	bool m_WFCIsOver{ false };
	bool m_WFCIsRunning{ true };
};
