#pragma once
#include <unordered_map>
#include "stdafx.h"
class Event
{
public:
	enum ID {
		OnInject,
		OnDraw,
		OnWndProc,
		OnPresent,

		OnGameStart,
		OnGameTick,

		OnCreateSpell,
		OnImpactSpell,
		OnDeleteSpell,
		OnCreateObject,
		OnDeleteObject,
		OnIssueOrder,
		OnIssueMove,
		OnCastSpell,
		OnSpellCast,
		OnDoCast,
		OnDoCastDelayed,
		OnProcessSpell,
		OnPlayAnimation,
		OnFinishCast,
		OnStopCast,
		OnBeforeAttack,
		OnAfterAttack,
		OnReleaseSpell,
		OnGapCloserSpell,
		OnInterruptibleSpell,
		OnNewPath,
		Size
	};

	static void Subscribe(ID event, void* callback) {
		_subscribers[event].emplace_back(callback);
	}

	static void UnSubscribe(ID event, void* callback) {

		auto& callbacks = _subscribers[event];
		for (auto it = callbacks.begin(); it != callbacks.end(); it++)
			if (*it == callback) {
				callbacks.erase(it);
				break;
			}
	}

	template <typename... Args>
	constexpr static void Publish(ID event, Args ... args) {
		auto found = _subscribers.find(event);

		if (found == _subscribers.end())
			return;


		for (auto sub : found->second)
			static_cast<void(*)(Args...)>(sub)(args ...);
	}

private:
	static inline std::unordered_map<ID, std::vector<void*>> _subscribers;
};

