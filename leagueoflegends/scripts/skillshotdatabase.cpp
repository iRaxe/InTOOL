#include "../stdafx.h"

void Skillshot::SetSlotIndex(int slotId)
{
	this->slotIndex = slotId;
}

float Skillshot::GetRange()
{
	return this->range;
}

float Skillshot::GetRadius()
{
	return this->radius;
}

float Skillshot::GetSpeed()
{
	return this->speed;
}

float Skillshot::GetCastTime()
{
	return this->castTime;
}

int Skillshot::GetType()
{
	return this->type;
}

bool Skillshot::IsCollidableWith(int type)
{
	return std::find(this->collidableWith.begin(), this->collidableWith.end(), type) != this->collidableWith.end();
}

float Skillshot::GetMaxRange()
{
	return (this->type == SkillshotType::SkillshotCircle) ? this->range + (this->radius - min(this->radius, 70.0f)) : this->range;
}

std::string Skillshot::GetName()
{
	return globals::localPlayer->GetSpellBySlotId(this->slotIndex)->GetName();
}

bool Skillshot::IsCastable()
{
	return globals::localPlayer->CanCastSpell(this->slotIndex);
}

bool Skillshot::IsLearned()
{
	return functions::GetSpellState(this->slotIndex) != SpellState::IsNotLearned;
}

bool Skillshot::IsProcessing()
{
	return functions::GetSpellState(this->slotIndex) == SpellState::IsProcessing;
}

int Skillshot::GetStacks()
{
	return globals::localPlayer->GetSpellBySlotId(this->slotIndex)->GetStacks();
}

void Skillshot::PopulateSpellsDB()
{
	database.AatroxQ = SkillshotManager::RegisterSpell(SP_STRING("Aatrox"), SpellIndex::Q, Skillshot(650, 130, FLT_MAX, 0.6, SkillshotType::SkillshotLine, {}));
	database.AatroxQ2 = SkillshotManager::RegisterSpell(SP_STRING("Aatrox"), SpellIndex::Q, Skillshot(500, 200, FLT_MAX, 0.6, SkillshotType::SkillshotCone, {}));
	database.AatroxQ3 = SkillshotManager::RegisterSpell(SP_STRING("Aatrox"), SpellIndex::Q, Skillshot(200, 300, FLT_MAX, 0.6, SkillshotType::SkillshotCircle, {}));
	database.AatroxW = SkillshotManager::RegisterSpell(SP_STRING("Aatrox"), SpellIndex::W, Skillshot(825, 80, 1800, 0.25, SkillshotType::SkillshotLine, {}));
	database.AhriQ = SkillshotManager::RegisterSpell(SP_STRING("Ahri"), SpellIndex::Q, Skillshot(880, 100, 2500, 0.25, SkillshotType::SkillshotLine, {}));
	database.AhriE = SkillshotManager::RegisterSpell(SP_STRING("Ahri"), SpellIndex::E, Skillshot(975, 60, 1500, 0.25, SkillshotType::SkillshotLine, {}));
	database.AkaliQ = SkillshotManager::RegisterSpell(SP_STRING("Akali"), SpellIndex::Q, Skillshot(550, 60, 3200, 0.25, SkillshotType::SkillshotCone, {}));
	database.AkaliE = SkillshotManager::RegisterSpell(SP_STRING("Akali"), SpellIndex::E, Skillshot(825, 70, 1800, 0.25, SkillshotType::SkillshotLine, {}));
	database.AkaliR = SkillshotManager::RegisterSpell(SP_STRING("Akali"), SpellIndex::R, Skillshot(675, 65, 1800, 0, SkillshotType::SkillshotLine, {}));
	database.AkaliR2 = SkillshotManager::RegisterSpell(SP_STRING("Akali"), SpellIndex::R, Skillshot(525, 65, 3600, 0, SkillshotType::SkillshotLine, {}));
	database.AlistarQ = SkillshotManager::RegisterSpell(SP_STRING("Alistar"), SpellIndex::Q, Skillshot(0, 365, FLT_MAX, 0.25, SkillshotType::SkillshotCircle, {}));
	database.AmumuQ = SkillshotManager::RegisterSpell(SP_STRING("Amumu"), SpellIndex::Q, Skillshot(1100, 80, 2000, 0.25, SkillshotType::SkillshotLine, {}));
	database.AmumuR = SkillshotManager::RegisterSpell(SP_STRING("Amumu"), SpellIndex::R, Skillshot(0, 550, FLT_MAX, 0.25, SkillshotType::SkillshotCircle, {}));
	database.AniviaQ = SkillshotManager::RegisterSpell(SP_STRING("Anivia"), SpellIndex::Q, Skillshot(1100, 110, 950, 0.25, SkillshotType::SkillshotLine, {}));
	database.AnnieW = SkillshotManager::RegisterSpell(SP_STRING("Annie"), SpellIndex::W, Skillshot(600, 0, FLT_MAX, 0.25, SkillshotType::SkillshotCone, {}));
	database.AnnieR = SkillshotManager::RegisterSpell(SP_STRING("Annie"), SpellIndex::R, Skillshot(600, 290, FLT_MAX, 0.25, SkillshotType::SkillshotCircle, {}));
	database.ApheliosQ = SkillshotManager::RegisterSpell(SP_STRING("Aphelios"), SpellIndex::Q, Skillshot(1450, 60, 1850, 0.35, SkillshotType::SkillshotLine, {}));
	database.ApheliosQ2 = SkillshotManager::RegisterSpell(SP_STRING("Aphelios"), SpellIndex::Q, Skillshot(850, 65, 1500, 0.25, SkillshotType::SkillshotCone, {}));
	database.ApheliosR = SkillshotManager::RegisterSpell(SP_STRING("Aphelios"), SpellIndex::R, Skillshot(1600, 125, 2050, 0.5, SkillshotType::SkillshotLine, {}));
	database.AsheW = SkillshotManager::RegisterSpell(SP_STRING("Ashe"), SpellIndex::W, Skillshot(1200, 20, 2000, 0.25, SkillshotType::SkillshotCone, { CollidableObjects::Objects }));
	database.AsheR = SkillshotManager::RegisterSpell(SP_STRING("Ashe"), SpellIndex::R, Skillshot(12500, 130, 1600, 0.25, SkillshotType::SkillshotLine, {}));
	database.AurelionSolQ = SkillshotManager::RegisterSpell(SP_STRING("AurelionSol"), SpellIndex::Q, Skillshot(1075, 110, 850, 0, SkillshotType::SkillshotLine, {}));
	database.AurelionSolR = SkillshotManager::RegisterSpell(SP_STRING("AurelionSol"), SpellIndex::R, Skillshot(1500, 120, 4500, 0.35, SkillshotType::SkillshotLine, {}));
	database.AzirR = SkillshotManager::RegisterSpell(SP_STRING("Azir"), SpellIndex::R, Skillshot(500, 250, 1400, 0.3, SkillshotType::SkillshotLine, {}));
	database.BelVethQ = SkillshotManager::RegisterSpell(SP_STRING("BelVeth"), SpellIndex::Q, Skillshot(450, 100, 1200, 0.0, SkillshotType::SkillshotLine, {}));
	database.BelVethW = SkillshotManager::RegisterSpell(SP_STRING("BelVeth"), SpellIndex::W, Skillshot(715, 200, 500, 0.5, SkillshotType::SkillshotLine, {}));
	database.BelVethE = SkillshotManager::RegisterSpell(SP_STRING("BelVeth"), SpellIndex::E, Skillshot(0.0, 500, FLT_MAX, 1.5, SkillshotType::SkillshotCircle, {}));
	database.BelVethR = SkillshotManager::RegisterSpell(SP_STRING("BelVeth"), SpellIndex::R, Skillshot(275, 500, FLT_MAX, 1.0, SkillshotType::SkillshotCircle, {}));
	database.BardQ = SkillshotManager::RegisterSpell(SP_STRING("Bard"), SpellIndex::Q, Skillshot(950, 60, 1500, 0.25, SkillshotType::SkillshotLine, {}));
	database.BardR = SkillshotManager::RegisterSpell(SP_STRING("Bard"), SpellIndex::R, Skillshot(3400, 350, 2100, 0.5, SkillshotType::SkillshotCircle, {}));
	database.BlitzcrankQ = SkillshotManager::RegisterSpell(SP_STRING("Blitzcrank"), SpellIndex::Q, Skillshot(1150, 70, 1800, 0.25, SkillshotType::SkillshotLine, {}));
	database.BlitzcrankR = SkillshotManager::RegisterSpell(SP_STRING("Blitzcrank"), SpellIndex::R, Skillshot(0, 600, FLT_MAX, 0.25, SkillshotType::SkillshotCircle, {}));
	database.BrandQ = SkillshotManager::RegisterSpell(SP_STRING("Brand"), SpellIndex::Q, Skillshot(1050, 60, 1600, 0.25, SkillshotType::SkillshotLine, {}));
	database.BrandW = SkillshotManager::RegisterSpell(SP_STRING("Brand"), SpellIndex::W, Skillshot(900, 250, FLT_MAX, 0.85, SkillshotType::SkillshotCircle, {}));
	database.BraumQ = SkillshotManager::RegisterSpell(SP_STRING("Braum"), SpellIndex::Q, Skillshot(1000, 70, 1700, 0.25, SkillshotType::SkillshotLine, {}));
	database.BraumR = SkillshotManager::RegisterSpell(SP_STRING("Braum"), SpellIndex::R, Skillshot(1250, 115, 1400, 0.5, SkillshotType::SkillshotLine, {}));
	database.CaitlynQ = SkillshotManager::RegisterSpell(SP_STRING("Caitlyn"), SpellIndex::Q, Skillshot(1250, 90, 2200, 0.625, SkillshotType::SkillshotLine, {}));
	database.CaitlynW = SkillshotManager::RegisterSpell(SP_STRING("Caitlyn"), SpellIndex::W, Skillshot(800, 75, FLT_MAX, 0.35, SkillshotType::SkillshotCircle, {}));
	database.CaitlynE = SkillshotManager::RegisterSpell(SP_STRING("Caitlyn"), SpellIndex::E, Skillshot(750, 70, 1600, 0.15, SkillshotType::SkillshotLine, {}));
	database.CamilleE = SkillshotManager::RegisterSpell(SP_STRING("Camille"), SpellIndex::E, Skillshot(800, 60, 1900, 0, SkillshotType::SkillshotLine, {}));
	database.CamilleE2 = SkillshotManager::RegisterSpell(SP_STRING("Camille"), SpellIndex::E, Skillshot(400, 60, 1900, 0, SkillshotType::SkillshotLine, {}));
	database.CassiopeiaQ = SkillshotManager::RegisterSpell(SP_STRING("Cassiopeia"), SpellIndex::Q, Skillshot(850, 150, FLT_MAX, 0.75, SkillshotType::SkillshotCircle, {}));
	database.CassiopeiaW = SkillshotManager::RegisterSpell(SP_STRING("Cassiopeia"), SpellIndex::W, Skillshot(800, 160, 2500, 0.75, SkillshotType::SkillshotCircle, {}));
	database.CassiopeiaR = SkillshotManager::RegisterSpell(SP_STRING("Cassiopeia"), SpellIndex::R, Skillshot(825, 0, FLT_MAX, 0.5, SkillshotType::SkillshotCone, {}));
	database.ChogathQ = SkillshotManager::RegisterSpell(SP_STRING("Chogath"), SpellIndex::Q, Skillshot(950, 250, FLT_MAX, 1.2, SkillshotType::SkillshotCircle, {}));
	database.ChogathW = SkillshotManager::RegisterSpell(SP_STRING("Chogath"), SpellIndex::W, Skillshot(650, 0, FLT_MAX, 0.5, SkillshotType::SkillshotCone, {}));
	database.CorkiQ = SkillshotManager::RegisterSpell(SP_STRING("Corki"), SpellIndex::Q, Skillshot(825, 250, 1000, 0.25, SkillshotType::SkillshotCircle, {}));
	database.CorkiR = SkillshotManager::RegisterSpell(SP_STRING("Corki"), SpellIndex::R, Skillshot(1300, 40, 2000, 0.175, SkillshotType::SkillshotLine, {}));
	database.CorkiR2 = SkillshotManager::RegisterSpell(SP_STRING("Corki"), SpellIndex::R, Skillshot(1500, 40, 2000, 0.175, SkillshotType::SkillshotLine, {}));
	database.DianaQ = SkillshotManager::RegisterSpell(SP_STRING("Diana"), SpellIndex::Q, Skillshot(900, 185, 1900, 0.25, SkillshotType::SkillshotCircle, {}));
	database.DravenE = SkillshotManager::RegisterSpell(SP_STRING("Draven"), SpellIndex::E, Skillshot(1050, 130, 1600, 0.25, SkillshotType::SkillshotLine, {}));
	database.DravenR = SkillshotManager::RegisterSpell(SP_STRING("Draven"), SpellIndex::R, Skillshot(12500, 160, 2000, 0.25, SkillshotType::SkillshotLine, {}));
	database.DrMundoQ = SkillshotManager::RegisterSpell(SP_STRING("DrMundo"), SpellIndex::Q, Skillshot(990, 120, 2000, 0.25, SkillshotType::SkillshotLine, {}));
	database.EkkoQ = SkillshotManager::RegisterSpell(SP_STRING("Ekko"), SpellIndex::Q, Skillshot(1175, 60, 1650, 0.25, SkillshotType::SkillshotLine, {}));
	database.EkkoW = SkillshotManager::RegisterSpell(SP_STRING("Ekko"), SpellIndex::W, Skillshot(1600, 400, FLT_MAX, 3.35, SkillshotType::SkillshotCircle, {}));
	database.EliseE = SkillshotManager::RegisterSpell(SP_STRING("Elise"), SpellIndex::E, Skillshot(1075, 55, 1600, 0.25, SkillshotType::SkillshotLine, {}));
	database.EvelynnQ = SkillshotManager::RegisterSpell(SP_STRING("Evelynn"), SpellIndex::Q, Skillshot(800, 60, 2400, 0.25, SkillshotType::SkillshotLine, {}));
	database.EvelynnR = SkillshotManager::RegisterSpell(SP_STRING("Evelynn"), SpellIndex::R, Skillshot(450, 180, FLT_MAX, 0.35, SkillshotType::SkillshotCone, {}));
	database.EzrealQ = SkillshotManager::RegisterSpell(SP_STRING("Ezreal"), SpellIndex::Q, Skillshot(1200, 60, 2000, 0.25, SkillshotType::SkillshotLine, { CollidableObjects::Objects }));
	database.EzrealW = SkillshotManager::RegisterSpell(SP_STRING("Ezreal"), SpellIndex::W, Skillshot(1200, 60, 1700, 0.25, SkillshotType::SkillshotLine, {}));
	database.EzrealE = SkillshotManager::RegisterSpell(SP_STRING("Ezreal"), SpellIndex::E, Skillshot(750, 60, 2000, 0.25, SkillshotType::SkillshotNone, {}));
	database.EzrealR = SkillshotManager::RegisterSpell(SP_STRING("Ezreal"), SpellIndex::R, Skillshot(12500, 160, 2000, 1, SkillshotType::SkillshotLine, {}));
	database.FioraW = SkillshotManager::RegisterSpell(SP_STRING("Fiora"), SpellIndex::W, Skillshot(750, 70, 3200, 0.75, SkillshotType::SkillshotLine, {}));
	database.FizzR = SkillshotManager::RegisterSpell(SP_STRING("Fizz"), SpellIndex::R, Skillshot(1300, 150, 1300, 0.25, SkillshotType::SkillshotLine, {}));
	database.GalioQ = SkillshotManager::RegisterSpell(SP_STRING("Galio"), SpellIndex::Q, Skillshot(825, 235, 1150, 0.25, SkillshotType::SkillshotCircle, {}));
	database.GalioE = SkillshotManager::RegisterSpell(SP_STRING("Galio"), SpellIndex::E, Skillshot(650, 160, 2300, 0.4, SkillshotType::SkillshotLine, {}));
	database.GnarQ = SkillshotManager::RegisterSpell(SP_STRING("Gnar"), SpellIndex::Q, Skillshot(1125, 55, 2500, 0.25, SkillshotType::SkillshotLine, {}));
	database.GnarQ2 = SkillshotManager::RegisterSpell(SP_STRING("Gnar"), SpellIndex::Q, Skillshot(1125, 90, 2100, 0.5, SkillshotType::SkillshotLine, {}));
	database.GnarW = SkillshotManager::RegisterSpell(SP_STRING("Gnar"), SpellIndex::W, Skillshot(575, 100, FLT_MAX, 0.6, SkillshotType::SkillshotLine, {}));
	database.GnarR = SkillshotManager::RegisterSpell(SP_STRING("Gnar"), SpellIndex::R, Skillshot(0, 475, FLT_MAX, 0.25, SkillshotType::SkillshotCircle, {}));
	database.GragasQ = SkillshotManager::RegisterSpell(SP_STRING("Gragas"), SpellIndex::Q, Skillshot(850, 275, 1000, 0.25, SkillshotType::SkillshotCircle, {}));
	database.GragasR = SkillshotManager::RegisterSpell(SP_STRING("Gragas"), SpellIndex::R, Skillshot(1000, 400, 1800, 0.25, SkillshotType::SkillshotCircle, {}));
	database.GravesQ = SkillshotManager::RegisterSpell(SP_STRING("Graves"), SpellIndex::Q, Skillshot(800, 20, FLT_MAX, 1.4, SkillshotType::SkillshotCone, {}));
	database.GravesW = SkillshotManager::RegisterSpell(SP_STRING("Graves"), SpellIndex::W, Skillshot(950, 250, 1500, 0.15, SkillshotType::SkillshotCircle, {}));
	database.GravesR = SkillshotManager::RegisterSpell(SP_STRING("Graves"), SpellIndex::R, Skillshot(1000, 100, 2100, 0.25, SkillshotType::SkillshotCone, {}));
	database.GwenQ = SkillshotManager::RegisterSpell(SP_STRING("Gwen"), SpellIndex::Q, Skillshot(450, 275, 1500, 0, SkillshotType::SkillshotCircle, {}));
	database.GwenR = SkillshotManager::RegisterSpell(SP_STRING("Gwen"), SpellIndex::R, Skillshot(1230, 250, 1800, 0.25, SkillshotType::SkillshotLine, {}));
	database.HecarimR = SkillshotManager::RegisterSpell(SP_STRING("Hecarim"), SpellIndex::R, Skillshot(1650, 280, 1100, 0.2, SkillshotType::SkillshotLine, {}));
	database.HeimerdingerW = SkillshotManager::RegisterSpell(SP_STRING("Heimerdinger"), SpellIndex::W, Skillshot(1325, 100, 2050, 0.25, SkillshotType::SkillshotLine, {}));
	database.HeimerdingerE = SkillshotManager::RegisterSpell(SP_STRING("Heimerdinger"), SpellIndex::E, Skillshot(970, 250, 1200, 0.25, SkillshotType::SkillshotCircle, {}));
	database.HeimerdingerE2 = SkillshotManager::RegisterSpell(SP_STRING("Heimerdinger"), SpellIndex::E, Skillshot(970, 250, 1200, 0.25, SkillshotType::SkillshotCircle, {}));
	database.IllaoiQ = SkillshotManager::RegisterSpell(SP_STRING("Illaoi"), SpellIndex::Q, Skillshot(850, 100, FLT_MAX, 0.75, SkillshotType::SkillshotLine, {}));
	database.IllaoiE = SkillshotManager::RegisterSpell(SP_STRING("Illaoi"), SpellIndex::E, Skillshot(900, 50, 1900, 0.25, SkillshotType::SkillshotLine, {}));
	database.IreliaW = SkillshotManager::RegisterSpell(SP_STRING("Irelia"), SpellIndex::W, Skillshot(825, 120, FLT_MAX, 0.25, SkillshotType::SkillshotLine, {}));
	database.IreliaR = SkillshotManager::RegisterSpell(SP_STRING("Irelia"), SpellIndex::R, Skillshot(950, 160, 2000, 0.4, SkillshotType::SkillshotLine, {}));
	database.IvernQ = SkillshotManager::RegisterSpell(SP_STRING("Ivern"), SpellIndex::Q, Skillshot(1075, 80, 1300, 0.25, SkillshotType::SkillshotLine, {}));
	database.JannaQ = SkillshotManager::RegisterSpell(SP_STRING("Janna"), SpellIndex::Q, Skillshot(1750, 100, 667, 0.25, SkillshotType::SkillshotLine, {}));
	database.JarvanIVQ = SkillshotManager::RegisterSpell(SP_STRING("JarvanIV"), SpellIndex::Q, Skillshot(770, 70, FLT_MAX, 0.4, SkillshotType::SkillshotLine, {}));
	database.JarvanIVE = SkillshotManager::RegisterSpell(SP_STRING("JarvanIV"), SpellIndex::E, Skillshot(860, 175, 3440, 0, SkillshotType::SkillshotCircle, {}));
	database.JayceQ = SkillshotManager::RegisterSpell(SP_STRING("Jayce"), SpellIndex::Q, Skillshot(1050, 70, 1450, 0.214, SkillshotType::SkillshotLine, {}));
	database.JayceQ2 = SkillshotManager::RegisterSpell(SP_STRING("Jayce"), SpellIndex::Q, Skillshot(1600, 115, 2350, 0.152, SkillshotType::SkillshotLine, {}));
	database.JhinW = SkillshotManager::RegisterSpell(SP_STRING("Jhin"), SpellIndex::W, Skillshot(2550, 40, 5000, 0.75, SkillshotType::SkillshotLine, {}));
	database.JhinE = SkillshotManager::RegisterSpell(SP_STRING("Jhin"), SpellIndex::E, Skillshot(750, 130, 1600, 0.25, SkillshotType::SkillshotCircle, {}));
	database.JhinR = SkillshotManager::RegisterSpell(SP_STRING("Jhin"), SpellIndex::R, Skillshot(3500, 80, 5000, 0.25, SkillshotType::SkillshotLine, {}));
	database.JinxW = SkillshotManager::RegisterSpell(SP_STRING("Jinx"), SpellIndex::W, Skillshot(1450, 60, 3300, 0.6, SkillshotType::SkillshotLine, {}));
	database.JinxE = SkillshotManager::RegisterSpell(SP_STRING("Jinx"), SpellIndex::E, Skillshot(900, 120, 1100, 1.5, SkillshotType::SkillshotCone, {}));
	database.JinxR = SkillshotManager::RegisterSpell(SP_STRING("Jinx"), SpellIndex::R, Skillshot(12500, 140, 1700, 0.6, SkillshotType::SkillshotLine, {}));
	database.KaisaW = SkillshotManager::RegisterSpell(SP_STRING("Kaisa"), SpellIndex::W, Skillshot(3000, 100, 1750, 0.4, SkillshotType::SkillshotLine, {}));
	database.KalistaQ = SkillshotManager::RegisterSpell(SP_STRING("Kalista"), SpellIndex::Q, Skillshot(1150, 40, 2400, 0.25, SkillshotType::SkillshotLine, {}));
	database.KarmaQ = SkillshotManager::RegisterSpell(SP_STRING("Karma"), SpellIndex::Q, Skillshot(950, 60, 1700, 0.25, SkillshotType::SkillshotLine, {}));
	database.KarmaQ2 = SkillshotManager::RegisterSpell(SP_STRING("Karma"), SpellIndex::Q, Skillshot(950, 80, 1700, 0.25, SkillshotType::SkillshotLine, {}));
	database.KarthusQ = SkillshotManager::RegisterSpell(SP_STRING("Karthus"), SpellIndex::Q, Skillshot(875, 175, FLT_MAX, 0.9, SkillshotType::SkillshotCircle, {}));
	database.KarthusQ2 = SkillshotManager::RegisterSpell(SP_STRING("Karthus"), SpellIndex::Q, Skillshot(875, 175, FLT_MAX, 0.9, SkillshotType::SkillshotCircle, {}));
	database.KarthusQ3 = SkillshotManager::RegisterSpell(SP_STRING("Karthus"), SpellIndex::Q, Skillshot(875, 175, FLT_MAX, 0.9, SkillshotType::SkillshotCircle, {}));
	database.KassadinE = SkillshotManager::RegisterSpell(SP_STRING("Kassadin"), SpellIndex::E, Skillshot(600, 0, FLT_MAX, 0.3, SkillshotType::SkillshotCone, {}));
	database.KassadinR = SkillshotManager::RegisterSpell(SP_STRING("Kassadin"), SpellIndex::R, Skillshot(500, 250, FLT_MAX, 0.25, SkillshotType::SkillshotCircle, {}));
	database.KayleQ = SkillshotManager::RegisterSpell(SP_STRING("Kayle"), SpellIndex::Q, Skillshot(900, 60, 1600, 0.25, SkillshotType::SkillshotLine, {}));
	database.KaynW = SkillshotManager::RegisterSpell(SP_STRING("Kayn"), SpellIndex::W, Skillshot(700, 90, FLT_MAX, 0.55, SkillshotType::SkillshotLine, {}));
	database.KennenQ = SkillshotManager::RegisterSpell(SP_STRING("Kennen"), SpellIndex::Q, Skillshot(1050, 50, 1700, 0.175, SkillshotType::SkillshotLine, {}));
	database.KhazixW = SkillshotManager::RegisterSpell(SP_STRING("Khazix"), SpellIndex::W, Skillshot(1000, 70, 1700, 0.25, SkillshotType::SkillshotLine, {}));
	database.KhazixW2 = SkillshotManager::RegisterSpell(SP_STRING("Khazix"), SpellIndex::W, Skillshot(1000, 70, 1700, 0.25, SkillshotType::SkillshotCone, {}));
	database.KledQ = SkillshotManager::RegisterSpell(SP_STRING("Kled"), SpellIndex::Q, Skillshot(800, 45, 1600, 0.25, SkillshotType::SkillshotLine, {}));
	database.KledQ2 = SkillshotManager::RegisterSpell(SP_STRING("Kled"), SpellIndex::Q, Skillshot(700, 0, 3000, 0.25, SkillshotType::SkillshotCone, {}));
	database.KogMawQ = SkillshotManager::RegisterSpell(SP_STRING("KogMaw"), SpellIndex::Q, Skillshot(1175, 70, 1650, 0.25, SkillshotType::SkillshotLine, {}));
	database.KogMawE = SkillshotManager::RegisterSpell(SP_STRING("KogMaw"), SpellIndex::E, Skillshot(1360, 120, 1400, 0.25, SkillshotType::SkillshotLine, {}));
	database.KogMawR = SkillshotManager::RegisterSpell(SP_STRING("KogMaw"), SpellIndex::R, Skillshot(1300, 200, FLT_MAX, 1.1, SkillshotType::SkillshotCircle, {}));
	database.KSanteQ = SkillshotManager::RegisterSpell(SP_STRING("KSante"), SpellIndex::Q, Skillshot(465, 75, 1800, 0.25, SkillshotType::SkillshotLine, {}));
	database.KSanteQ2 = SkillshotManager::RegisterSpell(SP_STRING("KSante"), SpellIndex::Q, Skillshot(750, 70, 1100, 0.34, SkillshotType::SkillshotLine, {}));
	database.LeblancE = SkillshotManager::RegisterSpell(SP_STRING("Leblanc"), SpellIndex::E, Skillshot(925, 55, 1750, 0.25, SkillshotType::SkillshotLine, {}));
	database.LeblancE2 = SkillshotManager::RegisterSpell(SP_STRING("Leblanc"), SpellIndex::E, Skillshot(925, 55, 1750, 0.25, SkillshotType::SkillshotLine, {}));
	database.LeeSinQ = SkillshotManager::RegisterSpell(SP_STRING("LeeSin"), SpellIndex::Q, Skillshot(1100, 60, 1800, 0.25, SkillshotType::SkillshotLine, {}));
	database.LeonaE = SkillshotManager::RegisterSpell(SP_STRING("Leona"), SpellIndex::E, Skillshot(875, 70, 2000, 0.25, SkillshotType::SkillshotLine, {}));
	database.LeonaR = SkillshotManager::RegisterSpell(SP_STRING("Leona"), SpellIndex::R, Skillshot(1200, 300, FLT_MAX, 0.85, SkillshotType::SkillshotCircle, {}));
	database.LissandraQ = SkillshotManager::RegisterSpell(SP_STRING("Lissandra"), SpellIndex::Q, Skillshot(750, 75, 2200, 0.25, SkillshotType::SkillshotLine, {}));
	database.LissandraE = SkillshotManager::RegisterSpell(SP_STRING("Lissandra"), SpellIndex::E, Skillshot(1025, 125, 850, 0.25, SkillshotType::SkillshotLine, {}));
	database.LucianQ = SkillshotManager::RegisterSpell(SP_STRING("Lucian"), SpellIndex::Q, Skillshot(900, 65, FLT_MAX, 0.35, SkillshotType::SkillshotLine, {}));
	database.LucianW = SkillshotManager::RegisterSpell(SP_STRING("Lucian"), SpellIndex::W, Skillshot(900, 80, 1600, 0.25, SkillshotType::SkillshotLine, {}));
	database.LuluQ = SkillshotManager::RegisterSpell(SP_STRING("Lulu"), SpellIndex::Q, Skillshot(925, 60, 1450, 0.25, SkillshotType::SkillshotLine, {}));
	database.LuxQ = SkillshotManager::RegisterSpell(SP_STRING("Lux"), SpellIndex::Q, Skillshot(1175, 70, 1200, 0.25, SkillshotType::SkillshotLine, {}));
	database.LuxE = SkillshotManager::RegisterSpell(SP_STRING("Lux"), SpellIndex::E, Skillshot(1100, 300, 1200, 0.25, SkillshotType::SkillshotCircle, {}));
	database.LuxR = SkillshotManager::RegisterSpell(SP_STRING("Lux"), SpellIndex::R, Skillshot(3340, 120, FLT_MAX, 1, SkillshotType::SkillshotLine, {}));
	database.MalphiteE = SkillshotManager::RegisterSpell(SP_STRING("Malphite"), SpellIndex::E, Skillshot(0, 400, FLT_MAX, 0.242, SkillshotType::SkillshotCircle, {}));
	database.MalzaharQ = SkillshotManager::RegisterSpell(SP_STRING("Malzahar"), SpellIndex::Q, Skillshot(900, 100, 1600, 0.5, SkillshotType::SkillshotLine, {}));
	database.MaokaiQ = SkillshotManager::RegisterSpell(SP_STRING("Maokai"), SpellIndex::Q, Skillshot(600, 110, 1600, 0.375, SkillshotType::SkillshotLine, {}));
	database.MissFortuneR = SkillshotManager::RegisterSpell(SP_STRING("MissFortune"), SpellIndex::R, Skillshot(1400, 100, 2000, 0.25, SkillshotType::SkillshotCone, {}));
	database.MilioQ = SkillshotManager::RegisterSpell(SP_STRING("Milio"), SpellIndex::Q, Skillshot(675, 200, FLT_MAX, 0.4, SkillshotType::SkillshotCone, {}));
	database.MilioE = SkillshotManager::RegisterSpell(SP_STRING("Milio"), SpellIndex::E, Skillshot(900, 140, FLT_MAX, 0.9, SkillshotType::SkillshotCone, {}));
	database.MorganaQ = SkillshotManager::RegisterSpell(SP_STRING("Morgana"), SpellIndex::Q, Skillshot(1250, 70, 1200, 0.25, SkillshotType::SkillshotLine, {}));
	database.NaafiriQ = SkillshotManager::RegisterSpell(SP_STRING("Naafiri"), SpellIndex::Q, Skillshot(875, 180, FLT_MAX, 1, SkillshotType::SkillshotCircle, {}));
	database.NaafiriR = SkillshotManager::RegisterSpell(SP_STRING("Naafiri"), SpellIndex::R, Skillshot(2750, 250, 850, 0.5, SkillshotType::SkillshotLine, {}));
	database.NautilusQ = SkillshotManager::RegisterSpell(SP_STRING("Nautilus"), SpellIndex::Q, Skillshot(925, 90, 2000, 0.25, SkillshotType::SkillshotLine, {CollidableObjects::Objects, CollidableObjects::Walls}));
	database.NautilusW = SkillshotManager::RegisterSpell(SP_STRING("Nautilus"), SpellIndex::W, Skillshot(300, 300, 2000, 0.25, SkillshotType::SkillshotNone, {}));
	database.NautilusE = SkillshotManager::RegisterSpell(SP_STRING("Nautilus"), SpellIndex::E, Skillshot(350, 350, 2000, 0.25, SkillshotType::SkillshotLine, {}));
	database.NautilusR = SkillshotManager::RegisterSpell(SP_STRING("Nautilus"), SpellIndex::R, Skillshot(825, 90, 2000, 0.25, SkillshotType::SkillshotNone, {}));
	database.NeekoQ = SkillshotManager::RegisterSpell(SP_STRING("Neeko"), SpellIndex::Q, Skillshot(800, 200, 1500, 0.25, SkillshotType::SkillshotCircle, {}));
	database.NeekoE = SkillshotManager::RegisterSpell(SP_STRING("Neeko"), SpellIndex::E, Skillshot(1000, 70, 1300, 0.25, SkillshotType::SkillshotLine, {}));
	database.NidaleeQ = SkillshotManager::RegisterSpell(SP_STRING("Nidalee"), SpellIndex::Q, Skillshot(1500, 40, 1300, 0.25, SkillshotType::SkillshotLine, {}));
	database.NidaleeW = SkillshotManager::RegisterSpell(SP_STRING("Nidalee"), SpellIndex::W, Skillshot(900, 85, FLT_MAX, 1.25, SkillshotType::SkillshotCircle, {}));
	database.NidaleeE = SkillshotManager::RegisterSpell(SP_STRING("Nidalee"), SpellIndex::E, Skillshot(350, 0, FLT_MAX, 0.25, SkillshotType::SkillshotCone, {}));
	database.NilahQ = SkillshotManager::RegisterSpell(SP_STRING("Nilah"), SpellIndex::Q, Skillshot(600, 150, 500, 0.25, SkillshotType::SkillshotLine, {}));
	database.NilahE = SkillshotManager::RegisterSpell(SP_STRING("Nilah"), SpellIndex::E, Skillshot(550, 150, 2200, 0.00, SkillshotType::SkillshotLine, {}));
	database.NilahR = SkillshotManager::RegisterSpell(SP_STRING("Nilah"), SpellIndex::R, Skillshot(0, 450, FLT_MAX, 1.0, SkillshotType::SkillshotCircle, {}));
	database.NocturneQ = SkillshotManager::RegisterSpell(SP_STRING("Nocturne"), SpellIndex::Q, Skillshot(1200, 60, 1600, 0.25, SkillshotType::SkillshotLine, {}));
	database.NunuR = SkillshotManager::RegisterSpell(SP_STRING("Nunu"), SpellIndex::R, Skillshot(0, 650, FLT_MAX, 3, SkillshotType::SkillshotCircle, {}));
	database.OlafQ = SkillshotManager::RegisterSpell(SP_STRING("Olaf"), SpellIndex::Q, Skillshot(1000, 90, 1600, 0.25, SkillshotType::SkillshotLine, {}));
	database.OriannaQ = SkillshotManager::RegisterSpell(SP_STRING("Orianna"), SpellIndex::Q, Skillshot(825, 80, 1400, 0.25, SkillshotType::SkillshotCone, {}));
	database.OrnnQ = SkillshotManager::RegisterSpell(SP_STRING("Ornn"), SpellIndex::Q, Skillshot(800, 65, 1800, 0.3, SkillshotType::SkillshotLine, {}));
	database.OrnnE = SkillshotManager::RegisterSpell(SP_STRING("Ornn"), SpellIndex::E, Skillshot(800, 150, 1600, 0.35, SkillshotType::SkillshotLine, {}));
	database.OrnnR = SkillshotManager::RegisterSpell(SP_STRING("Ornn"), SpellIndex::R, Skillshot(2500, 200, 1650, 0.5, SkillshotType::SkillshotLine, {}));
	database.PantheonQ = SkillshotManager::RegisterSpell(SP_STRING("Pantheon"), SpellIndex::Q, Skillshot(575, 80, FLT_MAX, 0.25, SkillshotType::SkillshotLine, {}));
	database.PantheonQ2 = SkillshotManager::RegisterSpell(SP_STRING("Pantheon"), SpellIndex::Q, Skillshot(1200, 60, 2700, 0.25, SkillshotType::SkillshotLine, {}));
	database.PantheonR = SkillshotManager::RegisterSpell(SP_STRING("Pantheon"), SpellIndex::R, Skillshot(1350, 250, 2250, 4, SkillshotType::SkillshotLine, {}));
	database.PoppyQ = SkillshotManager::RegisterSpell(SP_STRING("Poppy"), SpellIndex::Q, Skillshot(430, 100, FLT_MAX, 0.332, SkillshotType::SkillshotLine, {}));
	database.PoppyR = SkillshotManager::RegisterSpell(SP_STRING("Poppy"), SpellIndex::R, Skillshot(1200, 100, 2000, 0.33, SkillshotType::SkillshotLine, {}));
	database.PykeQ = SkillshotManager::RegisterSpell(SP_STRING("Pyke"), SpellIndex::Q, Skillshot(400, 70, FLT_MAX, 0.25, SkillshotType::SkillshotLine, {}));
	database.PykeQ2 = SkillshotManager::RegisterSpell(SP_STRING("Pyke"), SpellIndex::Q, Skillshot(1100, 70, 2000, 0.2, SkillshotType::SkillshotLine, {}));
	database.PykeE = SkillshotManager::RegisterSpell(SP_STRING("Pyke"), SpellIndex::E, Skillshot(12500, 110, 3000, 0, SkillshotType::SkillshotLine, {}));
	database.PykeR = SkillshotManager::RegisterSpell(SP_STRING("Pyke"), SpellIndex::R, Skillshot(750, 100, FLT_MAX, 0.5, SkillshotType::SkillshotCircle, {}));
	database.QiyanaQ = SkillshotManager::RegisterSpell(SP_STRING("Qiyana"), SpellIndex::Q, Skillshot(500, 60, FLT_MAX, 0.25, SkillshotType::SkillshotLine, {}));
	database.QiyanaQ2 = SkillshotManager::RegisterSpell(SP_STRING("Qiyana"), SpellIndex::Q, Skillshot(925, 70, 1600, 0.25, SkillshotType::SkillshotLine, {}));
	database.QiyanaQ3 = SkillshotManager::RegisterSpell(SP_STRING("Qiyana"), SpellIndex::Q, Skillshot(925, 70, 1600, 0.25, SkillshotType::SkillshotLine, {}));
	database.QiyanaQ4 = SkillshotManager::RegisterSpell(SP_STRING("Qiyana"), SpellIndex::Q, Skillshot(925, 70, 1600, 0.25, SkillshotType::SkillshotLine, {}));
	database.QiyanaR = SkillshotManager::RegisterSpell(SP_STRING("Qiyana"), SpellIndex::R, Skillshot(950, 190, 2000, 0.25, SkillshotType::SkillshotLine, {}));
	database.QuinnQ = SkillshotManager::RegisterSpell(SP_STRING("Quinn"), SpellIndex::Q, Skillshot(1025, 60, 1550, 0.25, SkillshotType::SkillshotLine, {}));
	database.RakanQ = SkillshotManager::RegisterSpell(SP_STRING("Rakan"), SpellIndex::Q, Skillshot(850, 65, 1850, 0.25, SkillshotType::SkillshotLine, {}));
	database.RakanW = SkillshotManager::RegisterSpell(SP_STRING("Rakan"), SpellIndex::W, Skillshot(650, 265, FLT_MAX, 0.7, SkillshotType::SkillshotCircle, {}));
	database.RekSaiQ = SkillshotManager::RegisterSpell(SP_STRING("RekSai"), SpellIndex::Q, Skillshot(1625, 65, 1950, 0.125, SkillshotType::SkillshotLine, {}));
	database.RellQ = SkillshotManager::RegisterSpell(SP_STRING("Rell"), SpellIndex::Q, Skillshot(685, 80, FLT_MAX, 0.35, SkillshotType::SkillshotLine, {}));
	database.RellW = SkillshotManager::RegisterSpell(SP_STRING("Rell"), SpellIndex::W, Skillshot(500, 200, FLT_MAX, 0.625, SkillshotType::SkillshotLine, {}));
	database.RellE = SkillshotManager::RegisterSpell(SP_STRING("Rell"), SpellIndex::E, Skillshot(1500, 250, FLT_MAX, 0.35, SkillshotType::SkillshotLine, {}));
	database.RellR = SkillshotManager::RegisterSpell(SP_STRING("Rell"), SpellIndex::R, Skillshot(0, 400, FLT_MAX, 0.25, SkillshotType::SkillshotCircle, {}));
	database.RengarE = SkillshotManager::RegisterSpell(SP_STRING("Rengar"), SpellIndex::E, Skillshot(1000, 70, 1500, 0.25, SkillshotType::SkillshotLine, {}));
	database.RivenR = SkillshotManager::RegisterSpell(SP_STRING("Riven"), SpellIndex::R, Skillshot(900, 0, 1600, 0.25, SkillshotType::SkillshotCone, {}));
	database.RumbleE = SkillshotManager::RegisterSpell(SP_STRING("Rumble"), SpellIndex::E, Skillshot(850, 60, 2000, 0.25, SkillshotType::SkillshotLine, {}));
	database.RyzeQ = SkillshotManager::RegisterSpell(SP_STRING("Ryze"), SpellIndex::Q, Skillshot(1000, 55, 1700, 0.25, SkillshotType::SkillshotLine, {}));
	database.SemiraQ = SkillshotManager::RegisterSpell(SP_STRING("Semira"), SpellIndex::Q, Skillshot(1000, 60, 2600, 0.25, SkillshotType::SkillshotLine, {}));
	database.SejuaniR = SkillshotManager::RegisterSpell(SP_STRING("Sejuani"), SpellIndex::R, Skillshot(1300, 120, 1600, 0.25, SkillshotType::SkillshotLine, {}));
	database.SennaQ = SkillshotManager::RegisterSpell(SP_STRING("Senna"), SpellIndex::Q, Skillshot(1400, 80, FLT_MAX, 0.4, SkillshotType::SkillshotLine, {}));
	database.SennaW = SkillshotManager::RegisterSpell(SP_STRING("Senna"), SpellIndex::W, Skillshot(1300, 60, 1150, 0.25, SkillshotType::SkillshotLine, {}));
	database.SennaR = SkillshotManager::RegisterSpell(SP_STRING("Senna"), SpellIndex::R, Skillshot(12500, 180, 20000, 1, SkillshotType::SkillshotLine, {}));
	database.SeraphineQ = SkillshotManager::RegisterSpell(SP_STRING("Seraphine"), SpellIndex::Q, Skillshot(900, 350, 1200, 0.25, SkillshotType::SkillshotCircle, {}));
	database.SeraphineE = SkillshotManager::RegisterSpell(SP_STRING("Seraphine"), SpellIndex::E, Skillshot(1300, 70, 1200, 0.25, SkillshotType::SkillshotLine, {}));
	database.SeraphineR = SkillshotManager::RegisterSpell(SP_STRING("Seraphine"), SpellIndex::R, Skillshot(1300, 160, 1600, 0.5, SkillshotType::SkillshotLine, {}));
	database.SettW = SkillshotManager::RegisterSpell(SP_STRING("Sett"), SpellIndex::W, Skillshot(790, 160, FLT_MAX, 0.75, SkillshotType::SkillshotCone, {}));
	database.SettE = SkillshotManager::RegisterSpell(SP_STRING("Sett"), SpellIndex::E, Skillshot(490, 175, FLT_MAX, 0.25, SkillshotType::SkillshotCone, {}));
	database.ShyvanaE = SkillshotManager::RegisterSpell(SP_STRING("Shyvana"), SpellIndex::E, Skillshot(925, 60, 1575, 0.25, SkillshotType::SkillshotLine, {}));
	database.ShyvanaE2 = SkillshotManager::RegisterSpell(SP_STRING("Shyvana"), SpellIndex::E, Skillshot(975, 60, 1575, 0.333, SkillshotType::SkillshotLine, {}));
	database.ShyvanaR = SkillshotManager::RegisterSpell(SP_STRING("Shyvana"), SpellIndex::R, Skillshot(850, 150, 700, 0.25, SkillshotType::SkillshotLine, {}));
	database.SionQ = SkillshotManager::RegisterSpell(SP_STRING("Sion"), SpellIndex::Q, Skillshot(750, 150, FLT_MAX, 2, SkillshotType::SkillshotLine, {}));
	database.SionE = SkillshotManager::RegisterSpell(SP_STRING("Sion"), SpellIndex::E, Skillshot(800, 80, 1800, 0.25, SkillshotType::SkillshotLine, {}));
	database.SivirQ = SkillshotManager::RegisterSpell(SP_STRING("Sivir"), SpellIndex::Q, Skillshot(1250, 90, 1350, 0.25, SkillshotType::SkillshotLine, {}));
	database.SkarnerE = SkillshotManager::RegisterSpell(SP_STRING("Skarner"), SpellIndex::E, Skillshot(1000, 70, 1500, 0.25, SkillshotType::SkillshotLine, {}));
	database.SonaR = SkillshotManager::RegisterSpell(SP_STRING("Sona"), SpellIndex::R, Skillshot(1000, 140, 2400, 0.25, SkillshotType::SkillshotLine, {}));
	database.SorakaQ = SkillshotManager::RegisterSpell(SP_STRING("Soraka"), SpellIndex::Q, Skillshot(810, 235, 1150, 0.25, SkillshotType::SkillshotCircle, {}));
	database.SwainQ = SkillshotManager::RegisterSpell(SP_STRING("Swain"), SpellIndex::Q, Skillshot(725, 0, 5000, 0.25, SkillshotType::SkillshotCone, {}));
	database.SwainW = SkillshotManager::RegisterSpell(SP_STRING("Swain"), SpellIndex::W, Skillshot(3500, 300, FLT_MAX, 1.5, SkillshotType::SkillshotCircle, {}));
	database.SwainE = SkillshotManager::RegisterSpell(SP_STRING("Swain"), SpellIndex::E, Skillshot(850, 85, 1800, 0.25, SkillshotType::SkillshotLine, {}));
	database.SylasQ = SkillshotManager::RegisterSpell(SP_STRING("Sylas"), SpellIndex::Q, Skillshot(775, 45, FLT_MAX, 0.4, SkillshotType::SkillshotCone, {}));
	database.SylasE = SkillshotManager::RegisterSpell(SP_STRING("Sylas"), SpellIndex::E, Skillshot(850, 60, 1600, 0.25, SkillshotType::SkillshotLine, {}));
	database.SyndraQ = SkillshotManager::RegisterSpell(SP_STRING("Syndra"), SpellIndex::Q, Skillshot(800, 200, FLT_MAX, 0.625, SkillshotType::SkillshotCircle, {}));
	database.SyndraE = SkillshotManager::RegisterSpell(SP_STRING("Syndra"), SpellIndex::E, Skillshot(700, 0, 1600, 0.25, SkillshotType::SkillshotCone, {}));
	database.SyndraE2 = SkillshotManager::RegisterSpell(SP_STRING("Syndra"), SpellIndex::E, Skillshot(1250, 100, 2000, 0.25, SkillshotType::SkillshotLine, {}));
	database.TahmKenchQ = SkillshotManager::RegisterSpell(SP_STRING("TahmKench"), SpellIndex::Q, Skillshot(900, 70, 2800, 0.25, SkillshotType::SkillshotLine, {}));
	database.TaliyahQ = SkillshotManager::RegisterSpell(SP_STRING("Taliyah"), SpellIndex::Q, Skillshot(1000, 100, 3600, 0.25, SkillshotType::SkillshotLine, {}));
	database.TaliyahW = SkillshotManager::RegisterSpell(SP_STRING("Taliyah"), SpellIndex::W, Skillshot(900, 150, FLT_MAX, 0.85, SkillshotType::SkillshotCircle, {}));
	database.TaliyahE = SkillshotManager::RegisterSpell(SP_STRING("Taliyah"), SpellIndex::E, Skillshot(800, 0, 2000, 0.45, SkillshotType::SkillshotCone, {}));
	database.TaliyahR = SkillshotManager::RegisterSpell(SP_STRING("Taliyah"), SpellIndex::R, Skillshot(3000, 120, 1700, 1, SkillshotType::SkillshotLine, {}));
	database.TalonW = SkillshotManager::RegisterSpell(SP_STRING("Talon"), SpellIndex::W, Skillshot(650, 75, 2500, 0.25, SkillshotType::SkillshotCone, {}));
	database.ThreshQ = SkillshotManager::RegisterSpell(SP_STRING("Thresh"), SpellIndex::Q, Skillshot(1100, 70, 1900, 0.5, SkillshotType::SkillshotLine, {}));
	database.ThreshE = SkillshotManager::RegisterSpell(SP_STRING("Thresh"), SpellIndex::E, Skillshot(500, 110, FLT_MAX, 0.389, SkillshotType::SkillshotCone, {}));
	database.TristanaW = SkillshotManager::RegisterSpell(SP_STRING("Tristana"), SpellIndex::W, Skillshot(900, 300, 1100, 0.25, SkillshotType::SkillshotCircle, {}));
	database.TryndamereE = SkillshotManager::RegisterSpell(SP_STRING("Tryndamere"), SpellIndex::E, Skillshot(660, 225, 1300, 0, SkillshotType::SkillshotLine, {}));
	database.TwistedFateQ = SkillshotManager::RegisterSpell(SP_STRING("TwistedFate"), SpellIndex::Q, Skillshot(1450, 40, 1000, 0.25, SkillshotType::SkillshotCone, {}));
	database.UrgotQ = SkillshotManager::RegisterSpell(SP_STRING("Urgot"), SpellIndex::Q, Skillshot(800, 180, FLT_MAX, 0.6, SkillshotType::SkillshotCircle, {}));
	database.UrgotE = SkillshotManager::RegisterSpell(SP_STRING("Urgot"), SpellIndex::E, Skillshot(475, 100, 1540, 0.45, SkillshotType::SkillshotLine, {}));
	database.UrgotR = SkillshotManager::RegisterSpell(SP_STRING("Urgot"), SpellIndex::R, Skillshot(1600, 80, 3200, 0.5, SkillshotType::SkillshotLine, {}));
	database.VarusQ = SkillshotManager::RegisterSpell(SP_STRING("Varus"), SpellIndex::Q, Skillshot(1525, 70, 1900, 0.25, SkillshotType::SkillshotLine, {}));
	database.VarusE = SkillshotManager::RegisterSpell(SP_STRING("Varus"), SpellIndex::E, Skillshot(925, 260, 1500, 0.242, SkillshotType::SkillshotCircle, {}));
	database.VarusR = SkillshotManager::RegisterSpell(SP_STRING("Varus"), SpellIndex::R, Skillshot(1200, 120, 1500, 0.25, SkillshotType::SkillshotLine, {}));
	database.VayneQ = SkillshotManager::RegisterSpell(SP_STRING("Vayne"), SpellIndex::Q, Skillshot(300, 70, 2200, 0, SkillshotType::SkillshotLine, {}));
	database.VayneE = SkillshotManager::RegisterSpell(SP_STRING("Vayne"), SpellIndex::E, Skillshot(550, 70, 2000, 0.25, SkillshotType::SkillshotNone, {}));
	database.VayneR = SkillshotManager::RegisterSpell(SP_STRING("Vayne"), SpellIndex::R, Skillshot(globals::localPlayer->GetRealAttackRange(), globals::localPlayer->GetRealAttackRange(), 2000, 0.25, SkillshotType::SkillshotNone, {}));
	database.VeigarQ = SkillshotManager::RegisterSpell(SP_STRING("Veigar"), SpellIndex::Q, Skillshot(900, 70, 2200, 0.25, SkillshotType::SkillshotLine, {}));
	database.VeigarW = SkillshotManager::RegisterSpell(SP_STRING("Veigar"), SpellIndex::W, Skillshot(900, 200, FLT_MAX, 1.25, SkillshotType::SkillshotCircle, {}));
	database.VexQ = SkillshotManager::RegisterSpell(SP_STRING("Vex"), SpellIndex::Q, Skillshot(1200, 80, 2200, 0.15, SkillshotType::SkillshotCone, {}));
	database.VelkozQ = SkillshotManager::RegisterSpell(SP_STRING("Velkoz"), SpellIndex::Q, Skillshot(1100, 45, 2100, 0.25, SkillshotType::SkillshotLine, {}));
	database.VelkozQ2 = SkillshotManager::RegisterSpell(SP_STRING("Velkoz"), SpellIndex::Q, Skillshot(1050, 50, 1300, 0.25, SkillshotType::SkillshotLine, {}));
	database.VelkozW = SkillshotManager::RegisterSpell(SP_STRING("Velkoz"), SpellIndex::W, Skillshot(1050, 87.5, 1700, 0.25, SkillshotType::SkillshotLine, {}));
	database.VelkozE = SkillshotManager::RegisterSpell(SP_STRING("Velkoz"), SpellIndex::E, Skillshot(800, 185, FLT_MAX, 0.8, SkillshotType::SkillshotCircle, {}));
	database.ViQ = SkillshotManager::RegisterSpell(SP_STRING("Vi"), SpellIndex::Q, Skillshot(725, 90, 1500, 0, SkillshotType::SkillshotLine, {}));
	database.ViegoW = SkillshotManager::RegisterSpell(SP_STRING("Viego"), SpellIndex::W, Skillshot(760, 90, 1300, 0, SkillshotType::SkillshotLine, {}));
	database.ViktorW = SkillshotManager::RegisterSpell(SP_STRING("Viktor"), SpellIndex::W, Skillshot(800, 270, FLT_MAX, 1.75, SkillshotType::SkillshotCircle, {}));
	database.ViktorE = SkillshotManager::RegisterSpell(SP_STRING("Viktor"), SpellIndex::E, Skillshot(700, 80, 1050, 0.25, SkillshotType::SkillshotLine, {}));
	database.WarwickR = SkillshotManager::RegisterSpell(SP_STRING("Warwick"), SpellIndex::R, Skillshot(3000, 55, 1800, 0.1, SkillshotType::SkillshotLine, {}));
	database.XayahQ = SkillshotManager::RegisterSpell(SP_STRING("Xayah"), SpellIndex::Q, Skillshot(1100, 45, 2075, 0.5, SkillshotType::SkillshotLine, {}));
	database.XerathQ = SkillshotManager::RegisterSpell(SP_STRING("Xerath"), SpellIndex::Q, Skillshot(1450, 75, FLT_MAX, 0.65, SkillshotType::SkillshotLine, {}));
	database.XerathW = SkillshotManager::RegisterSpell(SP_STRING("Xerath"), SpellIndex::W, Skillshot(1000, 235, FLT_MAX, 0.75, SkillshotType::SkillshotCircle, {}));
	database.XerathE = SkillshotManager::RegisterSpell(SP_STRING("Xerath"), SpellIndex::E, Skillshot(1125, 120, 1400, 0.25, SkillshotType::SkillshotLine, {CollidableObjects::Objects}));
	database.XerathR = SkillshotManager::RegisterSpell(SP_STRING("Xerath"), SpellIndex::R, Skillshot(5000, 200, FLT_MAX, 0.7, SkillshotType::SkillshotCircle, {}));
	database.XinZhaoW = SkillshotManager::RegisterSpell(SP_STRING("XinZhao"), SpellIndex::W, Skillshot(900, 40, 5000, 0.5, SkillshotType::SkillshotLine, {}));
	database.YasuoQ = SkillshotManager::RegisterSpell(SP_STRING("Yasuo"), SpellIndex::Q, Skillshot(475, 40, 1500, 0.25, SkillshotType::SkillshotLine, {}));
	database.YasuoQ2 = SkillshotManager::RegisterSpell(SP_STRING("Yasuo"), SpellIndex::Q, Skillshot(475, 40, 1500, 0.25, SkillshotType::SkillshotLine, {}));
	database.YasuoQ3 = SkillshotManager::RegisterSpell(SP_STRING("Yasuo"), SpellIndex::Q, Skillshot(1100, 90, 1200, 0.03, SkillshotType::SkillshotLine, {}));
	database.YoneQ = SkillshotManager::RegisterSpell(SP_STRING("Yone"), SpellIndex::Q, Skillshot(450, 40, FLT_MAX, 0.25, SkillshotType::SkillshotLine, {}));
	database.YoneQ2 = SkillshotManager::RegisterSpell(SP_STRING("Yone"), SpellIndex::Q, Skillshot(1050, 80, 1500, 0.25, SkillshotType::SkillshotLine, {}));
	database.YoneW = SkillshotManager::RegisterSpell(SP_STRING("Yone"), SpellIndex::W, Skillshot(600, 0, FLT_MAX, 0.375, SkillshotType::SkillshotCone, {}));
	database.YoneR = SkillshotManager::RegisterSpell(SP_STRING("Yone"), SpellIndex::R, Skillshot(1000, 112.5, FLT_MAX, 0.75, SkillshotType::SkillshotLine, {}));
	database.YorickQ = SkillshotManager::RegisterSpell(SP_STRING("Yorick"), SpellIndex::Q, Skillshot(250, 250, FLT_MAX, 0.15, SkillshotType::SkillshotNone, {}));
	database.YorickW = SkillshotManager::RegisterSpell(SP_STRING("Yorick"), SpellIndex::W, Skillshot(600, 200, FLT_MAX, 0.75, SkillshotType::SkillshotCircle, {}));
	database.YorickE = SkillshotManager::RegisterSpell(SP_STRING("Yorick"), SpellIndex::E, Skillshot(1100, 250, FLT_MAX, 0.33, SkillshotType::SkillshotLine, {}));
	database.YorickR = SkillshotManager::RegisterSpell(SP_STRING("Yorick"), SpellIndex::R, Skillshot(600, 250, FLT_MAX, 0.50, SkillshotType::SkillshotCircle, {}));
	database.ZacQ = SkillshotManager::RegisterSpell(SP_STRING("Zac"), SpellIndex::Q, Skillshot(800, 120, 2800, 0.33, SkillshotType::SkillshotLine, {}));
	database.ZedQ = SkillshotManager::RegisterSpell(SP_STRING("Zed"), SpellIndex::Q, Skillshot(900, 50, 1700, 0.25, SkillshotType::SkillshotLine, {}));
	database.ZeriQ = SkillshotManager::RegisterSpell(SP_STRING("Zeri"), SpellIndex::Q, Skillshot(840, 80, 1500, 0.25, SkillshotType::SkillshotLine, {}));
	database.ZiggsQ = SkillshotManager::RegisterSpell(SP_STRING("Ziggs"), SpellIndex::Q, Skillshot(850, 150, 1750, 0.25, SkillshotType::SkillshotCone, {}));
	database.ZiggsW = SkillshotManager::RegisterSpell(SP_STRING("Ziggs"), SpellIndex::W, Skillshot(1000, 240, 1750, 0.25, SkillshotType::SkillshotCircle, {}));
	database.ZiggsE = SkillshotManager::RegisterSpell(SP_STRING("Ziggs"), SpellIndex::E, Skillshot(900, 250, 1800, 0.25, SkillshotType::SkillshotCircle, {}));
	database.ZiggsR = SkillshotManager::RegisterSpell(SP_STRING("Ziggs"), SpellIndex::R, Skillshot(5000, 480, 1550, 0.375, SkillshotType::SkillshotCircle, {}));
	database.ZileanQ = SkillshotManager::RegisterSpell(SP_STRING("Zilean"), SpellIndex::Q, Skillshot(900, 150, FLT_MAX, 0.8, SkillshotType::SkillshotCircle, {}));
	database.ZoeQ = SkillshotManager::RegisterSpell(SP_STRING("Zoe"), SpellIndex::Q, Skillshot(800, 50, 1200, 0.25, SkillshotType::SkillshotLine, {}));
	database.ZoeQ2 = SkillshotManager::RegisterSpell(SP_STRING("Zoe"), SpellIndex::Q, Skillshot(1600, 70, 2500, 0, SkillshotType::SkillshotLine, {}));
	database.ZoeE = SkillshotManager::RegisterSpell(SP_STRING("Zoe"), SpellIndex::E, Skillshot(800, 50, 1700, 0.3, SkillshotType::SkillshotLine, {}));
	database.ZyraQ = SkillshotManager::RegisterSpell(SP_STRING("Zyra"), SpellIndex::Q, Skillshot(800, 200, FLT_MAX, 0.825, SkillshotType::SkillshotLine, {}));
	database.ZyraE = SkillshotManager::RegisterSpell(SP_STRING("Zyra"), SpellIndex::E, Skillshot(1100, 70, 1150, 0.25, SkillshotType::SkillshotLine, {}));
	database.ZyraR = SkillshotManager::RegisterSpell(SP_STRING("Zyra"), SpellIndex::R, Skillshot(700, 500, FLT_MAX, 2, SkillshotType::SkillshotCircle, {}));
	LOG("Skills registered");
}
