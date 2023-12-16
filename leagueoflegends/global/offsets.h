#pragma once
#include "utils.h"

namespace UPasta
{
	namespace Offsets
	{
		namespace Instance
		{
			static uintptr_t constexpr SpoofGadget = 0xDB1180;								//E8 ? ? ? ? 44 8B 40 10
			static uintptr_t constexpr LocalPlayer = 0x223A0B8;								//48 8B 05 ? ? ? ? 4C 8B D2 4C 8B C1

			namespace Game {
				static uintptr_t constexpr GameTime = 0x2226D48;							//F3 0F 5C 35 ? ? ? ? 0F 28 F8
				static uintptr_t constexpr GameState = 0x221F5D0;							//48 8D 4D D7 48 8B 05 ? ? ? ?

				namespace MapInfo {
					static uintptr_t constexpr GameMap = 0x1AAC60;							//E8 ? ? ? ? 48 8B 8F ? ? ? ? E8 ? ? ? ? 48 8D 8D ? ? ? ? 
					static uintptr_t constexpr LobbyName = 0x38;
					static uintptr_t constexpr PlayerIP = 0x140;
				}

			}

			namespace Lists {
				static uintptr_t constexpr ObjManager = 0x221C500;							//48 8B 0D ? ? ? ? E8 ? ? ? ? 33 ED 48 8B F8
				static uintptr_t constexpr ManagerList = 0x8;								//E8 ? ? ? ? E9 ? ? ? ? 80 FA 05
				static uintptr_t constexpr ManagerListSize = 0x10;

				static uintptr_t constexpr HeroList = 0x221C620;							//48 8B 05 ? ? ? ? 45 33 E4 0F 57 C0
				static uintptr_t constexpr AttackableList = 0x221C660;						//EB CE 48 8B 82 ? ? ? ?
				static uintptr_t constexpr MinionList = 0x221F590;							//48 8B 0D ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 48 8B C8
				static uintptr_t constexpr TurretList = 0x2225FC0;							//48 8B 1D ? ? ? ? 48 8B 5B 28
				static uintptr_t constexpr InhibList = 0x223A3A8;							//48 8B 05 ? ? ? ? 48 89 7C 24 ? 48 8B 58 08
				static uintptr_t constexpr MissileList = 0x223A1B0;							//48 8B 0D ? ? ? ? 48 8D 54 24 ? E8 ? ? ? ? 48 8B 7C 24 ?
			}

			namespace ObjectTypeHolder {
				static uintptr_t constexpr Type = 0x8;
			}

			namespace HUD {

				static uintptr_t constexpr HudInstance = 0x221C510;							//48 8B 0D ? ? ? ? 8B 57 10
				static uintptr_t constexpr Input = 0x28;
				static uintptr_t constexpr UserData = 0x60;
				static uintptr_t constexpr UserDataSelectedNetId = 0x28;
				static uintptr_t constexpr SpellInfo = 0x68;

				namespace Mouse {
					static uintptr_t constexpr MouseInstance = 0x221F558;						//48 8B 0D ? ? ? ? 48 83 C1 20 E9 ? ? ? ?
					static uintptr_t constexpr Position = 0x0C;								//E9 ? ? ? ? 48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ?
					static uintptr_t constexpr UnderMouse = 0x221F788;						//48 8B 05 ? ? ? ? 48 8B F9 33 C9 48 8B DA

					static uintptr_t constexpr InputMouseWorldPos = 0x20;
				}

				namespace Camera {
					static uintptr_t constexpr CameraInstance = 0x2221C28;						//48 8B 05 ? ? ? ? 48 8B 5C 24 ? 48 89 47 70
					static uintptr_t constexpr Camera = 0x18;
					static uintptr_t constexpr Value = 0x2C0;
					static uintptr_t constexpr Limit = 0x28;
				}

				namespace Chat {
					static uintptr_t constexpr ChatInstance = 0x221F7C0;						//75 78 48 8B 1D ? ? ? ?
					static uintptr_t constexpr Client = 0x223A250;							//48 8B 05 ? ? ? ? 88 90 ? ? ? ? C3
					static uintptr_t constexpr IsOpen = 0xC90;
				}

				namespace Shop {
					static uintptr_t constexpr ShopInstance = 0x223A090;						//75 4B 48 8B 0D ? ? ? ? 48 85 C9 DA TROVARE
					static uintptr_t constexpr IsOpen = 0xC;
				}

				namespace Minimap {
					static uintptr_t constexpr MinimapInstance = 0x222C530;						//48 8B 1D ? ? ? ? 48 85 DB 74 22
					static uintptr_t constexpr HudInstance = 0x320;
					static uintptr_t constexpr Position = 0x60;
					static uintptr_t constexpr Size = 0x68;
					//https://www.unknowncheats.me/forum/3727406-post9100.html i need this shit
				}

				namespace Viewport {
					static uintptr_t constexpr ViewProjMatrices = 0x2284710;				//48 8D 0D ? ? ? ? 0F 10 00
					static uintptr_t constexpr ViewPort = 0x221F550;						//48 8B 3D ? ? ? ? FF 90 ? ? ? ?
					static uintptr_t constexpr W2S = 0x270;
				}
			}

			static uintptr_t constexpr PingNet = 0x221C4F0;									//48 8B 05 ? ? ? ? 4C 8B CA 48 8B D9
		}

		namespace Functions
		{
			namespace Drawings {
				static uintptr_t constexpr GetBaseDrawPosition = 0x208F50;					//E8 ? ? ? ? EB 06 49 8B 06
				static uintptr_t constexpr WorldToScreen = 0xE710B0;						//E8 ? ? ? ? 49 8D 97 ? ? ? ? 4C 8D 45 D8
				static uintptr_t constexpr GetCollisionFlags = 0xDE6720;					//48 83 EC 28 48 8B D1 48 8B 0D ? ? ? ? 48 8B 49 08 E8 ? ? ? ? 48 8B C8 48 85 C0 74 1A
			}

			namespace Orders {
				static uintptr_t constexpr IssueClick = 0x8C3010;							//45 33 C0 E8 ? ? ? ? 48 83 C4 48
				static uintptr_t constexpr IssueRClick = 0x8C3250;							//48 89 5C 24 ? 57 48 83 EC 50 48 8B D9 0F 29 74 24 ? 48 8B 0D ? ? ? ?
				static uintptr_t constexpr IssueMove = 0x8C2630;							//48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B F1 41 0F B6 F9
				static uintptr_t constexpr BuildNavPath = 0xDE2890;							//48 8B C4 48 89 58 10 55 56 57 41 54 41 55 41 56 41 57 48 8D
				static uintptr_t constexpr BuildSmoothPath = 0x917CD0;						//40 53 55 41 56 48 83 EC 50 48
			}

			namespace HUD {

				namespace Chat {
					static uintptr_t constexpr PrintChat = 0x877000;						//E8 ? ? ? ? 4C 8B C3 B2 01
				}

				static uintptr_t constexpr GetCircleIconPortrait = 0x208AB0;				//E8 ? ? ? ? 4C 3B F8 0F 94 C0
				static uintptr_t constexpr GetPing = 0x4B1860;								//E8 ? ? ? ? 8B F8 39 03
				static uintptr_t constexpr SendPing = 0x198C80;								//E8 ? ? ? ? 48 8B 8B ? ? ? ? 44 89 B9 ? ? ? ?

				static uintptr_t constexpr RenderUsualSuspects = 0x60C180;					//48 8B C4 48 89 58 18 89

				static uintptr_t constexpr GetHeightAtPosition = 0xDF1070;					//E8 ? ? ? ? F3 0F 10 45 ? B0 01
				static uintptr_t constexpr IsNotWall = 0xDECB70;							//E8 ? ? ? ? 34 01 44 3A F0

			}

			namespace AIMinion {
				static uintptr_t constexpr IsEliteMinion = 0x215B30;		//TODO			//48 83 EC 28 48 8B 01 FF 90 ? ? ? ? 8B 40 60
				static uintptr_t constexpr IsEpicMinion = 0x215B80;			//TODO			//48 83 EC 28 48 8B 01 FF 90 ? ? ? ? 0F B6 40 60
				static uintptr_t constexpr IsLaneMinion = 0x20D270;			//TODO			//48 8D 91 ? ? ? ? 33 C9 0F B6 42 04 44 0F B6 42 ? 44 0F B6 4C 10 ? 44 88 4C 24 ? 4D 85 C0 74 29 0F 1F 40 00 66 66 0F 1F 84 00 ? ? ? ? 48 8B 44 CA ? 48 F7 D0 48 31 44 CC ? 48 FF C1 49 3B C8 72 EB 44 0F B6 4C 24 ? 0F B6 42 02 4C 8B C2 84 C0 74 35 8B C8 B8 ? ? ? ? 48 2B C1 48 83 F8 01 73 25 66 66 0F 1F 84 00 ? ? ? ? 41 0F B6 4C 00 ? F6 D1 30 4C 04 08 48 FF C0 48 83 F8 01 72 EB 0F B6 44 24 ? C3 41 0F B6 C1 C3 48 8D 91 ? ? ? ? 33 C9 0F B6 42 04 44 0F B6 42 ? 44 0F B6 4C 10 ? 44 88 4C 24 ? 4D 85 C0 74 29 0F 1F 40 00 66 66 0F 1F 84 00 ? ? ? ? 48 8B 44 CA ? 48 F7 D0 48 31 44 CC ? 48 FF C1 49 3B C8 72 EB 44 0F B6 4C 24 ? 0F B6 42 02 4C 8B C2 84 C0 74 35 8B C8 B8 ? ? ? ? 48 2B C1 48 83 F8 01 73 25 66 66 0F 1F 84 00 ? ? ? ? 41 0F B6 4C 00 ? F6 D1 30 4C 04 08 48 FF C0 48 83 F8 01 72 EB 0F B6 44 24 ? C3 41 0F B6 C1 C3 48 89 5C 24 ?
				static uintptr_t constexpr IsDragon = 0x215A50;				//TODO			//E8 ? ? ? ? 48 8B CF 84 C0 74 20
				static uintptr_t constexpr IsElderDragon = 0x215A50;		//TODO			//E8 ? ? ? ? 48 8B CF 84 C0 74 20
				static uintptr_t constexpr IsBaron = 0x2184B0;				//TODO			//40 53 48 83 EC 20 48 81 C1 ? ? ? ? E8 ? ? ? ? 48 8D 15 ? ? ? ?
			}

			namespace CharacterState {
				static uintptr_t constexpr IsObscured = 0x2226D30;			//TODO			//E8 ? ? ? ? 02 C0 88 43 40
			}

			namespace Stats {
				static uintptr_t constexpr GetTeam = 0x2079B0;				//TODO			//48 89 5C 24 ? 57 48 83 EC 20 48 8B 01 48 8B D9 FF 90 ? ? ? ? 84 C0 74 29
				static uintptr_t constexpr IsValid = 0x26EB50;				//TODO			//48 83 EC 28 48 85 C9 74 15 48 8B 01
				static uintptr_t constexpr IsSpellslotUpgradable = 0x256400;				//E8 ? ? ? ? 84 C0 74 10 41 89 1F
				static uintptr_t constexpr HudUpgradeSpell = 0x8BB870;						//E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B 0D ? ? ? ? BA ? ? ? ? 48 89 7C 24 ?

				static uintptr_t constexpr IsCasting = 0x215420;			//TODO			//48 83 EC 28 48 81 C1 ? ? ? ? 48 8B 01 FF 50 50
				static uintptr_t constexpr IsTargetableToTeam = 0x2553D0;	//TODO			//E8 ? ? ? ? 84 C0 74 35 48 8D 8F ? ? ? ?

				static uintptr_t constexpr IsCellPassable = 0xDECB70;		//TODO			//E8 ? ? ? ? 44 3A E8

				static uintptr_t constexpr HasBuff = 0x211600;				//TODO			//48 89 5C 24 ? 57 48 83 EC 20 48 8B 01 41 8B D8
				static uintptr_t constexpr HasBuffOfType = 0x211640;		//TODO			//E9 ? ? ? ? 48 8B 74 24 ? 32 C0

				static uintptr_t constexpr GetStatTotal = 0x6A9990;			//TODO			//E8 ? ? ? ? 0F 28 F8 41 8B 47 18
				static uintptr_t constexpr GetOwner = 0x208AB0;				//TODO			//E8 ? ? ? ? 4C 3B F8 0F 94 C0
				static uintptr_t constexpr GetOwnerPlayer = 0x20C4C0;		//TODO			//E8 ? ? ? ? 40 38 30 75 24
				static uintptr_t constexpr GetAiManager = 0x20C700;			//TODO			//E8 ? ? ? ? 44 39 60 08
				static uintptr_t constexpr GetItemID = 0x3EEE80;			//TODO			//48 8B 41 38 8B 80 ? ? ? ? C3

				static uintptr_t constexpr GetMissiles = 0x6A7AE0;			//TODO			//E8 ? ? ? ? 48 8B 7C 24 ? 48 8B 5C 24 ? 48 3B DF
				static uintptr_t constexpr GetExperience = 0x207CE0;		//TODO			//48 8D 81 ? ? ? ? C3 CC CC CC CC CC CC CC CC 48 8B 91 ? ? ? ?

				static uintptr_t constexpr CanWindup = 0x72CC50;			//TODO			//E8 ? ? ? ? 80 3E 01


				static uintptr_t constexpr GetAttackDelay = 0x3D65C0;						//F3 0F 10 89 ? ? ? ? E9 ? ? ? ?
				static uintptr_t constexpr GetAttackCastDelay = 0x3D64C0;					//E8 ? ? ? ? 48 8B CE F3 0F 11 83 ? ? ? ?
				static uintptr_t constexpr GetBoundingRadius = 0x2064A0;					//E8 ? ? ? ? 0F 28 F8 48 8B D6
				static uintptr_t constexpr GetObjectFromNetID = 0x3B4BD0;					//4C 8B C1 85 D2 74 1B

				static uintptr_t constexpr IsNotLocalPlayer = 0x215290;						//33 C0 48 3B 0D ? ? ? ?
				static uintptr_t constexpr IsAttackingLocalPlayer = 0x216EF0;				//5B C3 CC CC CC CC CC 48 8D 91 ? ? ? ? SUB SOTTO (QUESTO E' SOLO UN RIFERIMENTO)

				static uintptr_t constexpr IsObjectType = 0x2167A0;							//0F B6 41 48 4C 8B C9
				static uintptr_t constexpr IsCanSee = 0x266AD0;								//40 53 48 83 EC 20 48 8B 01 48 8B D9 FF 90 ? ? ? ? 84 C0 75 19
				static uintptr_t constexpr IsVisible = 0x217DF0;							//48 89 5C 24 ? 56 48 83 EC 20 8B 91 ? ? ? ?
				static uintptr_t constexpr IsTargetable = 0xA48A90;							//40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 84 C0 74 41
				static uintptr_t constexpr IsAlive = 0x2553D0;								//E8 ? ? ? ? 84 C0 74 35 48 8D 8F ? ? ? ?
				static uintptr_t constexpr IsDead = 0x215890;								//E8 ? ? ? ? 0F B6 F0 EB 17
				static uintptr_t constexpr IsHero = 0x26ECF0;								//E8 ? ? ? ? 84 C0 48 0F 45 F3
				static uintptr_t constexpr IsMinion = 0x26ED50;								//E8 ? ? ? ? 48 8B 0B F3 0F 10 41 ?
				static uintptr_t constexpr IsTurret = 0x26EEA0;								//E8 ? ? ? ? 84 C0 74 56 48 8B 06
				static uintptr_t constexpr IsMissile = 0x26ED80;							//E8 ? ? ? ? 84 C0 74 0C 48 8B 17
				static uintptr_t constexpr IsInhibitor = 0x26EC30;							//E8 ? ? ? ? 84 C0 75 0C 33 D2
				static uintptr_t constexpr IsNexus = 0x26EC70;								//E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? F3 0F 10 44 24 ? 48 8D 54 24 ?

			}

			namespace Spells {
				static uintptr_t constexpr HudCastSpell = 0x8CE270;							//E8 ? ? ? ? 48 8B 53 08 48 8B 42 60
				static uintptr_t constexpr NewCastSpell = 0x8B5070;							//E8 ? ? ? ? 83 7B 4C 00 75 3D
				static uintptr_t constexpr ReleaseSpell = 0x8A5200;							//48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 40 8B DA 48 8B F9

				static uintptr_t constexpr GetSpellBook = 0xD87CE0;							//48 83 EC 28 4C 8B 41 08 44 8B CA ERA E8 ? ? ? ? 49 8B 5D 68 4C 8B F0 
				static uintptr_t constexpr GetSpellSlot = 0x8B0D60;							//E8 ? ? ? ? 83 F8 FF 74 0A
				static uintptr_t constexpr GetSpellState = 0x7090B0;						//E8 ? ? ? ? 48 8B CE 44 8B F8
				static uintptr_t constexpr GetSpellRange = 0x6A9990;						//E8 ? ? ? ? 0F 28 F8 41 8B 47 18

			}

			namespace Skin {
				static uintptr_t constexpr CharacterDataStackUpdate = 0x18D9F0;				//E8 ? ? ? ? 80 BD ? ? ? ? ? 74 0D
			}

			namespace Shop {
				//ShopActions 40 55 53 56 41 56 41 57 48 8D 6C
				static uintptr_t constexpr BuyItem = 0x1F3800;								//40 55 53 56 41 56 41 57 48 8D 6C 24 ? 48 81 EC
				static uintptr_t constexpr SellItem = 0x500950;									//40 57 48 83 EC 20 8B FA E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 89 5C 24 13.17 hotfix worka DA RITROVARE
				static uintptr_t constexpr UndoAction = 0xF0D170;							//48 89 5C 24 ? 57 48 83 EC 30 48 8B F9 48 8B 49 08 48 8B 01 FF 90 ? ? ? ? 48 85 C0 74 15 48 8B 4F 08 48 8B 01
			}

			namespace D3DX {
				static uintptr_t constexpr Riot_D3DX_SwapChain = 0x1c0; // Signature 448
				static uintptr_t constexpr Riot_D3DX_Direct3DDevice = 0x2b0; // Signature 688
				static uintptr_t constexpr Riot_D3DX_TextureManager_Instance = 0x221F4C8;	//TODO	// 48 8B 1D ? ? ? ? 48 8B 01 FF 50 70 4C 8B CE 
				static uintptr_t constexpr Riot_RiotWindow = 0x223A098; //TODO // C6 43 17 01 
				static uintptr_t constexpr Riot_RendererInstance = 0x228D1C0; // 48 8B 0D ? ? ? ? 4C 8D 44 24 ? 48 8B D0 

				namespace Functions {
					static uintptr_t constexpr Riot_D3DX_GetTexture = 0xE609E0; // E8 ? ? ? ? 44 8B 4D 08
					static uintptr_t constexpr Riot_GameRenderer_OnRenderGUI = 0x916660; // E8 ? ? ? ? EB 4A 48 8B 0D ? ? ? ? 
					static uintptr_t constexpr Riot_GameRenderer_OnRenderSpell = 0x8A9ED0; // E8 ? ? ? ? 80 7E 09 00 74 32 
					static uintptr_t constexpr Riot_RenderPipelineLOL_OnDrawEnvironment = 0xE91900; // 48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 60 4C 8B F2
					static uintptr_t constexpr Riot_RenderPipelineLOL_RenderMouseOvers = 0x3CD9D0; // E8 ? ? ? ? 80 BD ? ? ? ? ? 48 8B CD 74 07
					static uintptr_t constexpr Riot_RenderPipelineLOL_RenderScene = 0x50CA50; // E8 ? ? ? ? 49 8B CD 48 8B F8 E8 ? ? ? ? 44 0F B6 7C 24 ? 
					static uintptr_t constexpr Riot_MouseOverEffect_RenderUsualSuspects = 0x60C180; // E8 ? ? ? ? 0F 28 CF 49 8B CC 
					static uintptr_t constexpr Riot_Renderer_DrawCircleTexture = 0x711760; // E8 ? ? ? ? 80 7F 55 00
				}
			}
		}

		namespace Events
		{
			static uintptr_t constexpr OnUpdate = 0x3B2910;									//E8 ? ? ? ? 0F B6 43 09  

			namespace Objects {
				static uintptr_t constexpr OnCreateObject = 0x3C7CB0;						//E8 ? ? ? ? 48 8B 3D ? ? ? ? 8B 9E ? ? ? ? 
				static uintptr_t constexpr OnDeleteObject = 0x3B2B50;						//E8 ? ? ? ? 49 8D 4F 74
				static uintptr_t constexpr GetRespawnTimeRemaining = 0x20D0D0;				//40 53 48 83 EC 20 48 8B D9 48 81 C1 ? ? ? ? E8 ? ? ? ? 0F B6 83 ? ? ? ?
			}

			namespace Spellcast {
				static uintptr_t constexpr OnStart = 0x72C510;								//40 53 48 83 EC 30 4C 8B 0A 
				static uintptr_t constexpr OnProcess = 0x16C130;							//48 89 5C 24 ? 55 56 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 44 8B 05 ? ? ? ? 
				static uintptr_t constexpr ProcessIndex = 0x11B8;							//48 81 C1 ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 48 83 7B ? ? 72 03 48 8B 1B 4C 8B C3
				static uintptr_t constexpr OnImpact = 0x723980;								//48 89 5C 24 10 48 89 6C 24 18 57 41 54 41 55 41 56 41 57 48 81 QWORD* __fastcall Spell::SpellbookClient::OnSpellImpact(Spell::SpellbookClient* a1, const Spell::SpellCastInfo *a2) https://streamable.com/x4e7ib https://streamable.com/xuvq05 https://www.unknowncheats.me/forum/3755876-post9525.html
				static uintptr_t constexpr OnStop = 0x72C870;								//44 88 4C 24 20 4C 89 44 24 18 88 
			}
		}

		namespace CharData {
			static uintptr_t constexpr CharDataInstance = 0x3570;
			static uintptr_t constexpr Array = 0x28;
			static uintptr_t constexpr Size = 0xC4;
			static uintptr_t constexpr Type = 0x7A0;
			static uintptr_t constexpr TypeDetailed = 0x20;
			static uintptr_t constexpr HeroID = 0x858;
		}

		namespace CharacterStateIntermediate
		{
			static uintptr_t constexpr CharacterStateIntermediateStruct = 0x187560; //E8 ? ? ? ? 48 8B BB ? ? ? ? 4C 8D 0D ? ? ? ? 4C 8D 83 ? ? ? ? 48 8B D3 49 8B CF
			static uintptr_t constexpr oCharacterStateIntermediate = 0x1A20;
			static uintptr_t constexpr oAbilityHasteMod = 0x18;
			static uintptr_t constexpr oPercentCooldownCapMod = 0x30;
			static uintptr_t constexpr oPassiveCooldownEndTime = 0x48;
			static uintptr_t constexpr oPassiveCooldownTotalTime = 0x60;
			static uintptr_t constexpr oFlatPhysicalDamageMod = 0xC0;
			static uintptr_t constexpr oPercentPhysicalDamageMod = 0xD8;
			static uintptr_t constexpr oPercentBonusPhysicalDamageMod = 0xF0;
			static uintptr_t constexpr oPercentBasePhysicalDamageAsFlatBonusMod = 0x108;
			static uintptr_t constexpr oFlatMagicDamageMod = 0x120;
			static uintptr_t constexpr oPercentMagicDamageMod = 0x138;
			static uintptr_t constexpr oGetPercentDamageToBarracksMinionMod = 0x1A9C;
			static uintptr_t constexpr oGetFlatDamageReductionFromBarracksMinionMod = 0x1A9C; //4D 8D 86 ? ? ? ? 49 8B D6 48 8B CD
			static uintptr_t constexpr oFlatMagicReduction = 0x150;
			static uintptr_t constexpr oPercentMagicReduction = 0x168;
			static uintptr_t constexpr oFlatCastRangeMod = 0x180;
			static uintptr_t constexpr oAttackSpeedMod = 0x198;
			static uintptr_t constexpr oPercentAttackSpeedMod = 0x1B0;
			static uintptr_t constexpr oPercentMultiplicativeAttackSpeedMod = 0x1C8;
			static uintptr_t constexpr oBaseAttackDamage = 0x1E0;
			static uintptr_t constexpr oBaseAttackDamageSansPercentScale = 0x1F8;
			static uintptr_t constexpr oFlatBaseAttackDamageMod = 0x210;
			static uintptr_t constexpr oPercentBaseAttackDamageMod = 0x228;
			static uintptr_t constexpr oBaseAbilityDamage = 0x240;
			static uintptr_t constexpr oCritDamageMultiplier = 0x258;
			static uintptr_t constexpr oScaleSkinCoef = 0x270;
			static uintptr_t constexpr oDodge = 0x288;
			static uintptr_t constexpr oCritPercent = 0x2A0;
			static uintptr_t constexpr oFlatBaseHPPoolMod = 0x2B8;
			static uintptr_t constexpr oArmor = 0x2D0;
			static uintptr_t constexpr oBonusArmor = 0x2E8;
			static uintptr_t constexpr oSpellBlock = 0x300;
			static uintptr_t constexpr oBonusSpellBlock = 0x318;
			static uintptr_t constexpr oHPRegenRate = 0x330;
			static uintptr_t constexpr oBaseHPRegenRate = 0x348;
			static uintptr_t constexpr oMoveSpeed = 0x360;
			static uintptr_t constexpr oAttackRange = 0x390;
			static uintptr_t constexpr oFlatBubbleRadiusMod = 0x3A8;
			static uintptr_t constexpr oPercentBubbleRadiusMod = 0x3C0;
			static uintptr_t constexpr oFlatArmorPenetration = 0x3D8;
			static uintptr_t constexpr oPhysicalLethality = 0x3F0;
			static uintptr_t constexpr oPercentArmorPenetration = 0x408;
			static uintptr_t constexpr oPercentBonusArmorPenetration = 0x420;
			static uintptr_t constexpr oPercentCritBonusArmorPenetration = 0x438;
			static uintptr_t constexpr oPercentCritTotalArmorPenetration = 0x450;
			static uintptr_t constexpr oFlatMagicPenetration = 0x468;
			static uintptr_t constexpr oMagicLethality = 0x480;
			static uintptr_t constexpr oPercentMagicPenetration = 0x498;
			static uintptr_t constexpr oPercentBonusMagicPenetration = 0x4B0;
			static uintptr_t constexpr oPercentLifeStealMod = 0x4C8;
			static uintptr_t constexpr oPercentSpellVampMod = 0x4E0;
			static uintptr_t constexpr oPercentOmnivampMod = 0x4F8;
			static uintptr_t constexpr oPercentPhysicalVamp = 0x510;
			static uintptr_t constexpr oPathfindingRadiusMod = 0x528;
			static uintptr_t constexpr oPercentCCReduction = 0x540;
			static uintptr_t constexpr oPercentEXPBonus = 0x558;
			static uintptr_t constexpr oFlatBaseArmorMod = 0x570;
			static uintptr_t constexpr oFlatBaseSpellBlockMod = 0x588;
			static uintptr_t constexpr oPrimaryARBaseRegenRateRep = 0x5B8;
			static uintptr_t constexpr oSecondaryARRegenRateRep = 0x5D0;
			static uintptr_t constexpr oSecondaryARBaseRegenRateRep = 0x5E8;
		}

		namespace AIManager
		{
			static uintptr_t constexpr AiManagerInstance = 0x3740; // https://www.unknowncheats.me/forum/3735791-post9262.html
			static uintptr_t constexpr Manager = 0x10;
			static uintptr_t constexpr ServerPosition = 0x414;
			static uintptr_t constexpr Velocity = 0x02B8;
			static uintptr_t constexpr TargetPos = 0x14;

			namespace Path
			{
				static uintptr_t constexpr EndPath = 0x14;
				static uintptr_t constexpr MovementSpeed = 0x2B8;
				static uintptr_t constexpr IsMoving = 0x2BC;
				static uintptr_t constexpr PassedWayPoints = 0x2C0;
				static uintptr_t constexpr SegmentsCount = 0x2F0;
				static uintptr_t constexpr CurrentPathStartPosition = 0x2D0;
				static uintptr_t constexpr CurrentPathEndPosition = 0x2DC;
				static uintptr_t constexpr NavigationPoints = 0x2E8;
				static uintptr_t constexpr IsDashing = 0x324;
				static uintptr_t constexpr IsNotDashing = 0x2F9;
				static uintptr_t constexpr DashSpeed = 0x300;
				static uintptr_t constexpr MouseClickPosition = 0x348;
				static uintptr_t constexpr DashStartPosition = 0x354;
				static uintptr_t constexpr ServerPosition = 0x414;
				static uintptr_t constexpr Direction = 0x420;
			}
		}

		namespace Perks
		{
			static uintptr_t constexpr PerksManagerInstance = 0x4680;
			static uintptr_t constexpr Size = 0x90;
			static uintptr_t constexpr PerkID = 0x8;
			static uintptr_t constexpr PerkName = 0x10;
			static uintptr_t constexpr PerkRawName = 0x20;
			static uintptr_t constexpr PerkRawDescription = 0x30;
		}

		namespace ItemManager
		{
			//E8 ? ? ? ? 48 8B F8 EB 02 33 FF 48 8D 4B 30  DA VEDERE
			//https://www.unknowncheats.me/forum/3881148-post10893.html
			static uintptr_t constexpr ItemManagerInstance = 0x40C0;
			static uintptr_t constexpr List = 0x20;
			static uintptr_t constexpr Size = 0x20;
			static uintptr_t constexpr Wrapper = 0x10;
			static uintptr_t constexpr Gold = 0x10;
			static uintptr_t constexpr ItemSlot = 0x20;
			static uintptr_t constexpr InventorySlot = 0x38;
			static uintptr_t constexpr ItemId = 0x9C;
			static uintptr_t constexpr ItemTexturePath = 0x4E0;
			static uintptr_t constexpr ItemName = 0x520;
		}

		namespace BuffManager {
			static uintptr_t constexpr BuffManagerInstance = 0x2810;
			static uintptr_t constexpr OwnerHandle = 0x10;
			static uintptr_t constexpr BuffNamePtr = 0x10;
			static uintptr_t constexpr BuffName = 0x8;
			static uintptr_t constexpr BuffEntry = 0x10;
			static uintptr_t constexpr BuffType = 0x8;
			static uintptr_t constexpr BuffEntryBuffStartTime = 0x18;
			static uintptr_t constexpr BuffEntryBuffDurationTime = 0x20;
			static uintptr_t constexpr BuffEntryBuffEndTime = 0x1C;
			static uintptr_t constexpr BuffEntryBuffCount = 0x8C;
			static uintptr_t constexpr BuffEntryBuffCountAlt = 0x38;
			static uintptr_t constexpr IsActive = 0x38;
			static uintptr_t constexpr IsPermanent = 0x88;
			static uintptr_t constexpr BuffEntriesEnd = 0x10;
		}

		namespace MissileManager {
			static uintptr_t constexpr Index = 0x28; // Lui aveva 0x60

			static uintptr_t constexpr SpellName = 0x28; // Lui aveva 0x60
			static uintptr_t constexpr SpellNameLength = 0x38;
			static uintptr_t constexpr SpellNameMaxLength = 0x60;

			static uintptr_t constexpr MissileName = 0x28;
			static uintptr_t constexpr MissileNameLength = 0x38;
			static uintptr_t constexpr MissileNameMaxLength = 0x60;

			static uintptr_t constexpr Speed = 0x88;
			static uintptr_t constexpr Position = 0x104;
			static uintptr_t constexpr SrcIdx = 0x370;
			static uintptr_t constexpr DestIdx = 0x3C8;
			static uintptr_t constexpr DestCheck = 0x320;

			static uintptr_t constexpr StartPosition = 0x38C;
			static uintptr_t constexpr EndPosition = 0x398;

			namespace SpellInfo {
				static uintptr_t constexpr SpellInfoInstance = 0x2E8;
				static uintptr_t constexpr SpellName = 0x28;
				static uintptr_t constexpr MissileName = 0x118;
			}
			static uintptr_t constexpr SpellCast = 0x250;

		}

		namespace BaseObject {

			static uintptr_t constexpr Handle = 0x0010;
			static uintptr_t constexpr NetworkID = 0xC8;
			static uintptr_t constexpr Team = 0x3C;
			static uintptr_t constexpr Name = 0x60;
			static uintptr_t constexpr Position = 0x280;
		}

		namespace AttackableUnit {

			static uintptr_t constexpr Health = 0x1088;
			static uintptr_t constexpr BonusHealth = 0x15C8;
			static uintptr_t constexpr MaxHealth = 0x10A0;

			static uintptr_t constexpr Mana = 0x370;
			static uintptr_t constexpr MaxMana = 0x388;

			static uintptr_t constexpr Visibility = 0x340;
			static uintptr_t constexpr Targetable = 0xEE0;
			static uintptr_t constexpr State = 0x1408;
			static uintptr_t constexpr Invulnerability = 0x520;

			static uintptr_t constexpr fnCanHighlight = 0x24FCF0; // E8 ?? ?? ?? ?? 84 C0 74 35 48 8D 8F ?? ?? ?? ??
		}

		namespace Turret {
			static uintptr_t constexpr TargetNetworkID = 0x4158;
		}

		namespace Client {
			static uintptr_t constexpr Handle = 0x0010;

			static uintptr_t constexpr IssueClickFloatCheck1 = 0x84;
			static uintptr_t constexpr IssueClickFloatCheck2 = 0x88;
			static uintptr_t constexpr IssueClickCheck = 0xD4;

			static uintptr_t constexpr CombatType = 0x26A8;
			static uintptr_t constexpr Index = 0x10;

			static uintptr_t constexpr Name = 0x60;
			static uintptr_t constexpr AiName = 0x3628;
			static uintptr_t constexpr AttackRange = 0x16FC;

			static uintptr_t constexpr LifeRegeneration = 0x16F0;

			static uintptr_t constexpr BaseAttackDamage = 0x16B4;
			static uintptr_t constexpr BonusAttackDamage = 0x1620;

			static uintptr_t constexpr AbilityHaste = 0x14E0;
			static uintptr_t constexpr Lethality = 0x15E0;

			static uintptr_t constexpr ArmorPenetrationFlat = 0x15C8;

			static uintptr_t constexpr MagicPenetrationFlat = 0x15CC;
			static uintptr_t constexpr MagicPenetrationMultiplier = 0x11D0;

			static uintptr_t constexpr AbilityPower = 0x1630;

			static uintptr_t constexpr BaseArmor = 0x16DC;
			static uintptr_t constexpr BonusArmor = 0x16E0;

			static uintptr_t constexpr Shield = 0x10E8;
			static uintptr_t constexpr PhysicalShield = 0x1100;
			static uintptr_t constexpr MagicalShield = 0x1118;

			static uintptr_t constexpr CritFlat = 0x1CC0;
			static uintptr_t constexpr CritMultiplier = 0x1C78;

			static uintptr_t constexpr AttackSpeedBonus = 0x164C;
			static uintptr_t constexpr AttackSpeedMulti = 0x1684;

			static uintptr_t constexpr BonusMagicResist = 0x16E8;
			static uintptr_t constexpr TotalMagicResist = 0x16E4;

			static uintptr_t constexpr MovementSpeed = 0x16F4;

			static uintptr_t constexpr Ammo = 0x3F8;
			static uintptr_t constexpr MaxAmmo = 0x410;

			static uintptr_t constexpr ScaleMulti = 0x16CC;

			static uintptr_t constexpr Experience = 0x4030;
			static uintptr_t constexpr Level = 0x4048;

			namespace CharacterData {
				static uintptr_t constexpr CharacterDataInstance = 0x3570;
				static uintptr_t constexpr Base = 0x28;
				static uintptr_t constexpr HealthBarOffset = 0xC4;
				static uintptr_t constexpr Type = 0x7A0;
				static uintptr_t constexpr TypeDetailed = 0x20;
				static uintptr_t constexpr HeroID = 0x850;
			}

			static uintptr_t constexpr BuffInstance = 0x2810;
			static uintptr_t constexpr SpellBookInstance = 0x2A38;
			static uintptr_t constexpr NavigationInstance = 0x3740;							// E8 ? ? ? ? 44 39 60 08  first [rcx + xxxh]

			static uintptr_t constexpr FnGetHealthBarPosition = 0x80ABC0;					//13.12 0x7F50C0; // ACCESSES MAX ZOOM VALUE from debugger / E8 ? ? ? ? ? ? ? ? F3 0F 10 46 ? ? 4C 8D 4C 24 ? ?
		}

		namespace SpellData {
			static uintptr_t constexpr Coefficent1 = 0x228;
			static uintptr_t constexpr Coefficent2 = 0x22c;
			static uintptr_t constexpr CastTime = 0x2C0;
			static uintptr_t constexpr CooldownTime = 0x2E4;
			static uintptr_t constexpr DelayCastOffsetPerce = 0x300;
			static uintptr_t constexpr DelayTotalTimePerce = 0x304;
			static uintptr_t constexpr CastRange = 0x45C;
			static uintptr_t constexpr CastRangeDisplayOverride = 0x478;
			static uintptr_t constexpr CastRadius = 0x494;
			static uintptr_t constexpr CastFrame = 0x4EC;
			static uintptr_t constexpr MissileSpeed = 0x4F0;
			static uintptr_t constexpr LineWidth = 0x540;
		}

		namespace SpellCast {
			static uintptr_t constexpr SpellInfoInstance = 0x0;
			static uintptr_t constexpr SlotID = 0x8;
			static uintptr_t constexpr CasterName = 0x20;
			static uintptr_t constexpr CasterHandle = 0x88;
			static uintptr_t constexpr StartPosition = 0xa4;
			static uintptr_t constexpr EndPosition = 0xb0;
			static uintptr_t constexpr MousePosition = 0xbc;
			static uintptr_t constexpr DestinationHandlesArray = 0xe0;
			static uintptr_t constexpr HasDestinationHandle = 0xe8;
			static uintptr_t constexpr NumerOfDestinationHandles = 0xec;
			static uintptr_t constexpr IsSpell = 0x10C;
			static uintptr_t constexpr IsBasicAttack = 0x112;
			static uintptr_t constexpr CastedAtTime = 0x12C;
		}

		namespace SpellInfo
		{
			static uintptr_t constexpr Name = 0x28;
			static uintptr_t constexpr SpellDataInstance = 0x60;
			static uintptr_t constexpr TexturePath = 0x290;
			static uintptr_t constexpr DataName = 0x80;
			static uintptr_t constexpr DataManaCost = 0x604;
		}

		namespace SpellBook {
			static uintptr_t constexpr OwnerHandle = 0x30;
			static uintptr_t constexpr SpellSlotInstance = 0x6D0;

			namespace SpellSlot {
				static uintptr_t constexpr Name = 0x80;
				static uintptr_t constexpr Level = 0x28;
				static uintptr_t constexpr Cooldown = 0x30;
				static uintptr_t constexpr Charges = 0x54;
				static uintptr_t constexpr TotalCooldown = 0x74;
				static uintptr_t constexpr Stacks = 0x5C;
				static uintptr_t constexpr SpellInputInstance = 0x128;
				static uintptr_t constexpr SpellInfoInstance = 0x130;

				namespace SpellInput {
					static uintptr_t constexpr SourceHandle = 0x10;
					static uintptr_t constexpr TargetHandle = 0x14;
					static uintptr_t constexpr StartPosition = 0x18;
					static uintptr_t constexpr EndPosition = 0x24;
					static uintptr_t constexpr MousePosition = 0x30;
					static uintptr_t constexpr MousePosition2 = 0x3C;
				}
			}
		}

		namespace ActiveCast {
			static uintptr_t constexpr ActiveCastInstance = 0x2A70; //E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? 48 8B 0D ? ? ? ? 48 81 C1 ? ? ? ?
			static uintptr_t constexpr Info = 0x8;
			static uintptr_t constexpr Type = 0x8;
			static uintptr_t constexpr SpellID = 0x124;
		}
	}
}


