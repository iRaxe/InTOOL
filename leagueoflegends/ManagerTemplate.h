#pragma once

#include "stdafx.h"

namespace UPasta
{

	struct StopCast
	{
		Object* sender;
		bool stopAnimation;
		bool forceStop;
		bool executeCastFrame;
		bool destroyMissile;
		unsigned int missileNetworkID;
		unsigned int counter;
	};

	struct NewPath
	{
		Object* sender;
		Vector3 start;
		Vector3 end;
		Vector3 tail;
		float dashSpeed;
		unsigned dash;
	};

	extern Vector3 PredLastYasuoWallCastPos;
	extern std::map<int, NewPath> PredAllDashData;
	extern std::map <int, int> PredAllNewPathTicks;

#pragma  region  Delayaction
	struct Action
	{
		Action(int time, std::function<void()> callback)
		{
			Time = time + Engine::GetGameTick();
			CallbackObject = callback;
		}

		void Call()
		{
			CallbackObject();
		}

		std::function<void()> CallbackObject;
		int Time;

	};
	struct ActionIUnit
	{
		ActionIUnit(int time, Object* unit, std::function<void(Object*)> callback)
		{
			Time = time + Engine::GetGameTick();
			CallbackObject = callback;
			Unit = unit;
		}

		void Call()
		{
			if (Unit != nullptr)
				CallbackObject(Unit);
		}

		std::function<void(Object*)> CallbackObject;
		int Time;
		Object* Unit;
	};

	struct ActionPosition
	{
		ActionPosition(int time, Vector3 position, std::function<void(Vector3)> callback)
		{
			Time = time + Engine::GetGameTick();
			CallbackObject = callback;
			Position = position;
		}

		void Call()
		{
			CallbackObject(Position);
		}

		std::function<void(Vector3)> CallbackObject;
		int Time;
		Vector3 Position;
	};

	class DelayAction
	{
	public:
		void OnGameUpdate()
		{
			Actions.erase(std::remove_if(Actions.begin(), Actions.end(), [](Action& Args)
				{
					if (Engine::GetGameTick() >= Args.Time)
					{
						Args.Call();
						return true;
					}

					return false;
				}), Actions.end());

			ActionsIunit.erase(std::remove_if(ActionsIunit.begin(), ActionsIunit.end(), [](ActionIUnit& Args)
				{
					if (Engine::GetGameTick() >= Args.Time)
					{
						Args.Call();
						return true;
					}

					return false;
				}), ActionsIunit.end());
			ActionsPosition.erase(std::remove_if(ActionsPosition.begin(), ActionsPosition.end(), [](ActionPosition& Args)
				{
					if (Engine::GetGameTick() >= Args.Time)
					{
						Args.Call();
						return true;
					}

					return false;
				}), ActionsPosition.end());
		}

		void Add(int Time, std::function<void()> Callback)
		{
			Actions.emplace_back(Time, Callback);
		}
		void AddIUnit(int Time, Object* Unit, std::function<void(Object*)> Callback)
		{
			ActionsIunit.emplace_back(Time, Unit, Callback);
		}
		void AddPosition(int Time, Vector3 Position, std::function<void(Vector3)> Callback)
		{
			ActionsPosition.emplace_back(Time, Position, Callback);
		}
	private:
		std::vector<Action> Actions;
		std::vector<ActionIUnit> ActionsIunit;
		std::vector<ActionPosition> ActionsPosition;
	};
#pragma endregion
}