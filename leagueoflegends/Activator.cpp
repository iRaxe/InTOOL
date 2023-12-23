#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "./Orbwalker.h"
#include "Activator.h"



using namespace UPasta::SDK;
using namespace UPasta::Plugins::Activator;
using namespace UPasta::Plugins::Activator::Config;

float Activatorgametime = 0.0f;

float ActivatorIgniteCastedTime = 0.0f;
float ActivatorPotionCastedTime = 0.0f;
int IgniteSlot = -1;



[[nodiscard]] bool isTimeToCastActivatorIgnite() {
	return IgniteSlot != -1 && Activatorgametime > ActivatorIgniteCastedTime && ObjectManager::GetLocalPlayer()->CanCastSpell(static_cast<SpellIndex>(IgniteSlot)) && Engine::GetSpellState(IgniteSlot) == 0;
}


bool ActivatorIsUnderTower(Vector3 pos, Alliance team)
{
	const auto turret = TargetSelector::FindTurret(pos, 992.0f, team);
	if (turret == nullptr) return false;
	if (team == Alliance::Ally && !turret->IsAlly() || team == Alliance::Enemy && !turret->IsEnemy()) return false;

	return pos.Distance(turret->GetPosition()) < 992.0f;
}

int getIgnite()
{
	
	for (int i=4; i<6; i++)
	{
		auto spell = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(i);
		if (spell != nullptr)
		{
			
			if (spell->GetName() == "SummonerDot") return i;
			
		}
	}
	return -1;
}

int HasPotion() {


	Object* localPlayer = ObjectManager::GetLocalPlayer();
	if (localPlayer == nullptr) return -1;


	for (int i = 6; i <= 12; i++) {


		SpellIndex spellIndex = static_cast<SpellIndex>(i);
		auto spell = localPlayer->GetSpellBySlotId(spellIndex);
		if (spell != nullptr) {

			if (spell->GetSpellInfo() != nullptr) {
				LOG("Found: %s in slot %d", spell->GetName().c_str(), i);
				if (spell->GetName() == "Item2003" || spell->GetName() == "ItemCrystalFlask" || spell->GetName() == "ItemDarkCrystalFlask") {
					LOG("Found: %s in slot %d", spell->GetName().c_str(), i);
					return spellIndex;
				}
			}
		}
	}

	return -1;


}

bool hasPotionBuffs() {
	auto me = ObjectManager::GetLocalPlayer();
	if (me == nullptr) return false;

	std::vector<std::string> potionBuffNames = {
		"ItemDarkCrystalFlask",
		"ItemCrystalFlask",
		"Item2003"
	};

	for (const auto& buffName : potionBuffNames) {
		auto buff = me->GetBuffByName(buffName);
		if (buff != nullptr) {
			if (buff->isActive()) {
				return true;
			}
		}
	}

	return false;
}

void UsePotion(int spell)
{
	if (spell != -1)
	{
		if (hasPotionBuffs())return;
		if (Activatorgametime > ActivatorPotionCastedTime + .25 && Engine::GetSpellState(spell) == 0 && ObjectManager::GetLocalPlayer()->CanCastSpell(static_cast<SpellIndex>(spell)))
		{
			Engine::CastSelf(static_cast<SpellIndex>(spell));
			ActivatorPotionCastedTime = Activatorgametime;
		}
	}
}


void Functions::InitializeMenu()
{
	const auto ActivatorMenu = Menu::CreateMenu("Activator", "Activator Settings");



	const auto spellsMenu = ActivatorMenu->AddMenu("Ignite Settings", "Ignite Settings");

	ActivatorSettings::Ignite = spellsMenu->AddCheckBox("Use Ignite", "Use Ignite", true);
	ActivatorSettings::IgniteKS = spellsMenu->AddCheckBox("Ignite Only For KS", "Ignite Only For KS", false);
	ActivatorSettings::IgniteHP = spellsMenu->AddSlider("Ignite Below X HP", "Ignite Below X HP", 25, 1, 100, 5);
	ActivatorSettings::DrawIgnite = spellsMenu->AddCheckBox("Draw Ignite", "Draw Ignite", true);
	ActivatorSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);


}

void Events::Initialize() {
	TryCatch(Functions::InitializeMenu(), "Error initializing the menu");
	TryCatch(Subscribe(), "Error subscribing to events");
	IgniteSlot = getIgnite();
	LOG("Ignite is on Slot %d", IgniteSlot);
	
}

void Events::Subscribe() {
	TryCatch(Event::Subscribe(Event::OnDraw, &OnDraw), "Error subscribing to OnDraw event");
	TryCatch(Event::Subscribe(Event::OnGameTick, &OnGameUpdate), "Error subscribing to OnGameTick event");
	TryCatch(Event::Subscribe(Event::OnWndProc, &OnWndProc), "Error subscribing to OnWndProc event");

}

void Events::Unsubscribe() {
	TryCatch(Event::UnSubscribe(Event::OnDraw, &OnDraw), "Error unsubscribing to OnDraw event");
	TryCatch(Event::UnSubscribe(Event::OnGameTick, &OnGameUpdate), "Error unsubscribing to OnGameTick event");
	TryCatch(Event::UnSubscribe(Event::OnWndProc, &OnWndProc), "Error unsubscribing to OnWndProc event");
}

void Functions::UseIgnite(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastActivatorIgnite()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > 600 + obj->GetBoundingRadius() / 2) return;

	

	if (static_cast<float>(ActivatorSettings::IgniteHP->Value) >= (obj->GetHealth() / obj->GetMaxHealth()) * 100.0f) {
		if (ActivatorSettings::IgniteKS->Value and Damages::Ignite::GetDamage() < obj->GetHealth() + obj->GetShield()) return;
		Engine::CastTargeted(static_cast<SpellIndex>(IgniteSlot), obj);
		ActivatorIgniteCastedTime = Activatorgametime;
	}
}






void Functions::DrawSpellRadius(float range) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), range, COLOR_WHITE, 1.0f);
	return;
}




void Events::OnDraw() {
	//if (ActivatorSettings::DrawIgnite->Value == true && (ActivatorSettings::ShouldDrawOnlyIfReady() && isTimeToCastActivatorIgnite() || !ActivatorSettings::ShouldDrawOnlyIfReady()))
	//	Functions::DrawSpellRadius(600.0f);

}

void Events::OnGameUpdate() {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (Orbwalker::State == Orbwalker::CHANNELING || Orbwalker::State == Orbwalker::DODGING) return;

	Activatorgametime = Engine::GetGameTime();

	if (Orbwalker::Mode == Attack) {
		Modes::Combo();
	}
	if (Orbwalker::Mode == Harass) {
		Modes::Harass();
	}
	if (Orbwalker::Mode == Clear) {
		Modes::Clear();
	}



	Modes::Auto();
	// TODO: HANDLE IS EVADING SPELL
}

void Events::OnWndProc(UINT msg, WPARAM param) {
	if (param == OrbwalkerConfig::comboKey->Key) {
		switch (msg) {
		case WM_KEYDOWN: Modes::Combo(); break;
		case WM_KEYUP: break;
		}
	}

	if (param == OrbwalkerConfig::harassKey->Key) {
		switch (msg) {
		case WM_KEYDOWN: Modes::Harass(); break;
		case WM_KEYUP: break;
		}
	}

	if (param == OrbwalkerConfig::laneClearKey->Key) {
		switch (msg) {
		case WM_KEYDOWN: Modes::Clear(); break;
		case WM_KEYUP: break;
		}
	}

	if (param == OrbwalkerConfig::fastClearKey->Key) {
		switch (msg) {
		case WM_KEYDOWN: Modes::Clear(); break;
		case WM_KEYUP: break;
		}
	}

}

void Modes::Combo() {
	if (!Orbwalker::CanCastAfterAttack()) return;


}

void Modes::Clear() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	
}

void Modes::Harass() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	
}

void Modes::Killsteal() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	
}
//ItemDarkCrystalFlask
void Modes::Auto() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;

	
	//int buffs_index = ObjectManager::GetLocalPlayer()->GetBuffListSize();
	//for (int i = 0; i < ObjectManager::GetLocalPlayer()->GetBuffListSize(); i++) {
	//	const auto buffEntry = ObjectManager::GetLocalPlayer()->GetBuffManager()->GetBuffEntryByIndex(i);
	//	if (buffEntry) {
	//		const auto buff = buffEntry->GetBuff();
	//		if (buff && buff->GetEndTime() >= Engine::GetGameTime()) {
	//			Engine::PrintChat(buff->GetName());
	//		}
	//	}
	//}

	if ((ObjectManager::GetLocalPlayer()->GetHealth() / ObjectManager::GetLocalPlayer()->GetMaxHealth()) * 100 < 25)
	{
		auto potion = HasPotion();

		if (potion != -1)
		{
			UsePotion(potion);
		}
	}
	

	if (ObjectManager::GetLocalPlayer()->IsUnderTower(Alliance::Enemy)) return;
	if (ActivatorSettings::Ignite->Value) {
		auto hero = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), 600.0f);
		if (hero != nullptr) {
			if (hero->GetDistanceTo(ObjectManager::GetLocalPlayer()) < 600.0f and !hero->IsInvulnerable() and hero->IsAlive()) {

				if (isTimeToCastActivatorIgnite()) {
					Functions::UseIgnite(hero);
				}
			}
		}
	}
}



//           