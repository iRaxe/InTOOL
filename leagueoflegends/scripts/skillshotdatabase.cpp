#include "../stdafx.h"

void Skillshot::SetSlotIndex(int slotId) {
	this->slotIndex = slotId;
}

float Skillshot::GetRange() const {
	return this->range;
}

float Skillshot::GetRadius() const {
	return this->radius;
}

float Skillshot::GetSpeed() const {
	return this->speed;
}

float Skillshot::GetCastTime() const {
	return this->castTime;
}

int Skillshot::GetType() const {
	return this->type;
}

bool Skillshot::IsCollidableWith(int type) const {
	return std::ranges::any_of(this->collidableWith,
		[type](int collidableType) { return collidableType == type; });
}

float Skillshot::GetMaxRange() const {
	if (type == SkillshotCircle) {
		const auto adjustedRadius = this->radius - min(this->radius, 70.0f);
		return this->range + adjustedRadius;
	}
	return this->range;
}

std::string Skillshot::GetName() const {
	return globals::localPlayer->GetSpellBySlotId(this->slotIndex)->GetName();
}

bool Skillshot::IsCastable() const {
	return globals::localPlayer->CanCastSpell(this->slotIndex);
}

bool Skillshot::IsLearned() const {
	return Engine::GetSpellState(this->slotIndex) != IsNotLearned;
}

bool Skillshot::IsProcessing() const {
	return Engine::GetSpellState(this->slotIndex) == SpellState::IsProcessing;
}

int Skillshot::GetStacks() const {
	return globals::localPlayer->GetSpellBySlotId(this->slotIndex)->GetStacks();
}

void Skillshot::PopulateSpellsDB()
{
	database.AatroxQ = SkillshotManager::RegisterSpell(SP_STRING("Aatrox"), Q, Skillshot(650, 130, FLT_MAX, 0.6, SkillshotLine, {}));
	database.AatroxQ2 = SkillshotManager::RegisterSpell(SP_STRING("Aatrox"), Q, Skillshot(500, 200, FLT_MAX, 0.6, SkillshotCone, {}));
	database.AatroxQ3 = SkillshotManager::RegisterSpell(SP_STRING("Aatrox"), Q, Skillshot(200, 300, FLT_MAX, 0.6, SkillshotCircle, {}));
	database.AatroxW = SkillshotManager::RegisterSpell(SP_STRING("Aatrox"), W, Skillshot(825, 80, 1800, 0.25, SkillshotLine, {}));
	database.AhriQ = SkillshotManager::RegisterSpell(SP_STRING("Ahri"), Q, Skillshot(880, 100, 2500, 0.25, SkillshotLine, {}));
	database.AhriE = SkillshotManager::RegisterSpell(SP_STRING("Ahri"), E, Skillshot(975, 60, 1500, 0.25, SkillshotLine, {}));
	database.AkaliQ = SkillshotManager::RegisterSpell(SP_STRING("Akali"), Q, Skillshot(550, 60, 3200, 0.25, SkillshotCone, {}));
	database.AkaliE = SkillshotManager::RegisterSpell(SP_STRING("Akali"), E, Skillshot(825, 70, 1800, 0.25, SkillshotLine, {}));
	database.AkaliR = SkillshotManager::RegisterSpell(SP_STRING("Akali"), R, Skillshot(675, 65, 1800, 0, SkillshotLine, {}));
	database.AkaliR2 = SkillshotManager::RegisterSpell(SP_STRING("Akali"), R, Skillshot(525, 65, 3600, 0, SkillshotLine, {}));
	database.AlistarQ = SkillshotManager::RegisterSpell(SP_STRING("Alistar"), Q, Skillshot(0, 365, FLT_MAX, 0.25, SkillshotCircle, {}));
	database.AmumuQ = SkillshotManager::RegisterSpell(SP_STRING("Amumu"), Q, Skillshot(1100, 80, 2000, 0.25, SkillshotLine, {}));
	database.AmumuR = SkillshotManager::RegisterSpell(SP_STRING("Amumu"), R, Skillshot(0, 550, FLT_MAX, 0.25, SkillshotCircle, {}));
	database.AniviaQ = SkillshotManager::RegisterSpell(SP_STRING("Anivia"), Q, Skillshot(1100, 110, 950, 0.25, SkillshotLine, {}));
	database.AnnieW = SkillshotManager::RegisterSpell(SP_STRING("Annie"), W, Skillshot(600, 0, FLT_MAX, 0.25, SkillshotCone, {}));
	database.AnnieR = SkillshotManager::RegisterSpell(SP_STRING("Annie"), R, Skillshot(600, 290, FLT_MAX, 0.25, SkillshotCircle, {}));
	database.ApheliosQ = SkillshotManager::RegisterSpell(SP_STRING("Aphelios"), Q, Skillshot(1450, 60, 1850, 0.35, SkillshotLine, {}));
	database.ApheliosQ2 = SkillshotManager::RegisterSpell(SP_STRING("Aphelios"), Q, Skillshot(850, 65, 1500, 0.25, SkillshotCone, {}));
	database.ApheliosR = SkillshotManager::RegisterSpell(SP_STRING("Aphelios"), R, Skillshot(1600, 125, 2050, 0.5, SkillshotLine, {}));
	database.AsheW = SkillshotManager::RegisterSpell(SP_STRING("Ashe"), W, Skillshot(1200, 20, 2000, 0.25, SkillshotCone, { CollidableObjects::Objects }));
	database.AsheR = SkillshotManager::RegisterSpell(SP_STRING("Ashe"), R, Skillshot(12500, 130, 1600, 0.25, SkillshotLine, {}));
	database.AurelionSolQ = SkillshotManager::RegisterSpell(SP_STRING("AurelionSol"), Q, Skillshot(1075, 110, 850, 0, SkillshotLine, {}));
	database.AurelionSolR = SkillshotManager::RegisterSpell(SP_STRING("AurelionSol"), R, Skillshot(1500, 120, 4500, 0.35, SkillshotLine, {}));
	database.AzirR = SkillshotManager::RegisterSpell(SP_STRING("Azir"), R, Skillshot(500, 250, 1400, 0.3, SkillshotLine, {}));
	database.BelVethQ = SkillshotManager::RegisterSpell(SP_STRING("BelVeth"), Q, Skillshot(450, 100, 1200, 0.0, SkillshotLine, {}));
	database.BelVethW = SkillshotManager::RegisterSpell(SP_STRING("BelVeth"), W, Skillshot(715, 200, 500, 0.5, SkillshotLine, {}));
	database.BelVethE = SkillshotManager::RegisterSpell(SP_STRING("BelVeth"), E, Skillshot(0.0, 500, FLT_MAX, 1.5, SkillshotCircle, {}));
	database.BelVethR = SkillshotManager::RegisterSpell(SP_STRING("BelVeth"), R, Skillshot(275, 500, FLT_MAX, 1.0, SkillshotCircle, {}));
	database.BardQ = SkillshotManager::RegisterSpell(SP_STRING("Bard"), Q, Skillshot(950, 60, 1500, 0.25, SkillshotLine, {}));
	database.BardR = SkillshotManager::RegisterSpell(SP_STRING("Bard"), R, Skillshot(3400, 350, 2100, 0.5, SkillshotCircle, {}));
	database.BlitzcrankQ = SkillshotManager::RegisterSpell(SP_STRING("Blitzcrank"), Q, Skillshot(1150, 70, 1800, 0.25, SkillshotLine, {}));
	database.BlitzcrankR = SkillshotManager::RegisterSpell(SP_STRING("Blitzcrank"), R, Skillshot(0, 600, FLT_MAX, 0.25, SkillshotCircle, {}));
	database.BrandQ = SkillshotManager::RegisterSpell(SP_STRING("Brand"), Q, Skillshot(1050, 60, 1600, 0.25, SkillshotLine, {}));
	database.BrandW = SkillshotManager::RegisterSpell(SP_STRING("Brand"), W, Skillshot(900, 250, FLT_MAX, 0.85, SkillshotCircle, {}));
	database.BraumQ = SkillshotManager::RegisterSpell(SP_STRING("Braum"), Q, Skillshot(1000, 70, 1700, 0.25, SkillshotLine, {}));
	database.BraumR = SkillshotManager::RegisterSpell(SP_STRING("Braum"), R, Skillshot(1250, 115, 1400, 0.5, SkillshotLine, {}));
	database.CaitlynQ = SkillshotManager::RegisterSpell(SP_STRING("Caitlyn"), Q, Skillshot(1250, 90, 2200, 0.625, SkillshotLine, {}));
	database.CaitlynW = SkillshotManager::RegisterSpell(SP_STRING("Caitlyn"), W, Skillshot(800, 75, FLT_MAX, 0.35, SkillshotCircle, {}));
	database.CaitlynE = SkillshotManager::RegisterSpell(SP_STRING("Caitlyn"), E, Skillshot(750, 70, 1600, 0.15, SkillshotLine, {}));
	database.CamilleE = SkillshotManager::RegisterSpell(SP_STRING("Camille"), E, Skillshot(800, 60, 1900, 0, SkillshotLine, {}));
	database.CamilleE2 = SkillshotManager::RegisterSpell(SP_STRING("Camille"), E, Skillshot(400, 60, 1900, 0, SkillshotLine, {}));
	database.CassiopeiaQ = SkillshotManager::RegisterSpell(SP_STRING("Cassiopeia"), Q, Skillshot(850, 150, FLT_MAX, 0.75, SkillshotCircle, {}));
	database.CassiopeiaW = SkillshotManager::RegisterSpell(SP_STRING("Cassiopeia"), W, Skillshot(800, 160, 2500, 0.75, SkillshotCircle, {}));
	database.CassiopeiaR = SkillshotManager::RegisterSpell(SP_STRING("Cassiopeia"), R, Skillshot(825, 0, FLT_MAX, 0.5, SkillshotCone, {}));
	database.ChogathQ = SkillshotManager::RegisterSpell(SP_STRING("Chogath"), Q, Skillshot(950, 250, FLT_MAX, 1.2, SkillshotCircle, {}));
	database.ChogathW = SkillshotManager::RegisterSpell(SP_STRING("Chogath"), W, Skillshot(650, 0, FLT_MAX, 0.5, SkillshotCone, {}));
	database.CorkiQ = SkillshotManager::RegisterSpell(SP_STRING("Corki"), Q, Skillshot(825, 250, 1000, 0.25, SkillshotCircle, {}));
	database.CorkiR = SkillshotManager::RegisterSpell(SP_STRING("Corki"), R, Skillshot(1300, 40, 2000, 0.175, SkillshotLine, {}));
	database.CorkiR2 = SkillshotManager::RegisterSpell(SP_STRING("Corki"), R, Skillshot(1500, 40, 2000, 0.175, SkillshotLine, {}));
	database.DianaQ = SkillshotManager::RegisterSpell(SP_STRING("Diana"), Q, Skillshot(900, 185, 1900, 0.25, SkillshotCircle, {}));
	database.DravenE = SkillshotManager::RegisterSpell(SP_STRING("Draven"), E, Skillshot(1050, 130, 1600, 0.25, SkillshotLine, {}));
	database.DravenR = SkillshotManager::RegisterSpell(SP_STRING("Draven"), R, Skillshot(12500, 160, 2000, 0.25, SkillshotLine, {}));
	database.DrMundoQ = SkillshotManager::RegisterSpell(SP_STRING("DrMundo"), Q, Skillshot(990, 120, 2000, 0.25, SkillshotLine, {}));
	database.EkkoQ = SkillshotManager::RegisterSpell(SP_STRING("Ekko"), Q, Skillshot(1175, 60, 1650, 0.25, SkillshotLine, {}));
	database.EkkoW = SkillshotManager::RegisterSpell(SP_STRING("Ekko"), W, Skillshot(1600, 400, FLT_MAX, 3.35, SkillshotCircle, {}));
	database.EliseE = SkillshotManager::RegisterSpell(SP_STRING("Elise"), E, Skillshot(1075, 55, 1600, 0.25, SkillshotLine, {}));
	database.EvelynnQ = SkillshotManager::RegisterSpell(SP_STRING("Evelynn"), Q, Skillshot(800, 60, 2400, 0.25, SkillshotLine, {}));
	database.EvelynnR = SkillshotManager::RegisterSpell(SP_STRING("Evelynn"), R, Skillshot(450, 180, FLT_MAX, 0.35, SkillshotCone, {}));
	database.EzrealQ = SkillshotManager::RegisterSpell(SP_STRING("Ezreal"), Q, Skillshot(1100, 60, 2000, 0.25, SkillshotLine, { CollidableObjects::Objects }));
	database.EzrealW = SkillshotManager::RegisterSpell(SP_STRING("Ezreal"), W, Skillshot(1100, 60, 1700, 0.25, SkillshotLine, {}));
	database.EzrealE = SkillshotManager::RegisterSpell(SP_STRING("Ezreal"), E, Skillshot(750, 60, 2000, 0.25, SkillshotNone, {}));
	database.EzrealR = SkillshotManager::RegisterSpell(SP_STRING("Ezreal"), R, Skillshot(12500, 160, 2000, 1, SkillshotLine, {}));
	database.FioraW = SkillshotManager::RegisterSpell(SP_STRING("Fiora"), W, Skillshot(750, 70, 3200, 0.75, SkillshotLine, {}));
	database.FizzR = SkillshotManager::RegisterSpell(SP_STRING("Fizz"), R, Skillshot(1300, 150, 1300, 0.25, SkillshotLine, {}));
	database.GalioQ = SkillshotManager::RegisterSpell(SP_STRING("Galio"), Q, Skillshot(825, 235, 1150, 0.25, SkillshotCircle, {}));
	database.GalioE = SkillshotManager::RegisterSpell(SP_STRING("Galio"), E, Skillshot(650, 160, 2300, 0.4, SkillshotLine, {}));
	database.GnarQ = SkillshotManager::RegisterSpell(SP_STRING("Gnar"), Q, Skillshot(1125, 55, 2500, 0.25, SkillshotLine, {}));
	database.GnarQ2 = SkillshotManager::RegisterSpell(SP_STRING("Gnar"), Q, Skillshot(1125, 90, 2100, 0.5, SkillshotLine, {}));
	database.GnarW = SkillshotManager::RegisterSpell(SP_STRING("Gnar"), W, Skillshot(575, 100, FLT_MAX, 0.6, SkillshotLine, {}));
	database.GnarR = SkillshotManager::RegisterSpell(SP_STRING("Gnar"), R, Skillshot(0, 475, FLT_MAX, 0.25, SkillshotCircle, {}));
	database.GragasQ = SkillshotManager::RegisterSpell(SP_STRING("Gragas"), Q, Skillshot(850, 275, 1000, 0.25, SkillshotCircle, {}));
	database.GragasR = SkillshotManager::RegisterSpell(SP_STRING("Gragas"), R, Skillshot(1000, 400, 1800, 0.25, SkillshotCircle, {}));
	database.GravesQ = SkillshotManager::RegisterSpell(SP_STRING("Graves"), Q, Skillshot(800, 20, FLT_MAX, 1.4, SkillshotCone, {}));
	database.GravesW = SkillshotManager::RegisterSpell(SP_STRING("Graves"), W, Skillshot(950, 250, 1500, 0.15, SkillshotCircle, {}));
	database.GravesR = SkillshotManager::RegisterSpell(SP_STRING("Graves"), R, Skillshot(1000, 100, 2100, 0.25, SkillshotCone, {}));
	database.GwenQ = SkillshotManager::RegisterSpell(SP_STRING("Gwen"), Q, Skillshot(450, 275, 1500, 0, SkillshotCircle, {}));
	database.GwenR = SkillshotManager::RegisterSpell(SP_STRING("Gwen"), R, Skillshot(1230, 250, 1800, 0.25, SkillshotLine, {}));
	database.HecarimR = SkillshotManager::RegisterSpell(SP_STRING("Hecarim"), R, Skillshot(1650, 280, 1100, 0.2, SkillshotLine, {}));
	database.HeimerdingerW = SkillshotManager::RegisterSpell(SP_STRING("Heimerdinger"), W, Skillshot(1325, 100, 2050, 0.25, SkillshotLine, {}));
	database.HeimerdingerE = SkillshotManager::RegisterSpell(SP_STRING("Heimerdinger"), E, Skillshot(970, 250, 1200, 0.25, SkillshotCircle, {}));
	database.HeimerdingerE2 = SkillshotManager::RegisterSpell(SP_STRING("Heimerdinger"), E, Skillshot(970, 250, 1200, 0.25, SkillshotCircle, {}));
	database.IllaoiQ = SkillshotManager::RegisterSpell(SP_STRING("Illaoi"), Q, Skillshot(850, 100, FLT_MAX, 0.75, SkillshotLine, {}));
	database.IllaoiE = SkillshotManager::RegisterSpell(SP_STRING("Illaoi"), E, Skillshot(900, 50, 1900, 0.25, SkillshotLine, {}));
	database.IreliaW = SkillshotManager::RegisterSpell(SP_STRING("Irelia"), W, Skillshot(825, 120, FLT_MAX, 0.25, SkillshotLine, {}));
	database.IreliaR = SkillshotManager::RegisterSpell(SP_STRING("Irelia"), R, Skillshot(950, 160, 2000, 0.4, SkillshotLine, {}));
	database.IvernQ = SkillshotManager::RegisterSpell(SP_STRING("Ivern"), Q, Skillshot(1075, 80, 1300, 0.25, SkillshotLine, {}));
	database.JannaQ = SkillshotManager::RegisterSpell(SP_STRING("Janna"), Q, Skillshot(1750, 100, 667, 0.25, SkillshotLine, {}));
	database.JarvanIVQ = SkillshotManager::RegisterSpell(SP_STRING("JarvanIV"), Q, Skillshot(770, 70, FLT_MAX, 0.4, SkillshotLine, {}));
	database.JarvanIVE = SkillshotManager::RegisterSpell(SP_STRING("JarvanIV"), E, Skillshot(860, 175, 3440, 0, SkillshotCircle, {}));
	database.JayceQ = SkillshotManager::RegisterSpell(SP_STRING("Jayce"), Q, Skillshot(1050, 70, 1450, 0.214, SkillshotLine, {}));
	database.JayceQ2 = SkillshotManager::RegisterSpell(SP_STRING("Jayce"), Q, Skillshot(1600, 115, 2350, 0.152, SkillshotLine, {}));
	database.JhinW = SkillshotManager::RegisterSpell(SP_STRING("Jhin"), W, Skillshot(2550, 40, 5000, 0.75, SkillshotLine, {}));
	database.JhinE = SkillshotManager::RegisterSpell(SP_STRING("Jhin"), E, Skillshot(750, 130, 1600, 0.25, SkillshotCircle, {}));
	database.JhinR = SkillshotManager::RegisterSpell(SP_STRING("Jhin"), R, Skillshot(3500, 80, 5000, 0.25, SkillshotLine, {}));
	database.JinxW = SkillshotManager::RegisterSpell(SP_STRING("Jinx"), W, Skillshot(1450, 60, 3300, max(0.4f, 0.6f - ((globals::localPlayer->ReadClientStat(Object::AttackSpeed) - 1) * 0.25f / 2.5f)), SkillshotLine, { CollidableObjects::Objects }));
	database.JinxE = SkillshotManager::RegisterSpell(SP_STRING("Jinx"), E, Skillshot(900, 80, 1100, 1.5, SkillshotCircle, {}));
	database.JinxR = SkillshotManager::RegisterSpell(SP_STRING("Jinx"), R, Skillshot(12500, 140, 1700, 0.6, SkillshotLine, {}));
	database.KaisaQ = SkillshotManager::RegisterSpell(SP_STRING("Kaisa"), Q, Skillshot(600, 600, FLT_MAX, 0.4, SkillshotNone, {}));
	database.KaisaW = SkillshotManager::RegisterSpell(SP_STRING("Kaisa"), W, Skillshot(3000, 100, 1750, 0.4, SkillshotLine, {CollidableObjects::Objects}));
	database.KaisaE = SkillshotManager::RegisterSpell(SP_STRING("Kaisa"), E, Skillshot(1000, 1000, 1750, 1.2, SkillshotNone, {}));
	database.KaisaR = SkillshotManager::RegisterSpell(SP_STRING("Kaisa"), R, Skillshot(globals::localPlayer->GetSpellBySlotId(R)->GetLevel() == 3 ? 3000 : globals::localPlayer->GetSpellBySlotId(R)->GetLevel() == 2 ? 2250 : 1500, 525, 1750, 0.4, SkillshotNone, {}));
	database.KalistaQ = SkillshotManager::RegisterSpell(SP_STRING("Kalista"), Q, Skillshot(1150, 40, 2400, 0.25, SkillshotLine, {}));
	database.KarmaQ = SkillshotManager::RegisterSpell(SP_STRING("Karma"), Q, Skillshot(950, 60, 1700, 0.25, SkillshotLine, {}));
	database.KarmaQ2 = SkillshotManager::RegisterSpell(SP_STRING("Karma"), Q, Skillshot(950, 80, 1700, 0.25, SkillshotLine, {}));
	database.KarthusQ = SkillshotManager::RegisterSpell(SP_STRING("Karthus"), Q, Skillshot(875, 175, FLT_MAX, 0.9, SkillshotCircle, {}));
	database.KarthusQ2 = SkillshotManager::RegisterSpell(SP_STRING("Karthus"), Q, Skillshot(875, 175, FLT_MAX, 0.9, SkillshotCircle, {}));
	database.KarthusQ3 = SkillshotManager::RegisterSpell(SP_STRING("Karthus"), Q, Skillshot(875, 175, FLT_MAX, 0.9, SkillshotCircle, {}));
	database.KassadinE = SkillshotManager::RegisterSpell(SP_STRING("Kassadin"), E, Skillshot(600, 0, FLT_MAX, 0.3, SkillshotCone, {}));
	database.KassadinR = SkillshotManager::RegisterSpell(SP_STRING("Kassadin"), R, Skillshot(500, 250, FLT_MAX, 0.25, SkillshotCircle, {}));
	database.KayleQ = SkillshotManager::RegisterSpell(SP_STRING("Kayle"), Q, Skillshot(900, 60, 1600, 0.25, SkillshotLine, {}));
	database.KaynW = SkillshotManager::RegisterSpell(SP_STRING("Kayn"), W, Skillshot(700, 90, FLT_MAX, 0.55, SkillshotLine, {}));
	database.KennenQ = SkillshotManager::RegisterSpell(SP_STRING("Kennen"), Q, Skillshot(1050, 50, 1700, 0.175, SkillshotLine, {}));
	database.KhazixW = SkillshotManager::RegisterSpell(SP_STRING("Khazix"), W, Skillshot(1000, 70, 1700, 0.25, SkillshotLine, {}));
	database.KhazixW2 = SkillshotManager::RegisterSpell(SP_STRING("Khazix"), W, Skillshot(1000, 70, 1700, 0.25, SkillshotCone, {}));
	database.KledQ = SkillshotManager::RegisterSpell(SP_STRING("Kled"), Q, Skillshot(800, 45, 1600, 0.25, SkillshotLine, {}));
	database.KledQ2 = SkillshotManager::RegisterSpell(SP_STRING("Kled"), Q, Skillshot(700, 0, 3000, 0.25, SkillshotCone, {}));
	database.KogMawQ = SkillshotManager::RegisterSpell(SP_STRING("KogMaw"), Q, Skillshot(1175, 70, 1650, 0.25, SkillshotLine, {}));
	database.KogMawE = SkillshotManager::RegisterSpell(SP_STRING("KogMaw"), E, Skillshot(1360, 120, 1400, 0.25, SkillshotLine, {}));
	database.KogMawR = SkillshotManager::RegisterSpell(SP_STRING("KogMaw"), R, Skillshot(1300, 200, FLT_MAX, 1.1, SkillshotCircle, {}));
	database.KSanteQ = SkillshotManager::RegisterSpell(SP_STRING("KSante"), Q, Skillshot(465, 75, 1800, 0.25, SkillshotLine, {}));
	database.KSanteQ2 = SkillshotManager::RegisterSpell(SP_STRING("KSante"), Q, Skillshot(750, 70, 1100, 0.34, SkillshotLine, {}));
	database.LeblancE = SkillshotManager::RegisterSpell(SP_STRING("Leblanc"), E, Skillshot(925, 55, 1750, 0.25, SkillshotLine, {}));
	database.LeblancE2 = SkillshotManager::RegisterSpell(SP_STRING("Leblanc"), E, Skillshot(925, 55, 1750, 0.25, SkillshotLine, {}));
	database.LeeSinQ = SkillshotManager::RegisterSpell(SP_STRING("LeeSin"), Q, Skillshot(1100, 60, 1800, 0.25, SkillshotLine, {}));
	database.LeonaE = SkillshotManager::RegisterSpell(SP_STRING("Leona"), E, Skillshot(875, 70, 2000, 0.25, SkillshotLine, {}));
	database.LeonaR = SkillshotManager::RegisterSpell(SP_STRING("Leona"), R, Skillshot(1200, 300, FLT_MAX, 0.85, SkillshotCircle, {}));
	database.LissandraQ = SkillshotManager::RegisterSpell(SP_STRING("Lissandra"), Q, Skillshot(750, 75, 2200, 0.25, SkillshotLine, {}));
	database.LissandraE = SkillshotManager::RegisterSpell(SP_STRING("Lissandra"), E, Skillshot(1025, 125, 850, 0.25, SkillshotLine, {}));
	database.LucianQ = SkillshotManager::RegisterSpell(SP_STRING("Lucian"), Q, Skillshot(900, 65, FLT_MAX, 0.35, SkillshotLine, {}));
	database.LucianW = SkillshotManager::RegisterSpell(SP_STRING("Lucian"), W, Skillshot(900, 80, 1600, 0.25, SkillshotLine, {}));
	database.LuluQ = SkillshotManager::RegisterSpell(SP_STRING("Lulu"), Q, Skillshot(925, 60, 1450, 0.25, SkillshotLine, {}));
	database.LuxQ = SkillshotManager::RegisterSpell(SP_STRING("Lux"), Q, Skillshot(1175, 70, 1200, 0.25, SkillshotLine, {}));
	database.LuxE = SkillshotManager::RegisterSpell(SP_STRING("Lux"), E, Skillshot(1100, 300, 1200, 0.25, SkillshotCircle, {}));
	database.LuxR = SkillshotManager::RegisterSpell(SP_STRING("Lux"), R, Skillshot(3340, 120, FLT_MAX, 1, SkillshotLine, {}));
	database.MalphiteE = SkillshotManager::RegisterSpell(SP_STRING("Malphite"), E, Skillshot(0, 400, FLT_MAX, 0.242, SkillshotCircle, {}));
	database.MalzaharQ = SkillshotManager::RegisterSpell(SP_STRING("Malzahar"), Q, Skillshot(900, 100, 1600, 0.5, SkillshotLine, {}));
	database.MaokaiQ = SkillshotManager::RegisterSpell(SP_STRING("Maokai"), Q, Skillshot(600, 110, 1600, 0.375, SkillshotLine, {}));
	database.MissFortuneR = SkillshotManager::RegisterSpell(SP_STRING("MissFortune"), R, Skillshot(1400, 100, 2000, 0.25, SkillshotCone, {}));
	database.MilioQ = SkillshotManager::RegisterSpell(SP_STRING("Milio"), Q, Skillshot(675, 200, FLT_MAX, 0.4, SkillshotCone, {}));
	database.MilioE = SkillshotManager::RegisterSpell(SP_STRING("Milio"), E, Skillshot(900, 140, FLT_MAX, 0.9, SkillshotCone, {}));
	database.MorganaQ = SkillshotManager::RegisterSpell(SP_STRING("Morgana"), Q, Skillshot(1250, 70, 1200, 0.25, SkillshotLine, {}));
	database.NaafiriQ = SkillshotManager::RegisterSpell(SP_STRING("Naafiri"), Q, Skillshot(875, 180, FLT_MAX, 1, SkillshotCircle, {}));
	database.NaafiriR = SkillshotManager::RegisterSpell(SP_STRING("Naafiri"), R, Skillshot(2750, 250, 850, 0.5, SkillshotLine, {}));
	database.NautilusQ = SkillshotManager::RegisterSpell(SP_STRING("Nautilus"), Q, Skillshot(925, 90, 2000, 0.25, SkillshotLine, {CollidableObjects::Objects, CollidableObjects::Walls}));
	database.NautilusW = SkillshotManager::RegisterSpell(SP_STRING("Nautilus"), W, Skillshot(300, 300, 2000, 0.25, SkillshotNone, {}));
	database.NautilusE = SkillshotManager::RegisterSpell(SP_STRING("Nautilus"), E, Skillshot(350, 350, 2000, 0.25, SkillshotLine, {}));
	database.NautilusR = SkillshotManager::RegisterSpell(SP_STRING("Nautilus"), R, Skillshot(825, 90, 2000, 0.25, SkillshotNone, {}));
	database.NeekoQ = SkillshotManager::RegisterSpell(SP_STRING("Neeko"), Q, Skillshot(800, 200, 1500, 0.25, SkillshotCircle, {}));
	database.NeekoE = SkillshotManager::RegisterSpell(SP_STRING("Neeko"), E, Skillshot(1000, 70, 1300, 0.25, SkillshotLine, {}));
	database.NidaleeQ = SkillshotManager::RegisterSpell(SP_STRING("Nidalee"), Q, Skillshot(1500, 40, 1300, 0.25, SkillshotLine, {}));
	database.NidaleeW = SkillshotManager::RegisterSpell(SP_STRING("Nidalee"), W, Skillshot(900, 85, FLT_MAX, 1.25, SkillshotCircle, {}));
	database.NidaleeE = SkillshotManager::RegisterSpell(SP_STRING("Nidalee"), E, Skillshot(350, 0, FLT_MAX, 0.25, SkillshotCone, {}));
	database.NilahQ = SkillshotManager::RegisterSpell(SP_STRING("Nilah"), Q, Skillshot(600, 150, 500, 0.25, SkillshotLine, {}));
	database.NilahE = SkillshotManager::RegisterSpell(SP_STRING("Nilah"), E, Skillshot(550, 150, 2200, 0.00, SkillshotLine, {}));
	database.NilahR = SkillshotManager::RegisterSpell(SP_STRING("Nilah"), R, Skillshot(0, 450, FLT_MAX, 1.0, SkillshotCircle, {}));
	database.NocturneQ = SkillshotManager::RegisterSpell(SP_STRING("Nocturne"), Q, Skillshot(1200, 60, 1600, 0.25, SkillshotLine, {}));
	database.NunuR = SkillshotManager::RegisterSpell(SP_STRING("Nunu"), R, Skillshot(0, 650, FLT_MAX, 3, SkillshotCircle, {}));
	database.OlafQ = SkillshotManager::RegisterSpell(SP_STRING("Olaf"), Q, Skillshot(1000, 90, 1600, 0.25, SkillshotLine, {}));
	database.OriannaQ = SkillshotManager::RegisterSpell(SP_STRING("Orianna"), Q, Skillshot(825, 80, 1400, 0.25, SkillshotCone, {}));
	database.OrnnQ = SkillshotManager::RegisterSpell(SP_STRING("Ornn"), Q, Skillshot(800, 65, 1800, 0.3, SkillshotLine, {}));
	database.OrnnE = SkillshotManager::RegisterSpell(SP_STRING("Ornn"), E, Skillshot(800, 150, 1600, 0.35, SkillshotLine, {}));
	database.OrnnR = SkillshotManager::RegisterSpell(SP_STRING("Ornn"), R, Skillshot(2500, 200, 1650, 0.5, SkillshotLine, {}));
	database.PantheonQ = SkillshotManager::RegisterSpell(SP_STRING("Pantheon"), Q, Skillshot(575, 80, FLT_MAX, 0.25, SkillshotLine, {}));
	database.PantheonQ2 = SkillshotManager::RegisterSpell(SP_STRING("Pantheon"), Q, Skillshot(1200, 60, 2700, 0.25, SkillshotLine, {}));
	database.PantheonR = SkillshotManager::RegisterSpell(SP_STRING("Pantheon"), R, Skillshot(1350, 250, 2250, 4, SkillshotLine, {}));
	database.PoppyQ = SkillshotManager::RegisterSpell(SP_STRING("Poppy"), Q, Skillshot(430, 100, FLT_MAX, 0.332, SkillshotLine, {}));
	database.PoppyR = SkillshotManager::RegisterSpell(SP_STRING("Poppy"), R, Skillshot(1200, 100, 2000, 0.33, SkillshotLine, {}));
	database.PykeQ = SkillshotManager::RegisterSpell(SP_STRING("Pyke"), Q, Skillshot(400, 70, FLT_MAX, 0.25, SkillshotLine, {}));
	database.PykeQ2 = SkillshotManager::RegisterSpell(SP_STRING("Pyke"), Q, Skillshot(1100, 70, 2000, 0.2, SkillshotLine, {}));
	database.PykeE = SkillshotManager::RegisterSpell(SP_STRING("Pyke"), E, Skillshot(12500, 110, 3000, 0, SkillshotLine, {}));
	database.PykeR = SkillshotManager::RegisterSpell(SP_STRING("Pyke"), R, Skillshot(750, 100, FLT_MAX, 0.5, SkillshotCircle, {}));
	database.QiyanaQ = SkillshotManager::RegisterSpell(SP_STRING("Qiyana"), Q, Skillshot(500, 60, FLT_MAX, 0.25, SkillshotLine, {}));
	database.QiyanaQ2 = SkillshotManager::RegisterSpell(SP_STRING("Qiyana"), Q, Skillshot(925, 70, 1600, 0.25, SkillshotLine, {}));
	database.QiyanaQ3 = SkillshotManager::RegisterSpell(SP_STRING("Qiyana"), Q, Skillshot(925, 70, 1600, 0.25, SkillshotLine, {}));
	database.QiyanaQ4 = SkillshotManager::RegisterSpell(SP_STRING("Qiyana"), Q, Skillshot(925, 70, 1600, 0.25, SkillshotLine, {}));
	database.QiyanaR = SkillshotManager::RegisterSpell(SP_STRING("Qiyana"), R, Skillshot(950, 190, 2000, 0.25, SkillshotLine, {}));
	database.QuinnQ = SkillshotManager::RegisterSpell(SP_STRING("Quinn"), Q, Skillshot(1025, 60, 1550, 0.25, SkillshotLine, {}));
	database.RakanQ = SkillshotManager::RegisterSpell(SP_STRING("Rakan"), Q, Skillshot(850, 65, 1850, 0.25, SkillshotLine, {}));
	database.RakanW = SkillshotManager::RegisterSpell(SP_STRING("Rakan"), W, Skillshot(650, 265, FLT_MAX, 0.7, SkillshotCircle, {}));
	database.RekSaiQ = SkillshotManager::RegisterSpell(SP_STRING("RekSai"), Q, Skillshot(1625, 65, 1950, 0.125, SkillshotLine, {}));
	database.RellQ = SkillshotManager::RegisterSpell(SP_STRING("Rell"), Q, Skillshot(685, 80, FLT_MAX, 0.35, SkillshotLine, {}));
	database.RellW = SkillshotManager::RegisterSpell(SP_STRING("Rell"), W, Skillshot(500, 200, FLT_MAX, 0.625, SkillshotLine, {}));
	database.RellE = SkillshotManager::RegisterSpell(SP_STRING("Rell"), E, Skillshot(1500, 250, FLT_MAX, 0.35, SkillshotLine, {}));
	database.RellR = SkillshotManager::RegisterSpell(SP_STRING("Rell"), R, Skillshot(0, 400, FLT_MAX, 0.25, SkillshotCircle, {}));
	database.RengarE = SkillshotManager::RegisterSpell(SP_STRING("Rengar"), E, Skillshot(1000, 70, 1500, 0.25, SkillshotLine, {}));
	database.RivenR = SkillshotManager::RegisterSpell(SP_STRING("Riven"), R, Skillshot(900, 0, 1600, 0.25, SkillshotCone, {}));
	database.RumbleE = SkillshotManager::RegisterSpell(SP_STRING("Rumble"), E, Skillshot(850, 60, 2000, 0.25, SkillshotLine, {}));
	database.RyzeQ = SkillshotManager::RegisterSpell(SP_STRING("Ryze"), Q, Skillshot(1000, 55, 1700, 0.25, SkillshotLine, {}));
	database.SemiraQ = SkillshotManager::RegisterSpell(SP_STRING("Semira"), Q, Skillshot(1000, 60, 2600, 0.25, SkillshotLine, {}));
	database.SejuaniR = SkillshotManager::RegisterSpell(SP_STRING("Sejuani"), R, Skillshot(1300, 120, 1600, 0.25, SkillshotLine, {}));
	database.SennaQ = SkillshotManager::RegisterSpell(SP_STRING("Senna"), Q, Skillshot(1100, 80, FLT_MAX, 0.4, SkillshotLine, {}));
	database.SennaW = SkillshotManager::RegisterSpell(SP_STRING("Senna"), W, Skillshot(1300, 60, 1150, 0.25, SkillshotLine, {CollidableObjects::Objects}));
	database.SennaE = SkillshotManager::RegisterSpell(SP_STRING("Senna"), E, Skillshot(globals::localPlayer->GetRealAttackRange(), globals::localPlayer->GetRealAttackRange(), 1750, 0.25, SkillshotNone, {}));
	database.SennaR = SkillshotManager::RegisterSpell(SP_STRING("Senna"), R, Skillshot(12500, 180, 20000, 1, SkillshotLine, {}));
	database.SeraphineQ = SkillshotManager::RegisterSpell(SP_STRING("Seraphine"), Q, Skillshot(900, 350, 1200, 0.25, SkillshotCircle, {}));
	database.SeraphineE = SkillshotManager::RegisterSpell(SP_STRING("Seraphine"), E, Skillshot(1300, 70, 1200, 0.25, SkillshotLine, {}));
	database.SeraphineR = SkillshotManager::RegisterSpell(SP_STRING("Seraphine"), R, Skillshot(1300, 160, 1600, 0.5, SkillshotLine, {}));
	database.SettW = SkillshotManager::RegisterSpell(SP_STRING("Sett"), W, Skillshot(790, 160, FLT_MAX, 0.75, SkillshotCone, {}));
	database.SettE = SkillshotManager::RegisterSpell(SP_STRING("Sett"), E, Skillshot(490, 175, FLT_MAX, 0.25, SkillshotCone, {}));
	database.ShyvanaE = SkillshotManager::RegisterSpell(SP_STRING("Shyvana"), E, Skillshot(925, 60, 1575, 0.25, SkillshotLine, {}));
	database.ShyvanaE2 = SkillshotManager::RegisterSpell(SP_STRING("Shyvana"), E, Skillshot(975, 60, 1575, 0.333, SkillshotLine, {}));
	database.ShyvanaR = SkillshotManager::RegisterSpell(SP_STRING("Shyvana"), R, Skillshot(850, 150, 700, 0.25, SkillshotLine, {}));
	database.SionQ = SkillshotManager::RegisterSpell(SP_STRING("Sion"), Q, Skillshot(750, 150, FLT_MAX, 2, SkillshotLine, {}));
	database.SionE = SkillshotManager::RegisterSpell(SP_STRING("Sion"), E, Skillshot(800, 80, 1800, 0.25, SkillshotLine, {}));
	database.SivirQ = SkillshotManager::RegisterSpell(SP_STRING("Sivir"), Q, Skillshot(1250, 90, 1350, 0.25, SkillshotLine, {}));
	database.SkarnerE = SkillshotManager::RegisterSpell(SP_STRING("Skarner"), E, Skillshot(1000, 70, 1500, 0.25, SkillshotLine, {}));
	database.SonaR = SkillshotManager::RegisterSpell(SP_STRING("Sona"), R, Skillshot(1000, 140, 2400, 0.25, SkillshotLine, {}));
	database.SorakaQ = SkillshotManager::RegisterSpell(SP_STRING("Soraka"), Q, Skillshot(810, 235, 1150, 0.25, SkillshotCircle, {}));
	database.SwainQ = SkillshotManager::RegisterSpell(SP_STRING("Swain"), Q, Skillshot(725, 0, 5000, 0.25, SkillshotCone, {}));
	database.SwainW = SkillshotManager::RegisterSpell(SP_STRING("Swain"), W, Skillshot(3500, 300, FLT_MAX, 1.5, SkillshotCircle, {}));
	database.SwainE = SkillshotManager::RegisterSpell(SP_STRING("Swain"), E, Skillshot(850, 85, 1800, 0.25, SkillshotLine, {}));
	database.SylasQ = SkillshotManager::RegisterSpell(SP_STRING("Sylas"), Q, Skillshot(775, 45, FLT_MAX, 0.4, SkillshotCone, {}));
	database.SylasE = SkillshotManager::RegisterSpell(SP_STRING("Sylas"), E, Skillshot(850, 60, 1600, 0.25, SkillshotLine, {}));
	database.SyndraQ = SkillshotManager::RegisterSpell(SP_STRING("Syndra"), Q, Skillshot(800, 200, FLT_MAX, 0.625, SkillshotCircle, {}));
	database.SyndraE = SkillshotManager::RegisterSpell(SP_STRING("Syndra"), E, Skillshot(700, 0, 1600, 0.25, SkillshotCone, {}));
	database.SyndraE2 = SkillshotManager::RegisterSpell(SP_STRING("Syndra"), E, Skillshot(1250, 100, 2000, 0.25, SkillshotLine, {}));
	database.TahmKenchQ = SkillshotManager::RegisterSpell(SP_STRING("TahmKench"), Q, Skillshot(900, 70, 2800, 0.25, SkillshotLine, {}));
	database.TaliyahQ = SkillshotManager::RegisterSpell(SP_STRING("Taliyah"), Q, Skillshot(1000, 100, 3600, 0.25, SkillshotLine, {}));
	database.TaliyahW = SkillshotManager::RegisterSpell(SP_STRING("Taliyah"), W, Skillshot(900, 150, FLT_MAX, 0.85, SkillshotCircle, {}));
	database.TaliyahE = SkillshotManager::RegisterSpell(SP_STRING("Taliyah"), E, Skillshot(800, 0, 2000, 0.45, SkillshotCone, {}));
	database.TaliyahR = SkillshotManager::RegisterSpell(SP_STRING("Taliyah"), R, Skillshot(3000, 120, 1700, 1, SkillshotLine, {}));
	database.TalonW = SkillshotManager::RegisterSpell(SP_STRING("Talon"), W, Skillshot(650, 75, 2500, 0.25, SkillshotCone, {}));
	database.TwitchQ = SkillshotManager::RegisterSpell(SP_STRING("Twitch"), Q, Skillshot(globals::localPlayer->GetRealAttackRange(), globals::localPlayer->GetRealAttackRange(), globals::localPlayer->ReadClientStat(Object::AttackSpeed), 0.5, SkillshotNone, { }));
	database.TwitchW = SkillshotManager::RegisterSpell(SP_STRING("Twitch"), W, Skillshot(950, 100, 1500, 3.00, SkillshotCircle, {}));
	database.TwitchE = SkillshotManager::RegisterSpell(SP_STRING("Twitch"), E, Skillshot(1200, 110, 2000, 0.25, SkillshotNone, {}));
	database.TwitchR = SkillshotManager::RegisterSpell(SP_STRING("Twitch"), R, Skillshot(globals::localPlayer->GetRealAttackRange() + 300.0f, globals::localPlayer->GetRealAttackRange() + 300.0f, 5000, 0, SkillshotNone, {}));
	database.ThreshQ = SkillshotManager::RegisterSpell(SP_STRING("Thresh"), Q, Skillshot(1100, 70, 1900, 0.5, SkillshotLine, { CollidableObjects::Objects }));
	database.ThreshW = SkillshotManager::RegisterSpell(SP_STRING("Thresh"), W, Skillshot(950, 150, 1900, 0.5, SkillshotLine, {}));
	database.ThreshE = SkillshotManager::RegisterSpell(SP_STRING("Thresh"), E, Skillshot(500, 110, 2000, 0.389, SkillshotLine, {}));
	database.ThreshR = SkillshotManager::RegisterSpell(SP_STRING("Thresh"), R, Skillshot(470, 470, 1900, 0.45, SkillshotCircle, {}));
	database.TristanaW = SkillshotManager::RegisterSpell(SP_STRING("Tristana"), W, Skillshot(900, 300, 1100, 0.25, SkillshotCircle, {}));
	database.TryndamereE = SkillshotManager::RegisterSpell(SP_STRING("Tryndamere"), E, Skillshot(660, 225, 1300, 0, SkillshotLine, {}));
	database.TwistedFateQ = SkillshotManager::RegisterSpell(SP_STRING("TwistedFate"), Q, Skillshot(1450, 40, 1000, 0.25, SkillshotCone, {}));
	database.UrgotQ = SkillshotManager::RegisterSpell(SP_STRING("Urgot"), Q, Skillshot(800, 180, FLT_MAX, 0.6, SkillshotCircle, {}));
	database.UrgotE = SkillshotManager::RegisterSpell(SP_STRING("Urgot"), E, Skillshot(475, 100, 1540, 0.45, SkillshotLine, {}));
	database.UrgotR = SkillshotManager::RegisterSpell(SP_STRING("Urgot"), R, Skillshot(1600, 80, 3200, 0.5, SkillshotLine, {}));
	database.VarusQ = SkillshotManager::RegisterSpell(SP_STRING("Varus"), Q, Skillshot(1525, 70, 1900, 0.25, SkillshotLine, {}));
	database.VarusE = SkillshotManager::RegisterSpell(SP_STRING("Varus"), E, Skillshot(925, 260, 1500, 0.242, SkillshotCircle, {}));
	database.VarusR = SkillshotManager::RegisterSpell(SP_STRING("Varus"), R, Skillshot(1200, 120, 1500, 0.25, SkillshotLine, {}));
	database.VayneQ = SkillshotManager::RegisterSpell(SP_STRING("Vayne"), Q, Skillshot(300, 70, 2200, 0, SkillshotLine, {}));
	database.VayneE = SkillshotManager::RegisterSpell(SP_STRING("Vayne"), E, Skillshot(550, 70, 2000, 0.25, SkillshotNone, {}));
	database.VayneR = SkillshotManager::RegisterSpell(SP_STRING("Vayne"), R, Skillshot(globals::localPlayer->GetRealAttackRange(), globals::localPlayer->GetRealAttackRange(), 2000, 0.25, SkillshotNone, {}));
	database.VeigarQ = SkillshotManager::RegisterSpell(SP_STRING("Veigar"), Q, Skillshot(900, 70, 2200, 0.25, SkillshotLine, {}));
	database.VeigarW = SkillshotManager::RegisterSpell(SP_STRING("Veigar"), W, Skillshot(900, 200, FLT_MAX, 1.25, SkillshotCircle, {}));
	database.VexQ = SkillshotManager::RegisterSpell(SP_STRING("Vex"), Q, Skillshot(1200, 80, 2200, 0.15, SkillshotCone, {}));
	database.VelkozQ = SkillshotManager::RegisterSpell(SP_STRING("Velkoz"), Q, Skillshot(1100, 45, 2100, 0.25, SkillshotLine, {}));
	database.VelkozQ2 = SkillshotManager::RegisterSpell(SP_STRING("Velkoz"), Q, Skillshot(1050, 50, 1300, 0.25, SkillshotLine, {}));
	database.VelkozW = SkillshotManager::RegisterSpell(SP_STRING("Velkoz"), W, Skillshot(1050, 87.5, 1700, 0.25, SkillshotLine, {}));
	database.VelkozE = SkillshotManager::RegisterSpell(SP_STRING("Velkoz"), E, Skillshot(800, 185, FLT_MAX, 0.8, SkillshotCircle, {}));
	database.ViQ = SkillshotManager::RegisterSpell(SP_STRING("Vi"), Q, Skillshot(725, 90, 1500, 0, SkillshotLine, {}));
	database.ViegoW = SkillshotManager::RegisterSpell(SP_STRING("Viego"), W, Skillshot(760, 90, 1300, 0, SkillshotLine, {}));
	database.ViktorW = SkillshotManager::RegisterSpell(SP_STRING("Viktor"), W, Skillshot(800, 270, FLT_MAX, 1.75, SkillshotCircle, {}));
	database.ViktorE = SkillshotManager::RegisterSpell(SP_STRING("Viktor"), E, Skillshot(700, 80, 1050, 0.25, SkillshotLine, {}));
	database.WarwickR = SkillshotManager::RegisterSpell(SP_STRING("Warwick"), R, Skillshot(3000, 55, 1800, 0.1, SkillshotLine, {}));
	database.XayahQ = SkillshotManager::RegisterSpell(SP_STRING("Xayah"), Q, Skillshot(1100, 45, 2075, 0.5, SkillshotLine, {}));
	database.XerathQ = SkillshotManager::RegisterSpell(SP_STRING("Xerath"), Q, Skillshot(1450, 75, FLT_MAX, 0.65, SkillshotLine, {}));
	database.XerathW = SkillshotManager::RegisterSpell(SP_STRING("Xerath"), W, Skillshot(1000, 235, FLT_MAX, 0.75, SkillshotCircle, {}));
	database.XerathE = SkillshotManager::RegisterSpell(SP_STRING("Xerath"), E, Skillshot(1125, 120, 1400, 0.25, SkillshotLine, {CollidableObjects::Objects}));
	database.XerathR = SkillshotManager::RegisterSpell(SP_STRING("Xerath"), R, Skillshot(5000, 200, FLT_MAX, 0.7, SkillshotCircle, {}));
	database.XinZhaoW = SkillshotManager::RegisterSpell(SP_STRING("XinZhao"), W, Skillshot(900, 40, 5000, 0.5, SkillshotLine, {}));
	database.YasuoQ = SkillshotManager::RegisterSpell(SP_STRING("Yasuo"), Q, Skillshot(475, 40, 1500, 0.25, SkillshotLine, {}));
	database.YasuoQ2 = SkillshotManager::RegisterSpell(SP_STRING("Yasuo"), Q, Skillshot(475, 40, 1500, 0.25, SkillshotLine, {}));
	database.YasuoQ3 = SkillshotManager::RegisterSpell(SP_STRING("Yasuo"), Q, Skillshot(1100, 90, 1200, 0.03, SkillshotLine, {}));
	database.YoneQ = SkillshotManager::RegisterSpell(SP_STRING("Yone"), Q, Skillshot(450, 40, FLT_MAX, 0.25, SkillshotLine, {}));
	database.YoneQ2 = SkillshotManager::RegisterSpell(SP_STRING("Yone"), Q, Skillshot(1050, 80, 1500, 0.25, SkillshotLine, {}));
	database.YoneW = SkillshotManager::RegisterSpell(SP_STRING("Yone"), W, Skillshot(600, 0, FLT_MAX, 0.375, SkillshotCone, {}));
	database.YoneR = SkillshotManager::RegisterSpell(SP_STRING("Yone"), R, Skillshot(1000, 112.5, FLT_MAX, 0.75, SkillshotLine, {}));
	database.YorickQ = SkillshotManager::RegisterSpell(SP_STRING("Yorick"), Q, Skillshot(250, 250, FLT_MAX, 0.15, SkillshotNone, {}));
	database.YorickW = SkillshotManager::RegisterSpell(SP_STRING("Yorick"), W, Skillshot(600, 200, FLT_MAX, 0.75, SkillshotCircle, {}));
	database.YorickE = SkillshotManager::RegisterSpell(SP_STRING("Yorick"), E, Skillshot(1100, 250, FLT_MAX, 0.33, SkillshotLine, {}));
	database.YorickR = SkillshotManager::RegisterSpell(SP_STRING("Yorick"), R, Skillshot(600, 250, FLT_MAX, 0.50, SkillshotCircle, {}));
	database.ZacQ = SkillshotManager::RegisterSpell(SP_STRING("Zac"), Q, Skillshot(800, 120, 2800, 0.33, SkillshotLine, {}));
	database.ZedQ = SkillshotManager::RegisterSpell(SP_STRING("Zed"), Q, Skillshot(900, 50, 1700, 0.25, SkillshotLine, {}));
	database.ZeriQ = SkillshotManager::RegisterSpell(SP_STRING("Zeri"), Q, Skillshot(840, 80, 1500, 0.25, SkillshotLine, {}));
	database.ZiggsQ = SkillshotManager::RegisterSpell(SP_STRING("Ziggs"), Q, Skillshot(850, 150, 1750, 0.25, SkillshotCone, {}));
	database.ZiggsW = SkillshotManager::RegisterSpell(SP_STRING("Ziggs"), W, Skillshot(1000, 240, 1750, 0.25, SkillshotCircle, {}));
	database.ZiggsE = SkillshotManager::RegisterSpell(SP_STRING("Ziggs"), E, Skillshot(900, 250, 1800, 0.25, SkillshotCircle, {}));
	database.ZiggsR = SkillshotManager::RegisterSpell(SP_STRING("Ziggs"), R, Skillshot(5000, 480, 1550, 0.375, SkillshotCircle, {}));
	database.ZileanQ = SkillshotManager::RegisterSpell(SP_STRING("Zilean"), Q, Skillshot(900, 150, FLT_MAX, 0.8, SkillshotCircle, {}));
	database.ZoeQ = SkillshotManager::RegisterSpell(SP_STRING("Zoe"), Q, Skillshot(800, 50, 1200, 0.25, SkillshotLine, {}));
	database.ZoeQ2 = SkillshotManager::RegisterSpell(SP_STRING("Zoe"), Q, Skillshot(1600, 70, 2500, 0, SkillshotLine, {}));
	database.ZoeE = SkillshotManager::RegisterSpell(SP_STRING("Zoe"), E, Skillshot(800, 50, 1700, 0.3, SkillshotLine, {}));
	database.ZyraQ = SkillshotManager::RegisterSpell(SP_STRING("Zyra"), Q, Skillshot(800, 200, FLT_MAX, 0.825, SkillshotLine, {}));
	database.ZyraE = SkillshotManager::RegisterSpell(SP_STRING("Zyra"), E, Skillshot(1100, 70, 1150, 0.25, SkillshotLine, {}));
	database.ZyraR = SkillshotManager::RegisterSpell(SP_STRING("Zyra"), R, Skillshot(700, 500, FLT_MAX, 2, SkillshotCircle, {}));
	LOG("Skills registered");
}
