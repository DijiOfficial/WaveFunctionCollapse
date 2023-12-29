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

//we take all the available tiles of a neighbor and add them as possible connection
//we then check the opposite direction to get the original tile that affected the neighbor
//we now check that for every current tile available it can connect to the original tile (the one that was collapsed)
//if it can't connect we remove it from the available tiles and return true so we can propagate to neihbor tiles
bool Tiles::Constrain(std::vector<TileName>& availableTilesVec, const Direction& direction)
{
    bool wasReduced = false;
 
    if (m_Entropy == 0)
        return false;
    
    std::vector<TileType> connectorsVec;
    for (const auto& availableTile : availableTilesVec)
    {
        connectorsVec.push_back(TileRuleSet.find(availableTile)->second.m_TileTypesVec[static_cast<int>(direction)]);
    }
    
    const Direction oppositeDirection = static_cast<Direction>((static_cast<int>(direction) + 2) % 4);
    const std::vector<TileName> availableTilesVecCopy{ m_AvailableTiles };

    for (const auto& availableTile : availableTilesVecCopy)
    {
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

//bool Tiles::Constrain(std::vector<TileName>& availableTilesVec, const Direction& direction)
//{
//    bool wasReduced = false;
//
//    if (m_Entropy == 0)
//        return false;
//
//    std::vector<TileType> connectorsVec;
//    for (const auto& availableTile : availableTilesVec)
//    {
//        connectorsVec.push_back(TileRuleSet.find(availableTile)->second.m_TileTypesVec[static_cast<int>(direction)]);
//    }
//
//    const Direction oppositeDirection = static_cast<Direction>((static_cast<int>(direction) + 2) % 4);
//    const std::vector<TileName> availableTilesVecCopy{ availableTilesVec };
//
//    for (const auto& availableTile : availableTilesVecCopy)
//    {
//        TileType possibleConnection = TileRuleSet.find(availableTile)->second.m_TileTypesVec[static_cast<int>(oppositeDirection)];
//
//        const auto it = std::find(connectorsVec.begin(), connectorsVec.end(), possibleConnection);
//        if (it == connectorsVec.end())
//        {
//            // The item was not found
//            availableTilesVec.erase(std::remove(availableTilesVec.begin(), availableTilesVec.end(), availableTile), availableTilesVec.end());
//            wasReduced = true;
//        }
//    }
//
//    m_Entropy = static_cast<int>(availableTilesVec.size());
//
//    return wasReduced;
//}