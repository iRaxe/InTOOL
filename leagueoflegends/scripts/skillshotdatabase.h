#pragma once

#include "../stdafx.h"

class Skillshot
{
private:
	int slotIndex;
	float range;
	float radius;
	float speed;
	float castTime;
	int type;
	std::vector<int> collidableWith;

public:
	Skillshot() : slotIndex(0), range(0.0f), radius(0.0f), speed(0.0f), castTime(0.0f), type(SkillshotType::SkillshotNone) {}

	Skillshot(float range, float radius, float speed, float castTime, int type, std::vector<int> collidableWith = {}) :
		slotIndex(0),
		range(range),
		radius(radius),
		speed(speed),
		castTime(castTime),
		type(type),
		collidableWith(collidableWith) {}

	void SetSlotIndex(int slotIndex);
	float GetRange() const;
	float GetRadius() const;
	float GetSpeed() const;
	float GetCastTime() const;
	int GetType() const;
	bool IsCollidableWith(int type) const;
	static void PopulateSpellsDB();
public:
	float GetMaxRange() const;
	std::string GetName() const;
	bool IsCastable() const;
	bool IsLearned() const;
	bool IsProcessing() const;
	int GetStacks() const;

};

struct SkillShotDB
{
	Skillshot AatroxQ;
	Skillshot AatroxQ2;
	Skillshot AatroxQ3;
	Skillshot AatroxW;
	Skillshot AhriQ;
	Skillshot AhriE;
	Skillshot AkaliQ;
	Skillshot AkaliE;
	Skillshot AkaliR;
	Skillshot AkaliR2;
	Skillshot AlistarQ;
	Skillshot AmumuQ;
	Skillshot AmumuR;
	Skillshot AniviaQ;
	Skillshot AnnieW;
	Skillshot AnnieR;
	Skillshot ApheliosQ;
	Skillshot ApheliosQ2;
	Skillshot ApheliosR;
	Skillshot AsheW;
	Skillshot AsheR;
	Skillshot AurelionSolQ;
	Skillshot AurelionSolR;
	Skillshot AzirR;
	Skillshot BelVethQ;
	Skillshot BelVethW;
	Skillshot BelVethE;
	Skillshot BelVethR;
	Skillshot BardQ;
	Skillshot BardR;
	Skillshot BlitzcrankQ;
	Skillshot BlitzcrankR;
	Skillshot BrandQ;
	Skillshot BrandW;
	Skillshot BraumQ;
	Skillshot BraumR;
	Skillshot CaitlynQ;
	Skillshot CaitlynW;
	Skillshot CaitlynE;
	Skillshot CamilleE;
	Skillshot CamilleE2;
	Skillshot CassiopeiaQ;
	Skillshot CassiopeiaW;
	Skillshot CassiopeiaR;
	Skillshot ChogathQ;
	Skillshot ChogathW;
	Skillshot CorkiQ;
	Skillshot CorkiR;
	Skillshot CorkiR2;
	Skillshot DianaQ;
	Skillshot DravenE;
	Skillshot DravenR;
	Skillshot DrMundoQ;
	Skillshot EkkoQ;
	Skillshot EkkoW;
	Skillshot EliseE;
	Skillshot EvelynnQ;
	Skillshot EvelynnR;
	Skillshot EzrealQ;
	Skillshot EzrealW;
	Skillshot EzrealE;
	Skillshot EzrealR;
	Skillshot FioraW;
	Skillshot FizzR;
	Skillshot GalioQ;
	Skillshot GalioE;
	Skillshot GnarQ;
	Skillshot GnarQ2;
	Skillshot GnarW;
	Skillshot GnarR;
	Skillshot GragasQ;
	Skillshot GragasR;
	Skillshot GravesQ;
	Skillshot GravesW;
	Skillshot GravesR;
	Skillshot GwenQ;
	Skillshot GwenR;
	Skillshot HecarimR;
	Skillshot HeimerdingerW;
	Skillshot HeimerdingerE;
	Skillshot HeimerdingerE2;
	Skillshot IllaoiQ;
	Skillshot IllaoiE;
	Skillshot IreliaW;
	Skillshot IreliaR;
	Skillshot IvernQ;
	Skillshot JannaQ;
	Skillshot JarvanIVQ;
	Skillshot JarvanIVE;
	Skillshot JayceQ;
	Skillshot JayceQ2;
	Skillshot JhinW;
	Skillshot JhinE;
	Skillshot JhinR;
	Skillshot JinxW;
	Skillshot JinxE;
	Skillshot JinxR;
	Skillshot KaisaQ;
	Skillshot KaisaW;
	Skillshot KaisaE;
	Skillshot KaisaR;
	Skillshot KalistaQ;
	Skillshot KarmaQ;
	Skillshot KarmaQ2;
	Skillshot KarthusQ;
	Skillshot KarthusQ2;
	Skillshot KarthusQ3;
	Skillshot KassadinE;
	Skillshot KassadinR;
	Skillshot KayleQ;
	Skillshot KaynW;
	Skillshot KennenQ;
	Skillshot KhazixW;
	Skillshot KhazixW2;
	Skillshot KledQ;
	Skillshot KledQ2;
	Skillshot KogMawQ;
	Skillshot KogMawE;
	Skillshot KogMawR;
	Skillshot KSanteQ;
	Skillshot KSanteQ2;
	Skillshot LeblancE;
	Skillshot LeblancE2;
	Skillshot LeeSinQ;
	Skillshot LeonaE;
	Skillshot LeonaR;
	Skillshot LissandraQ;
	Skillshot LissandraE;
	Skillshot LucianQ;
	Skillshot LucianW;
	Skillshot LuluQ;
	Skillshot LuxQ;
	Skillshot LuxE;
	Skillshot LuxR;
	Skillshot MalphiteE;
	Skillshot MalzaharQ;
	Skillshot MaokaiQ;
	Skillshot MissFortuneR;
	Skillshot MilioQ;
	Skillshot MilioE;
	Skillshot MorganaQ;
	Skillshot NaafiriQ;
	Skillshot NaafiriR;
	Skillshot NautilusQ;
	Skillshot NautilusW;
	Skillshot NautilusE;
	Skillshot NautilusR;
	Skillshot NeekoQ;
	Skillshot NeekoE;
	Skillshot NidaleeQ;
	Skillshot NidaleeW;
	Skillshot NidaleeE;
	Skillshot NilahQ;
	Skillshot NilahE;
	Skillshot NilahR;
	Skillshot NocturneQ;
	Skillshot NunuR;
	Skillshot OlafQ;
	Skillshot OriannaQ;
	Skillshot OrnnQ;
	Skillshot OrnnE;
	Skillshot OrnnR;
	Skillshot PantheonQ;
	Skillshot PantheonQ2;
	Skillshot PantheonR;
	Skillshot PoppyQ;
	Skillshot PoppyR;
	Skillshot PykeQ;
	Skillshot PykeQ2;
	Skillshot PykeE;
	Skillshot PykeR;
	Skillshot QiyanaQ;
	Skillshot QiyanaQ2;
	Skillshot QiyanaQ3;
	Skillshot QiyanaQ4;
	Skillshot QiyanaR;
	Skillshot QuinnQ;
	Skillshot RakanQ;
	Skillshot RakanW;
	Skillshot RekSaiQ;
	Skillshot RellQ;
	Skillshot RellW;
	Skillshot RellE;
	Skillshot RellR;
	Skillshot RengarE;
	Skillshot RivenR;
	Skillshot RumbleE;
	Skillshot RyzeQ;
	Skillshot SemiraQ;
	Skillshot SejuaniR;
	Skillshot SennaQ;
	Skillshot SennaW;
	Skillshot SennaE;
	Skillshot SennaR;
	Skillshot SeraphineQ;
	Skillshot SeraphineE;
	Skillshot SeraphineR;
	Skillshot SettW;
	Skillshot SettE;
	Skillshot ShyvanaE;
	Skillshot ShyvanaE2;
	Skillshot ShyvanaR;
	Skillshot SionQ;
	Skillshot SionE;
	Skillshot SivirQ;
	Skillshot SkarnerE;
	Skillshot SonaR;
	Skillshot SorakaQ;
	Skillshot SwainQ;
	Skillshot SwainW;
	Skillshot SwainE;
	Skillshot SylasQ;
	Skillshot SylasE;
	Skillshot SyndraQ;
	Skillshot SyndraE;
	Skillshot SyndraE2;
	Skillshot TahmKenchQ;
	Skillshot TaliyahQ;
	Skillshot TaliyahW;
	Skillshot TaliyahE;
	Skillshot TaliyahR;
	Skillshot TalonW;
	Skillshot TwitchQ;
	Skillshot TwitchW;
	Skillshot TwitchE;
	Skillshot TwitchR;
	Skillshot ThreshQ;
	Skillshot ThreshW;
	Skillshot ThreshE;
	Skillshot ThreshR;
	Skillshot TristanaW;
	Skillshot TryndamereE;
	Skillshot TwistedFateQ;
	Skillshot UrgotQ;
	Skillshot UrgotE;
	Skillshot UrgotR;
	Skillshot VarusQ;
	Skillshot VarusE;
	Skillshot VarusR;
	Skillshot VayneQ;
	Skillshot VayneE;
	Skillshot VayneR;
	Skillshot VeigarQ;
	Skillshot VeigarW;
	Skillshot VexQ;
	Skillshot VelkozQ;
	Skillshot VelkozQ2;
	Skillshot VelkozW;
	Skillshot VelkozE;
	Skillshot ViQ;
	Skillshot ViegoW;
	Skillshot ViktorW;
	Skillshot ViktorE;
	Skillshot WarwickR;
	Skillshot XayahQ;
	Skillshot XerathQ;
	Skillshot XerathW;
	Skillshot XerathE;
	Skillshot XerathR;
	Skillshot XinZhaoW;
	Skillshot YasuoQ;
	Skillshot YasuoQ2;
	Skillshot YasuoQ3;
	Skillshot YoneQ;
	Skillshot YoneQ2;
	Skillshot YoneW;
	Skillshot YoneR;
	Skillshot YorickQ;
	Skillshot YorickW;
	Skillshot YorickE;
	Skillshot YorickR;
	Skillshot ZacQ;
	Skillshot ZedQ;
	Skillshot ZeriQ;
	Skillshot ZiggsQ;
	Skillshot ZiggsW;
	Skillshot ZiggsE;
	Skillshot ZiggsR;
	Skillshot ZileanQ;
	Skillshot ZoeQ;
	Skillshot ZoeQ2;
	Skillshot ZoeE;
	Skillshot ZyraQ;
	Skillshot ZyraE;
	Skillshot ZyraR;
};

class SkillshotManager
{
private:
	std::map<std::string, std::map<int, Skillshot>> spells;

	static SkillshotManager& GetInstance() {
		static SkillshotManager instance;
		return instance;
	}

	SkillshotManager() {}

public:
	static Skillshot RegisterSpell(const std::string& name, const int& slotIndex, Skillshot spell) {
		spell.SetSlotIndex(slotIndex);
		GetInstance().spells[name][slotIndex] = spell;
		return spell;
	}

	static bool GetSpell(const std::string& name, const int& slotIndex, Skillshot &out) {
		auto& instance = GetInstance();
		auto itName = instance.spells.find(name);
		if (itName != instance.spells.end()) {
			auto itSpellId = (*itName).second.find(slotIndex);
			if (itSpellId != (*itName).second.end()) {
				out = itSpellId->second;
				return true;
			}
		}
		return false;
	}
};