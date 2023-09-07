#include <algorithm>

#include "../stdafx.h"
#include "../LoadImages.h"
#include "../NewMenu.h"
#include "../TargetSelector.h"
#include "../Awareness.h"


#include <cmath>
#include "../zoom.h"

using namespace UPasta::SDK::TargetSelector;
namespace render
{
	ImFont* imFont = NULL;
	void Init()
	{
		ImGuiIO& io = ImGui::GetIO();

		io.Fonts->AddFontDefault();
		imFont = io.Fonts->AddFontDefault();
		UPasta::SDK::Awareness::Functions::Initialize();
		initAngles();
		
	}

	void Update()
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::Begin(SP_STRING("transparentwindow"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);

		EventManager::Trigger(EventManager::EventType::OnDraw);


		scriptsrender::Update();


		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->PushClipRectFullScreen();

		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);
	}

	namespace scriptsrender
	{
		void Update()
		{
			static bool loaded = false;
			if (!loaded)
			{
				__try { EventManager::AddEventHandler(EventManager::EventType::OnDraw, scripts::champions::RenderUpdate); }
				__except (1) { LOG("[Event Handler - Add] Error in champions drawings"); }

				__try { EventManager::AddEventHandler(EventManager::EventType::OnDraw, drawings::Update); }
				__except (1) { LOG("[Event Handler - Add] Error in utilities drawings"); }

				__try { EventManager::AddEventHandler(EventManager::EventType::OnDraw, UPasta::SDK::Awareness::Functions::Update); }
				__except (1) { LOG("[Event Handler - Add] Error in awareness drawings"); }

				LOG("OnDraw triggered");
				loaded = true;
			}
			debug::Update();
		}

		namespace drawings
		{
			void DrawCooldownBar(Object* obj)
			{
				if (!obj->IsAlive()) return;

				float width = 128.0f;
				float height = 8.0f;
				float cdWidth = 20.0f;
				float cdHeight = 6.0f;

				float yOffset = 0.0f;
				float xOffset = -width / 2.0f + 8.0f;

				Vector2 screenPos = functions::GetHpBarPosition(obj);

				Vector2 basePos = Vector2(screenPos.x + xOffset, screenPos.y + yOffset);

				Vector2 innerBorderAngle1 = Vector2(basePos.x, basePos.y);
				Vector2 innerBorderAngle2 = Vector2(innerBorderAngle1.x + width - 1.0f, innerBorderAngle1.y + height - 1.0f);

				Vector2 middleBorderAngle1 = Vector2(innerBorderAngle1.x - 1, innerBorderAngle1.y - 1);
				Vector2 middleBorderAngle2 = Vector2(innerBorderAngle2.x + 1, innerBorderAngle2.y + 1);

				Vector2 outerBorderAngle1 = Vector2(middleBorderAngle1.x - 1, middleBorderAngle1.y - 1);
				Vector2 outerBorderAngle2 = Vector2(middleBorderAngle2.x + 1, middleBorderAngle2.y + 1);

				RenderRectFilled(outerBorderAngle1.ToImVec(), outerBorderAngle2.ToImVec(), COLOR_DARK_TRANSPARENT, 0.0f, 0);
				RenderRectFilled(middleBorderAngle1.ToImVec(), middleBorderAngle2.ToImVec(), COLOR_GRAY, 0.0f, 0);
				RenderRectFilled(innerBorderAngle1.ToImVec(), innerBorderAngle2.ToImVec(), COLOR_DARK, 0.0f, 0);

				for (int i = 0; i < 6; i++) 
				{
					
					Vector2 spellAngle1 = Vector2(innerBorderAngle1.x + i * (cdWidth + 1.0f) + 1.0f, innerBorderAngle1.y + 1.0f);
					Vector2 spellAngle2 = Vector2(innerBorderAngle1.x + (i + 1) * (cdWidth + 1.0f), innerBorderAngle1.y + cdHeight);

					Spell* spell = obj->GetSpellBySlotId(i);

					float relativeCooldown = spell->GetRelativeCooldown();

					spellAngle2.x -= relativeCooldown * cdWidth;

					uint32_t color = relativeCooldown == 0.0f ? COLOR_LIGHT_GREEN : COLOR_ORANGE;
					RenderRectFilled(spellAngle1.ToImVec(), spellAngle2.ToImVec(), color, 0.0f, 0);
				}
			}
			
			void DrawEnemyListNames()
			{
				Vector2 screenPos = functions::WorldToScreen(globals::localPlayer->GetPosition());
				float baseDraw = 0;

				for (int i = 0; i < 6; i++) 
				{
					Spell* spell = globals::localPlayer->GetSpellBySlotId(i);

					RenderText(spell->GetName(), (screenPos - Vector2(0.0f, 22.0f + baseDraw)).ToImVec(), 18.0f, COLOR_WHITE, true);
					baseDraw += 22.0f;
				}
				
			}

			void DrawBuffNames(Object* obj)
			{
				Vector2 screenPos = functions::WorldToScreen(obj->GetPosition());
				float baseDraw2 = 0;

				for (int i = 0; i < obj->GetBuffListSize(); i++)
				{
					auto buffEntry = obj->GetBuffManager()->GetBuffEntryByIndex(i);
					if (!buffEntry) return;

					auto buff = buffEntry->GetBuff();
					if (buff && buff->GetEndTime() >= functions::GetGameTime())
					{
						RenderText(buff->GetName(), (screenPos - Vector2(0.0f, 22.0f + baseDraw2)).ToImVec(), 18.0f, COLOR_WHITE, true);
						baseDraw2 += 22.0f;
					}
				}
			}

			void Update()
			{
				if (SETTINGS_BOOL("Drawings", "Draw EnemyList"))
				{
					DrawEnemyListNames();
				}

				for (int i = 0; i < globals::heroManager->GetListSize(); i++)
				{
					auto obj = globals::heroManager->GetIndex(i);
					if (obj->IsValidTarget())
					{
						if (SETTINGS_BOOL("Drawings", "Draw BuffList"))
						{
							DrawBuffNames(obj);
						}

						if (SETTINGS_BOOL("Drawings", "Draw Spells cooldown"))
						{
							//DrawCooldownBar(obj);
						}
					}
				}
			}
		}

		namespace debug
		{
			void DrawData(Object* obj, int index)
			{
				Vector2 screenPos = functions::WorldToScreen(obj->GetPosition());
				if (!IsOnScreen(screenPos)) return;
				auto minion = Functions::GetEnemyMinionInRange(globals::localPlayer->GetRealAttackRange());
				if (minion)
				{
					RenderText(minion->GetName(), (screenPos - Vector2(0.0f, 22.0f)).ToImVec(), 18.0f, COLOR_WHITE, true);
				}
				//drawings::DrawEnemyListNames();
				//RenderText(std::to_string(functions::GetMinimapSize()), (screenPos - Vector2(0.0f, 22.0f)).ToImVec(), 18.0f, COLOR_WHITE, true);

			}

			void DrawObjectData()
			{
				auto minion = Functions::GetEnemyMinionInRange(globals::localPlayer->GetRealAttackRange());
				if (minion)
				{
					RenderText(minion->GetName(), functions::WorldToScreen(minion->GetPosition()).ToImVec(), 18.0f, COLOR_WHITE, true);
				}
				/*for (int i = 0; i < globals::minionManager->GetListSize(); i++)
				{
					auto obj = globals::minionManager->GetIndex(i);
					if (obj->IsAlive() && obj->IsVisible())
						DrawData(obj, i);
				}

				for (int i = 0; i < globals::heroManager->GetListSize(); i++)
				{
					auto obj = globals::heroManager->GetIndex(i);
					//if (obj->IsValidTarget())
						DrawData(obj, i);
				}

				for (int i = 0; i < globals::turretManager->GetListSize(); i++)
				{
					auto obj = globals::turretManager->GetIndex(i);
					if (obj->IsAlive() && obj->IsVisible())
						DrawData(obj, i);
				}*/

				/*auto test = Functions::GetEnemyInhibitorInRange(globals::localPlayer->GetRealAttackRange());
				if (test != nullptr)
				{
					Vector3 playerPos = test->GetPosition();
					RenderCircleWorld(playerPos, 200, 500, COLOR_WHITE, 1.0f, false);
				}*/

				/*
				auto spellCast = globals::localPlayer->GetActiveSpellCast();
				if (spellCast)
				{
					Spell* spell = globals::localPlayer->GetSpellBySlotId(64);
					if (spell)
					{
						QWORD spellInput = (QWORD)spell->GetSpellInput();

						auto spellInputStartPos = spell->GetSpellInfo()->GetSpellData()->GetSpellEndPos();

						Vector3 test = spellCast->GetSpellInfo()->GetSpellData()->GetSpellEndPos();

						LOG("Source X =  %f | Source Y = %f | Source Z = %f", spellInputStartPos.x, spellInputStartPos.y, spellInputStartPos.z);
					}
					

				}

				*/

			}

			void DrawObjectBoundingRadius()
			{
				auto pool = globals::minionManager;
				for (int i = 0; i < 2; ++i)
				{
					for (int i = 0; i < pool->GetListSize(); i++)
					{
						auto obj = pool->GetIndex(i);
						if (obj->IsAlive() && obj->IsVisible())
						{
							//DrawBoundingRadius(obj);
						}

					}
					pool = globals::heroManager;
				}
			}

			void DrawCursorWorld()
			{
				if (!IsOnScreen(functions::GetMousePos())) return;

				Vector3 mouseWorldPos = functions::GetMouseWorldPos();
				Vector2 mouseWorldScreenPos = functions::WorldToScreen(mouseWorldPos);

				if (functions::IsWall(mouseWorldPos))
					RenderText(SP_STRING("wall"), (mouseWorldScreenPos - Vector2(0.0f, 58.0f)).ToImVec(), 18.0f, COLOR_WHITE, true);
				if (functions::IsBrush(mouseWorldPos))
					RenderText(SP_STRING("grass"), (mouseWorldScreenPos - Vector2(0.0f, 58.0f)).ToImVec(), 18.0f, COLOR_WHITE, true);

				RenderCircleWorld(mouseWorldPos, 12, 30.0f, COLOR_WHITE, 2.0f, false);
			}

			void DrawPlayerPaths()
			{
				for (int i = 0; i < globals::heroManager->GetListSize(); i++)
				{
					auto obj = globals::heroManager->GetIndex(i);
					if (obj->IsAlive() && obj->IsVisible())
					{
						auto path = obj->GetAiManager()->GetFutureSegments();
						int countSegments = (int)path.size();
						if (countSegments)
						{
							for (int i = -1; i < countSegments - 1; i++)
							{
								Vector2 screenPos1 = functions::WorldToScreen((i < 0) ? obj->GetPosition() : path[i]);
								Vector2 screenPos2 = functions::WorldToScreen(path[i + 1]);
								RenderLine(screenPos1.ToImVec(), screenPos2.ToImVec(), COLOR_WHITE, 1.0f);
							}
						}
					}
				}
			}

			void Update()
			{
				//DrawObjectData();
				if (SETTINGS_BOOL("debug", "draw object data"))  
				{
					DrawObjectData();
					//EventManager::AddEventHandler(EventManager::EventType::OnDraw, DrawObjectData);
				}
				//DrawPlayerPaths();
				Drawings::DrawSelectedTarget();
				//Drawings::DrawMinionsPriority();

				if (SETTINGS_BOOL("debug", "draw bounding radius")) DrawObjectBoundingRadius();
				if (SETTINGS_BOOL("debug", "draw cursor world")) DrawCursorWorld();
				if (SETTINGS_BOOL("debug", "draw player paths")) DrawPlayerPaths();
			}
		}
	}
}