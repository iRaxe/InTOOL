#include "Orbwalker.h"

#include "Awareness.h"
#include "Damage.h"
#include "TargetSelector.h"
#include "JustEvade.h"
static bool is_kalista = false;

int Orbwalker::GetLatency(int extra) {
	return  extra + 80 / 2;
}

bool Orbwalker::CanAttack() {
	return GetTickCount64() >= (_last_aa + (globals::localPlayer->GetAttackDelay() * 1000)); //(Engine::GetGameTime() * 1000)
}

bool Orbwalker::CanMove() {
	if (is_kalista) return true;
	return GetTickCount64() >= max(_last_action, (_last_aa + (globals::localPlayer->GetAttackWindup() * 1000))); //&& !Evade::Core::Evading; //
}

bool Orbwalker::CanCastAfterAttack() {
	const std::string _championName = globals::localPlayer->GetName();

	auto localExtraWindup = 0;
	if (_championName == "Rengar" && (globals::localPlayer->GetBuffByName("rengarqbase") || globals::localPlayer->GetBuffByName("rengarqemp")))	{
		localExtraWindup = 200;
	}

	return (GetTickCount64() + GetLatency() / 2 >= _last_aa + globals::localPlayer->GetAttackDelay() * 1000 + localExtraWindup);
}

void Orbwalker::InitializeMenu()
{
	using namespace UPasta::SDK::OrbwalkerConfig;

	const auto OrbwalkerMenu = UPasta::SDK::Menu::CreateMenu("vezOrbwalker", "Orbwalker Settings");

	const auto StatusMenu = OrbwalkerMenu->AddMenu("statusSection", "Status settings");
	statusFollowMouse = StatusMenu->AddCheckBox("statusFollowMouse", "Enable Mouse follow", true);
	statusComboMode = StatusMenu->AddCheckBox("statusComboMode", "Enable Combo mode", true);
	statusLaneClearMode = StatusMenu->AddCheckBox("statusLaneClearMode", "Enable LaneClear mode", true);
	statusFastClearMode = StatusMenu->AddCheckBox("statusFastClearMode", "Enable FastClear mode", true);
	statusLastHitMode = StatusMenu->AddCheckBox("statusLastHitMode", "Enable LastHit mode", true);
	statusHarassMode = StatusMenu->AddCheckBox("statusHarassMode", "Enable Harass mode", true);
	statusFleeMode = StatusMenu->AddCheckBox("statusFleeMode", "Enable Flee mode", true);

	const auto KeyBindingsMenu = OrbwalkerMenu->AddMenu("keybindingsSection", "KeyBindings settings");
	comboKey = KeyBindingsMenu->AddKeyBind("comboKey", "Combo Key", ' ', false, false);
	harassKey = KeyBindingsMenu->AddKeyBind("harassKey", "Harass Key", 'C', false, false);
	lastHitKey = KeyBindingsMenu->AddKeyBind("lastHitKey", "LastHit Key", 'X', false, false);
	laneClearKey = KeyBindingsMenu->AddKeyBind("laneClearKey", "LaneClear Key", 'V', false, false);
	fastClearKey = KeyBindingsMenu->AddKeyBind("fastClearKey", "FastClear Key", 'CTRL', false, false);
	fleeKey = KeyBindingsMenu->AddKeyBind("fleeKey", "Flee Key", 'T', false, false);

	const auto DrawingsMenu = OrbwalkerMenu->AddMenu("drawingsSection", "Drawings settings");
	status = DrawingsMenu->AddCheckBox("status", "Show Range Drawings", true);

	const auto HeroDrawingsMenu = DrawingsMenu->AddMenu("heroDrawingsSection", "Heroes settings");
	showHeroes = HeroDrawingsMenu->AddCheckBox("showHeroes", "Show Heroes Range", true);
	showSelf = HeroDrawingsMenu->AddCheckBox("showSelf", "Show Self Range", true);
	showAllies = HeroDrawingsMenu->AddCheckBox("showAllies", "Show Allies Range", true);
	showEnemies = HeroDrawingsMenu->AddCheckBox("showEnemies", "Show Enemies Range", true);

	const auto TurretDrawingsMenu = DrawingsMenu->AddMenu("turretDrawingsSection", "Turrets settings");
	showTurrets = TurretDrawingsMenu->AddCheckBox("showTurrets", "Show Turrets Range", true);
	showAlliesTurrets = TurretDrawingsMenu->AddCheckBox("showAlliesTurrets", "Show Allies Turrets Range", true);
	showEnemiesTurrets = TurretDrawingsMenu->AddCheckBox("showEnemiesTurrets", "Show Enemies Turrets Range", true);

}

void Orbwalker::Init() {
	if (globals::localPlayer->GetName() == "Kalista") is_kalista = true;
	InitializeMenu();
	Event::Subscribe(Event::OnWndProc, &OnWndProc);
	Event::Subscribe(Event::OnDraw, &OnDraw);
	Event::Subscribe(Event::OnGameTick, &OnTick);
	Event::Subscribe(Event::OnProcessSpell, &OnCastSound);
}

void Orbwalker::OnWndProc(UINT msg, WPARAM param) {
	if (param == UPasta::SDK::OrbwalkerConfig::comboKey->Key) {
		switch (msg) {
		case WM_KEYDOWN: _mode = Attack; break;
		case WM_KEYUP: _mode = Off; break;
		}
	}

	if (param == UPasta::SDK::OrbwalkerConfig::harassKey->Key) {
		switch (msg) {
		case WM_KEYDOWN: _mode = Harass; break;
		case WM_KEYUP: _mode = Off; break;
		}
	}

	if (param == UPasta::SDK::OrbwalkerConfig::lastHitKey->Key) {
		switch (msg) {
		case WM_KEYDOWN: _mode = Lasthit; break;
		case WM_KEYUP: _mode = Off; break;
		}
	}

	if (param == UPasta::SDK::OrbwalkerConfig::laneClearKey->Key) {
		switch (msg) {
		case WM_KEYDOWN: _mode = Clear; break;
		case WM_KEYUP: _mode = Off; break;
		}
	}

	if (param == UPasta::SDK::OrbwalkerConfig::fastClearKey->Key) {
		switch (msg) {
		case WM_KEYDOWN: _mode = FastClear; break;
		case WM_KEYUP: _mode = Off; break;
		}
	}

	if (param == UPasta::SDK::OrbwalkerConfig::fleeKey->Key) {
		switch (msg) {
		case WM_KEYDOWN: _mode = Flee; break;
		case WM_KEYUP: _mode = Off; break;
		}
	}
}

void Orbwalker::OnDraw() {
	auto me = globals::localPlayer;
	if (!me) return;

	if (!UPasta::SDK::OrbwalkerConfig::status->Value) return;

	if (UPasta::SDK::OrbwalkerConfig::showHeroes->Value) {
		if (UPasta::SDK::OrbwalkerConfig::showSelf->Value) {
			DrawRange(me);
		}

		if (UPasta::SDK::OrbwalkerConfig::showAllies->Value) {
			for (auto hero : ObjectManager::GetHeroesAs(Alliance::Ally)) {
				if (!hero) continue;
				if (hero->GetPosition().Distance(me->GetPosition()) <= _draw_range + hero->GetBoundingRadius() / 2) {
					DrawRange(hero);
					continue;
				}
			}
		}

		if (UPasta::SDK::OrbwalkerConfig::showEnemies->Value) {
			for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
				if (!hero) continue;
				if (hero->GetPosition().Distance(me->GetPosition()) <= _draw_range + hero->GetBoundingRadius() / 2) {
					DrawRange(hero);
					continue;
				}
			}
		}
	}

	if (UPasta::SDK::OrbwalkerConfig::showTurrets->Value) {
		if (UPasta::SDK::OrbwalkerConfig::showAlliesTurrets->Value) {
			for (auto turret : ObjectManager::GetTurretsAs(Alliance::Ally)) {
				if (!turret) continue;
				if (turret->GetPosition().Distance(me->GetPosition()) <= _draw_range + turret->GetBoundingRadius() / 2) {
					DrawRange(turret);
					continue;
				}
			}
		}

		if (UPasta::SDK::OrbwalkerConfig::showEnemiesTurrets->Value) {
			for (auto turret : ObjectManager::GetTurretsAs(Alliance::Enemy)) {
				if (!turret) continue;
				if (turret->GetPosition().Distance(me->GetPosition()) <= _draw_range + turret->GetBoundingRadius() / 2) {
					DrawRange(turret);
					continue;
				}
			}
		}
	}
}

void Orbwalker::DrawRange(Object* obj) {
	if (!obj) return;
	if (!obj->IsAlive()) return;

	if (UPasta::SDK::OrbwalkerConfig::showHeroes->Value and obj->IsHero())
	{
		if (!UPasta::SDK::OrbwalkerConfig::showAllies->Value and obj->IsAlly() and !obj->IsLocalPlayer()) return;
		if (!UPasta::SDK::OrbwalkerConfig::showSelf->Value and obj->IsLocalPlayer()) return;
		if (!UPasta::SDK::OrbwalkerConfig::showEnemies->Value and obj->IsEnemy()) return;

		UPasta::SDK::Awareness::Functions::Radius::DrawRadius(obj->GetPosition(), obj->GetRealAttackRange(), COLOR_RED, 1.0f);
		return;
	}

	if (UPasta::SDK::OrbwalkerConfig::showTurrets->Value and obj->IsTurret())
	{
		if (!UPasta::SDK::OrbwalkerConfig::showAlliesTurrets->Value and obj->IsAlly()) {
			UPasta::SDK::Awareness::Functions::Radius::DrawRadius(obj->GetPosition(), 992.0f + globals::localPlayer->GetBoundingRadius(), COLOR_GREEN, 1.0f);
			return;
		}

		if (UPasta::SDK::OrbwalkerConfig::showEnemiesTurrets->Value and obj->IsEnemy()) {
			auto col = COLOR_GREEN;
			auto localPlayerNetID = ReadINT(globals::localPlayer, UPasta::Offsets::BaseObject::NetworkID);
			if (obj->GetTurretTargetNetworkID()) {
				if (obj->GetTurretTargetNetworkID() == localPlayerNetID)
					col = COLOR_RED;
				else
					col = COLOR_GREEN;
			}

			UPasta::SDK::Awareness::Functions::Radius::DrawRadius(obj->GetPosition(), 992.0f + globals::localPlayer->GetBoundingRadius(), col, 1.0f);
			return;
		}
		return;
	}
}

void Orbwalker::OnTick() {
	if (_mode == Off) return;
	auto me = globals::localPlayer;
	if (!me) return;
	if (!me->IsAlive()) return;

	if (!CanMove()) return;

	if (_state == CHANNELING or _state == DODGING) return;

	auto current_cast = me->GetActiveSpellCast();
	if (current_cast and current_cast->IsSpell()) {
		Engine::MoveToMousePos();
		_last_action = GetTickCount64() + GetLatency();
		return;
	}

	auto nav = me->GetAiManager();
	if (nav and nav->IsDashing()) {
		Engine::MoveToMousePos();
		_last_action = GetTickCount64() + GetLatency();
		return;
	}

	if (!CanAttack()) {
		Engine::MoveToMousePos();
		_last_action = GetTickCount64() + GetLatency();
		return;
	}

	if (_mode == Attack) {
		OnCombo();
	}
	if (_mode == Harass) {
		OnHarass();
	}
	if (_mode == Clear) {
		OnClear();
	}
}

void Orbwalker::OnCastSound(uintptr_t state, SpellCast* cast) {
	if (cast->GetCasterHandle() != globals::localPlayer->GetHandle()) return;
	if (!cast->IsAutoAttack()) return;

	_state = IDLE;
	Event::Publish(Event::OnAfterAttack);
	return;
}

void Orbwalker::AttackTarget(Object* target) {
	if (!target) {
		Engine::MoveToMousePos();
		_last_action = GetTickCount64() + GetLatency();
		return;
	}
	if (target->GetDistanceTo(globals::localPlayer) <= globals::localPlayer->GetRealAttackRange()) {
		Engine::AttackObject(target->GetPosition());
		_last_aa = GetTickCount64() + GetLatency();
		_state = ATTACKING;
		return;
	}
	return;
}

void Orbwalker::OnCombo() {
	auto target = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), globals::localPlayer->GetRealAttackRange());
	if (!target) {
		Engine::MoveToMousePos();
		_last_action = GetTickCount64() + GetLatency();
		return;
	}

	Event::Publish(Event::OnBeforeAttack);
	Engine::AttackObject(target->GetPosition());
	_last_aa = GetTickCount64() + GetLatency();
	_state = ATTACKING;
	return;

}

void Orbwalker::OnHarass() {
	auto target = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), globals::localPlayer->GetRealAttackRange());
	if (!target) {
		Engine::MoveToMousePos();
		_last_action = GetTickCount64() + GetLatency();
		return;
	}

	if (!target) {
		Engine::MoveToMousePos();
		_last_action = GetTickCount64() + GetLatency();
		return;
	}

	Event::Publish(Event::OnBeforeAttack);
	Engine::AttackObject(target->GetPosition());
	_last_aa = GetTickCount64() + GetLatency();//(Engine::GetGameTime() * 1000)
	_state = ATTACKING;
	return;

}

void Orbwalker::OnClear() {
	auto target = TargetSelector::FindBestLaneClear(globals::localPlayer->GetPosition(), globals::localPlayer->GetRealAttackRange());
	if (!target) {
		Engine::MoveToMousePos();
		_last_action = GetTickCount64() + GetLatency();
		return;
	}

	if (target->IsHero()) return;

	Event::Publish(Event::OnBeforeAttack);
	Engine::AttackObject(target->GetPosition());
	_last_aa = GetTickCount64() + GetLatency();//(Engine::GetGameTime() * 1000)
	_state = ATTACKING;
	return;
}