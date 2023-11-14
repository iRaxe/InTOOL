#ifndef LIST_MANAGER_FUNCTIONS_H
#define LIST_MANAGER_FUNCTIONS_H

#include "../NewMenu.h"
#include "../stdafx.h"
#include "../ListManager.h"
#include "./Damage.h"

namespace UPasta::SDK::ListManager
{
    namespace Functions
    {
        inline std::map<std::string, std::map<int, std::map<std::string, std::vector<int>>> > playerSpells;
    	void PopulateSpellsMap();
        inline std::map<std::pair<Object*, int>, float> ChampionCooldownsMap;
        void InsertCooldown(Object* champion, int spellSlot, float readyAt);
        float GetCooldown(Object* champion, int spellSlot);

        inline std::map<Object*, int> playersMap;
        void PopulatePlayersMap();

        inline std::map<Object*, int> enemiesMap;
        inline std::map<Object*, int> alliesMap;
        void PopulatePlayersMaps();

        inline std::map<Object*, int> minionsMap;
        void PopulateMinionsMap();

        inline std::map<Object*, int> enemiesMinionsMap;
        inline std::map<Object*, int> alliesMinionsMap;
        inline std::map<Object*, int> jungleMonstersMap;
        inline std::map<Object*, int> respawnMarkersMap;
        void PopulateMinionsMaps();

        inline std::map<Object*, int> turretsMap;
        void PopulateTurretsMap();

        inline std::map<Object*, int> enemiesTurretsMap;
        inline std::map<Object*, int> alliesTurretsMap;
        void PopulateTurretsMaps();

        inline std::map<Object*, int> inhibitorsMap;
        void PopulateInhibitorsMap();

        inline std::map<Object*, int> enemiesInhibitorsMap;
        inline std::map<Object*, int> alliesInhibitorsMap;
        void PopulateInhibitorsMaps();

        /*inline std::map<Missile*, std::tuple<Missile*, Vector3, Vector3, Vector3, std::string>> missileMap;
        void PopulateMissilesMap();*/

        void Initialize();
        void Refresh();
    }

}

#endif