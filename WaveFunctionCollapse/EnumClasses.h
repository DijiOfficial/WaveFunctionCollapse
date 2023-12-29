#pragma once
#include <map>
#include <vector>

// All available tiles
enum class TileName 
{
	TILE_GRASS = 0,
	TILE_WATER = 1,
	TILE_FOREST = 2,
	TILE_COASTN = 3,
	TILE_COASTE = 4,
	TILE_COASTS = 5,
	TILE_COASTW = 6,
	TILE_COASTNE = 7,
	TILE_COASTSE = 8,
	TILE_COASTSW = 9,
	TILE_COASTNW = 10,
	TILE_COASTNE2 = 11,
	TILE_COASTSE2 = 12,
	TILE_COASTSW2 = 13,
	TILE_COASTNW2 = 14,
	TILE_ROCKN = 15,
	TILE_ROCKE = 16,
	TILE_ROCKS = 17,
	TILE_ROCKW = 18,
	TILE_ROCKNE = 19,
	TILE_ROCKSE = 20,
	TILE_ROCKSW = 21,
	TILE_ROCKNW = 22,
	TILE_FORESTN = 23,
	TILE_FORESTE = 24,
	TILE_FORESTS = 25,
	TILE_FORESTW = 26,
	TILE_FORESTNE = 27,
	TILE_FORESTSE = 28,
	TILE_FORESTSW = 29,
	TILE_FORESTNW = 30,
	TILE_FORESTNE2 = 31,
	TILE_FORESTSE2 = 32,
	TILE_FORESTSW2 = 33,
	TILE_FORESTNW2 = 34
};

// All possible tile edge types, Used for determining which tile can connect to which set in the rules (see below)
enum class TileType
{
	GRASS = 0,
	WATER = 1,
	FOREST = 2,
	COAST_N = 3,
	COAST_E = 4,
	COAST_S = 5,
	COAST_W = 6,
	FOREST_N = 7,
	FOREST_E = 8,
	FOREST_S = 9,
	FOREST_W = 10,
	ROCK_N = 11,
	ROCK_E = 12,
	ROCK_S = 13,
	ROCK_W = 14,
	ROCK = 15
};

// All possible tile edge sets, Used for determining which tile can connect to which type (see above)
struct TileInfo
{
	std::vector<TileType> m_TileTypesVec;
	int m_TileWeight;
};

using TileNameToRulesMap = const std::map<TileName, TileInfo>;
inline TileNameToRulesMap TileRuleSet
{ 
	{ TileName::TILE_GRASS,		{{ TileType::GRASS, TileType::GRASS, TileType::GRASS, TileType::GRASS }			, 16	}}, 
	{ TileName::TILE_WATER,		{{ TileType::WATER, TileType::WATER, TileType::WATER, TileType::WATER }			, 4		}}, 
	{ TileName::TILE_FOREST,	{{ TileType::FOREST, TileType::FOREST, TileType::FOREST, TileType::FOREST }		, 5		}}, 
	{ TileName::TILE_COASTN,	{{ TileType::GRASS, TileType::COAST_N, TileType::WATER, TileType::COAST_N }		, 5		}}, 
	{ TileName::TILE_COASTE,	{{ TileType::COAST_E, TileType::GRASS, TileType::COAST_E, TileType::WATER }		, 5		}}, 
	{ TileName::TILE_COASTS,	{{ TileType::WATER, TileType::COAST_S, TileType::GRASS, TileType::COAST_S }		, 5		}}, 
	{ TileName::TILE_COASTW,	{{ TileType::COAST_W, TileType::WATER, TileType::COAST_W, TileType::GRASS }		, 5		}}, 
	{ TileName::TILE_COASTNE,	{{ TileType::GRASS, TileType::GRASS, TileType::COAST_E, TileType::COAST_N }		, 5		}}, 
	{ TileName::TILE_COASTSE,	{{ TileType::COAST_E, TileType::GRASS, TileType::GRASS, TileType::COAST_S }		, 5		}}, 
	{ TileName::TILE_COASTSW,	{{ TileType::COAST_W, TileType::COAST_S, TileType::GRASS, TileType::GRASS }		, 5		}}, 
	{ TileName::TILE_COASTNW,	{{ TileType::GRASS, TileType::COAST_N, TileType::COAST_W, TileType::GRASS }		, 5		}}, 
	{ TileName::TILE_COASTNE2,	{{ TileType::COAST_E, TileType::COAST_N, TileType::WATER, TileType::WATER }		, 2		}}, 
	{ TileName::TILE_COASTSE2,	{{ TileType::WATER, TileType::COAST_S, TileType::COAST_E, TileType::WATER }		, 2		}}, 
	{ TileName::TILE_COASTSW2,	{{ TileType::WATER, TileType::WATER, TileType::COAST_W, TileType::COAST_S }		, 2		}}, 
	{ TileName::TILE_COASTNW2,	{{ TileType::COAST_W, TileType::WATER, TileType::WATER, TileType::COAST_N }		, 2		}}, 
	{ TileName::TILE_ROCKN,		{{ TileType::ROCK, TileType::ROCK_N, TileType::GRASS, TileType::ROCK_N }		, 4		}}, 
	{ TileName::TILE_ROCKE,		{{ TileType::ROCK_E, TileType::ROCK, TileType::ROCK_E, TileType::GRASS }		, 4		}}, 
	{ TileName::TILE_ROCKS,		{{ TileType::GRASS, TileType::ROCK_S, TileType::ROCK, TileType::ROCK_S }		, 4		}}, 
	{ TileName::TILE_ROCKW,		{{ TileType::ROCK_W, TileType::GRASS, TileType::ROCK_W, TileType::ROCK }		, 4		}}, 
	{ TileName::TILE_ROCKNE,	{{ TileType::ROCK_E, TileType::ROCK_N, TileType::GRASS, TileType::GRASS }		, 4		}}, 
	{ TileName::TILE_ROCKSE,	{{ TileType::GRASS, TileType::ROCK_S, TileType::ROCK_E, TileType::GRASS }		, 4		}}, 
	{ TileName::TILE_ROCKSW,	{{ TileType::GRASS, TileType::GRASS, TileType::ROCK_W, TileType::ROCK_S }		, 4		}}, 
	{ TileName::TILE_ROCKNW,	{{ TileType::ROCK_W, TileType::GRASS, TileType::GRASS, TileType::ROCK_N }		, 4		}}, 
	{ TileName::TILE_FORESTN,	{{ TileType::FOREST, TileType::FOREST_N, TileType::GRASS, TileType::FOREST_N }	, 4		}}, 
	{ TileName::TILE_FORESTE,	{{ TileType::FOREST_E, TileType::FOREST, TileType::FOREST_E, TileType::GRASS }	, 4		}}, 
	{ TileName::TILE_FORESTS,	{{ TileType::GRASS, TileType::FOREST_S, TileType::FOREST, TileType::FOREST_S }	, 4		}}, 
	{ TileName::TILE_FORESTW,	{{ TileType::FOREST_W, TileType::GRASS, TileType::FOREST_W, TileType::FOREST }	, 4		}}, 
	{ TileName::TILE_FORESTNE,	{{ TileType::FOREST_E, TileType::FOREST_N, TileType::GRASS, TileType::GRASS }	, 4		}}, 
	{ TileName::TILE_FORESTSE,	{{ TileType::GRASS, TileType::FOREST_S, TileType::FOREST_E, TileType::GRASS }	, 4		}}, 
	{ TileName::TILE_FORESTSW,	{{ TileType::GRASS, TileType::GRASS, TileType::FOREST_W, TileType::FOREST_S }	, 4		}}, 
	{ TileName::TILE_FORESTNW,	{{ TileType::FOREST_W, TileType::GRASS, TileType::GRASS, TileType::FOREST_N }	, 4		}}, 
	{ TileName::TILE_FORESTNE2,	{{ TileType::FOREST, TileType::FOREST, TileType::FOREST_E, TileType::FOREST_N }	, 2		}}, 
	{ TileName::TILE_FORESTSE2,	{{ TileType::FOREST_E, TileType::FOREST, TileType::FOREST, TileType::FOREST_S }	, 2		}}, 
	{ TileName::TILE_FORESTSW2,	{{ TileType::FOREST_W, TileType::FOREST_S, TileType::FOREST, TileType::FOREST }	, 2		}}, 
	{ TileName::TILE_FORESTNW2,	{{ TileType::FOREST, TileType::FOREST_N, TileType::FOREST_W, TileType::FOREST }	, 2		}}
};
//enum class TileRuleSet
//{
	//TILE_WATER : [WATER, WATER, WATER, WATER],
	//TILE_FOREST : [FOREST, FOREST, FOREST, FOREST],
	//TILE_COASTN : [GRASS, COAST_N, WATER, COAST_N],
	//TILE_COASTE : [COAST_E, GRASS, COAST_E, WATER],
	//TILE_COASTS : [WATER, COAST_S, GRASS, COAST_S],
	//TILE_COASTW : [COAST_W, WATER, COAST_W, GRASS],
	//TILE_COASTNE : [GRASS, GRASS, COAST_E, COAST_N],
	//TILE_COASTSE : [COAST_E, GRASS, GRASS, COAST_S],
	//TILE_COASTSW : [COAST_W, COAST_S, GRASS, GRASS],
	//TILE_COASTNW : [GRASS, COAST_N, COAST_W, GRASS],
	//TILE_COASTNE2 : [COAST_E, COAST_N, WATER, WATER],
	//TILE_COASTSE2 : [WATER, COAST_S, COAST_E, WATER],
	//TILE_COASTSW2 : [WATER, WATER, COAST_W, COAST_S],
	//TILE_COASTNW2 : [COAST_W, WATER, WATER, COAST_N],
	//TILE_ROCKN : [ROCK, ROCK_N, GRASS, ROCK_N],
	//TILE_ROCKE : [ROCK_E, ROCK, ROCK_E, GRASS],
	//TILE_ROCKS : [GRASS, ROCK_S, ROCK, ROCK_S],
	//TILE_ROCKW : [ROCK_W, GRASS, ROCK_W, ROCK],
	//TILE_ROCKNE : [ROCK_E, ROCK_N, GRASS, GRASS],
	//TILE_ROCKSE : [GRASS, ROCK_S, ROCK_E, GRASS],
	//TILE_ROCKSW : [GRASS, GRASS, ROCK_W, ROCK_S],
	//TILE_ROCKNW : [ROCK_W, GRASS, GRASS, ROCK_N],
	//TILE_FORESTN : [FOREST, FOREST_N, GRASS, FOREST_N],
	//TILE_FORESTE : [FOREST_E, FOREST, FOREST_E, GRASS],
	//TILE_FORESTS : [GRASS, FOREST_S, FOREST, FOREST_S],
	//TILE_FORESTW : [FOREST_W, GRASS, FOREST_W, FOREST],
	//TILE_FORESTNE : [FOREST_E, FOREST_N, GRASS, GRASS],
	//TILE_FORESTSE : [GRASS, FOREST_S, FOREST_E, GRASS],
	//TILE_FORESTSW : [GRASS, GRASS, FOREST_W, FOREST_S],
	//TILE_FORESTNW : [FOREST_W, GRASS, GRASS, FOREST_N],
	//TILE_FORESTNE2 : [FOREST, FOREST, FOREST_E, FOREST_N],
	//TILE_FORESTSE2 : [FOREST_E, FOREST, FOREST, FOREST_S],
	//TILE_FORESTSW2 : [FOREST_W, FOREST_S, FOREST, FOREST],
	//TILE_FORESTNW2 : [FOREST, FOREST_N, FOREST_W, FOREST]
//};

// All available directions for each tile
enum class Direction
{
	NORTH = 0,
	EAST = 1,
	SOUTH = 2,
	WEST = 3
};

// Tile weights to determine the probability of a tile being chosen
//enum class TileWeight
//{
//	TILE_GRASS		= 16,
//	TILE_WATER		= 4,
//	TILE_FOREST		= 5,
//	TILE_COASTN		= 5,
//	TILE_COASTE		= 5,
//	TILE_COASTS		= 5,
//	TILE_COASTW		= 5,
//	TILE_COASTNE	= 5,
//	TILE_COASTSE	= 5,
//	TILE_COASTSW	= 5,
//	TILE_COASTNW	= 5,
//	TILE_COASTNE2	= 2,
//	TILE_COASTSE2	= 2,
//	TILE_COASTSW2	= 2,
//	TILE_COASTNW2	= 2,
//	TILE_ROCKN		= 4,
//	TILE_ROCKE		= 4,
//	TILE_ROCKS		= 4,
//	TILE_ROCKW		= 4,
//	TILE_ROCKNE		= 4,
//	TILE_ROCKSE		= 4,
//	TILE_ROCKSW		= 4,
//	TILE_ROCKNW		= 4,
//	TILE_FORESTN	= 4,
//	TILE_FORESTE	= 4,
//	TILE_FORESTS	= 4,
//	TILE_FORESTW	= 4,
//	TILE_FORESTNE	= 4,
//	TILE_FORESTSE	= 4,
//	TILE_FORESTSW	= 4,
//	TILE_FORESTNW	= 4,
//	TILE_FORESTNE2	= 2,
//	TILE_FORESTSE2	= 2,
//	TILE_FORESTSW2	= 2,
//	TILE_FORESTNW2	= 2,
//};
