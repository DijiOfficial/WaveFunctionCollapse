#pragma once
#include <vector>
#include <map>

enum class Direction;
enum class TileName;

class Tiles
{
public:
	Tiles();
	~Tiles() {};

	// Rule Of Five
	Tiles(const Tiles& other) = delete;
	Tiles(Tiles&& other) noexcept = delete;
	Tiles& operator=(const Tiles& other) = delete;
	Tiles& operator=(Tiles&& other) noexcept = delete;

	TileName GetTileName() const { return m_TileName; };
	int GetEntropy() const { return m_Entropy; };
	void Collapse();
	std::vector<TileName> GetAvailableTiles() const { return m_AvailableTiles; };
	void AddNeighbor(const Direction& direction, Tiles*& tile) { m_NeighborsMap[direction] = tile; };
	std::vector<Direction> GetDirections() const;
	Tiles* GetNeighbor(const Direction& direction) const { return m_NeighborsMap.find(direction)->second; };
    bool Constrain(std::vector<TileName>& availableTiles, const Direction& direction);

private:
	std::map<const Direction, Tiles*> m_NeighborsMap;
	std::vector<TileName> m_AvailableTiles{};
	int m_Entropy{ 36 };

	TileName m_TileName;
};

