using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using UnityEngine;

public class WFC : MonoBehaviour
{
    [SerializeField] private ModuleTracker _moduleTracker;
    [SerializeField] private int _sizeX;
    [SerializeField] private int _sizeY;
    [SerializeField] private int _sizeZ;

    private const int _tileSize = 4;
    // private List<Module>[,,] _grid; // 3D array of lists of modules
    private Tile[,,] _grid; // 3D array of Tile objects
    private int _maxEntropy;
    private bool _isCollapsed = false;
    private void Awake()
    {
        // Step 1: Initialize the 3D grid
        InitializeGrid();

        // Step 2: Populate the grid with list of modules
        PopulateGridWithModules();
        // Testing();

        //Test();
        //ClearGrid();
    }

    void Update()
    {
        if (!_isCollapsed)
            _isCollapsed = CollapseAlgorithm();
    }

    private void Testing()
    {
        int count = 0;

        // foreach (var module in _moduleTracker.Modules)
        // {
        var module = _moduleTracker.Modules[0]; // For testing, just use the first module
        var originalModule = module.GetComponent<Module>();

        var baseRotation = Quaternion.identity; // Default rotation
        int rotationCount = originalModule.CanRotate ? 4 : 1; // If it rotates, create 4 instances

        for (int r = 0; r < rotationCount; r++)
        {
            Vector3 position = new Vector3(count * 4, 0, 0);
            var moduleInstance = Instantiate(module, position, baseRotation * Quaternion.Euler(0, 90 * r, 0));
            count++;
            var moduleScript = moduleInstance.GetComponent<Module>();
            var wallPrefab = moduleScript.Walls[0]; // random?

            // Adjust sockets for rotation
            if (moduleScript.CanRotate)
                moduleScript.RotateSockets(r); // Update sockets based on rotation

            List<Quaternion> rotations = new List<Quaternion>
            {
                Quaternion.Euler(0, 0, 0),
                Quaternion.Euler(0, 90, 0),
                Quaternion.Euler(0, 180, 0),
                Quaternion.Euler(0, 270, 0)
            };

            HashSet<string> instantiatedCombinations = new HashSet<string>();

            // Generate combinations of 1 wall
            foreach (var rotation in rotations)
            {
                string combinationKey = rotation.eulerAngles.ToString();
                if (!instantiatedCombinations.Add(combinationKey)) continue;
                
                var wallInstance = Instantiate(wallPrefab, Vector3.zero, rotation);
            
                // Instantiate a new module for each wall rotation
                // Vector3 newPosition = new Vector3(position.x, position.y, position.z + (rotations.IndexOf(rotation) * 4));
                var newModuleInstance = Instantiate(module, position, baseRotation * Quaternion.Euler(0, 90 * r, 0));
                wallInstance.transform.SetParent(newModuleInstance.transform);
                wallInstance.transform.localPosition = Vector3.zero;
                newModuleInstance.GetComponent<Module>().UpdateSocketWithRotation(rotation);
            }

            // Generate combinations of 2 walls
            // if (r>0)
            //     continue;  
            
            for (int i = 0; i < rotations.Count; i++)
            {
                for (int j = 0; j < rotations.Count; j++)
                {
                    if (i == j) continue; // Skip if the rotations are the same
            
                    // Sort the indices to ensure consistent hashing
                    int first = Mathf.Min(i, j);
                    int second = Mathf.Max(i, j);
            
                    // Create a unique and consistent combination key
                    string combinationKey = $"{rotations[first].eulerAngles}-{rotations[second].eulerAngles}";
                    if (!instantiatedCombinations.Add(combinationKey)) continue;
            
                    var wallInstance0 = Instantiate(wallPrefab, Vector3.zero, rotations[first]);
                    var wallInstance1 = Instantiate(wallPrefab, Vector3.zero, rotations[second]);
            
                    // Instantiate a new module for each wall rotation
                    // Vector3 newPosition = new Vector3(position.x + (first + 1) * 5, position.y, position.z + (second + 1) * 5);
                    var newModuleInstance = Instantiate(module, position, baseRotation * Quaternion.Euler(0, 90 * r, 0));
                    wallInstance0.transform.SetParent(newModuleInstance.transform);
                    wallInstance0.transform.localPosition = Vector3.zero;
                    wallInstance1.transform.SetParent(newModuleInstance.transform);
                    wallInstance1.transform.localPosition = Vector3.zero;
                    
                    var moduleInstanceScript = newModuleInstance.GetComponent<Module>();
                    moduleInstanceScript.UpdateSocketWithRotation(rotations[first]);
                    moduleInstanceScript.UpdateSocketWithRotation(rotations[second]);
                }
            }

            foreach (var rotation in rotations)
            {
                var wallInstance0 = Instantiate(wallPrefab, Vector3.zero, rotation);
                var wallInstance1 = Instantiate(wallPrefab, Vector3.zero, rotation * Quaternion.Euler(0, 90, 0));
                var wallInstance2 = Instantiate(wallPrefab, Vector3.zero, rotation * Quaternion.Euler(0, 180, 0));

                // Vector3 newPosition = new Vector3(position.x + temp * 5, position.y, position.z + temp * 5);
                var newModuleInstance = Instantiate(module, position, baseRotation * rotation);
            
                // Set the walls as children of the module
                wallInstance0.transform.SetParent(newModuleInstance.transform);
                wallInstance0.transform.localPosition = Vector3.zero;
            
                wallInstance1.transform.SetParent(newModuleInstance.transform);
                wallInstance1.transform.localPosition = Vector3.zero;
            
                wallInstance2.transform.SetParent(newModuleInstance.transform);
                wallInstance2.transform.localPosition = Vector3.zero;
                
                var moduleInstanceScript = newModuleInstance.GetComponent<Module>();
                moduleInstanceScript.UpdateSocketWithRotation(rotation);
                moduleInstanceScript.UpdateSocketWithRotation(rotation * Quaternion.Euler(0, 90, 0));
                moduleInstanceScript.UpdateSocketWithRotation(rotation * Quaternion.Euler(0, 180, 0));
            }

            //// === HANDLE DOOR FRAMES ===
            //if (moduleScript.HasDoorFrames)
            //{
            //    foreach (var doorEntry in moduleScript.DoorFrames)
            //    {
            //        var doorInstance = Instantiate(doorEntry.DoorFrame, moduleInstance.transform);
            //        doorInstance.transform.localPosition = Vector3.zero; // Adjust as needed

            //        // Update socket value based on door placement
            //        moduleScript.UpdateSocketForDoor(doorEntry);
            //    }
            //}
        }
    }
    private void InitializeGrid()
    {
        _grid = new Tile[_sizeX, _sizeY, _sizeZ];

        // Initialize each cell with a new Tile object
        for (int x = 0; x < _sizeX; x++)
            for (int y = 0; y < _sizeY; y++)
                for (int z = 0; z < _sizeZ; z++)
                    _grid[x, y, z] = new Tile(x,y,z);
    }
    
    private void PopulateGridWithModules() //for neighbors keep a reference to each module and after assign neighbors??
    {
        // Loop through every grid cell
        for (int x = 0; x < _sizeX; x++)
            for (int y = 0; y < _sizeY; y++)
                for (int z = 0; z < _sizeZ; z++)
                    foreach (var module in _moduleTracker.Modules)
                    {
                        var moduleScript = module.GetComponent<Module>();
                        if (moduleScript == null)
                            continue;

                        var baseRotation = Quaternion.identity; // Default rotation
                        // int rotationCount = 1; // If it rotates, create 4 instances
                        int rotationCount = moduleScript.CanRotate ? 4 : 1; // If it rotates, create 4 instances

                        for (int r = 0; r < rotationCount; r++)
                        {
                            var position = new Vector3(x * 4, y * 4, z * 4);
                            var moduleInstance = Instantiate(module, position, baseRotation * Quaternion.Euler(0, 90 * r, 0));
                            var instanceScript = moduleInstance.GetComponent<Module>();

                            // Adjust sockets for rotation
                            if (instanceScript.CanRotate)
                                instanceScript.RotateSockets(r); // Update sockets based on rotation
                            
                            _grid[x, y, z].AddModule(moduleInstance);
                            
                            // === HANDLE WALLS ===
                            if (moduleScript.HasWalls)
                            {
                                List<Quaternion> rotations = new List<Quaternion>
                                {
                                    Quaternion.Euler(0, 0, 0),
                                    Quaternion.Euler(0, 90, 0),
                                    Quaternion.Euler(0, 180, 0),
                                    Quaternion.Euler(0, 270, 0)
                                };

                                HashSet<string> instantiatedCombinations = new HashSet<string>();

                                // Generate combinations of 1 wall
                                foreach (var rotation in rotations)
                                {
                                    var wallPrefab = moduleScript.Walls[UnityEngine.Random.Range(0, moduleScript.Walls.Count)];

                                    string combinationKey = rotation.eulerAngles.ToString();
                                    if (!instantiatedCombinations.Add(combinationKey)) continue;
                                    
                                    var wallInstance = Instantiate(wallPrefab, Vector3.zero, rotation);
                                
                                    // Instantiate a new module for each wall rotation
                                    // Vector3 newPosition = new Vector3(position.x, position.y, position.z + (rotations.IndexOf(rotation) * 4));
                                    var newModuleInstance = Instantiate(module, position, baseRotation * Quaternion.Euler(0, 90 * r, 0));
                                    wallInstance.transform.SetParent(newModuleInstance.transform);
                                    wallInstance.transform.localPosition = Vector3.zero;
                                    newModuleInstance.GetComponent<Module>().UpdateSocketWithRotation(rotation);
                                    _grid[x, y, z].AddModule(newModuleInstance);
                                }

                                // Generate combinations of 2 walls
                                // if (r>0)
                                //     continue;  
                                
                                for (int i = 0; i < rotations.Count; i++)
                                {
                                    for (int j = 0; j < rotations.Count; j++)
                                    {
                                        if (i == j) continue; // Skip if the rotations are the same
                                
                                        // Sort the indices to ensure consistent hashing
                                        int first = Mathf.Min(i, j);
                                        int second = Mathf.Max(i, j);
                                
                                        // Create a unique and consistent combination key
                                        string combinationKey = $"{rotations[first].eulerAngles}-{rotations[second].eulerAngles}";
                                        if (!instantiatedCombinations.Add(combinationKey)) continue;
                                
                                        var wallPrefab = moduleScript.Walls[UnityEngine.Random.Range(0, moduleScript.Walls.Count)];
                                        var wallPrefab1 = moduleScript.Walls[UnityEngine.Random.Range(0, moduleScript.Walls.Count)];

                                        var wallInstance0 = Instantiate(wallPrefab, Vector3.zero, rotations[first]);
                                        var wallInstance1 = Instantiate(wallPrefab1, Vector3.zero, rotations[second]);
                                
                                        // Instantiate a new module for each wall rotation
                                        // Vector3 newPosition = new Vector3(position.x + (first + 1) * 5, position.y, position.z + (second + 1) * 5);
                                        var newModuleInstance = Instantiate(module, position, baseRotation * Quaternion.Euler(0, 90 * r, 0));
                                        wallInstance0.transform.SetParent(newModuleInstance.transform);
                                        wallInstance0.transform.localPosition = Vector3.zero;
                                        wallInstance1.transform.SetParent(newModuleInstance.transform);
                                        wallInstance1.transform.localPosition = Vector3.zero;
                                        
                                        var moduleInstanceScript = newModuleInstance.GetComponent<Module>();
                                        moduleInstanceScript.UpdateSocketWithRotation(rotations[first]);
                                        moduleInstanceScript.UpdateSocketWithRotation(rotations[second]);
                                        
                                        _grid[x, y, z].AddModule(newModuleInstance);
                                    }
                                }

                                foreach (var rotation in rotations)
                                {
                                    var wallPrefab = moduleScript.Walls[UnityEngine.Random.Range(0, moduleScript.Walls.Count)];
                                    var wallPrefab1 = moduleScript.Walls[UnityEngine.Random.Range(0, moduleScript.Walls.Count)];
                                    var wallPrefab2 = moduleScript.Walls[UnityEngine.Random.Range(0, moduleScript.Walls.Count)];
                                    var wallInstance0 = Instantiate(wallPrefab, Vector3.zero, rotation);
                                    var wallInstance1 = Instantiate(wallPrefab1, Vector3.zero, rotation * Quaternion.Euler(0, 90, 0));
                                    var wallInstance2 = Instantiate(wallPrefab2, Vector3.zero, rotation * Quaternion.Euler(0, 180, 0));

                                    // Vector3 newPosition = new Vector3(position.x + temp * 5, position.y, position.z + temp * 5);
                                    var newModuleInstance = Instantiate(module, position, baseRotation * rotation);
                                
                                    // Set the walls as children of the module
                                    wallInstance0.transform.SetParent(newModuleInstance.transform);
                                    wallInstance0.transform.localPosition = Vector3.zero;
                                
                                    wallInstance1.transform.SetParent(newModuleInstance.transform);
                                    wallInstance1.transform.localPosition = Vector3.zero;
                                
                                    wallInstance2.transform.SetParent(newModuleInstance.transform);
                                    wallInstance2.transform.localPosition = Vector3.zero;
                                    
                                    var moduleInstanceScript = newModuleInstance.GetComponent<Module>();
                                    moduleInstanceScript.UpdateSocketWithRotation(rotation);
                                    moduleInstanceScript.UpdateSocketWithRotation(rotation * Quaternion.Euler(0, 90, 0));
                                    moduleInstanceScript.UpdateSocketWithRotation(rotation * Quaternion.Euler(0, 180, 0));
                                    
                                    _grid[x, y, z].AddModule(newModuleInstance);
                                }
                            }
                            
                            //// === HANDLE DOOR FRAMES ===
                            //if (moduleScript.HasDoorFrames)
                            //{
                            //    foreach (var doorEntry in moduleScript.DoorFrames)
                            //    {
                            //        var doorInstance = Instantiate(doorEntry.DoorFrame, moduleInstance.transform);
                            //        doorInstance.transform.localPosition = Vector3.zero; // Adjust as needed

                            //        // Update socket value based on door placement
                            //        moduleScript.UpdateSocketForDoor(doorEntry);
                            //    }
                            //}
                        }
                    }

        //foreach (var modulePrefab in _moduleTracker.Modules)
        //{
        //    var moduleInstance = Instantiate(modulePrefab, Vector3.zero, Quaternion.identity);
        //    var moduleScript = moduleInstance.GetComponent<Module>(); // Get the Module component
        //    if (moduleScript != null)
        //    {
        //        moduleScript.RegisterNeighbors(_moduleTracker.Modules); // Register neighbors
        //        _grid[x, y, z].Add(moduleScript); // Add the Module script to the grid
        //    }
        //    else
        //        Debug.LogError($"Module component missing on prefab: {modulePrefab.name}");
        //}

        // if (x == 0 && y == 0 && z == 0) // optimize this
        // {
        //     _moduleTracker.TotalEntropy++;
        // }
        
        for (int x = 0; x < _sizeX; x++)
            for (int y = 0; y < _sizeY; y++)
                for (int z = 0; z < _sizeZ; z++)
                    _grid[x,y,z].Init();

        _maxEntropy = _grid[0, 0, 0].GetEntropy();
    }

    // Example function to access a cell in the grid
    public List<Module> GetModulesAtCell(int x, int y, int z)
    {
        // if (x < 0 || x >= _sizeX || y < 0 || y >= _sizeY || z < 0 || z >= _sizeZ)
        // {
        //     Debug.LogError("Grid coordinates out of bounds!");
        //     return null;
        // }
        //
        // return _grid[x, y, z];
        return new List<Module>();
    }

    public void Test()
    {
        // for (int x = 0; x < _grid.GetLength(0); x++)
        // {
        //     for (int y = 0; y < _grid.GetLength(1); y++)
        //     {
        //         for (int z = 0; z < _grid.GetLength(2); z++)
        //         {
        //             var moduleList = _grid[x, y, z];
        //             if (moduleList == null || moduleList.Count == 0)
        //             {
        //                 Debug.LogWarning($"Grid cell [{x},{y},{z}] is empty or null.");
        //                 continue;
        //             }
        //
        //             // Instantiate the first module in the list
        //             var moduleToInstantiate = moduleList[0];
        //             if (moduleToInstantiate != null)
        //             {
        //                 Vector3 position = new Vector3(x * _tileSize, y * _tileSize, z * _tileSize);
        //                 Instantiate(moduleToInstantiate.gameObject, position, Quaternion.identity);
        //             }
        //             else
        //             {
        //                 Debug.LogWarning($"First module in grid cell [{x},{y},{z}] is null.");
        //             }
        //         }
        //     }
        // }
    }

    public void ClearGrid()
    {
        // // Loop through each cell in the grid
        // for (int x = 0; x < _grid.GetLength(0); x++)
        // {
        //     for (int y = 0; y < _grid.GetLength(1); y++)
        //     {
        //         for (int z = 0; z < _grid.GetLength(2); z++)
        //         {
        //             var moduleList = _grid[x, y, z];
        //             if (moduleList != null)
        //             {
        //                 // Destroy each module instance in the list
        //                 foreach (var module in moduleList)
        //                 {
        //                     if (module != null)
        //                     {
        //                         Destroy(module.gameObject);
        //                     }
        //                 }
        //             }
        //
        //             // Clear the list at this cell
        //             _grid[x, y, z] = null;
        //         }
        //     }
        // }
        //
        // // Clear the grid reference
        // _grid = null;
        //
        // Debug.Log("Grid and all module instances have been cleared.");
    }
    private List<Tile> GetLowestEntropyTileList()
    {
        int lowestEntropy = _maxEntropy;
        List<Tile> tileList = new List<Tile>();

        for (int x = 0; x < _sizeX; x++)
            for (int y = 0; y < _sizeY; y++)
                for (int z = 0; z < _sizeZ; z++)
                {
                    Tile tile = _grid[x, y, z];
                    int entropy = tile.GetEntropy();

                    if (entropy <= 0)
                        continue;

                    if (entropy < lowestEntropy)
                    {
                        lowestEntropy = entropy;
                        tileList.Clear();
                    }

                    if (entropy == lowestEntropy)
                        tileList.Add(tile);
                }

        return tileList;
    }

    private bool CollapseAlgorithm()
    {
        var lowestEntropyTileList = GetLowestEntropyTileList();
        if (lowestEntropyTileList.Count == 0)
            return true;

        Tile randomTile = lowestEntropyTileList[new System.Random().Next(lowestEntropyTileList.Count)];
        
        randomTile.Collapse();
        
        Stack<Tile> neighborsToConstrain = new Stack<Tile>();
        neighborsToConstrain.Push(randomTile);

        while (neighborsToConstrain.Count > 0)
        {
            Tile currentTile = neighborsToConstrain.Pop();
            Vector3Int currentPosition = currentTile.GetPosition();

            Dictionary<Direction, Vector3Int> neighborPositions = new Dictionary<Direction, Vector3Int>
            {
                { Direction.NegX, new Vector3Int(currentPosition.x - 1, currentPosition.y, currentPosition.z) },
                { Direction.PosX, new Vector3Int(currentPosition.x + 1, currentPosition.y, currentPosition.z) },
                { Direction.PosZ, new Vector3Int(currentPosition.x, currentPosition.y, currentPosition.z + 1) },
                { Direction.NegZ, new Vector3Int(currentPosition.x, currentPosition.y, currentPosition.z - 1) },
                { Direction.PosY, new Vector3Int(currentPosition.x, currentPosition.y + 1, currentPosition.z) },
                { Direction.NegY, new Vector3Int(currentPosition.x, currentPosition.y - 1, currentPosition.z) }
            };

            foreach (var kvp in neighborPositions)
            {
                Direction direction = kvp.Key;
                Vector3Int neighborPosition = kvp.Value;

                int x = neighborPosition.x;
                int y = neighborPosition.y;
                int z = neighborPosition.z;

                if (x < 0 || x >= _sizeX || y < 0 || y >= _sizeY || z < 0 || z >= _sizeZ)
                    continue;

                Tile neighborTile = _grid[x, y, z];
                if (neighborTile.GetEntropy() <= 0)
                    continue;

                if (neighborTile.Constrain(direction, currentTile.GetAllPossibleSockets(direction)))
                    neighborsToConstrain.Push(neighborTile);
            }
        }

        return false;
    }
}

