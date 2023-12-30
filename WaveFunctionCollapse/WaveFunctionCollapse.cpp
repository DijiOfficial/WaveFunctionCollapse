//-----------------------------------------------------------------
// Main Game File
// C++ Source - WaveFunctionCollapse.cpp - version v7_02
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "WaveFunctionCollapse.h"																				
#include "Tiles.h"
#include <memory>
#include "EnumClasses.h"
#include <stack>
#include <vld.h>
#include <string>

//-----------------------------------------------------------------
// WaveFunctionCollapse methods																				
//-----------------------------------------------------------------

WaveFunctionCollapse::WaveFunctionCollapse() 																	
{
	// nothing to create
}

WaveFunctionCollapse::~WaveFunctionCollapse()																						
{
	// nothing to destroy
}

void WaveFunctionCollapse::Initialize(HINSTANCE hInstance)			
{
	// Set the required values
	AbstractGame::Initialize(hInstance);
	GAME_ENGINE->SetTitle(_T("Wave Function Collapse"));					
	GAME_ENGINE->RunGameLoop(true);		
	
	// Set the optional values
	GAME_ENGINE->SetWidth(m_WorldWidth * TILE_SIZE);
	GAME_ENGINE->SetHeight(m_WorldHeight * TILE_SIZE);
    GAME_ENGINE->SetFrameRate(144);
	
	// Load the bitmap
	m_BmpTileTexturePtr = new Bitmap(_T("Assets/Tiles4.bmp"));
	//m_BmpTileTexturePtr->SetTransparencyColor(RGB(255, 0, 255));

	// Set the keys that the game needs to listen to
	//tstringstream buffer;
	//buffer << _T("KLMO");
	//buffer << (TCHAR) VK_LEFT;
	//buffer << (TCHAR) VK_RIGHT;
	//GAME_ENGINE->SetKeyList(buffer.str());
}

void WaveFunctionCollapse::Start(bool reset)
{
	for (int i = 0; i < m_WorldWidth * m_WorldHeight; ++i)
		m_TilesPtrVec.push_back(new Tiles());

	// Assign to every tile its neighbors (not necessary but it makes the code clearer in the algorithm)
	for (int it = 0; it < m_TilesPtrVec.size(); ++it)
	{
		const int posY = static_cast<int>(it / m_WorldWidth);
		const int posX = it % m_WorldWidth;

		if (posY > 0)
			m_TilesPtrVec[it]->AddNeighbor(Direction::NORTH, m_TilesPtrVec[it - m_WorldWidth]);
		if (posX < m_WorldWidth - 1)
			m_TilesPtrVec[it]->AddNeighbor(Direction::EAST, m_TilesPtrVec[it + 1]);
		if (posY < m_WorldHeight - 1)
			m_TilesPtrVec[it]->AddNeighbor(Direction::SOUTH, m_TilesPtrVec[it + m_WorldWidth]);
		if (posX > 0)
			m_TilesPtrVec[it]->AddNeighbor(Direction::WEST, m_TilesPtrVec[it - 1]);
	}

	if (not reset)
		CreateUI();
}

void WaveFunctionCollapse::End()
{
	delete m_BmpTileTexturePtr;

	for (auto& tile : m_TilesPtrVec)
	{
		delete tile;
	}
	m_TilesPtrVec.clear();

	delete m_TxtInputPtr;
	delete m_BtnAllowTileClickPtr;
	delete m_BtnClickPtr;
	delete m_BtnResetPtr;
	delete m_BtnCompletePtr;
	delete m_TxtViewWidthPtr;
	delete m_TxtViewHeightPtr;
}

void WaveFunctionCollapse::Paint(RECT rect)
{
	const int lowestEntropy = GetLowestEntropy();
	for (int it = 0; it < m_TilesPtrVec.size(); ++it)
	{
		//std::shared_ptr<Tiles> currentTile = std::make_shared<Tiles>(m_TilesPtrVec[it]); //cant make it work
		const int posX = it % m_WorldWidth * TILE_SIZE;
		const int posY = static_cast<int>(it / m_WorldWidth) * TILE_SIZE;

		const LONG srcX = static_cast<LONG>(m_TilesPtrVec[it]->GetTileName());
		const RECT srcRect = { srcX * TILE_SIZE, 0, (srcX + 1) * TILE_SIZE, TILE_SIZE };

		const int entropy = m_TilesPtrVec[it]->GetEntropy();
		const TileName tileName = m_TilesPtrVec[it]->GetTileName();

		// If tile is not chosen yet, paint it black and give it a number representing its entropy
		if (entropy > 0)
		{
			GAME_ENGINE->SetColor(RGB(0,0,0));
			GAME_ENGINE->FillRect(posX, posY, TILE_SIZE, TILE_SIZE);
			
			if (entropy >= static_cast<int>(TileRuleSet.size()) / 3)
				GAME_ENGINE->SetColor(RGB(128, 128, 128));
			else
			{
				if (entropy == lowestEntropy)
					GAME_ENGINE->SetColor(RGB(0, 255, 0));
				else
					GAME_ENGINE->SetColor(RGB(255, 255, 255));
			}

			GAME_ENGINE->DrawString(std::to_wstring(entropy), posX, posY, TILE_SIZE, TILE_SIZE);
		}
		else // If tile is chosen, paint it with its texture
		{
			// If it's a forest tile, paint a grass tile underneath it
			if (static_cast<int>(tileName) >= static_cast<int>(TileName::TILE_FORESTN))
				GAME_ENGINE->DrawBitmap(m_BmpTileTexturePtr, posX, posY, { 0, 0, TILE_SIZE, TILE_SIZE });
	
			GAME_ENGINE->DrawBitmap(m_BmpTileTexturePtr, posX, posY, srcRect);
		}
	}

	std::wstring buffer{ L"Toggle Generate On Click" };
	const RECT stringRect = m_BtnAllowTileClickPtr->GetRect();
	GAME_ENGINE->SetColor(RGB(255,255,255));
	GAME_ENGINE->DrawString(buffer, stringRect.left - 180, stringRect.bottom - m_UI.buttonHeight / 1.5f, 300, 300);
}

void WaveFunctionCollapse::Tick()
{
	if (m_WFCIsRunning and not m_WFCIsOver)
		m_WFCIsOver = CollapseFunctionAlgorithm();
}

void WaveFunctionCollapse::MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam)
{	
	if (isLeft and not isDown and m_AllowTileClick)
	{
		CollapseFunctionAlgorithm(x, y, true);
	}
}
void WaveFunctionCollapse::MouseWheelAction(int x, int y, int distance, WPARAM wParam)
{	
	// Insert the code that needs to be executed when the game registers a mouse wheel action
}
void WaveFunctionCollapse::MouseMove(int x, int y, WPARAM wParam)
{	
	// Insert the code that needs to be executed when the mouse pointer moves across the game window

	/* Example:
	if ( x > 261 && x < 261 + 117 ) // check if mouse position is within x coordinates of choice
	{
		if ( y > 182 && y < 182 + 33 ) // check if mouse position also is within y coordinates of choice
		{
			GAME_ENGINE->MessageBox(_T("Da mouse wuz here."));
		}
	}
	*/
}
void WaveFunctionCollapse::CheckKeyboard()
{	
	// Here you can check if a key of choice is held down
	// Is executed once per frame if the Game Loop is running 

	/* Example:
	if (GAME_ENGINE->IsKeyDown(_T('K'))) xIcon -= xSpeed;
	if (GAME_ENGINE->IsKeyDown(_T('L'))) yIcon += xSpeed;
	if (GAME_ENGINE->IsKeyDown(_T('M'))) xIcon += xSpeed;
	if (GAME_ENGINE->IsKeyDown(_T('O'))) yIcon -= ySpeed;
	*/
}
void WaveFunctionCollapse::KeyPressed(TCHAR cKey)
{	
	// DO NOT FORGET to use SetKeyList() !!

	// Insert the code that needs to be executed when a key of choice is pressed
	// Is executed as soon as the key is released
	// You first need to specify the keys that the game engine needs to watch by using the SetKeyList() method

	/* Example:
	switch (cKey)
	{
	case _T('K'): case VK_LEFT:
		GAME_ENGINE->MessageBox(_T("Moving left."));
		break;
	case _T('L'): case VK_DOWN:
		GAME_ENGINE->MessageBox(_T("Moving down."));
		break;
	case _T('M'): case VK_RIGHT:
		GAME_ENGINE->MessageBox(_T("Moving right."));
		break;
	case _T('O'): case VK_UP:
		GAME_ENGINE->MessageBox(_T("Moving up."));
		break;
	case VK_ESCAPE:
		GAME_ENGINE->MessageBox(_T("Escape menu."));
	}
	*/
}
void WaveFunctionCollapse::CallAction(Caller* callerPtr)
{
	if (callerPtr == m_BtnClickPtr)
	{
		if (m_WFCIsRunning)
			m_BtnClickPtr->SetText(_T("Start"));
		else
			m_BtnClickPtr->SetText(_T("Stop"));

		if (not m_WFCIsOver)
			m_WFCIsRunning = !m_WFCIsRunning;
	}
	else if (callerPtr == m_BtnAllowTileClickPtr)
	{
		m_AllowTileClick = !m_AllowTileClick;
		if (m_AllowTileClick)
			m_BtnAllowTileClickPtr->SetText(_T(""));
		else
			m_BtnAllowTileClickPtr->SetText(_T("X"));
	}
	else if (callerPtr == m_BtnCompletePtr)
	{
		while (not m_WFCIsOver)
			m_WFCIsOver = CollapseFunctionAlgorithm();
	}
	else if (callerPtr == m_BtnResetPtr)
		Reset();
}

std::vector<Tiles*> WaveFunctionCollapse::GetLowestEntropyTiles()
{
	//Set lowest entropy to the highest possible value and create an empty list of tiles
	int tilesLowestEntropy = TileRuleSet.size();
	std::vector<Tiles*> tileList;

	//Loop through all tiles
	for (const auto& Tile : m_TilesPtrVec)
	{
		const int tileEntropy = Tile->GetEntropy();
		
		//If the tile has no entropy, skip it because it is already collapsed
		if (tileEntropy <= 0)
			continue;

		//Reset the list of tiles if a tile with a lower entropy is found
		if (tileEntropy < tilesLowestEntropy)
		{
			tileList.clear();
			tilesLowestEntropy = tileEntropy;
		}
		
		//Add the tile to the list of tiles with the lowest entropy
		if (tileEntropy == tilesLowestEntropy)
			tileList.push_back(Tile);
	}

	return tileList;
}

int WaveFunctionCollapse::GetLowestEntropy() const
{
	int lowestEntropy = TileRuleSet.size();
	for (const auto& tile : m_TilesPtrVec)
	{
		const int tileEntropy = tile->GetEntropy();
		if (tileEntropy > 0 and tileEntropy < lowestEntropy)
			lowestEntropy = tileEntropy;
	}

	return lowestEntropy;
}

void WaveFunctionCollapse::CreateUI()
{
	m_UI.SetViewWidth(GAME_ENGINE->GetWidth());
	m_UI.SetViewHeight(GAME_ENGINE->GetHeight());

	m_TxtViewWidthPtr = new TextBox(std::to_wstring(m_WorldWidth));
	m_TxtViewWidthPtr->SetBounds(m_UI.alignRight + m_UI.padding, m_UI.alignTop - 4 * m_UI.translateY, m_UI.buttonHeight, m_UI.buttonHeight);
	m_TxtViewWidthPtr->AddActionListener(this);
	m_TxtViewWidthPtr->SetFont(_T("Sans Serif"), true, false, false, 20);
	m_TxtViewWidthPtr->Show();

	m_TxtViewHeightPtr = new TextBox(std::to_wstring(m_WorldHeight));
	m_TxtViewHeightPtr->SetBounds(m_UI.alignRight + m_UI.buttonWidth - m_UI.buttonHeight - m_UI.padding, m_UI.alignTop - 4 * m_UI.translateY, m_UI.buttonHeight, m_UI.buttonHeight);
	m_TxtViewHeightPtr->AddActionListener(this);
	m_TxtViewHeightPtr->SetFont(_T("Sans Serif"), true, false, false, 20);
	m_TxtViewHeightPtr->Show();

	m_BtnClickPtr = new Button(_T("Start"));
	m_BtnClickPtr->SetBounds(m_UI.alignRight, m_UI.alignTop, m_UI.buttonWidth, m_UI.buttonHeight);
	m_BtnClickPtr->AddActionListener(this);
	m_BtnClickPtr->SetFont(_T("Sans Serif"), true, false, false, 30);
	m_BtnClickPtr->Show();

	m_BtnAllowTileClickPtr = new Button(_T("X"));
	m_BtnAllowTileClickPtr->SetBounds(m_UI.alignRight + m_UI.buttonWidth - m_UI.buttonHeight, m_UI.alignTop - m_UI.translateY, m_UI.buttonHeight, m_UI.buttonHeight);
	m_BtnAllowTileClickPtr->AddActionListener(this);
	m_BtnAllowTileClickPtr->SetFont(_T("Sans Serif"), true, false, false, 30);
	m_BtnAllowTileClickPtr->Show(); 

	m_BtnCompletePtr = new Button(_T("Complete"));
	m_BtnCompletePtr->SetBounds(m_UI.alignRight, m_UI.alignTop - 2 * m_UI.translateY, m_UI.buttonWidth, m_UI.buttonHeight);
	m_BtnCompletePtr->AddActionListener(this);
	m_BtnCompletePtr->SetFont(_T("Sans Serif"), true, false, false, 30);
	m_BtnCompletePtr->Show();

	m_BtnResetPtr = new Button(_T("Reset"));
	m_BtnResetPtr->SetBounds(m_UI.alignRight, m_UI.alignTop - 3 * m_UI.translateY, m_UI.buttonWidth, m_UI.buttonHeight);
	m_BtnResetPtr->AddActionListener(this);
	m_BtnResetPtr->SetFont(_T("Sans Serif"), true, false, false, 30);
	m_BtnResetPtr->Show();
}

void WaveFunctionCollapse::Reset()
{
	m_WFCIsOver = { false };
	m_WFCIsRunning = { false };
	m_AllowTileClick = { false };

	m_BtnClickPtr->SetText(_T("Start"));
	m_BtnAllowTileClickPtr->SetText(_T("X"));

	for (auto& tile : m_TilesPtrVec)
	{
		delete tile;
	}
	m_TilesPtrVec.clear();

	Start(true);
}

bool WaveFunctionCollapse::CollapseFunctionAlgorithm(int x, int y, bool isClicked)
{
	const auto lowestEntropyTilesList = GetLowestEntropyTiles();
	if (lowestEntropyTilesList.empty())
		return true;

	int index = (y / TILE_SIZE) * m_WorldWidth + (x / TILE_SIZE);

	// Choose a random available tile to collapse and push it to the stack
	Tiles* tileToCollapse = nullptr;
	if (isClicked)
		tileToCollapse = m_TilesPtrVec[index];
	else
		tileToCollapse = lowestEntropyTilesList[rand() % static_cast<int>(lowestEntropyTilesList.size())];

	tileToCollapse->Collapse();

	std::stack<Tiles*> stack;
	stack.push(tileToCollapse);

	// Propagate the constraints to the neighbors (remove all of the neighbor's unavailable tiles based on the previous neighbor)
	while (not stack.empty())
	{
		Tiles* currentTile = stack.top();
		stack.pop();

		// Get the available tiles and directions of the current tile
		std::vector<TileName> AvailableTilesForNeighborVec{ currentTile->GetAvailableTiles() };
		std::vector<Direction> directionsVec{ currentTile->GetDirections() };
	
		// Loop through all the nieghbors of the current tile
		for (const auto& direction : directionsVec)
		{
			Tiles* neighbor = currentTile->GetNeighbor(direction);
			if (neighbor->GetEntropy() == 0)
				continue;

			// If the neighbor was constrained*, push it to the stack. (If the neighbor had some tiles removed from his available tiles choices)
			// then add that neighbor to the stack so his neighbors can be checked
			// *see Constrain() implementation for more details
			if (neighbor->Constrain(AvailableTilesForNeighborVec, direction))
				stack.push(neighbor);
		}
	}

	return false;
}



