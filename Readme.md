# The Wave Function Collapse

### The Project

#### Project 1
The First part of this project is a 2D Dungeon/Texture/Terrain Generator. It has a variety of [Uses and Implementations](#uses-and-implementations).
However its main usage is Terrain Generation.
(May implement UI later).

The project was made in a C++ Win32 engine developed by a teacher at DAE of the university of Howest. This project was based on the [Python](https://github.com/CodingQuest2023/Algorithms) version of this function also presented in [this video](https://www.youtube.com/watch?v=qRtrj6Pua2A&t=51s) by [CodingQuest](https://www.youtube.com/@CodingQuest2023). This is also where the original sprites I used come from, although I have a different stylesheet that simplifies the code significantly when drawing with it.

The variety of the [Algorithm](#Algorithm) I used for this first project consists of defining all the available enum [Tiles](https://github.com/Howest-DAE-GD/gpp-researchtopic-DijiOfficial/blob/master/WaveFunctionCollapse/Assets/Tiles2.bmp) I can use. Then defining all the ```enum TileTypes``` that that will be used to connect tiles together. There can be multiple tiles per type eg: ```FOREST_N```, ```FOREST_NE``` and ```FOREST_NW``` tiles all fall under the ```FOREST_N``` ```TileType```. We also define ```enum Direction``` to link every tile to its neighbor(s) given a direction(s). It is not necessary but it simplfies the math for the algorithm when we need to loop through the neighbors. 
Finally we make a ```struct TileInfo``` containing a ```vector``` of ```TileType``` and ```int TileWeight``` that we will use in a ```std::map<TileName, TileInfo> TileRuleSet``` to define which ```TileType``` can connect to each ```enum Tile``` and give them weights to influence their pick rate. This structure can be viewed in the [EnumClasses.h](https://github.com/Howest-DAE-GD/gpp-researchtopic-DijiOfficial/blob/master/WaveFunctionCollapse/EnumClasses.h) file. It is the main disadvantage of this variety of the the [Algorithm](#Algorithm), it's straight forward as everything is predifined so it's not complicated to understand and visualize. However the complexity of the system can grow exponentially as we add more and more tile options, as well as taking longer to initialize than other methods.

#### Project 2
(not yet implemented) This version, also a 2D generator, will solve the issue of the complex initial setup of the map and enums by sampling everything from a bitmap, trading initialization complexity for general complexity.

#### Project 3
(not yet implemented, may not be implemented at all) This version is for generating 3D terrain.

### Motivations
I have always been passionate about Algorithms, Math, Physics and Video Games and how they intersect each other. And the passion for this project started when I first saw [this video](https://www.youtube.com/watch?v=2SuvO4Gi7uY&t=19s) by [Martin Donald](https://www.youtube.com/@MartinDonald) back in 2020. I found the explanation absolutely fantastic and it scratched just the right itch by combining Algorithms, Games, Infinite Generation, Exploration and Possibilites as well as the potential for infinite replayability. One of my very first "complete" project [MazeRunner](https://github.com/DijiOfficial/2D-Grid-Game) also features an infinite amount of levels by generating new ones every time. So I thought I would explore what [The Wave Function Collapse](#the-wave-function-collapse) had to offer and it did not disappoint as well as being slighlty harder to implement than I orginally thought it would be making it all the more fun and interstiing.

### Final result
![quick gif of the project in effect](https://github.com/Howest-DAE-GD/gpp-researchtopic-DijiOfficial/blob/master/WaveFunctionCollapse/Assets/Project1Demo.gif)

## Algorithm
In the following paragraphs the terms "tile" and "module" are interchangeable. A 'tile' refers to a 2D image with dimensions Width x Height, while a 'module' refers to a 3D box with dimensions Width x Height x Depth containing a 3D asset. It's important to note that they don't necessarily need to be square or cubic, as long as the rule set prohibits overlapping tiles or modules. For the 2D version of this algorithm, you can decompose a Width x Height tile into multiple smaller square tiles, completing the rule set with these subset tiles. To avoid confusion, I will refer to Tile objects as 'Tobj,' distinguishing them from individual tiles. Tobj contains various values, including a list of available tiles it can choose from.

1. [Initialize the rule set](#1-initialize-the-rule-set)
2. [Find lowest Entropy tile object](#2-find-lowest-entropy-tile-object)
3. [Tile Collapse](#3-tile-collapse)
4. [Constriction](#4-constriction)

### 1. Initialize the rule set

To get started with this algorithm, you will need to do an initial setup based on the specific variation you are working on. For [Project1](#project-1), begin by defining all the tiles you will be using and establish the rule set that will dictate how the tiles connect to each other — in other words, specify which tile can connect to which. Create an array of Tile objects (Tobj) containing a list of available tiles and an entropy value. By default, the entropy will be set to the total number of tiles. Entropy represents the number of available tiles left to choose from after the constriction process (more on that later). Additionally, provide each Tobj with links to its neighbors, which will simplify the code later. I assigned a set of directions (N-E-S-W) to each Tobj and later defined how each direction identifies its neighbors in the array.

Here's the rule set for [Project1](#project-1) an extract from the [EnumClasses.h](https://github.com/Howest-DAE-GD/gpp-researchtopic-DijiOfficial/blob/master/WaveFunctionCollapse/EnumClasses.h), containing for every tile a set of possible type connections for every direction (neighbor) and weights.
```cpp
using TileNameToRulesMap = const std::map<TileName, TileInfo>;
inline TileNameToRulesMap TileRuleSet
{ 
	{ TileName::TILE_GRASS,		{{ GRASS, GRASS, GRASS, GRASS }		, 16	}}, 
	{ TileName::TILE_WATER,		{{ WATER, WATER, WATER, WATER }		, 4	}}, 
	{ TileName::TILE_FOREST,	{{ FOREST, FOREST, FOREST, FOREST }	, 5	}}, 
	{ TileName::TILE_COASTN,	{{ GRASS, COAST_N, WATER, COAST_N }	, 5	}}, 
	{ TileName::TILE_COASTE,	{{ COAST_E, GRASS, COAST_E, WATER }	, 5	}}, 
	{ TileName::TILE_COASTS,	{{ WATER, COAST_S, GRASS, COAST_S }	, 5	}}, 
	{ TileName::TILE_COASTW,	{{ COAST_W, WATER, COAST_W, GRASS }	, 5	}}, 
	{ TileName::TILE_COASTNE,	{{ GRASS, GRASS, COAST_E, COAST_N }	, 5	}}, 
	{ TileName::TILE_COASTSE,	{{ COAST_E, GRASS, GRASS, COAST_S }	, 5	}}, 
	{ TileName::TILE_COASTSW,	{{ COAST_W, COAST_S, GRASS, GRASS }	, 5	}}, 
	{ TileName::TILE_COASTNW,	{{ GRASS, COAST_N, COAST_W, GRASS }	, 5	}}, 
	{ TileName::TILE_COASTNE2,	{{ COAST_E, COAST_N, WATER, WATER }	, 2	}}, 
	{ TileName::TILE_COASTSE2,	{{ WATER, COAST_S, COAST_E, WATER }	, 2	}}, 
	{ TileName::TILE_COASTSW2,	{{ WATER, WATER, COAST_W, COAST_S }	, 2	}}, 
	{ TileName::TILE_COASTNW2,	{{ COAST_W, WATER, WATER, COAST_N }	, 2	}}, 
	{ TileName::TILE_ROCKN,		{{ ROCK, ROCK_N, GRASS, ROCK_N }	, 4	}}, 
	{ TileName::TILE_ROCKE,		{{ ROCK_E, ROCK, ROCK_E, GRASS }	, 4	}}, 
	{ TileName::TILE_ROCKS,		{{ GRASS, ROCK_S, ROCK, ROCK_S }	, 4	}}, 
	{ TileName::TILE_ROCKW,		{{ ROCK_W, GRASS, ROCK_W, ROCK }	, 4	}}, 
	{ TileName::TILE_ROCKNE,	{{ ROCK_E, ROCK_N, GRASS, GRASS }	, 4	}}, 
	{ TileName::TILE_ROCKSE,	{{ GRASS, ROCK_S, ROCK_E, GRASS }	, 4	}}, 
	{ TileName::TILE_ROCKSW,	{{ GRASS, GRASS, ROCK_W, ROCK_S }	, 4	}}, 
	{ TileName::TILE_ROCKNW,	{{ ROCK_W, GRASS, GRASS, ROCK_N }	, 4	}}, 
	{ TileName::TILE_FORESTN,	{{ FOREST, FOREST_N, GRASS, FOREST_N }	, 4	}}, 
	{ TileName::TILE_FORESTE,	{{ FOREST_E, FOREST, FOREST_E, GRASS }	, 4	}}, 
	{ TileName::TILE_FORESTS,	{{ GRASS, FOREST_S, FOREST, FOREST_S }	, 4	}}, 
	{ TileName::TILE_FORESTW,	{{ FOREST_W, GRASS, FOREST_W, FOREST }	, 4	}}, 
	{ TileName::TILE_FORESTNE,	{{ FOREST_E, FOREST_N, GRASS, GRASS }	, 4	}}, 
	{ TileName::TILE_FORESTSE,	{{ GRASS, FOREST_S, FOREST_E, GRASS }	, 4	}}, 
	{ TileName::TILE_FORESTSW,	{{ GRASS, GRASS, FOREST_W, FOREST_S }	, 4	}}, 
	{ TileName::TILE_FORESTNW,	{{ FOREST_W, GRASS, GRASS, FOREST_N }	, 4	}}, 
	{ TileName::TILE_FORESTNE2,	{{ FOREST, FOREST, FOREST_E, FOREST_N }	, 2	}}, 
	{ TileName::TILE_FORESTSE2,	{{ FOREST_E, FOREST, FOREST, FOREST_S }	, 2	}}, 
	{ TileName::TILE_FORESTSW2,	{{ FOREST_W, FOREST_S, FOREST, FOREST }	, 2	}}, 
	{ TileName::TILE_FORESTNW2,	{{ FOREST, FOREST_N, FOREST_W, FOREST }	, 2	}}
};
```

The Initalization of the array of tiles and assigning neighboring tiles.
```c++
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
}
```

The private member of the tile objects (Tobj).
```c++
class Tiles
{
private:
	std::map<const Direction, Tiles*> m_NeighborsMap;
	std::vector<TileName> m_AvailableTiles{};
	int m_Entropy{ 35 };

	TileName m_TileName;
}
```

### 2. Find lowest Entropy tile object

Title is self explanatory, follow the comments.
```c++
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
```

### 3. Tile Collapse

To collapse a tile choose a random available tile choices left, you can use weights to influence the choice.
Clear the available tiles list of the Tobj and add the chosen tile.
Set the entropy to 0 because there are no more choices available.
```c++
void Tiles::Collapse()
{
    std::vector<int> tileWeightsVec;
    //Loop through all available tiles and get their weights from the TileRuleSet
    for (const auto& tile : m_AvailableTiles)
    {
        const auto it = TileRuleSet.find(tile);
        
        if (it != TileRuleSet.end())
            tileWeightsVec.push_back(it->second.m_TileWeight);
    }
        
    //Make a random choice based on the weights of the tiles by using a discrete distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> distribution(tileWeightsVec.begin(), tileWeightsVec.end());
    const int randomWeightedChoice = distribution(gen);

    //Use the random choice to set the tile
    m_TileName = m_AvailableTiles[randomWeightedChoice];
    m_AvailableTiles.clear(); 
    m_AvailableTiles.push_back(m_TileName);
    m_Entropy = 0;
}
```

### 4. Constriction

The constriction may not be the hardest step but it definitely is the hardest one to understand, in essence it is simply telling all neighboring Tobj of the one that was collapsed which tiles are now unavailable and continuing this processs until there are no more tile to constrict. It goes as follow:

1. set a bool Constriction = false
2. Create a list of all possible connections from available tiles list of the to the neighbor
3. Loop through a copy of the available tiles list of the neighbor
     - Check if the available tile has a connection that is in the list of possible connections (2.)
     - If not then remove the tile from the neighbor's list of available tiles as it's no longer a valid tile
     - Constriction = true
4. Update the entropy
5. return Constriction bool

```c++
// We take all the available tiles of the previous neighbor and add all the possible connections to the current tile
// We then check the opposite direction to get the original tile the current tile
// We now check that for every current tile available it can connect (possible connections) to the original tile (the one that was collapsed/constrained aka the previous neighbor)
// If it can't connect we remove it (tile from possible connections) from the available tiles and return true so we can propagate to (constrain) neighbor tiles
bool Tiles::Constrain(std::vector<TileName>& availableTilesVec, const Direction& direction)
{
    bool wasReduced = false;
 
    if (m_Entropy == 0)
        return false;
    
    // Add all possible connections from previous neighbor to neighbor(current tile) in given direction
    std::vector<TileType> connectorsVec;
    for (const auto& availableTile : availableTilesVec)
    {
        connectorsVec.push_back(TileRuleSet.find(availableTile)->second.m_TileTypesVec[static_cast<int>(direction)]);
    }
    
    // Invert the roles and now loop through a copy of the current tile's available tiles to compare connections from this tile to the previous neighbor
    const Direction oppositeDirection = static_cast<Direction>((static_cast<int>(direction) + 2) % 4);
    const std::vector<TileName> availableTilesVecCopy{ m_AvailableTiles };

    for (const auto& availableTile : availableTilesVecCopy)
    {
        // Find the possible connection from the current tile to the previous neighbor and
        // If it's not a valid connection then remove asciociated tile from the available tiles of current tile
        TileType possibleConnection = TileRuleSet.find(availableTile)->second.m_TileTypesVec[static_cast<int>(oppositeDirection)];
        
        const auto it = std::find(connectorsVec.begin(), connectorsVec.end(), possibleConnection);
        if (it == connectorsVec.end())
        {
            // The item was not found
            m_AvailableTiles.erase(std::remove(m_AvailableTiles.begin(), m_AvailableTiles.end(), availableTile), m_AvailableTiles.end());
            wasReduced = true;
        }
    }

    m_Entropy = static_cast<int>(m_AvailableTiles.size());
    
    return wasReduced;
}
```

### Complete function

1.Get a list of the lowest entropy Tobj (ignore tiles with entropy of value 0)
   - If the list is empty then all tiles have been collapsed and the generation is complete
   - return true
2. Choose a random Tobj from the list and Collapse it 
3. Create a stack and add the collapsed Tobj to the top
4. Loop while the stack is not empty
   - Pop the top Tobj on the stack and store it
   - Get the list of all available tiles from the Tobj *used later for Constriction
   - Loop throught all available directions or neighbors
     1. Get the neighbor in current direction and verify his entropy != 0
     2. Constrain the neighbor
     3. If there was a constriction push that neighbor to the stack to check its neighbors
5. return false
   
```c++
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
```
You can now call the function every game tick or while the function return false call it again.

## Background:

- It's history and development in gaming and possibly other fields?
  
## Uses and Implementations

- List of uses of the algorithm
  
## Sources

- Link to all the sources used
