#pragma once
#include "Pointer.h"

class Object2 : public Pointer
{
protected:
    template< typename Function > Function CallVirtual(PVOID Base, DWORD Index)
    {
        QWORD** VTablePointer = (QWORD**)Base;
        QWORD* VTableFunctionBase = *VTablePointer;
        QWORD dwAddress = VTableFunctionBase[Index];

        return (Function)(dwAddress);
    }

    union {
        DEFINE_MEMBER_N(DWORD _handle, UPasta::Offsets::BaseObject::Handle);
        DEFINE_MEMBER_N(DWORD _network_id, 0x10);
        DEFINE_MEMBER_N(DWORD _team, UPasta::Offsets::BaseObject::Team);
        DEFINE_MEMBER_N(LolString _name, UPasta::Offsets::BaseObject::Name);
        DEFINE_MEMBER_N(Vector3 _position, UPasta::Offsets::BaseObject::Position);
    };

public:

    int GetHandle();
    int GetNetworkID();
    int GetTeam();
    bool IsEnemyTo(Object* target);
    std::string GetName();
    Vector3 GetWorldPosition();
    float GetDistanceTo(Object* target);
    float GetDistanceTo(Vector3 pos);
    enum TYPE : DWORD {

        NeutralMinionCamp = 0xFE7449A3,
        AIHeroClient = 0xE260302C, //HEROES
        AIMarker = 0x11F7583D,
        AIMinionClient = 0xCAA58CB2, //ALL MINIONS
        ObjectAttacher = 0x9E317160,
        LevelPropAIClient = 0x12E24BCD,
        AITurretClient = 0xBEBA9102, //LANETURRETS
        AITurretCommon = 0x70678BD0,
        obj_GeneralParticleEmitter = 0xDD4DE76F, // includes troys
        GameObject = 0x1FAC8B64,
        MissileClient = 0x9C8ADE94, //MISSILES
        DrawFX = 0x42D144F5,
        UnrevealedTarget = 0xB98F49AF,
        BarracksDampener = 0x60BB49C0, //INHIB
        Barracks = 0xD1ED70FE,
        AnimatedBuilding = 0x8F83FB9C,
        BuildingClient = 0x3CCABB2E,
        obj_Levelsizer = 0x6F2E6CAC,
        obj_NavPoint = 0x96B0A5E6,
        obj_SpawnPoint = 0xE3E9B36C,
        GrassObject = 0xAA2B7AB2, // IVERN BUSH?
        HQ = 0x503AD0D2, //NEXUS
        obj_InfoPoint = 0xF4753AD3,
        LevelPropGameObject = 0x5A730CB9,
        LevelPropSpawnerPoint = 0x4D8B713A,
        Shop = 0xA847E0A9,
        obj_Turret = 0x3D775D09 // AZIR TURRET?

    };

    TYPE GetType();
};

