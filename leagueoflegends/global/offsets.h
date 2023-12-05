#pragma once
#include "utils.h"

namespace UPasta
{
	namespace Offsets
	{
		namespace Instance
		{
			constexpr inline uintptr_t SpoofGadget = 0xD8D630;								//E8 ? ? ? ? 44 8B 40 10
			constexpr inline uintptr_t LocalPlayer = 0x22118D8;								//48 8B 05 ? ? ? ? 4C 8B D2 4C 8B C1

			namespace Game	{
				constexpr inline uintptr_t GameTime = 0x21FE6F8;							//F3 0F 5C 35 ? ? ? ? 0F 28 F8
				constexpr inline uintptr_t GameState = 0x21F6F80;							//48 8D 4D D7 48 8B 05 ? ? ? ?

				namespace MapInfo {
					constexpr inline uintptr_t GameMap = 0x1A9740;							//E8 ? ? ? ? 48 8B 8F ? ? ? ? E8 ? ? ? ? 48 8D 8D ? ? ? ? 
					constexpr inline uintptr_t LobbyName = 0x38;								
					constexpr inline uintptr_t PlayerIP = 0x140;								
				}

			}
			
			namespace Lists {
				constexpr inline uintptr_t ObjManager = 0x21F3EC0;							//48 8B 0D ? ? ? ? E8 ? ? ? ? 33 ED 48 8B F8
				constexpr inline uintptr_t ManagerList = 0x8;								//E8 ? ? ? ? E9 ? ? ? ? 80 FA 05
				constexpr inline uintptr_t ManagerListSize = 0x10;							

				constexpr inline uintptr_t HeroList = 0x21F3FE0;							//48 8B 05 ? ? ? ? 45 33 E4 0F 57 C0
				constexpr inline uintptr_t AttackableList = 0x21F4020;						//EB CE 48 8B 82 ? ? ? ?
				constexpr inline uintptr_t MinionList = 0x21F6F40;							//48 8B 0D ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 48 8B C8
				constexpr inline uintptr_t TurretList = 0x21FD970;							//48 8B 1D ? ? ? ? 48 8B 5B 28
				constexpr inline uintptr_t InhibList = 0x2211BB8;							//48 8B 05 ? ? ? ? 48 89 7C 24 ? 48 8B 58 08
				constexpr inline uintptr_t MissileList = 0x22119C0;							//48 8B 0D ? ? ? ? 48 8D 54 24 ? E8 ? ? ? ? 48 8B 7C 24 ?
			}

			namespace ObjectTypeHolder {
				constexpr inline uintptr_t Type = 0x8;
			}

			namespace HUD	{

				constexpr inline uintptr_t HudInstance = 0x21F3ED0;							//48 8B 0D ? ? ? ? 8B 57 10
				constexpr inline uintptr_t Input = 0x28;
				constexpr inline uintptr_t UserData = 0x60;
				constexpr inline uintptr_t UserDataSelectedNetId = 0x28;
				constexpr inline uintptr_t SpellInfo = 0x68;

				namespace Mouse {
					constexpr inline uintptr_t Instance = 0x21F6F08;						//48 8B 0D ? ? ? ? 48 83 C1 20 E9 ? ? ? ?
					constexpr inline uintptr_t Position = 0x0C;								//E9 ? ? ? ? 48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ?
					constexpr inline uintptr_t UnderMouse = 0x21F7138;						//48 8B 05 ? ? ? ? 48 8B F9 33 C9 48 8B DA

					constexpr inline uintptr_t InputMouseWorldPos = 0x20;					
				}

				namespace Camera	{
					constexpr inline uintptr_t Instance = 0x21F95E0;						//48 8B 05 ? ? ? ? 48 8B 5C 24 ? 48 89 47 70
					constexpr inline uintptr_t Camera = 0x18;						
					constexpr inline uintptr_t Value = 0x2C0;						
					constexpr inline uintptr_t Limit = 0x28;						
				}

				namespace Chat {
					constexpr inline uintptr_t Instance = 0x21F7170;						//75 78 48 8B 1D ? ? ? ?
					constexpr inline uintptr_t Client = 0x2211A60;							//48 8B 05 ? ? ? ? 88 90 ? ? ? ? C3
					constexpr inline uintptr_t IsOpen = 0xC90;							
				}

				namespace Shop {
					constexpr inline uintptr_t Instance = 0x21F2EB0;						//75 4B 48 8B 0D ? ? ? ? 48 85 C9 DA TROVARE
					constexpr inline uintptr_t IsOpen = 0xC;							
				}

				namespace Minimap {
					constexpr inline uintptr_t Instance = 0x2203EE0;						//48 8B 1D ? ? ? ? 48 85 DB 74 22
					constexpr inline uintptr_t HudInstance = 0x320;
					constexpr inline uintptr_t Position = 0x60;
					constexpr inline uintptr_t Size = 0x68;
					//https://www.unknowncheats.me/forum/3727406-post9100.html i need this shit
				}

				namespace Viewport {
					constexpr inline uintptr_t ViewProjMatrices = 0x225B2C0;				//48 8D 0D ? ? ? ? 0F 10 00
					constexpr inline uintptr_t ViewPort = 0x21F6F00;						//48 8B 3D ? ? ? ? FF 90 ? ? ? ?
					constexpr inline uintptr_t W2S = 0x270;							
				}
			}

			constexpr inline uintptr_t PingNet = 0x21F3EB0;									//48 8B 05 ? ? ? ? 4C 8B CA 48 8B D9
		}

		namespace Functions
		{
			namespace Drawings {
				constexpr inline uintptr_t GetBaseDrawPosition = 0x207120;					//E8 ? ? ? ? EB 06 49 8B 06
				constexpr inline uintptr_t WorldToScreen = 0xE4B480;						//E8 ? ? ? ? 49 8D 97 ? ? ? ? 4C 8D 45 D8
				constexpr inline uintptr_t GetCollisionFlags = 0xDC14B0;					//48 83 EC 28 48 8B D1 48 8B 0D ? ? ? ? 48 8B 49 08 E8 ? ? ? ? 48 8B C8 48 85 C0 74 1A
			}

			namespace Orders {
				constexpr inline uintptr_t IssueClick = 0x8CDE50;							//45 33 C0 E8 ? ? ? ? 48 83 C4 48
				constexpr inline uintptr_t IssueRClick = 0x8B7A10;							//48 89 5C 24 ? 57 48 83 EC 50 48 8B D9 0F 29 74 24 ? 48 8B 0D ? ? ? ?
				constexpr inline uintptr_t IssueMove = 0x8B6C50;							//48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B F1 41 0F B6 F9
				constexpr inline uintptr_t BuildNavPath = 0xDBD620;							//48 8B C4 48 89 58 10 55 56 57 41 54 41 55 41 56 41 57 48 8D
				constexpr inline uintptr_t BuildSmoothPath = 0x90A010;						//40 53 55 41 56 48 83 EC 50 48
			}

			namespace HUD {

				namespace Chat {
					constexpr inline uintptr_t PrintChat = 0x86ACB0;						//E8 ? ? ? ? 4C 8B C3 B2 01
				}

				constexpr inline uintptr_t GetCircleIconPortrait = 0x206C80;				//E8 ? ? ? ? 4C 3B F8 0F 94 C0
				constexpr inline uintptr_t GetPing = 0x4A3800;								//E8 ? ? ? ? 8B F8 39 03
				constexpr inline uintptr_t SendPing = 0x197760;								//E8 ? ? ? ? 48 8B 8B ? ? ? ? 44 89 B9 ? ? ? ?

				constexpr inline uintptr_t RenderUsualSuspects = 0x60A0B0;					//48 8B C4 48 89 58 18 89

				constexpr inline uintptr_t GetHeightAtPosition = 0xDCBA30;					//E8 ? ? ? ? F3 0F 10 45 ? B0 01
				constexpr inline uintptr_t IsNotWall = 0xDC7770;							//E8 ? ? ? ? 34 01 44 3A F0

			}

			namespace AIMinion {
				constexpr inline uintptr_t IsEliteMinion = 0x2139C0;		//TODO			//48 83 EC 28 48 8B 01 FF 90 ? ? ? ? 8B 40 60
				constexpr inline uintptr_t IsEpicMinion = 0x213A10;			//TODO			//48 83 EC 28 48 8B 01 FF 90 ? ? ? ? 0F B6 40 60
				constexpr inline uintptr_t IsLaneMinion = 0x20B440;			//TODO			//48 8D 91 ? ? ? ? 33 C9 0F B6 42 04 44 0F B6 42 ? 44 0F B6 4C 10 ? 44 88 4C 24 ? 4D 85 C0 74 29 0F 1F 40 00 66 66 0F 1F 84 00 ? ? ? ? 48 8B 44 CA ? 48 F7 D0 48 31 44 CC ? 48 FF C1 49 3B C8 72 EB 44 0F B6 4C 24 ? 0F B6 42 02 4C 8B C2 84 C0 74 35 8B C8 B8 ? ? ? ? 48 2B C1 48 83 F8 01 73 25 66 66 0F 1F 84 00 ? ? ? ? 41 0F B6 4C 00 ? F6 D1 30 4C 04 08 48 FF C0 48 83 F8 01 72 EB 0F B6 44 24 ? C3 41 0F B6 C1 C3 48 8D 91 ? ? ? ? 33 C9 0F B6 42 04 44 0F B6 42 ? 44 0F B6 4C 10 ? 44 88 4C 24 ? 4D 85 C0 74 29 0F 1F 40 00 66 66 0F 1F 84 00 ? ? ? ? 48 8B 44 CA ? 48 F7 D0 48 31 44 CC ? 48 FF C1 49 3B C8 72 EB 44 0F B6 4C 24 ? 0F B6 42 02 4C 8B C2 84 C0 74 35 8B C8 B8 ? ? ? ? 48 2B C1 48 83 F8 01 73 25 66 66 0F 1F 84 00 ? ? ? ? 41 0F B6 4C 00 ? F6 D1 30 4C 04 08 48 FF C0 48 83 F8 01 72 EB 0F B6 44 24 ? C3 41 0F B6 C1 C3 48 89 5C 24 ?
				constexpr inline uintptr_t IsDragon = 0x2138E0;				//TODO			//E8 ? ? ? ? 48 8B CF 84 C0 74 20
				constexpr inline uintptr_t IsElderDragon = 0x213990;		//TODO			//E8 ? ? ? ? 48 8B CF 84 C0 74 20
				constexpr inline uintptr_t IsBaron = 0x216180;				//TODO			//40 53 48 83 EC 20 48 81 C1 ? ? ? ? E8 ? ? ? ? 48 8D 15 ? ? ? ?
			}

			namespace CharacterState {
				constexpr inline uintptr_t IsObscured = 0xEDFCD0;			//TODO			//E8 ? ? ? ? 02 C0 88 43 40
			}

			namespace Stats {
				constexpr inline uintptr_t GetTeam = 0x205B80;				//TODO			//48 89 5C 24 ? 57 48 83 EC 20 48 8B 01 48 8B D9 FF 90 ? ? ? ? 84 C0 74 29
				constexpr inline uintptr_t IsValid = 0x2691E0;				//TODO			//48 83 EC 28 48 85 C9 74 15 48 8B 01
				constexpr inline uintptr_t IsSpellslotUpgradable = 0x250D00;				//E8 ? ? ? ? 84 C0 74 10 41 89 1F
				constexpr inline uintptr_t HudUpgradeSpell = 0x8B0110;						//E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B 0D ? ? ? ? BA ? ? ? ? 48 89 7C 24 ?

				constexpr inline uintptr_t IsCasting = 0x2132B0;			//TODO			//48 83 EC 28 48 81 C1 ? ? ? ? 48 8B 01 FF 50 50
				constexpr inline uintptr_t IsTargetableToTeam = 0x24FCF0;	//TODO			//E8 ? ? ? ? 84 C0 74 35 48 8D 8F ? ? ? ?
				
				constexpr inline uintptr_t IsCellPassable = 0xDC7770;		//TODO			//E8 ? ? ? ? 44 3A E8

				constexpr inline uintptr_t HasBuff = 0x20F5A0;				//TODO			//48 89 5C 24 ? 57 48 83 EC 20 48 8B 01 41 8B D8
				constexpr inline uintptr_t HasBuffOfType = 0x20F5E0;		//TODO			//E9 ? ? ? ? 48 8B 74 24 ? 32 C0

				constexpr inline uintptr_t GetStatTotal = 0x2979E0;			//TODO			//E8 ? ? ? ? 0F 28 F8 41 8B 47 18
				constexpr inline uintptr_t GetOwner = 0x206C80;				//TODO			//E8 ? ? ? ? 4C 3B F8 0F 94 C0
				constexpr inline uintptr_t GetOwnerPlayer = 0x20A690;		//TODO			//E8 ? ? ? ? 40 38 30 75 24
				constexpr inline uintptr_t GetAiManager = 0x20A8D0;			//TODO			//E8 ? ? ? ? 44 39 60 08
				constexpr inline uintptr_t GetItemID = 0x3E4100;			//TODO			//48 8B 41 38 8B 80 ? ? ? ? C3

				constexpr inline uintptr_t GetMissiles = 0x6A1740;			//TODO			//E8 ? ? ? ? 48 8B 7C 24 ? 48 8B 5C 24 ? 48 3B DF
				constexpr inline uintptr_t GetExperience = 0x205EB0;		//TODO			//48 8D 81 ? ? ? ? C3 CC CC CC CC CC CC CC CC 48 8B 91 ? ? ? ?

				constexpr inline uintptr_t CanWindup = 0x721720;			//TODO			//E8 ? ? ? ? 80 3E 01


				constexpr inline uintptr_t GetAttackDelay = 0x3CD100;						//F3 0F 10 89 ? ? ? ? E9 ? ? ? ?
				constexpr inline uintptr_t GetAttackCastDelay = 0x3CD000;					//E8 ? ? ? ? 48 8B CE F3 0F 11 83 ? ? ? ?
				constexpr inline uintptr_t GetBoundingRadius = 0x204670;					//E8 ? ? ? ? 0F 28 F8 48 8B D6
				constexpr inline uintptr_t GetObjectFromNetID = 0x3AB300;					//4C 8B C1 85 D2 74 1B

				constexpr inline uintptr_t IsNotLocalPlayer = 0x213120;						//33 C0 48 3B 0D ? ? ? ?
				constexpr inline uintptr_t IsAttackingLocalPlayer = 0x214BC0;				//5B C3 CC CC CC CC CC 48 8D 91 ? ? ? ? SUB SOTTO (QUESTO E' SOLO UN RIFERIMENTO)

				constexpr inline uintptr_t IsObjectType = 0x214520;							//0F B6 41 48 4C 8B C9
				constexpr inline uintptr_t IsCanSee = 0x261310;								//40 53 48 83 EC 20 48 8B 01 48 8B D9 FF 90 ? ? ? ? 84 C0 75 19
				constexpr inline uintptr_t IsVisible = 0x215AC0;							//48 89 5C 24 ? 56 48 83 EC 20 8B 91 ? ? ? ?
				constexpr inline uintptr_t IsTargetable = 0xA48A90;							//40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 84 C0 74 41
				constexpr inline uintptr_t IsAlive = 0x24FCF0;								//E8 ? ? ? ? 84 C0 74 35 48 8D 8F ? ? ? ?
				constexpr inline uintptr_t IsDead = 0x213720;								//E8 ? ? ? ? 0F B6 F0 EB 17
				constexpr inline uintptr_t IsHero = 0x269380;								//E8 ? ? ? ? 84 C0 48 0F 45 F3
				constexpr inline uintptr_t IsMinion = 0x2693E0;								//E8 ? ? ? ? 48 8B 0B F3 0F 10 41 ?
				constexpr inline uintptr_t IsTurret = 0x269530;								//E8 ? ? ? ? 84 C0 74 56 48 8B 06
				constexpr inline uintptr_t IsMissile = 0x269410;							//E8 ? ? ? ? 84 C0 74 0C 48 8B 17
				constexpr inline uintptr_t IsInhibitor = 0x2692C0;							//E8 ? ? ? ? 84 C0 75 0C 33 D2
				constexpr inline uintptr_t IsNexus = 0x269300;								//E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? F3 0F 10 44 24 ? 48 8D 54 24 ?

			}

			namespace Spells {
				constexpr inline uintptr_t HudCastSpell = 0x8C2880;							//E8 ? ? ? ? 48 8B 53 08 48 8B 42 60
				constexpr inline uintptr_t NewCastSpell = 0x8A9B40;							//E8 ? ? ? ? 83 7B 4C 00 75 3D
				constexpr inline uintptr_t ReleaseSpell = 0x899E40;							//48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 40 8B DA 48 8B F9

				constexpr inline uintptr_t GetSpellBook = 0xD65250;							//48 83 EC 28 4C 8B 41 08 44 8B CA ERA E8 ? ? ? ? 49 8B 5D 68 4C 8B F0 
				constexpr inline uintptr_t GetSpellSlot = 0x8A5860;							//E8 ? ? ? ? 83 F8 FF 74 0A
				constexpr inline uintptr_t GetSpellState = 0x6FE000;						//E8 ? ? ? ? 48 8B CE 44 8B F8
				constexpr inline uintptr_t GetSpellRange = 0x6A35F0;						//E8 ? ? ? ? 0F 28 F8 41 8B 47 18

			}

			namespace Skin {
				constexpr inline uintptr_t CharacterDataStackUpdate = 0x18C450;				//E8 ? ? ? ? 80 BD ? ? ? ? ? 74 0D
			}

			namespace Shop	{
				//ShopActions 40 55 53 56 41 56 41 57 48 8D 6C
				constexpr inline uintptr_t BuyItem = 0x1F1A20;								//40 55 53 56 41 56 41 57 48 8D 6C 24 ? 48 81 EC
				constexpr inline uintptr_t SellItem = 0x4F1930;									//40 57 48 83 EC 20 8B FA E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 89 5C 24 13.17 hotfix worka DA RITROVARE
				constexpr inline uintptr_t UndoAction = 0xEE7360;							//48 89 5C 24 ? 57 48 83 EC 30 48 8B F9 48 8B 49 08 48 8B 01 FF 90 ? ? ? ? 48 85 C0 74 15 48 8B 4F 08 48 8B 01
			}

			namespace D3DX {
				constexpr inline uintptr_t Riot_D3DX_SwapChain = 0x1c0; // Signature 448
				constexpr inline uintptr_t Riot_D3DX_Direct3DDevice = 0x2b0; // Signature 688
				constexpr inline uintptr_t Riot_D3DX_TextureManager_Instance = 0x21F6E78;	//TODO	// 48 8B 1D ? ? ? ? 48 8B 01 FF 50 70 4C 8B CE 
				constexpr inline uintptr_t Riot_RiotWindow = 0x2210518; //TODO // C6 43 17 01 
				constexpr inline uintptr_t Riot_RendererInstance = 0x2263D70; // 48 8B 0D ? ? ? ? 4C 8D 44 24 ? 48 8B D0 

				namespace Functions {
					constexpr inline uintptr_t Riot_D3DX_GetTexture = 0xE3ADB0; // E8 ? ? ? ? 44 8B 4D 08
					constexpr inline uintptr_t Riot_GameRenderer_OnRenderGUI = 0x9089A0; // E8 ? ? ? ? EB 4A 48 8B 0D ? ? ? ? 
					constexpr inline uintptr_t Riot_GameRenderer_OnRenderSpell = 0x89EB20; // E8 ? ? ? ? 80 7E 09 00 74 32 
					constexpr inline uintptr_t Riot_RenderPipelineLOL_OnDrawEnvironment = 0xE6BBB0; // 48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 60 4C 8B F2
					constexpr inline uintptr_t Riot_RenderPipelineLOL_RenderMouseOvers = 0x3C4510; // E8 ? ? ? ? 80 BD ? ? ? ? ? 48 8B CD 74 07
					constexpr inline uintptr_t Riot_RenderPipelineLOL_RenderScene = 0x4FDB70; // E8 ? ? ? ? 49 8B CD 48 8B F8 E8 ? ? ? ? 44 0F B6 7C 24 ? 
					constexpr inline uintptr_t Riot_MouseOverEffect_RenderUsualSuspects = 0x60A0B0; // E8 ? ? ? ? 0F 28 CF 49 8B CC 
					constexpr inline uintptr_t Riot_Renderer_DrawCircleTexture = 0x7066C0; // E8 ? ? ? ? 80 7F 55 00
				}
			}
		}

		namespace Events
		{
			constexpr inline uintptr_t OnUpdate = 0x389240;						//E8 ? ? ? ? 0F B6 43 09  

			namespace Objects	{
				constexpr inline uintptr_t OnCreateObject = 0x3BE7B0;						//E8 ? ? ? ? 48 8B 3D ? ? ? ? 8B 9E ? ? ? ? 
				constexpr inline uintptr_t OnDeleteObject = 0x3A92E0;						//E8 ? ? ? ? 49 8D 4F 74
				constexpr inline uintptr_t GetRespawnTimeRemaining = 0x20B2A0;				//40 53 48 83 EC 20 48 8B D9 48 81 C1 ? ? ? ? E8 ? ? ? ? 0F B6 83 ? ? ? ?
			}

			namespace Spellcast {
				constexpr inline uintptr_t OnStart = 0x720FE0;								//40 53 48 83 EC 30 4C 8B 0A 
				constexpr inline uintptr_t OnProcess = 0x16A490;							//48 89 5C 24 ? 55 56 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 44 8B 05 ? ? ? ? 
				constexpr inline uintptr_t ProcessIndex = 0x11B8;							//48 81 C1 ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 48 83 7B ? ? 72 03 48 8B 1B 4C 8B C3
				constexpr inline uintptr_t OnImpact = 0x718410;								//48 89 5C 24 10 48 89 6C 24 18 57 41 54 41 55 41 56 41 57 48 81 QWORD* __fastcall Spell::SpellbookClient::OnSpellImpact(Spell::SpellbookClient* a1, const Spell::SpellCastInfo *a2) https://streamable.com/x4e7ib https://streamable.com/xuvq05 https://www.unknowncheats.me/forum/3755876-post9525.html
				constexpr inline uintptr_t OnStop = 0x721340;								//44 88 4C 24 20 4C 89 44 24 18 88 
			}
		}

		namespace GameObject
		{
			constexpr inline uintptr_t IssueClickFloatCheck1 = 0x84;
			constexpr inline uintptr_t IssueClickFloatCheck2 = 0x88;
			constexpr inline uintptr_t IssueClickCheck = 0xD4;

			constexpr inline uintptr_t Index = 0x10;
			constexpr inline uintptr_t Name = 0x60;
			constexpr inline uintptr_t AiName = 0x3628;
			constexpr inline uintptr_t Experience = 0x4010;
			constexpr inline uintptr_t Level = 0x4028;
			constexpr inline uintptr_t Handle = 0x0010;
			constexpr inline uintptr_t Team = 0x3C;
			constexpr inline uintptr_t Alive = 0x358;

			constexpr inline uintptr_t BaseAtk = 0x16B4;
			constexpr inline uintptr_t BonusAtk = 0x1620;//0x1AE0

			constexpr inline uintptr_t Crit = 0x1CC0;
			constexpr inline uintptr_t CritMulti = 0x1C78;

			constexpr inline uintptr_t AbilityPower = 0x1630;

			constexpr inline uintptr_t Armor = 0x16DC;
			constexpr inline uintptr_t BonusArmor = 0x16e0;
			constexpr inline uintptr_t MagicRes = 0x16E4;
			constexpr inline uintptr_t BonusMagicRes = 0x16E8;

			constexpr inline uintptr_t Shield = 0x10E8;
			constexpr inline uintptr_t PhysicalShield = 0x1100;
			constexpr inline uintptr_t MagicalShield = 0x1118;

			constexpr inline uintptr_t ArmorPen = 0x15C8;
			constexpr inline uintptr_t MagicPen = 0x15CC;
			constexpr inline uintptr_t MagicPenMulti = 0x11D0;

			constexpr inline uintptr_t BonusAtkSpeed = 0x164C;
			constexpr inline uintptr_t AtkSpeedMulti = 0x1668;

			constexpr inline uintptr_t AbilityHaste = 0x14E0; //0x1A38

			constexpr inline uintptr_t MoveSpeed = 0x16F4;
			constexpr inline uintptr_t Scale = 0x16CC;

			constexpr inline uintptr_t Health = 0x1088;
			constexpr inline uintptr_t MaxHealth = 0x10A0;
			constexpr inline uintptr_t BonusHealth = 0x15C8; //non sicuro
			constexpr inline uintptr_t LifeRegen = 0x16F0;

			constexpr inline uintptr_t CombatType = 0x26A8;

			constexpr inline uintptr_t Mana = 0x370;
			constexpr inline uintptr_t MaxMana = 0x388;

			constexpr inline uintptr_t Ammo = 0x3F8;
			constexpr inline uintptr_t MaxAmmo = 0x410;

			constexpr inline uintptr_t MagicPenetration = 0x1584;
			constexpr inline uintptr_t Lethality = 0x15E0;
			constexpr inline uintptr_t AttackRange = 0x16FC;

			constexpr inline uintptr_t Position = 0x220;
			constexpr inline uintptr_t Visibility = 0x340;
			constexpr inline uintptr_t Targetable = 0xEE0;
			constexpr inline uintptr_t State = 0x1408;
			constexpr inline uintptr_t Invulnerability = 0x520;

			constexpr inline uintptr_t SpellBook = 0x2A38; //E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? 48 8B 0D ? ? ? ? 48 81 C1 ? ? ? ?

			namespace ActiveCast {
				constexpr inline uintptr_t ActiveCast = 0x2A70; //E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? 48 8B 0D ? ? ? ? 48 81 C1 ? ? ? ?
				constexpr inline uintptr_t Info = 0x8;
				constexpr inline uintptr_t Type = 0x8;
				constexpr inline uintptr_t SpellID = 0x124;
			}

			namespace CharData	{
				constexpr inline uintptr_t Instance = 0x3570;
				constexpr inline uintptr_t Array = 0x28;
				constexpr inline uintptr_t Size = 0xC4;
				constexpr inline uintptr_t Type = 0x7A0;
				constexpr inline uintptr_t TypeDetailed = 0x20;
				constexpr inline uintptr_t HeroID = 0x850;
			}

			namespace CharacterStateIntermediate
			{
				constexpr inline uintptr_t CharacterStateIntermediateStruct = 0x187560; //E8 ? ? ? ? 48 8B BB ? ? ? ? 4C 8D 0D ? ? ? ? 4C 8D 83 ? ? ? ? 48 8B D3 49 8B CF
				constexpr inline uintptr_t oCharacterStateIntermediate = 0x1A20;
				constexpr inline uintptr_t oAbilityHasteMod = 0x18;
				constexpr inline uintptr_t oPercentCooldownCapMod = 0x30;
				constexpr inline uintptr_t oPassiveCooldownEndTime = 0x48;
				constexpr inline uintptr_t oPassiveCooldownTotalTime = 0x60;
				constexpr inline uintptr_t oFlatPhysicalDamageMod = 0xC0;
				constexpr inline uintptr_t oPercentPhysicalDamageMod = 0xD8;
				constexpr inline uintptr_t oPercentBonusPhysicalDamageMod = 0xF0;
				constexpr inline uintptr_t oPercentBasePhysicalDamageAsFlatBonusMod = 0x108;
				constexpr inline uintptr_t oFlatMagicDamageMod = 0x120;
				constexpr inline uintptr_t oPercentMagicDamageMod = 0x138;
				constexpr inline uintptr_t oGetPercentDamageToBarracksMinionMod = 0x1A9C;
				constexpr inline uintptr_t oGetFlatDamageReductionFromBarracksMinionMod = 0x1AB4; //4D 8D 86 ? ? ? ? 49 8B D6 48 8B CD
				constexpr inline uintptr_t oFlatMagicReduction = 0x150;
				constexpr inline uintptr_t oPercentMagicReduction = 0x168;
				constexpr inline uintptr_t oFlatCastRangeMod = 0x180;
				constexpr inline uintptr_t oAttackSpeedMod = 0x198;
				constexpr inline uintptr_t oPercentAttackSpeedMod = 0x1B0;
				constexpr inline uintptr_t oPercentMultiplicativeAttackSpeedMod = 0x1C8;
				constexpr inline uintptr_t oBaseAttackDamage = 0x1E0;
				constexpr inline uintptr_t oBaseAttackDamageSansPercentScale = 0x1F8;
				constexpr inline uintptr_t oFlatBaseAttackDamageMod = 0x210;
				constexpr inline uintptr_t oPercentBaseAttackDamageMod = 0x228;
				constexpr inline uintptr_t oBaseAbilityDamage = 0x240;
				constexpr inline uintptr_t oCritDamageMultiplier = 0x258;
				constexpr inline uintptr_t oScaleSkinCoef = 0x270;
				constexpr inline uintptr_t oDodge = 0x288;
				constexpr inline uintptr_t oCritPercent = 0x2A0;
				constexpr inline uintptr_t oFlatBaseHPPoolMod = 0x2B8;
				constexpr inline uintptr_t oArmor = 0x2D0;
				constexpr inline uintptr_t oBonusArmor = 0x2E8;
				constexpr inline uintptr_t oSpellBlock = 0x300;
				constexpr inline uintptr_t oBonusSpellBlock = 0x318;
				constexpr inline uintptr_t oHPRegenRate = 0x330;
				constexpr inline uintptr_t oBaseHPRegenRate = 0x348;
				constexpr inline uintptr_t oMoveSpeed = 0x360;
				constexpr inline uintptr_t oAttackRange = 0x390;
				constexpr inline uintptr_t oFlatBubbleRadiusMod = 0x3A8;
				constexpr inline uintptr_t oPercentBubbleRadiusMod = 0x3C0;
				constexpr inline uintptr_t oFlatArmorPenetration = 0x3D8;
				constexpr inline uintptr_t oPhysicalLethality = 0x3F0;
				constexpr inline uintptr_t oPercentArmorPenetration = 0x408;
				constexpr inline uintptr_t oPercentBonusArmorPenetration = 0x420;
				constexpr inline uintptr_t oPercentCritBonusArmorPenetration = 0x438;
				constexpr inline uintptr_t oPercentCritTotalArmorPenetration = 0x450;
				constexpr inline uintptr_t oFlatMagicPenetration = 0x468;
				constexpr inline uintptr_t oMagicLethality = 0x480;
				constexpr inline uintptr_t oPercentMagicPenetration = 0x498;
				constexpr inline uintptr_t oPercentBonusMagicPenetration = 0x4B0;
				constexpr inline uintptr_t oPercentLifeStealMod = 0x4C8;
				constexpr inline uintptr_t oPercentSpellVampMod = 0x4E0;
				constexpr inline uintptr_t oPercentOmnivampMod = 0x4F8;
				constexpr inline uintptr_t oPercentPhysicalVamp = 0x510;
				constexpr inline uintptr_t oPathfindingRadiusMod = 0x528;
				constexpr inline uintptr_t oPercentCCReduction = 0x540;
				constexpr inline uintptr_t oPercentEXPBonus = 0x558;
				constexpr inline uintptr_t oFlatBaseArmorMod = 0x570;
				constexpr inline uintptr_t oFlatBaseSpellBlockMod = 0x588;
				constexpr inline uintptr_t oPrimaryARBaseRegenRateRep = 0x5B8;
				constexpr inline uintptr_t oSecondaryARRegenRateRep = 0x5D0;
				constexpr inline uintptr_t oSecondaryARBaseRegenRateRep = 0x5E8;
			}

			namespace AIManager
			{
				constexpr inline uintptr_t AiManager = 0x3740; // https://www.unknowncheats.me/forum/3735791-post9262.html
				constexpr inline uintptr_t Manager = 0x10;
				constexpr inline uintptr_t ServerPosition = 0x414;
				constexpr inline uintptr_t TargetPos = 0x14;
				constexpr inline uintptr_t DashSpeed = 0x300;

				namespace Path
				{
					constexpr inline uintptr_t IsMoving = 0x2BC;
					constexpr inline uintptr_t IsDashing = 0x324;
					constexpr inline uintptr_t Segments = 0x2E8;
					constexpr inline uintptr_t CurrentSegment = 0x2C0;
					constexpr inline uintptr_t SegmentsCount = 0x2F0;
					constexpr inline uintptr_t PathStart = 0x2D0;
					constexpr inline uintptr_t PathEnd = 0x2DC;
				}
			}

			namespace Perks
			{
				constexpr inline uintptr_t Manager = 0x4680;
				constexpr inline uintptr_t Size = 0x90;
				constexpr inline uintptr_t PerkID = 0x8;
				constexpr inline uintptr_t PerkName = 0x10;
				constexpr inline uintptr_t PerkRawName = 0x20;
				constexpr inline uintptr_t PerkRawDescription = 0x30;
			}
		}

		namespace Inventory {
			//https://www.unknowncheats.me/forum/3881148-post10893.html
			constexpr inline uintptr_t Gold = 0x10;
			constexpr inline uintptr_t ItemSlot = 0x20;
			constexpr inline uintptr_t BaseItem = 0x10;
			constexpr inline uintptr_t ItemInfo = 0x38;
			constexpr inline uintptr_t ItemId = 0x9C;
		}

		namespace ItemManager
		{
			//E8 ? ? ? ? 48 8B F8 EB 02 33 FF 48 8D 4B 30  DA VEDERE
			constexpr inline uintptr_t Instance = 0x40A0;
			constexpr inline uintptr_t List = 0x20;
			constexpr inline uintptr_t Size = 0x20;
			constexpr inline uintptr_t Wrapper = 0x10;
			constexpr inline uintptr_t ItemSlot = 0x20;
			constexpr inline uintptr_t InventorySlot = 0x38;
			constexpr inline uintptr_t ItemId = 0x9C;
			constexpr inline uintptr_t ItemTexturePath = 0x4F0;
			constexpr inline uintptr_t ItemName = 0x530;
		}

		namespace SpellSlot {
			constexpr inline uintptr_t SpellBookSpellSlot = 0x6D0;

			constexpr inline uintptr_t Name = 0x80;
			constexpr inline uintptr_t Level = 0x28;
			constexpr inline uintptr_t Cooldown = 0x30;
			constexpr inline uintptr_t TotalCooldown = 0x74; // Oppure time charged
			constexpr inline uintptr_t Stacks = 0x5C;
			namespace Input	{
				constexpr inline uintptr_t SpellInput = 0x128;
				constexpr inline uintptr_t TargetNetID = 0x14;
				constexpr inline uintptr_t StartPos = 0x18;
				constexpr inline uintptr_t EndPos = 0x24;
			}

			namespace Info {
				constexpr inline uintptr_t SpellInfo = 0x130;
				constexpr inline uintptr_t Data = 0x60;
				constexpr inline uintptr_t TexturePath = 0x290;

				constexpr inline uintptr_t DataName = 0x80;
				constexpr inline uintptr_t DataManaCost = 0x604;
			}

		}

		namespace Buff {
			constexpr inline uintptr_t BuffManager = 0x2810;
			constexpr inline uintptr_t BuffNamePtr = 0x10;
			constexpr inline uintptr_t BuffName = 0x8;

			constexpr inline uintptr_t BuffEntry = 0x10;
			constexpr inline uintptr_t BuffType = 0x8;
			constexpr inline uintptr_t BuffEntryBuffStartTime = 0x18;
			constexpr inline uintptr_t BuffEntryBuffEndTime = 0x1C;
			constexpr inline uintptr_t BuffEntryBuffCount = 0x8C;
			constexpr inline uintptr_t BuffEntryBuffCountAlt = 0x38;
			constexpr inline uintptr_t BuffEntriesEnd = 0x10;
		}

		namespace Missile {
			constexpr inline uintptr_t Index = 0x28; // Lui aveva 0x60

			constexpr inline uintptr_t SpellName = 0x28; // Lui aveva 0x60
			constexpr inline uintptr_t SpellNameLength = 0x38; 
			constexpr inline uintptr_t SpellNameMaxLength = 0x60;

			constexpr inline uintptr_t MissileName = 0x28;
			constexpr inline uintptr_t MissileNameLength = 0x38; 
			constexpr inline uintptr_t MissileNameMaxLength = 0x60;

			constexpr inline uintptr_t Speed = 0x88;
			constexpr inline uintptr_t Position = 0x104;
			constexpr inline uintptr_t SrcIdx = 0x370;
			constexpr inline uintptr_t DestIdx = 0x3C8;
			constexpr inline uintptr_t DestCheck = 0x320;

			constexpr inline uintptr_t StartPosition = 0x38C;
			constexpr inline uintptr_t EndPosition = 0x398;

			namespace SpellInfo {
				constexpr inline uintptr_t SpellInfo = 0x2E8;
				constexpr inline uintptr_t SpellName = 0x28;
				constexpr inline uintptr_t MissileName = 0x118;
			}
			constexpr inline uintptr_t SpellCast = 0x250;

		}

		namespace AttackableUnit {

			constexpr inline uintptr_t Health = 0x1088;
			constexpr inline uintptr_t BonusHealth = 0x15C8;
			constexpr inline uintptr_t MaxHealth = 0x10A0;

			constexpr inline uintptr_t Mana = 0x370;
			constexpr inline uintptr_t MaxMana = 0x388;

			constexpr inline uintptr_t Visibility = 0x340;
			constexpr inline uintptr_t Targetable = 0xEE0;
			constexpr inline uintptr_t State = 0x1408;
			constexpr inline uintptr_t Invulnerability = 0x520;

			static uintptr_t constexpr fnCanHighlight = 0x24FCF0; // E8 ?? ?? ?? ?? 84 C0 74 35 48 8D 8F ?? ?? ?? ??
		}

		namespace Turret {
			static uintptr_t constexpr TargetNetworkID = 0x4138;
		}

		namespace Client {
			constexpr inline uintptr_t CombatType = 0x26A8; //UPDATED 13.13 -> MELEE 1? 2 == RANGED
			constexpr inline uintptr_t Name = 0x3628;  //UPDATED 13.13
			constexpr inline uintptr_t AttackRange = 0x16FC;

			constexpr inline uintptr_t LifeRegeneration = 0x16F0;

			constexpr inline uintptr_t BaseAttackDamage = 0x16B4;
			constexpr inline uintptr_t BonusAttackDamage = 0x1620;

			constexpr inline uintptr_t AbilityHaste = 0x14E0;
			constexpr inline uintptr_t Lethality = 0x15E0;

			constexpr inline uintptr_t ArmorPenetrationFlat = 0x15C8;

			constexpr inline uintptr_t MagicPenetrationFlat = 0x15CC;
			constexpr inline uintptr_t MagicPenetrationMultiplier = 0x11D0;

			constexpr inline uintptr_t AbilityPower = 0x1630;

			constexpr inline uintptr_t BonusArmor = 0x16E0;
			constexpr inline uintptr_t TotalArmor = 0x16DC;

			constexpr inline uintptr_t Shield = 0x10E8;
			constexpr inline uintptr_t PhysicalShield = 0x1100;
			constexpr inline uintptr_t MagicalShield = 0x1118;

			constexpr inline uintptr_t CritFlat = 0x1CC0;
			constexpr inline uintptr_t CritMultiplier = 0x1C78;

			constexpr inline uintptr_t AttackSpeedBonus = 0x164C;
			constexpr inline uintptr_t AttackSpeedMulti = 0x1668;

			constexpr inline uintptr_t BonusMagicResist = 0x16E8;
			constexpr inline uintptr_t TotalMagicResist = 0x16E4;

			constexpr inline uintptr_t MovementSpeed = 0x16F4;

			constexpr inline uintptr_t Ammo = 0x3F8;
			constexpr inline uintptr_t MaxAmmo = 0x410;

			constexpr inline uintptr_t ScaleMulti = 0x16CC;

			constexpr inline uintptr_t Experience = 0x4010;
			constexpr inline uintptr_t Level = 0x4028;

			namespace CharacterData {
				constexpr inline uintptr_t Instance = 0x3570;
				constexpr inline uintptr_t Base = 0x28;
				constexpr inline uintptr_t HealthBarOffset = 0xC4;
				constexpr inline uintptr_t Type = 0x7A0;
				constexpr inline uintptr_t TypeDetailed = 0x20;
				constexpr inline uintptr_t HeroID = 0x850;
			}

			static uintptr_t constexpr BuffInstance = 0x2810; //UPDATED 13.13
			static uintptr_t constexpr SpellBookInstance = 0x2A38;  //UPDATED 13.13
			static uintptr_t constexpr NavigationInstance = 0x3740; // UPDATED 13.13 // E8 ? ? ? ? 44 39 60 08  first [rcx + xxxh]

			static uintptr_t constexpr FnGetHealthBarPosition = 0x80ABC0; //13,12 0x7F50C0; // ACCESSES MAX ZOOM VALUE from debugger / E8 ? ? ? ? ? ? ? ? F3 0F 10 46 ? ? 4C 8D 4C 24 ? ?
			static uintptr_t constexpr FnGetBaseDrawPosition = 0x207120; //E8 ? ? ? ? EB 06 49 8B 06

			//ends 0x3ea4
		}
		namespace Hero {

			static QWORD constexpr FnGetAttackDelay = 0x3CD100;				//F3 0F 10 89 ? ? ? ? E9 ? ? ? ?
			static QWORD constexpr FnGetAttackCastDelay = 0x3CD000;			//E8 ? ? ? ? 48 8B CE F3 0F 11 83 ? ? ? ?
		}

		namespace BaseObject {

			static uintptr_t constexpr Handle = 0x0010; //UPDATED 13.13
			static uintptr_t constexpr NetworkID = 0xC8; //UPDATED 13.13
			static uintptr_t constexpr Team = 0x3C; //UPDATED 13.13
			static uintptr_t constexpr Name = 0x60; //UPDATED 13.13
			static uintptr_t constexpr Position = 0x280;//0x0f8;  //UPDATED 13.13  //PREVIOUS POS 0x220

			//ENDS 0X288
			//static QWORD constexpr FnIsAlive = 0x22DD60; // 13.12 0x229500;  // E8 ? ? ? ? 84 C0 74 35 48 8D 8F ? ? ? ? 
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

		namespace SpellBook {
			static uintptr_t constexpr OwnerHandle = 0x30;
			static uintptr_t constexpr SpellSlotInstance = 0x6D0;

			static uintptr_t constexpr FnGetSpellState = 0x6FE000;						//E8 ? ? ? ? 48 8B CE 44 8B F8
			static uintptr_t constexpr FnCastSpell = 0x8A9B40;							//E8 ? ? ? ? 83 7B 4C 00 75 3D  "ERROR: Client Tried to cast a spell from an invalid slot: %i.\n"
			//static QWORD constexpr FnHudCastSpell = 0x85A820; //13.12 0x845B60;   // E8 ?? ?? ?? ?? 48 8B 5C 24 ?? C6 87 ?? ?? ?? ?? ?? C6 87 ?? ?? ?? ?? ??
			//static QWORD constexpr ScreenMousePosition = 0x39985D0; //13.12 0x3968738; // 48 8B 0D ?? ?? ?? ?? 4C 8D 45 67

			namespace SpellSlot {

				static uintptr_t constexpr Level = 0x28;
				static uintptr_t constexpr CooldownExpire = 0x30;
				static uintptr_t constexpr Charges = 0x54;
				static uintptr_t constexpr CoolDown = 0x74;
				static uintptr_t constexpr TargetingClientInstance = 0x128;
				static uintptr_t constexpr SpellInfoInstance = 0x130;
				namespace TargetingClient {
					static uintptr_t constexpr SourceHandle = 0x10;
					static uintptr_t constexpr TargetHandle = 0x14;
					static uintptr_t constexpr StartPosition = 0x18;
					static uintptr_t constexpr EndPosition = 0x24;
					static uintptr_t constexpr MousePosition = 0x30;
					static uintptr_t constexpr MousePosition2 = 0x3c;
				}

			}
		}
	}
}


