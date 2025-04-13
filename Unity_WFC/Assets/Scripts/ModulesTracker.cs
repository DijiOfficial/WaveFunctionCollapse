using System.Collections.Generic;
using System.IO;
using UnityEngine;

[System.Serializable]
public class UpgradeData
{
    public string Title;
    public string Bonus;
    public string Description;
    public bool Acquired;
}

[System.Serializable]
public class UpgradeList
{
    public UpgradeData[] Upgrades;
}

[CreateAssetMenu(fileName = "ModuleTracker", menuName = "Persistence")]
public class ModuleTracker : ScriptableObject
{
    [Tooltip("Add a reference to every module prefab below")]
    public List<GameObject> Modules;
    private void Awake()
    {
        CustomReset();
        LoadPermanentUpgrades();
    }
    public void LoadPermanentUpgrades()
    {
 
    }

    public void CustomReset()
    {
        //_vitalEssence = 0;
        //_secondarySkill = 0;
        //_adrenalineRush = 0;
        //_restorativeVitality = 0;
        //_berserkerFury = 0;
        //_swiftStride = 0;
        //_keenEdge = 0;
        //_blazingAgility = 0;
        //_fortifiedResolve = 0;
        //_hasteOfTheWarrior = 0;
        //_curseProtection = 0;
        //_gravelordChosen = 0;
        //_goldCoins = 0;
        //_health = 100;

        //_hasRevived = false;
    }
}