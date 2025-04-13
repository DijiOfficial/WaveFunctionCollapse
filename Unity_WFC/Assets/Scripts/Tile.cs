using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine.PlayerLoop;
using UnityEngine;
using Object = UnityEngine.Object;

public enum Direction
{
    PosX,
    NegX,
    PosY,
    NegY,
    PosZ,
    NegZ
}
public class Tile
{
    private int _entropy;
    private List<GameObject> _modules;
    private Vector3Int _position;

    public int GetEntropy() 
    {
        return _entropy;
    }
    
    public List<GameObject> GetModules()
    {
        return _modules;
    }
    
    public List<string> GetAllPossibleSockets(Direction direction)
    {
        List<string> possibleSockets = new List<string>();

        // Determine the key for the given direction
        string directionKey = direction switch
        {
            Direction.PosX => "PosX",
            Direction.NegX => "NegX",
            Direction.PosY => "PosY",
            Direction.NegY => "NegY",
            Direction.PosZ => "PosZ",
            Direction.NegZ => "NegZ",
            _ => throw new ArgumentOutOfRangeException(nameof(direction), direction, null)
        };

        // Loop through all modules and get the socket value for the given direction
        foreach (var module in _modules)
        {
            var socket = module.GetComponent<Module>().Sockets.FirstOrDefault(s => s.Key == directionKey); // will this work?
            if (socket != null)
                possibleSockets.Add(socket.Value);
            else
                throw new InvalidOperationException($"Socket with key {directionKey} not found in module.");
        }

        return possibleSockets;
    }
    public Vector3Int GetPosition()
    {
        return _position;
    }
    public Tile(int x, int y, int z)
    {
        _modules = new List<GameObject>();
        _position = new Vector3Int(x, y, z);
    }

    public void Init()
    {
        _entropy =_modules.Count;
    }
    public void AddModule(GameObject module)
    {
        _modules.Add(module);
    }

    public void Collapse()
    {
        // todo: add weight logic here as well
    
        GameObject chosenModule = _modules[new System.Random().Next(_modules.Count)];

        // Destroy all instances in the list
        foreach (var module in _modules)
            if (module != chosenModule)
                Object.Destroy(module);

        _modules.Clear();
        _modules.Add(chosenModule);
        _entropy = 0;
    }

    public bool Constrain(Direction direction, List<string> possibleSockets)
    {
        bool isReduced = false;

        if (_entropy == 0)
            return false;
        
        // invert direction to get current neighbor sokcet (opposite socket)
        string directionKey = direction switch
        {
            Direction.PosX => "NegX",
            Direction.NegX => "PosX",
            Direction.PosY => "NegY",
            Direction.NegY => "PosY",
            Direction.PosZ => "NegZ",
            Direction.NegZ => "PosZ",
            _ => throw new ArgumentOutOfRangeException(nameof(direction), direction, null)
        };
        
        var modulesCopy = new List<GameObject>(_modules);
        foreach (var module in modulesCopy)
        {
            var socket = module.GetComponent<Module>().Sockets.FirstOrDefault(s => s.Key == directionKey);
            if (socket == null) continue;
            if (IsCompatible(socket.Value, possibleSockets)) continue;
                
            _modules.Remove(module);
            Object.Destroy(module);
            isReduced = true;
        }

        // Set the new entropy
        _entropy = _modules.Count;
        
        return isReduced;
    }

    private bool IsCompatible(string socket, List<string> possibleSockets)
    {
        if (socket == "0") return true;

        if (socket.Contains("i")) // check for non-self compatible sockets
        {
            string oppositeSocket = socket.StartsWith("-") ? socket.Substring(1) : "-" + socket;
            return possibleSockets.Contains(oppositeSocket);
        }

        if (socket.Contains("w")) 
            return !possibleSockets.Any(s => s.Contains("w"));

        return possibleSockets.Contains(socket.Replace("w", ""));
    }
}
