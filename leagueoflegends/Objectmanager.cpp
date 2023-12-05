#include "ObjectManager.h"

QWORD ObjectManager2::GetFirst(const QWORD& objectManager) {

    static QWORD v1 = *reinterpret_cast<QWORD*>(objectManager + 0x18);
    static QWORD v2 = *reinterpret_cast<QWORD*>(objectManager + 0x20);
    if (v1 == v2)
        return 0;
    while (*reinterpret_cast<BYTE*>(v1) & 1 || !*reinterpret_cast<QWORD*>(v1))
    {
        v1 += 8;
        if (v1 == v2)
            return 0;
    }
    return *reinterpret_cast<QWORD*>(v1);
}

QWORD ObjectManager2::GetNext(const QWORD& objectManager, const QWORD& obj) {

    unsigned int v3; // edx
    QWORD v5; // eax

    static QWORD v2 = *(QWORD*)(objectManager + 0x18);
    v3 = *(unsigned __int16*)(obj + 16);
    static unsigned __int64 v4 = (unsigned __int64)((*(QWORD*)(objectManager + 0x20) - v2) >> 3);
    while (++v3 < v4) {
        if ((*(BYTE*)(v2 + 8i64 * v3) & 1) == 0 && *(QWORD*)(v2 + 8i64 * v3))
            return *(QWORD*)(v2 + 8i64 * v3);
    }
    return 0i64;
}

void ObjectManager2::HandleObject(Object2* obj) {

    switch (obj->GetType()) {

    case Object2::AIHeroClient: 
        ObjectManager2::_hero_list.emplace_back((Hero*)obj);
        _client_map.insert({ obj->GetHandle(), (Client*)obj });
        break;

    case Object2::AIMinionClient:
        ObjectManager2::_minion_list.emplace_back((Client*)obj);
        _client_map.insert({ obj->GetHandle(), (Client*)obj });
    	break;

    case Object2::AITurretClient:
        ObjectManager2::_turret_list.emplace_back((Turret*)obj);
        break;

    case Object2::obj_GeneralParticleEmitter:
        _particle_list.emplace_back(obj);
        break;

        //case Object::MissileClient: ObjectManager::_hero_list.emplace_back((Hero*)obj); break;
    case Object2::BarracksDampener:
        ObjectManager2::_inhibitor_list.emplace_back((AttackableUnit*)obj);
        break;

    case Object2::HQ:
        ObjectManager2::_nexus_list.emplace_back((AttackableUnit*)obj);
        break;

    default: 
        break;
    }
}

void ObjectManager2::Flush() {

    _hero_list.clear();
    _minion_list.clear();
    _turret_list.clear();
    _inhibitor_list.clear();
    _nexus_list.clear();
    _particle_list.clear();
    _client_map.clear();
}

void ObjectManager2::Update() {

    Flush();

    static const QWORD obj_manager = *(QWORD*)(RVA2(UPasta::Offsets::Instance::Lists::ObjManager));
    QWORD obj = GetFirst(obj_manager);
    while (obj)
    {
        const std::array<uint8_t, 8> vfunc_bytes = *reinterpret_cast<std::array<std::uint8_t, 8> *>((ReadVTable(obj, 1)));
        if (vfunc_bytes.at(0) != 0x48 && vfunc_bytes.at(7) != 0xC3) {
            ;
        }
        else {
            //auto o = (Object*)obj;
            //Vector3 pos_2d;
            HandleObject((Object2*)obj);
        }
        obj = GetNext(obj_manager, obj);
    }

    //print("heroes: %i", _hero_list.size());
    //print("minions: %i", _minion_list.size());
    //print("turrets: %i", _turret_list.size());
    //print("inhibs: %i", _inhibitor_list.size());
    //print("nexus: %i", _nexus_list.size());
    //print("particles: %i", _particle_list.size());
}

Object* ObjectManager2::GetClientByHandle(DWORD handle) {
    auto it = _client_map.find(handle);
    if (it != _client_map.end())
    {
        const auto clientFound = Engine::GetObjectFromNetId(it->second->GetNetworkID());
        if (clientFound != nullptr)
            return clientFound;
    }
    return nullptr;
}