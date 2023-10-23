#pragma once

// Functions
#define oGetBaseDrawPosition 0x202830 // E8 ? ? ? ? EB 06 49 8B 06 Stesso sub della sig OK
#define oWorldToScreen 0xE22860 // 48 83 EC 38 49 8B C0  Sub sopra a tutto OK
#define oTryRightClick 0x89F6E0 // 48 89 5C 24 ? 57 48 83 EC 50 48 8B D9 0F 29 74 24 ? 48 8B 0D ? ? ? ? Sub sopra a tutto OK
#define oIssueClick  0x8B50B0 // 44 88 44 24 ? 48 89 4C 24 ? 55
#define oIssueMove  0x89EA60 // 48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B F1 41 0F B6 F9 
#define oCastSpellWrapper 0x8AA010 // E8 ? ? ? ? 48 8B 53 08 48 8B 42 60
#define oReleaseSpell 0x883190 // 48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 40 8B DA 48 8B F9 
#define oPrintChat 0x857560 // E8 ? ? ? ? 4C 8B C3 B2 01 Stesso sub della sig OK
#define oGetBoundingRadius 0x1FFDE0 // E8 ? ? ? ? 0F 28 F8 48 8B D6 Stesso sub della sig FORSE TROVATO
#define oGetAttackDelay 0x3C1860 // F3 0F 10 89 ? ? ? ? E9 ? ? ? ?
#define oGetAttackWindup 0x3C1760 // E8 ? ? ? ? 49 8B CE F3 0F 11 83 ? ? ? ? Stesso sub della sig OK
#define oGetObjectFromNetId 0x3A06B0 // 4C 8B C1 85 D2 74 1B Sub sopra a tutto OK
#define oGetCollisionFlags 0xD99070 // 48 83 EC 28 48 8B D1 48 8B 0D ? ? ? ? 48 8B 49 08 E8 ? ? ? ? 48 8B C8 48 85 C0 74 1A Stesso sub della sig FORSE TROVATO
#define oCharacterDataStackUpdate 0x188B40 // E8 ? ? ? ? 80 BD ? ? ? ? ? 74 0D Stesso sub della sig

#define fIsNotLocalPlayer  0x20D6B0 // 33 C0 48 3B 0D ? ? ? ? 
#define fIsAttackingLocalPlayer  0x20F0F0 // 5B C3 CC CC CC CC CC 48 8D 91 ? ? ? ? SUB SOTTO (QUESTO E' SOLO UN RIFERIMENTO)
#define fIsAlive 0x24A1D0 //E8 ? ? ? ? 84 C0 74 35 48 8D 8F ? ? ? ? 
#define fIsHero 0x2617C0 //E8 ? ? ? ? 84 C0 48 0F 45 F3 Sub sopra a tutto
#define fIsMinion 0x261820 //E8 ? ? ? ? 48 8B 0B F3 0F 10 41 ? Sub sopra a tutto
#define fIsTurret 0x261970 //E8 ? ? ? ? 84 C0 74 56 48 8B 06 Sub sopra a tutto
#define fIsMissile 0x261850 //E8 ? ? ? ? 84 C0 74 0C 48 8B 17 Sub sopra a tutto
#define fIsInhibitor 0x261700 //E8 ? ? ? ? 84 C0 75 0C 33 D2 Sub sopra a tutto
#define fIsNexus 0x7FF615B01740 //E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? F3 0F 10 44 24 ? 48 8D 54 24 ? Sub sopra a tutto
#define fIsObjectType 0x20EA80 //0F B6 41 48 4C 8B C9 Sub sopra a tutto
#define fIsDead 0x20DCB0 // E8 ? ? ? ? 0F B6 F0 EB 17 Sub sopra a tutto è un int
#define fIsCanSee  0x259820 // 40 53 48 83 EC 20 48 8B 01 48 8B D9 FF 90 ? ? ? ? 84 C0 75 19 Sub sopra a tutto è un int
#define fIsNotWall  0xD9F160 // E8 ? ? ? ? 34 01 44 3A F0
#define fGetPing 0x498700 // E8 ? ? ? ? 8B F8 39 03 Sub sopra a tutto
#define fRenderUsualSuspects 0x5F9180 // 48 8B C4 48 89 58 18 89
#define fIsTargetable  0xA236F0 // 40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 84 C0 74 41
#define fIsVisible  0x20FFA0 //40 56 48 83 EC 20 48 8B F1 48 81 C1 ? ? ? ? E8 ? ? ? ?
#define fGetSpellSlot 0x88E260 // E8 ? ? ? ? 83 F8 FF 74 0A
#define fGetSpellState 0x6E9360 // E8 ? ? ? ? 48 8B CE 44 8B F8
#define fGetSpellRange 0x68D340 // E8 ? ? ? ? 0F 28 F8 41 8B 47 18
#define fGetStatTotal 0x68D340 // E8 ? ? ? ? 0F 28 F8 41 8B 47 18
#define fGetOwnerPlayer  0x205CC0 //E8 ? ? ? ? 40 38 30 75 24
#define fGetHeightAtPosition 0xDA32D0 //E8 ? ? ? ? F3 0F 10 45 ? B0 01

// Events not needed now
#define fOnCreateObject  0x3B1B90  // 40 53 48 83 EC 30 4C 8B 0A DWORD64
#define fOnDeleteObject  0x39E870  // 40 53 48 83 EC 30 4C 8B 0A DWORD64
#define fOnStartSpellCast 0x70A480 // 40 53 48 83 EC 30 4C 8B 0A DWORD64
#define fOnProcessSpell 0x159EF0 // 48 89 5C 24 ? 55 56 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 44 8B 05 ? ? ? ? 
#define fOnProcessSpellIndex 0x1198 // 48 81 C1 ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 48 83 7B ? ? 72 03 48 8B 1B 4C 8B C3 https://www.unknowncheats.me/forum/3750580-post9420.html
#define fOnStopSpellCast 0x70A7E0 // 44 88 4C 24 20 4C 89 44 24 18 88 DWORD64
#define fOnSpellImpact 0x701D90 //48 89 5C 24 10 48 89 6C 24 18 57 41 54 41 55 41 56 41 57 48 81 QWORD* __fastcall Spell::SpellbookClient::OnSpellImpact(Spell::SpellbookClient* a1, const Spell::SpellCastInfo *a2) https://streamable.com/x4e7ib https://streamable.com/xuvq05 https://www.unknowncheats.me/forum/3755876-post9525.html
#define fGetRespawnTimeRemaining  0x206870 //40 53 48 83 EC 20 48 8B D9 48 81 C1 ? ? ? ? E8 ? ? ? ? 0F B6 83 ? ? ? ?
#define fShopActions 0x0 //40 55 53 56 41 56 41 57 48 8D 6C
#define fGetCircleIconPortrait 0x18D170 //E8 ? ? ? ? 4C 3B F8 0F 94 C0
#define fBuildNavPath 0xD95210 // 48 8B C4 48 89 58 10 55 56 57 41 54 41 55 41 56 41 57 48 8D
#define fSmoothPath  0x8F17C0 // 40 53 55 41 56 48 83 EC 50 48
#define fBuyItem  0x0 //40 55 53 56 41 56 41 57 48 8D 6C 24 ? 48 81 EC
#define fSellItem  0x0 //40 57 48 83 EC 20 8B FA E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 89 5C 24 13.17 hotfix worka DA RITROVARE
#define fUndoItem  0x0 //48 89 5C 24 ? 57 48 83 EC 30 48 8B F9 48 8B 49 08 48 8B 01 FF 90 ? ? ? ? 48 85 C0 74 15 48 8B 4F 08 48 8B 01

// Variables
#define oGameMap 0x1A5BB0 // E8 ? ? ? ? 48 8B 8F ? ? ? ? E8 ? ? ? ? 48 8D 8D ? ? ? ? 
#define oLobbyName 0x38
#define oServerIP 0x140

#define oSpoofGadget 0xD65FA0 // E8 ? ? ? ? 44 8B 40 10
#define oGameTime 0x21C6E50 // F3 0F 5C 35 ? ? ? ? 0F 28 F8 Stessa qword della sig OK
#define oLocalPlayer 0x21D3D70 // 48 8B 05 ? ? ? ? 4C 8B D2 4C 8B C1 Stessa qword della sig OK

//TUTTO IL RESTO DA CHECK 
#define oHudInstance 0x21B7390 // 48 8B 0D ? ? ? ? 8B 57 10 OK
#define oMouseInstance 0x21BA348 //48 8B 0D ? ? ? ? 48 83 C1 20 E9 ? ? ? ?
#define oZoomInstance 0x21BCA08 // 48 8B 05 ? ? ? ? 48 8B 5C 24 ? 48 89 47 70 OK
#define oChatClient 0x21D3F18 // 41 FF D1 48 8B 0D ? ? ? ? 0F B6 D8 
#define oChatInstance 0x21BA5A8 // 75 78 48 8B 1D ? ? ? ? 
#define oViewport 0x21BA340 // 48 8B 3D ? ? ? ? FF 90 ? ? ? ? OK
#define oObjManager 0x21B7380 // 48 8B 0D ? ? ? ? E8 ? ? ? ? 33 ED 48 8B F8 OK
#define oGetObjectFromNetIdParam 0x21B7380 // 48 8B 0D ? ? ? ? E8 ? ? ? ? 33 ED 48 8B F8 stesso di objmanager OK
#define oBuildingsManager 0x21BA380 // 4C 8B 05 ?? ?? ?? ?? 49 8B 78 08 41 8B 40 10 48 8D 0C C7 48 3B F9 73 49
#define oMinimapObject 0x21C6EB8 // 48 8B 1D ? ? ? ? 48 85 DB 74 22 OK
#define oMissilesList 0x21D3E78 // 48 8B 0D ? ? ? ? 48 8D 54 24 ? E8 ? ? ? ? 48 8B 7C 24 ? https://www.unknowncheats.me/forum/league-of-legends/310587-league-legends-reversal-structs-offsets-478.html https://www.unknowncheats.me/forum/league-of-legends/434211-reading-missiles-externally.html
#define oAttackableList 0x21B74E0 // EB CE 48 8B 82 ? ? ? ?  Stessa qword FORSE TROVATO
#define oHeroList 0x21B74A0 // 48 8B 05 ? ? ? ? 45 33 E4 0F 57 C0
#define oMinionsList 0x21BA380 // 48 89 0D ? ? ? ? 48 8D 05 ? ? ? ? 33 D2 48 89 01 48 8D 05 ? ? ? ? 
#define oTurretsList 0x21C0C80 // E8 ? ? ? ? 48 8B C8 EB 03 48 8B CB 
#define oInhibitorList 0x21D4078 // 48 8B 05 ? ? ? ? 48 89 7C 24 50

// Structs
#define oMousePosition 0x0C 

#define oManagerList 0x8
#define oManagerListSize 0x10

#define oViewportW2S 0x270

#define oHudInstanceCamera 0x18
#define oHudInstanceInput 0x28
#define oHudInstanceUserData 0x60
#define oHudInstanceSpellInfo 0x68

#define oHudInstanceCameraZoom 0x2B8

#define oHudInstanceInputMouseWorldPos 0x20

#define oHudInstanceUserDataSelectedObjectNetId 0x28

#define oZoomInstanceMaxZoom 0x28

#define oChatClientChatOpen 0xCC1

//structs
#define oObjNetId 0x10
#define oObjTeam 0x3C
#define oObjIssueClickFloatCheck1 0x84
#define oObjIssueClickFloatCheck2 0x88
#define oObjIssueClickCheck 0xD4 
#define oObjPosition 0x220
#define oObjVisible 0x320
#define oObjAlive 0x338

#define oObjMana 0x350
#define oObjMaxMana 0x368

#define oObjAmmo 0x3F8
#define oObjMaxAmmo 0x410

#define oObjIsInvulnerable 0x500
#define oObjTargetable 0xEC0

#define oObjHealth 0x1068
#define oObjMaxHealth 0x1080
#define oObjBonusHealth 0x15C8
#define oObjHealthRegen 0x16B8

#define oObjArmor 0x16A4
#define oObjBonusArmor 0x16A8
#define oObjArmorPen 0X1590
#define oObjectShield 0x10C8 
#define oObjectPhysicalShield 0x10F0

#define oObjMagicResist 0x16AC
#define oObjBonusMagicResist 0x16B0
#define oObjMagicPen 0x1594
#define oObjMagicPenMulti 0x11D0
#define oObjectMagicalShield 0x1108

#define oObjActionState 0x13D0

#define oObjLethality 0x15A8

#define oObjBonusAttackDamage 0x15E8
#define oObjBaseAttackDamage 0x167C
#define oObjScale 0x1694

#define oObjAbilityPower 0x15F8
#define oObjLevel 0x3FD0
#define oObjExperience 0x3FB8

#define oObjMovementSpeed 0x16BC
#define oObjAttackRange 0x16C4
#define oObjAtkSpeedMulti 0x1B98
#define oObjAbilityHaste  0x14A8

#define oObjBuffManager 0x27D8 
#define oObjSpellBook 0x29F8	
#define oObjActiveSpellCast 0x2A30

#define oObjName 0x35E8
#define oObjCharacterDataStack 0x35C0
#define oObjAiManager 0x3700 // https://www.unknowncheats.me/forum/3735791-post9262.html

#define oObjCharData 0x3530
#define oObjCharDataData 0x28
#define oObjCharDataDataSize 0xC4
#define oObjCharDataDataObjType 0x778 
#define oObjCharDataDataObjTypeDetailed 0x20
#define oObjCharDataDataHeroID 0x830

#define oObjBuffManagerEntriesEnd 0x10
#define oBuffEntryBuff 0x10
#define oBuffType 0x8
#define oBuffNamePtr 0x10
#define oBuffStartTime 0x18
#define oBuffEndTime 0x1C
#define oBuffStacksAlt 0x38
#define oBuffStacks 0x8C
#define oBuffNamePtrName 0x8

#define oObjSpellBookSpellSlot 0x6D0

#define oSpellSlotLevel 0x28
#define oSpellSlotCooldown 0x30
#define oSpellSlotStacks 0x5C
#define oSpellSlotTotalCooldown 0x74
#define oSpellSlotSpellInput 0x128
#define oSpellSlotSpellInfo 0x130
#define oSpellSlotSpellCharges 0x5C
#define oSpellSlotSpellTimeCharges 0x74

#define oSpellInputTargetNetId 0x14
#define oSpellInputStartPos 0x18
#define oSpellInputEndPos 0x24
#define oSpellInfoSpellData 0x60

#define oSpellDataSpellName 0x80
#define oSpellDataManaCost 0x600

#define oActiveSpellCastSpellInfo 0x8
#define oActiveSpellCastSpellType 0x10
#define oActiveSpellCastSpellId 0x124

#define oObjAiManagerManager 0x10
#define oObjAiManagerManagerTargetPosition 0x14
#define oObjAiManagerManagerIsMoving 0x2BC
#define oObjAiManagerManagerCurrentSegment 0x2C0
#define oObjAiManagerManagerPathStart 0x2D0
#define oObjAiManagerManagerPathEnd 0x2DC
#define oObjAiManagerManagerSegments 0x2E8
#define oObjAiManagerManagerSegmentsCount 0x2F0
#define oObjAiManagerManagerDashSpeed 0x300
#define oObjAiManagerManagerIsDashing 0x324
#define oObjAiManagerManagerPosition 0x414

#define oPlayerName 0x0 //https://www.unknowncheats.me/forum/league-of-legends/310587-league-legends-reversal-structs-offsets-483.html

#define UnderMouseObject 0x31C4818

// For Missile
#define oMissileSpellInfo 0x2E8

#define oMissileSpellName 0x28
#define oMissileSpellNameLength 0x38
#define oMissileSpellNameMaxLength 0x48

#define oMissileName 0x118
#define oMissileNameLength 0x128
#define oMissileNameMaxLength 0x148


#define oMissileMapCount 0X10
#define oMissileMapRoot 0x8
#define oMissileMapKey 0x20
#define oMissileMapVal 0x28

#define oMissileSpeed 0x88

#define oMissileSrcIdx 0x370
#define oMissileDestIdx 0x3C8
#define oMissileDestCheck 0x320
#define oMissileStartPos 0x38C
#define oMissilePos 0x104
#define oMissileEndPos 0x398
#define oObjMissileSpellCast 0x250

#define MinimapObjectHud 0x320
#define MinimapHudPos 0x60
#define MinimapHudSize 0x68
//https://www.unknowncheats.me/forum/3727406-post9100.html i need this shit

//CharacterStateIntermediate
#define CharacterStateIntermediateStruct 0x187560 //E8 ? ? ? ? 48 8B BB ? ? ? ? 4C 8D 0D ? ? ? ? 4C 8D 83 ? ? ? ? 48 8B D3 49 8B CF
#define oCharacterStateIntermediate 0x19E8 
#define oAbilityHasteMod 0x18
#define oPercentCooldownCapMod 0x30
#define oPassiveCooldownEndTime 0x48
#define oPassiveCooldownTotalTime 0x60
#define oFlatPhysicalDamageMod 0xC0
#define oPercentPhysicalDamageMod 0xD8
#define oPercentBonusPhysicalDamageMod 0xF0
#define oPercentBasePhysicalDamageAsFlatBonusMod 0x108
#define oFlatMagicDamageMod 0x120
#define oPercentMagicDamageMod 0x138
#define oGetPercentDamageToBarracksMinionMod 0x1A50
#define oGetFlatDamageReductionFromBarracksMinionMod 0x1A68
#define oFlatMagicReduction 0x150
#define oPercentMagicReduction 0x168
#define oFlatCastRangeMod 0x180
#define oAttackSpeedMod 0x198
#define oPercentAttackSpeedMod 0x1B0
#define oPercentMultiplicativeAttackSpeedMod 0x1C8
#define oBaseAttackDamage 0x1E0
#define oBaseAttackDamageSansPercentScale 0x1F8
#define oFlatBaseAttackDamageMod 0x210
#define oPercentBaseAttackDamageMod 0x228
#define oBaseAbilityDamage 0x240
#define oCritDamageMultiplier 0x258
#define oScaleSkinCoef 0x270
#define oDodge 0x288
#define oCritPercent 0x2A0
#define oFlatBaseHPPoolMod 0x2B8
#define oArmor 0x2D0
#define oBonusArmor 0x2E8
#define oSpellBlock 0x300
#define oBonusSpellBlock 0x318
#define oHPRegenRate 0x330
#define oBaseHPRegenRate 0x348
#define oMoveSpeed 0x360
#define oAttackRange 0x390
#define oFlatBubbleRadiusMod 0x3A8
#define oPercentBubbleRadiusMod 0x3C0
#define oFlatArmorPenetration 0x3D8
#define oPhysicalLethality 0x3F0
#define oPercentArmorPenetration 0x408
#define oPercentBonusArmorPenetration 0x420
#define oPercentCritBonusArmorPenetration 0x438
#define oPercentCritTotalArmorPenetration 0x450
#define oFlatMagicPenetration 0x468
#define oMagicLethality 0x480
#define oPercentMagicPenetration 0x498
#define oPercentBonusMagicPenetration 0x4B0
#define oPercentLifeStealMod 0x4C8
#define oPercentSpellVampMod 0x4E0
#define oPercentOmnivampMod 0x4F8
#define oPercentPhysicalVamp 0x510
#define oPathfindingRadiusMod 0x528
#define oPercentCCReduction 0x540
#define oPercentEXPBonus 0x558
#define oFlatBaseArmorMod 0x570
#define oFlatBaseSpellBlockMod 0x588
#define oPrimaryARBaseRegenRateRep 0x5B8
#define oSecondaryARRegenRateRep 0x5D0
#define oSecondaryARBaseRegenRateRep 0x5E8

//Items managament
#define oObjItemManager 0x4048

#define oItemManagerList 0x20
 
#define itemListObjectSize 0x20

#define oInventorySlotWrapper 0x10

#define oInventorySlot 0x38

#define oInventorySlotItemId 0x9C
#define oInventorySlotItemTexturePath 0x4E8
#define oInventorySlotItemName 0x528