# The Wave Function Collapse

### The Project

#### Project 1
The First part of this project is a 2D Dungeon/Texture/Terrain Generator. It has a variety of [Uses and Implementations](#uses-and-implementations).
However its main usage is Terrain Generation.
(May implement UI later).

The project was made in a C++ Win32 engine developed by a teacher at DAE of the university of Howest. This project was based on the [Python](https://github.com/CodingQuest2023/Algorithms) version of this function also presented in [this video](https://www.youtube.com/watch?v=qRtrj6Pua2A&t=51s) by [CodingQuest](https://www.youtube.com/@CodingQuest2023). This is also where the original sprites I used come from, although I have a different stylesheet that simplifies the code significantly when drawing with it.

The variety of the [Algorithm]() I used for this first project consist of defining all the available enum [Tiles](https://github.com/Howest-DAE-GD/gpp-researchtopic-DijiOfficial/blob/master/WaveFunctionCollapse/Assets/Tiles2.bmp) I can use. Then defining all the ```enum TileTypes``` that that will be used to connect tiles together, there can be multiple tiles per type eg: ```FOREST_N```, ```FOREST_NE``` and ```FOREST_NW``` tiles all fall under the ```FOREST_N``` ```TileType```. We also define ```enum Direction``` to link every tile to its neighbor(s) given a direction(s), it is not necessary but it simplfies the math for the algorithm when we need to loop through the neighbors. Finally we make a ```struct TileInfo``` containing a ```vector``` of ```TileType``` and ```int TileWeight``` that we will use in a ```std::map<TileName, TileInfo> TileRuleSet``` to define which ```TileType``` can connect to each ```enum Tile``` and give them weights to influence their pick rate. This structure can be viewed in the [EnumClasses.h](https://github.com/Howest-DAE-GD/gpp-researchtopic-DijiOfficial/blob/master/WaveFunctionCollapse/EnumClasses.h) file, and is the main disadvantage of this variety of the the [Algorithm](), it's straight forward as everything is predifined so it's not complicated to understand and visualise. However the complexity of the system can grow exponentially as we add more and more tile options, as well as taking longer to initialize than other methods.

#### Project 2
(not yet implemented) This version, also a 2D generator, will solve the issue of the complex initial setup of the map and enums by sampling everything from a bitmap, trading initialization complexity for general complexity.

#### Project 3
(not yet implemented, may not be implemented at all) This version is for generating 3D terrain.

### Motivations
I have always been passionate about Algorithms, Math, Physics and Video Games and how they intersect each other. And the passion for this project started when I first saw [this video](https://www.youtube.com/watch?v=2SuvO4Gi7uY&t=19s) by [Martin Donald](https://www.youtube.com/@MartinDonald) back in 2020. I found the explanation absolutely fantastic and it scratched just the right itch by combining Algorithms, Games, Infinite Generation, Exploration and Possibilites as well as the potential for infinite replayability. One of my very first "complete" project [MazeRunner](https://github.com/DijiOfficial/2D-Grid-Game) also features an infinite amount of levels by generating new ones every time. So I thought I would explore what [The Wave Function Collapse](#the-wave-function-collapse) had to offer and it did not disappoint as well as being slighlty harder to implement than I orginally thought it would be making it all the more fun and interstiing.

### Final result
(quick gif of the project in effect)

## Introduction to Algorithms in Games and their Application

- Why, When, What for?
  
## Background:

- It's history and development in gaming and possibly other fields?
  
## The Wave Collapse Function

- How does it work?

## Uses and Implementations

- List of uses of the algorithm
  
## Sources

- Link to all the sources used
