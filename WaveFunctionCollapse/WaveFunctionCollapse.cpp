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
	GAME_ENGINE->SetWidth(WORLD_WIDTH * TILE_SIZE);
	GAME_ENGINE->SetHeight(WORLD_HEIGHT * TILE_SIZE);
    GAME_ENGINE->SetFrameRate(50);
	
	// Load the bitmap
	try
	{
		m_BmpTileTexturePtr = new Bitmap(_T("Assets/Tiles.bmp"));
	}
	catch (const BitmapNotLoadedException& ex)
	{
		GAME_ENGINE->MessageBox(_T("Could not load Background BitMap"));
		GAME_ENGINE->Quit();
	}

	for (int i = 0; i < WORLD_WIDTH * WORLD_HEIGHT; ++i)
		m_TilesPtrVec.push_back(new Tiles());

	// Set the keys that the game needs to listen to
	//tstringstream buffer;
	//buffer << _T("KLMO");
	//buffer << (TCHAR) VK_LEFT;
	//buffer << (TCHAR) VK_RIGHT;
	//GAME_ENGINE->SetKeyList(buffer.str());
}

void WaveFunctionCollapse::Start()
{
	for (int it = 0; it < m_TilesPtrVec.size(); ++it)
	{
		const int posY = static_cast<int>(it / WORLD_WIDTH) * TILE_SIZE;
		const int posX = it % WORLD_WIDTH * TILE_SIZE;

		if (posY > 0)
			m_TilesPtrVec[it]->AddNeighbor(Direction::NORTH, m_TilesPtrVec[it - WORLD_WIDTH]);
		if (posX < WORLD_WIDTH - 1)
			m_TilesPtrVec[it]->AddNeighbor(Direction::EAST, m_TilesPtrVec[it + 1]);
		if (posY < WORLD_HEIGHT - 1)
			m_TilesPtrVec[it]->AddNeighbor(Direction::SOUTH, m_TilesPtrVec[it + WORLD_WIDTH]);
		if (posX > 0)
			m_TilesPtrVec[it]->AddNeighbor(Direction::WEST, m_TilesPtrVec[it - 1]);

	}
	//CollapseFunctionAlgorithm();
}

void WaveFunctionCollapse::End()
{
	delete m_BmpTileTexturePtr;

	for (auto& tile : m_TilesPtrVec)
	{
		delete tile;
	}
	m_TilesPtrVec.clear();
}

void WaveFunctionCollapse::Paint(RECT rect)
{
	for (int it = 0; it < m_TilesPtrVec.size(); ++it)
	{
		//std::shared_ptr<Tiles> currentTile = std::make_shared<Tiles>(m_TilesPtrVec[it]); //cant make it work
		const int posX = it % WORLD_WIDTH * TILE_SIZE;
		const int posY = static_cast<int>(it / WORLD_WIDTH) * TILE_SIZE;

		const LONG srcX = static_cast<LONG>(m_TilesPtrVec[it]->GetTileName());
		const RECT srcRect = { srcX * TILE_SIZE, 0, (srcX + 1) * TILE_SIZE, TILE_SIZE };

		GAME_ENGINE->DrawBitmap(m_BmpTileTexturePtr, posX, posY, srcRect);
	}
}

void WaveFunctionCollapse::Tick()
{
	CollapseFunctionAlgorithm();
	// Insert non-paint code that needs to be executed each tick 
}

void WaveFunctionCollapse::MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam)
{	
	// Insert the code that needs to be executed when the game registers a mouse button action

	/* Example:
	if (isLeft == true && isDown == true) // is it a left mouse click?
	{	
		if ( x > 261 && x < 261 + 117 ) // check if click lies within x coordinates of choice
		{
			if ( y > 182 && y < 182 + 33 ) // check if click also lies within y coordinates of choice
			{
				GAME_ENGINE->MessageBox(_T("Clicked."));
			}
		}
	}
	*/
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
	// Insert the code that needs to be executed when a Caller has to perform an action
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

bool WaveFunctionCollapse::CollapseFunctionAlgorithm()
{
	const auto lowestEntropyTilesList = GetLowestEntropyTiles();
	if (lowestEntropyTilesList.empty())
		return true;

	Tiles* tileToCollapse = lowestEntropyTilesList[0];
	tileToCollapse->Collapse();

	std::stack<Tiles*> stack;
	stack.push(tileToCollapse);

	while (not stack.empty())
	{
		Tiles* currentTile = stack.top();
		stack.pop();

		std::vector<TileName> NeighborsAvailableTilesVec{ currentTile->GetAvailableTiles() };
		std::vector<Direction> directionsVec{ currentTile->GetDirections() };
	
		for (const auto& direction : directionsVec)
		{
			Tiles* neighbor = currentTile->GetNeighbor(direction);
			if (neighbor->GetEntropy() == 0)
				continue;

			if (neighbor->Constrain(NeighborsAvailableTilesVec, direction))
				stack.push(neighbor);
		}
	}

	return false;
}



