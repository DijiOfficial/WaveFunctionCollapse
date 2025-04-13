using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using Unity.VisualScripting;
using UnityEditor;
using UnityEngine;

[System.Serializable] public class SocketEntry
{
    [HideInInspector] // Prevent key from being shown in the default inspector
    public string Key;
    public string Value; // Editable field
}

[System.Serializable] public class DoorFrameEntry
{
    public GameObject DoorFrame;
    public string Value;
}

public class Module : MonoBehaviour
{   
    [SerializeField] private bool _canRotate = false;
    [SerializeField] private bool _addWalls = false;
    [SerializeField] private bool _addDoorFrame = false;

    [HideInInspector][SerializeField] private List<GameObject> _walls = new();
    [HideInInspector][SerializeField] private List<DoorFrameEntry> _doorFrames = new();

    [SerializeField]
    [Tooltip("Define six socket key-value pairs.")]
    private List<SocketEntry> _sockets = new()
    {
        new SocketEntry { Key = "PosX", Value = "" },
        new SocketEntry { Key = "NegX", Value = "" },
        new SocketEntry { Key = "PosZ", Value = "" },
        new SocketEntry { Key = "NegZ", Value = "" },
        new SocketEntry { Key = "PosY", Value = "" },
        new SocketEntry { Key = "NegY", Value = "" }
    };

    // Store neighbors for each socket
    private Dictionary<string, List<Module>> _neighbors = new();
    private int _entropy;
    
    
    // Expose methods to check if properties should be shown in the editor
    public bool ShouldShowWalls() => _addWalls;
    public bool ShouldShowDoorFrames() => _addDoorFrame;

    // Prevent external changes to the list
    public IReadOnlyList<SocketEntry> Sockets => _sockets;
    public IReadOnlyList<GameObject> Walls => _walls;
    public bool CanRotate => _canRotate;
    public bool HasWalls => _addWalls;
    public bool HasDoorFrames => _addDoorFrame;
    // Function to register neighbors, called by the WFC manager
    public void RegisterNeighbors(List<GameObject> allModules)
    {
        foreach (var socket in _sockets)
        {
            string opposingSocketKey = GetOpposingSocketKey(socket.Key);

            // Initialize a new list for the socket if it doesn't exist yet
            if (!_neighbors.ContainsKey(socket.Key))
                _neighbors[socket.Key] = new List<Module>();

            // Search for a neighbor that matches the opposing socket
            foreach (var otherModuleGameObject in allModules)
            {
                // Get the Module component from the GameObject
                var otherModule = otherModuleGameObject.GetComponent<Module>();
                if (otherModule == null)
                    continue;

                // Look for a matching socket in the other module
                var opposingSocket = otherModule._sockets.FirstOrDefault(otherModuleSocket =>
                    otherModuleSocket.Key == opposingSocketKey && otherModuleSocket.Value == socket.Value);

                if (opposingSocket == null)
                    continue;

                // Register the other module as a neighbor for this socket
                _neighbors[socket.Key].Add(otherModule);
            }
        }
    }
    public void RotateSockets(int rotationCount)
    {
        if (rotationCount == 0)
            return;

        Dictionary<string, string> rotationMap90 = new Dictionary<string, string>
        {
            {"PosX", "PosZ"},
            {"NegX", "NegZ"},
            {"PosZ", "NegX"},
            {"NegZ", "PosX"},
            {"PosY", "PosY"}, // Y-axis remains unchanged
            {"NegY", "NegY"}
        };

        // Create a dictionary to map rotated values while maintaining original order
        Dictionary<string, string> rotatedValues = new Dictionary<string, string>();

        // Populate rotated values based on rotation
        foreach (var socket in _sockets)
        {
            string rotatedKey = socket.Key;
            for (int i = 0; i < rotationCount; i++)
            {
                rotatedKey = rotationMap90[rotatedKey]; // Rotate key
            }
            // Assign rotated value to its original key position
            rotatedValues[socket.Key] = _sockets.FirstOrDefault(s => s.Key == rotatedKey)?.Value ?? socket.Value;
        }

        // Apply the rotated values while keeping the original order
        for (int i = 0; i < _sockets.Count; i++)
        {
            _sockets[i].Value = rotatedValues[_sockets[i].Key];
        }
    }
    public void UpdateSocketWithRotation(Quaternion rotation)
    {
        // Clamp the rotation to 360 degrees
        int rotationY = (int)(rotation.eulerAngles.y % 360);
        // Map the clamped rotation to the corresponding socket key
        string socketKey = rotationY switch
        {
            0 => "NegZ",
            90 => "NegX",
            180 => "PosZ",
            270 => "PosX",
            360 => "NegZ",
            _ => throw new ArgumentException("Invalid rotation. Must be 0, 90, 180, or 270.")
        };

        // Find the socket and update its value
        var socket = _sockets.FirstOrDefault(s => s.Key == socketKey);
        if (socket == null) return;
        socket.Value += "w"; // Append "w" if not already present
    }
    public void PrintSocket()
    {
        foreach(var socket in _sockets)
        {
            Debug.Log($"Key: {socket.Key}, Value: {socket.Value}");
        }
    }
    private string GetOpposingSocketKey(string key) //todo: look into making this better (AI code)
    {
        return key switch
        {
            "PosX" => "NegX",
            "NegX" => "PosX",
            "PosZ" => "NegZ",
            "NegZ" => "PosZ",
            "PosY" => "NegY",
            "NegY" => "PosY",
            _ => null
        };
    }

    private void Awake()
    {
        
    }

    private void OnEnable()
    {
        
    }

    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}

[CustomEditor(typeof(Module))]
public class ModuleEditor : Editor
{
    public override void OnInspectorGUI()
    {
        Module module = (Module)target;
        serializedObject.Update();

        // Draw default inspector properties, except _sockets, _doorFrames, and _walls
        SerializedProperty property = serializedObject.GetIterator();
        property.NextVisible(true); // Skip 'Script' field

        while (property.NextVisible(false))
        {
            if (property.name == "_sockets" || property.name == "_doorFrames" || property.name == "_walls")
                continue;

            EditorGUILayout.PropertyField(property, true);
        }

        // === SOCKETS DISPLAY ===
        SerializedProperty socketsProp = serializedObject.FindProperty("_sockets");
        EditorGUILayout.Space();
        EditorGUILayout.LabelField("Sockets", EditorStyles.boldLabel);

        for (int i = 0; i < socketsProp.arraySize; i++)
        {
            SerializedProperty element = socketsProp.GetArrayElementAtIndex(i);
            SerializedProperty keyProp = element.FindPropertyRelative("Key");
            SerializedProperty valueProp = element.FindPropertyRelative("Value");

            using (new EditorGUILayout.HorizontalScope())
            {
                EditorGUILayout.LabelField(keyProp.stringValue, GUILayout.MaxWidth(100)); // Key (read-only)
                valueProp.stringValue = EditorGUILayout.TextField(valueProp.stringValue);  // Editable Value
            }
        }

        // === DOOR FRAMES ===
        if (module.ShouldShowDoorFrames())
        {
            SerializedProperty doorFramesProp = serializedObject.FindProperty("_doorFrames");

            EditorGUILayout.Space();
            EditorGUILayout.LabelField("Door Frames", EditorStyles.boldLabel);

            for (int i = 0; i < doorFramesProp.arraySize; i++)
            {
                SerializedProperty entry = doorFramesProp.GetArrayElementAtIndex(i);
                SerializedProperty doorFrameProp = entry.FindPropertyRelative("DoorFrame");
                SerializedProperty valueProp = entry.FindPropertyRelative("Value");

                using (new EditorGUILayout.HorizontalScope())
                {
                    doorFrameProp.objectReferenceValue = EditorGUILayout.ObjectField("Door Prefab", doorFrameProp.objectReferenceValue, typeof(GameObject), false);
                    valueProp.stringValue = EditorGUILayout.TextField("New Socket Value", valueProp.stringValue);
                }
            }

            if (GUILayout.Button("Add Door Frame"))
            {
                doorFramesProp.arraySize++;
            }

            if (GUILayout.Button("Remove Last Door Frame") && doorFramesProp.arraySize > 0)
            {
                doorFramesProp.arraySize--;
            }
        }

        // === WALLS ===
        if (module.ShouldShowWalls())
        {
            SerializedProperty wallsProp = serializedObject.FindProperty("_walls");

            EditorGUILayout.Space();
            EditorGUILayout.LabelField("Walls", EditorStyles.boldLabel);

            for (int i = 0; i < wallsProp.arraySize; i++)
            {
                SerializedProperty wallProp = wallsProp.GetArrayElementAtIndex(i);

                using (new EditorGUILayout.HorizontalScope())
                {
                    wallProp.objectReferenceValue = EditorGUILayout.ObjectField("Wall Prefab", wallProp.objectReferenceValue, typeof(GameObject), false);
                }
            }

            if (GUILayout.Button("Add Wall"))
            {
                wallsProp.arraySize++;
            }

            if (GUILayout.Button("Remove Last Wall") && wallsProp.arraySize > 0)
            {
                wallsProp.arraySize--;
            }
        }

        serializedObject.ApplyModifiedProperties();
    }
}
