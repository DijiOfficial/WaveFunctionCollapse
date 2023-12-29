#include "Tiles.h"
#include "EnumClasses.h"
#include <random>

Tiles::Tiles()
	: m_TileName{ TileName::TILE_GRASS }
{
    //m_Entropy is the size of the enum class
    for (int i = 0; i < m_Entropy; i++)
    {
        m_AvailableTiles.push_back(static_cast<TileName>(i));
    }

}

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

std::vector<Direction> Tiles::GetDirections() const
{
    std::vector<Direction> directionsVec{ };

    for (const auto& neighbor : m_NeighborsMap)
    {
        directionsVec.push_back(neighbor.first);
    }

    return directionsVec;
}

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