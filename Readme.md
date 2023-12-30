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
In the following paragraph the word "tile" and "module" are interchangeable. Tile refers to a 2D Width x Height image and module to a 3D Width x Height x Depth box containing a 3D asset. They don't neceserally need to be square/cube as long as the rule set doesn't allow for overlapping tiles/modules. However for the 2D version of this algorithm you can break down the Width x Height tile to a lower version of itself that is multiple square tiles and completing the rule set with those subset tiles. I will also refer to Tile objects as Tobj not to be confused with tiles. Tobj contain different values including a list of available tiles it can choose from.

To get started with this Algorithm you will need to do an initial set up for it based on the variation you are doing. For the [Project1](#rpoject-1) first start by defining all the tiles you will be using and the subsequent rule set that will link all the tiles to each other or in other words which tile can connect to which tile. Make an array of Tobj containing a list of available tiles and an entropy value, by default entropy will be set to the total amount of tiles. Entropy is simply the amount of available tiles left to choose from after Constriction, more on that later. Finally to get started with the Wave Function Collapse follow this pseudo-code:
1. Get a list of lowest entropy Tobj

## Background:

- It's history and development in gaming and possibly other fields?
  
## Uses and Implementations

- List of uses of the algorithm
  
## Sources

- Link to all the sources used
