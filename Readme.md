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
In the following paragraph the terms "tile" and "module" are interchangeable. A 'tile' refers to a 2D image with dimensions Width x Height, while a 'module' refers to a 3D box with dimensions Width x Height x Depth containing a 3D asset. It's important to note that they don't necessarily need to be square or cubic, as long as the rule set prohibits overlapping tiles or modules. For the 2D version of this algorithm, you can decompose a Width x Height tile into multiple smaller square tiles, completing the rule set with these subset tiles. To avoid confusion, I will refer to Tile objects as 'Tobj,' distinguishing them from individual tiles. Tobj contains various values, including a list of available tiles it can choose from.

To get started with this algorithm, you will need to do an initial setup based on the specific variation you are working on. For Project1, begin by defining all the tiles you will be using and establish the rule set that will dictate how the tiles connect to each other — in other words, specify which tile can connect to which. Create an array of Tile objects (Tobj) containing a list of available tiles and an entropy value. By default, the entropy will be set to the total number of tiles. Entropy represents the number of available tiles left to choose from after the constriction process (more on that later). Additionally, provide each Tobj with links to its neighbors, which will simplify the code later. I assigned a set of directions (N-E-S-W) to each Tobj and later defined how each direction identifies its neighbors in the array. Finally, to initiate the Wave Function Collapse, follow this pseudo-code:

1. Get a list of the lowest entropy Tobj (ignore tiles with entropy of value 0)
   1.1. If the list is empty then all tiles have been collapsed and the generation is complete
   1.2. return true
2. Choose a random Tobj from the list and Collapse it
   2.1. Choose a random tile from the available tile choices left (you can use weights to influence the choice)
   2.2. Clear the available tiles list of the Tobj and add the chosen tile
   2.3. Set the entropy to 0
3. Create a stack and add the collapsed Tobj to the top
4. Loop while the stack is not empty
   4.1. Pop the top Tobj on the stack and store it
   4.2. Get the list of all available tiles from the Tobj *used later for Constriction
   4.3. Loop throught all available directions or neighbors
     4.3.1. Get the neighbor in current direction and verify his entropy != 0
     4.3.2. Constrain the neighbor
        4.3.2.1. Bool Constriction = false
        4.3.2.2. Create a list of all possible connections from available tiles list (4.2) to the neighbor
        4.3.2.3. Loop through a copy of the available tiles list of the neighbor
             4.3.2.3.1. Check if the available tile can connect to any tile in the list of connections
             4.3.2.3.2. If not then remove the tile from the neighbor's list of available tiles
             4.3.2.3.3. Constriction = true
        4.3.2.4. Update the entropy
        4.3.2.5. return Constriction
     4.3.3. If there was a constriction push that neighbor to the stack to check its neighbors
5. return false

You can now call the function every game tick or while the function return false call it again.

## Background:

- It's history and development in gaming and possibly other fields?
  
## Uses and Implementations

- List of uses of the algorithm
  
## Sources

- Link to all the sources used
