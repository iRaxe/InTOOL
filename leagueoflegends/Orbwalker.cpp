#include "Orbwalker.h"

#include "Awareness.h"
#include "Damage.h"
#include "TargetSelector.h"

static bool is_kalista = false;

bool Orbwalker::CanAttack() {
	return GetTickCount64() >= (_last_aa + (globals::localPlayer->GetAttackDelay() * 1000)) && globals::localPlayer->CanAttack(); //(Engine::GetGameTime() * 1000)
}

bool Orbwalker::CanMove() {
	if (is_kalista) return true;
	return GetTickCount64() >= max(_last_action,(_last_aa + (globals::localPlayer->GetAttackWindup() * 1000))) && globals::localPlayer->CanMove(); //
}

int Orbwalker::GetLatency(int extra) {
	return  extra + 100 / 2;
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
	if (!me->IsAlive()) return;
	UPasta::SDK::Awareness::Functions::Radius::DrawRadius(me->GetPosition(), me->GetRealAttackRange(), COLOR_RED, 3.0f);
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

void Orbwalker::OnCombo() {
	auto target = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),globals::localPlayer->GetRealAttackRange());
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
	auto target = TargetSelector::FindBestMinion(globals::localPlayer->GetPosition(), globals::localPlayer->GetRealAttackRange(), Alliance::Enemy);
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