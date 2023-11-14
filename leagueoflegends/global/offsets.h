#pragma once
namespace UPasta
{
	namespace Offsets
	{
		namespace Instance
		{
			constexpr inline uintptr_t SpoofGadget = 0xD8C410;								//E8 ? ? ? ? 44 8B 40 10
			constexpr inline uintptr_t LocalPlayer = 0x220C7D8;								//48 8B 05 ? ? ? ? 4C 8B D2 4C 8B C1

			namespace Game	{
				constexpr inline uintptr_t GameTime = 0x21FA3A8;							//F3 0F 5C 35 ? ? ? ? 0F 28 F8
				constexpr inline uintptr_t GameState = 0x21F2CC0;							//48 8D 4D D7 48 8B 05 ? ? ? ?

				namespace MapInfo {
					constexpr inline uintptr_t GameMap = 0x1A7E50;							//E8 ? ? ? ? 48 8B 8F ? ? ? ? E8 ? ? ? ? 48 8D 8D ? ? ? ? 
					constexpr inline uintptr_t LobbyName = 0x38;								
					constexpr inline uintptr_t PlayerIP = 0x140;								
				}

			}
			
			namespace Lists {
				constexpr inline uintptr_t ObjManager = 0x21EFC00;							//48 8B 0D ? ? ? ? E8 ? ? ? ? 33 ED 48 8B F8
				constexpr inline uintptr_t ManagerList = 0x8;								//E8 ? ? ? ? E9 ? ? ? ? 80 FA 05
				constexpr inline uintptr_t ManagerListSize = 0x10;							

				constexpr inline uintptr_t HeroList = 0x21EFD20;							//48 8B 05 ? ? ? ? 45 33 E4 0F 57 C0
				constexpr inline uintptr_t AttackableList = 0x21EFD60;						//EB CE 48 8B 82 ? ? ? ?
				constexpr inline uintptr_t MinionList = 0x21F2C80;							//48 8B 0D ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 48 8B C8
				constexpr inline uintptr_t TurretList = 0x21F9620;							//48 8B 1D ? ? ? ? 48 8B 5B 28
				constexpr inline uintptr_t InhibList = 0x220CAB8;							//48 8B 05 ? ? ? ? 48 89 7C 24 ? 48 8B 58 08
				constexpr inline uintptr_t MissileList = 0x220C8C0;							//48 8B 0D ? ? ? ? 48 8D 54 24 ? E8 ? ? ? ? 48 8B 7C 24 ?
			}

			namespace HUD	{

				constexpr inline uintptr_t HudInstance = 0x21EFC10;							//48 8B 0D ? ? ? ? 8B 57 10
				constexpr inline uintptr_t Input = 0x28;
				constexpr inline uintptr_t UserData = 0x60;
				constexpr inline uintptr_t UserDataSelectedNetId = 0x28;
				constexpr inline uintptr_t SpellInfo = 0x68;

				namespace Mouse {
					constexpr inline uintptr_t Instance = 0x21F2C48;						//48 8B 0D ? ? ? ? 48 83 C1 20 E9 ? ? ? ?
					constexpr inline uintptr_t Position = 0x0C;								//E9 ? ? ? ? 48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ?
					constexpr inline uintptr_t UnderMouse = 0x21F2E78;						//48 8B 05 ? ? ? ? 48 8B F9 33 C9 48 8B DA

					constexpr inline uintptr_t InputMouseWorldPos = 0x20;					
				}

				namespace Camera	{
					constexpr inline uintptr_t Instance = 0x21F88C8;						//48 8B 05 ? ? ? ? 48 8B 5C 24 ? 48 89 47 70
					constexpr inline uintptr_t Camera = 0x18;						
					constexpr inline uintptr_t Value = 0x2B8;						
					constexpr inline uintptr_t Limit = 0x28;						
				}

				namespace Chat {
					constexpr inline uintptr_t Instance = 0x21F2EB0;						//75 78 48 8B 1D ? ? ? ? 
					constexpr inline uintptr_t Client = 0x220C960;							//48 8B 05 ? ? ? ? 88 90 ? ? ? ? C3
					constexpr inline uintptr_t IsOpen = 0xC90;							
				}

				namespace Shop {
					constexpr inline uintptr_t Instance = 0x21F2EB0;						//75 4B 48 8B 0D ? ? ? ? 48 85 C9 DA TROVARE
					constexpr inline uintptr_t IsOpen = 0xC;							
				}

				namespace Minimap {
					constexpr inline uintptr_t Instance = 0x21FF7F0;						//48 8B 1D ? ? ? ? 48 85 DB 74 22
					constexpr inline uintptr_t HudInstance = 0x320;
					constexpr inline uintptr_t Position = 0x60;
					constexpr inline uintptr_t Size = 0x68;
					//https://www.unknowncheats.me/forum/3727406-post9100.html i need this shit
				}

				namespace Viewport {
					constexpr inline uintptr_t ViewProjMatrices = 0x2255C90;				//48 8D 0D ? ? ? ? 0F 10 00
					constexpr inline uintptr_t ViewPort = 0x21F2C40;						//48 8B 3D ? ? ? ? FF 90 ? ? ? ?
					constexpr inline uintptr_t W2S = 0x270;							
				}
			}

			constexpr inline uintptr_t PingNet = 0x21EFBF0;									//48 8B 05 ? ? ? ? 4C 8B CA 48 8B D9
		}

		namespace Functions
		{
			namespace Drawings {
				constexpr inline uintptr_t GetBaseDrawPosition = 0x205B70;					//E8 ? ? ? ? EB 06 49 8B 06
				constexpr inline uintptr_t WorldToScreen = 0xE49C20;						//E8 ? ? ? ? 49 8D 97 ? ? ? ? 4C 8D 45 D8
				constexpr inline uintptr_t GetCollisionFlags = 0xDBFD60;					//48 83 EC 28 48 8B D1 48 8B 0D ? ? ? ? 48 8B 49 08 E8 ? ? ? ? 48 8B C8 48 85 C0 74 1A
			}

			namespace Orders {
				constexpr inline uintptr_t IssueClick = 0x8C4C80;							//45 33 C0 E8 ? ? ? ? 48 83 C4 48
				constexpr inline uintptr_t IssueRClick = 0x8AEEB0;							//48 89 5C 24 ? 57 48 83 EC 50 48 8B D9 0F 29 74 24 ? 48 8B 0D ? ? ? ?
				constexpr inline uintptr_t IssueMove = 0x8AE290;							//48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B F1 41 0F B6 F9
				constexpr inline uintptr_t BuildNavPath = 0xDBBEF0;							//48 8B C4 48 89 58 10 55 56 57 41 54 41 55 41 56 41 57 48 8D
				constexpr inline uintptr_t BuildSmoothPath = 0x900EA0;						//40 53 55 41 56 48 83 EC 50 48
			}

			namespace HUD {

				namespace Chat {
					constexpr inline uintptr_t PrintChat = 0x862CE0;						//E8 ? ? ? ? 4C 8B C3 B2 01
				}

				constexpr inline uintptr_t GetCircleIconPortrait = 0x2056D0;				//E8 ? ? ? ? 4C 3B F8 0F 94 C0
				constexpr inline uintptr_t GetPing = 0x49D160;								//E8 ? ? ? ? 8B F8 39 03
				constexpr inline uintptr_t SendPing = 0x195F80;								//E8 ? ? ? ? 48 8B 8B ? ? ? ? 44 89 B9 ? ? ? ?

				constexpr inline uintptr_t RenderUsualSuspects = 0x60DC20;					//48 8B C4 48 89 58 18 89

				constexpr inline uintptr_t GetHeightAtPosition = 0xDCA190;					//E8 ? ? ? ? F3 0F 10 45 ? B0 01
				constexpr inline uintptr_t IsNotWall = 0xDC6010;							//E8 ? ? ? ? 34 01 44 3A F0

			}

			namespace Stats {
				constexpr inline uintptr_t HasBuff = 0x20DFC0;								//48 89 5C 24 ? 57 48 83 EC 20 48 8B 01 41 8B D8
				constexpr inline uintptr_t GetStatTotal = 0x6A0E90;							//E8 ? ? ? ? 0F 28 F8 41 8B 47 18
				constexpr inline uintptr_t GetOwner = 0x18F2C0;								//E8 ? ? ? ? 4C 3B F8 0F 94 C0
				constexpr inline uintptr_t GetOwnerPlayer = 0x2090E0;						//E8 ? ? ? ? 40 38 30 75 24

				constexpr inline uintptr_t GetAttackDelay = 0x3A3BD0;						//F3 0F 10 89 ? ? ? ? E9 ? ? ? ?
				constexpr inline uintptr_t GetAttackCastDelay = 0x3A3AD0;					//E8 ? ? ? ? 48 8B CE F3 0F 11 83 ? ? ? ?
				constexpr inline uintptr_t GetBoundingRadius = 0x2030C0;					//E8 ? ? ? ? 0F 28 F8 48 8B D6
				constexpr inline uintptr_t GetObjectFromNetID = 0x3A7110;					//4C 8B C1 85 D2 74 1B

				constexpr inline uintptr_t IsNotLocalPlayer = 0x211C30;						//33 C0 48 3B 0D ? ? ? ? 
				constexpr inline uintptr_t IsAttackingLocalPlayer = 0x2136D0;				//5B C3 CC CC CC CC CC 48 8D 91 ? ? ? ? SUB SOTTO (QUESTO E' SOLO UN RIFERIMENTO)
				constexpr inline uintptr_t IsObjectType = 0x213030;							//0F B6 41 48 4C 8B C9
				constexpr inline uintptr_t IsCanSee = 0x25FA80;								//40 53 48 83 EC 20 48 8B 01 48 8B D9 FF 90 ? ? ? ? 84 C0 75 19
				constexpr inline uintptr_t IsVisible = 0x2145D0;							//48 89 5C 24 ? 56 48 83 EC 20 8B 91 ? ? ? ?
				constexpr inline uintptr_t IsTargetable = 0xA3C150;							//40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 84 C0 74 41
				constexpr inline uintptr_t IsAlive = 0x24F870;								//E8 ? ? ? ? 84 C0 74 35 48 8D 8F ? ? ? ?
				constexpr inline uintptr_t IsDead = 0x212230;								//E8 ? ? ? ? 0F B6 F0 EB 17
				constexpr inline uintptr_t IsHero = 0x267B10;								//E8 ? ? ? ? 84 C0 48 0F 45 F3
				constexpr inline uintptr_t IsMinion = 0x267B70;								//E8 ? ? ? ? 48 8B 0B F3 0F 10 41 ?
				constexpr inline uintptr_t IsTurret = 0x267CC0;								//E8 ? ? ? ? 84 C0 74 56 48 8B 06
				constexpr inline uintptr_t IsMissile = 0x267BA0;							//E8 ? ? ? ? 84 C0 74 0C 48 8B 17
				constexpr inline uintptr_t IsInhibitor = 0x267A50;							//E8 ? ? ? ? 84 C0 75 0C 33 D2
				constexpr inline uintptr_t IsNexus = 0x267A90;								//E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? F3 0F 10 44 24 ? 48 8D 54 24 ?
			}

			namespace Spells {
				constexpr inline uintptr_t HudCastSpell = 0x8B9AF0;							//E8 ? ? ? ? 48 8B 53 08 48 8B 42 60
				constexpr inline uintptr_t NewCastSpell = 0x8A16C0;							//E8 ? ? ? ? 83 7B 4C 00 75 3D
				constexpr inline uintptr_t ReleaseSpell = 0x891A50;							//48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 40 8B DA 48 8B F9

				constexpr inline uintptr_t GetSpellBook = 0xD63FA0;							//48 83 EC 28 4C 8B 41 08 44 8B CA ERA E8 ? ? ? ? 49 8B 5D 68 4C 8B F0 
				constexpr inline uintptr_t GetSpellSlot = 0x89D410;							//E8 ? ? ? ? 83 F8 FF 74 0A
				constexpr inline uintptr_t GetSpellState = 0x6F6140;						//E8 ? ? ? ? 48 8B CE 44 8B F8
				constexpr inline uintptr_t GetSpellRange = 0x6A0E90;						//E8 ? ? ? ? 0F 28 F8 41 8B 47 18

			}

			namespace Skin {
				constexpr inline uintptr_t CharacterDataStackUpdate = 0x18AC80;				//E8 ? ? ? ? 80 BD ? ? ? ? ? 74 0D
			}

			namespace Shop	{
				//ShopActions 40 55 53 56 41 56 41 57 48 8D 6C
				constexpr inline uintptr_t BuyItem = 0x1F05C0;								//40 55 53 56 41 56 41 57 48 8D 6C 24 ? 48 81 EC
				constexpr inline uintptr_t SellItem = 0x0;									//40 57 48 83 EC 20 8B FA E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 89 5C 24 13.17 hotfix worka DA RITROVARE
				constexpr inline uintptr_t UndoAction = 0x251A80;							//48 89 5C 24 ? 57 48 83 EC 30 48 8B F9 48 8B 49 08 48 8B 01 FF 90 ? ? ? ? 48 85 C0 74 15 48 8B 4F 08 48 8B 01

			}
		}

		namespace Events
		{
			namespace Objects	{
				constexpr inline uintptr_t OnCreateObject = 0x3BC8F0;						//E8 ? ? ? ? 48 8B 3D ? ? ? ? 8B 9E ? ? ? ? 
				constexpr inline uintptr_t OnDeleteObject = 0x3A5290;						//E8 ? ? ? ? 48 8B 73 50
				constexpr inline uintptr_t GetRespawnTimeRemaining = 0x209CF0;				//40 53 48 83 EC 20 48 8B D9 48 81 C1 ? ? ? ? E8 ? ? ? ? 0F B6 83 ? ? ? ?
			}

			namespace Spellcast {
				constexpr inline uintptr_t OnStart = 0x719B70;								//40 53 48 83 EC 30 4C 8B 0A  
				constexpr inline uintptr_t OnProcess = 0x161EE0;							//48 89 5C 24 ? 55 56 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 44 8B 05 ? ? ? ? 
				constexpr inline uintptr_t ProcessIndex = 0x11B8;							//48 81 C1 ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 48 83 7B ? ? 72 03 48 8B 1B 4C 8B C3 
				constexpr inline uintptr_t OnImpact = 0x710CD0;								//48 89 5C 24 10 48 89 6C 24 18 57 41 54 41 55 41 56 41 57 48 81 QWORD* __fastcall Spell::SpellbookClient::OnSpellImpact(Spell::SpellbookClient* a1, const Spell::SpellCastInfo *a2) https://streamable.com/x4e7ib https://streamable.com/xuvq05 https://www.unknowncheats.me/forum/3755876-post9525.html
				constexpr inline uintptr_t OnStop = 0x719ED0;								//44 88 4C 24 20 4C 89 44 24 18 88 
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
			constexpr inline uintptr_t Team = 0x3C;
			constexpr inline uintptr_t Alive = 0x358;

			constexpr inline uintptr_t BaseAtk = 0x16B4;
			constexpr inline uintptr_t BonusAtk = 0x1620;//0x1AE0

			constexpr inline uintptr_t Crit = 0x1CC0;
			constexpr inline uintptr_t CritMulti = 0x1C78;

			constexpr inline uintptr_t AbilityPower = 0x15E8;

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
				constexpr inline uintptr_t Type = 0x778;
				constexpr inline uintptr_t TypeDetailed = 0x20;
				constexpr inline uintptr_t HeroID = 0x830;
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
	}
}


