#pragma once

// Functions
#define oGetBaseDrawPosition 0x1FA420 // E8 ? ? ? ? EB 06 49 8B 06 Stesso sub della sig OK
#define oWorldToScreen 0xE0C4C0 // 48 83 EC 38 49 8B C0 Sub sopra a tutto OK
#define oTryRightClick 0x8A4AC0 // 48 89 5C 24 ? 57 48 83 EC 50 48 8B D9 0F 29 74 24 ? 48 8B 0D ? ? ? ? Sub sopra a tutto OK
#define oIssueOrder 0x8B8BF0 // E8 ? ? ? ? 84 C0 74 0A 33 F6 Stesso sub della sig OK mio sbagliato forse
#define oIssueMove 0x8A3CE0 // 41 0F B6 F9 48 8B 0D ? ? ? ? Sub sopra a tutto OK
#define oCastSpellWrapper 0x8AF600 // E8 ? ? ? ? 48 8B 53 08 48 8B 42 60 Stesso sub della sig OK
#define oPrintChat 0x860740 // E8 ? ? ? ? 4C 8B C3 B2 01 Stesso sub della sig OK
#define oGetBoundingRadius 0x1F7B90 // E8 ? ? ? ? 0F 28 F8 48 8B D6 Stesso sub della sig FORSE TROVATO
#define oGetAttackDelay 0x3B5B80 // F3 0F 10 89 ? ? ? ? E9 ? ? ? ? Sub sopra a tutto OK
#define oGetAttackWindup 0x3B5A80 // E8 ? ? ? ? 48 8B CE F3 0F 11 83 ? ? ? ? Stesso sub della sig OK
#define oGetObjectFromNetId 0x394270 // 4C 8B C1 85 D2 74 1B Sub sopra a tutto OK
#define oGetCollisionFlags 0xD83300 // 48 83 EC 28 48 8B D1 48 8B 0D ? ? ? ? 48 8B 49 08 E8 ? ? ? ? 48 8B C8 48 85 C0 74 1A Stesso sub della sig FORSE TROVATO
#define oCharacterDataStackUpdate 0x1843C0 // E8 ? ? ? ? 80 BD ? ? ? ? ? 74 0D Stesso sub della sig
//https://www.unknowncheats.me/forum/3788589-post9938.html

// Maybe not needed now da render, sono tutti uintptr?
#define fIsAlive 0x240ED0 //E8 ? ? ? ? 84 C0 74 35 48 8D 8F ? ? ? ? 
#define fIsHero 0x257650 //E8 ? ? ? ? 0F B6 B4 24 ? ? ? ? Sub sopra a tutto
#define fIsMinion 0x2576B0 //E8 ? ? ? ? 48 8B 0B F3 0F 10 41 ? Sub sopra a tutto
#define fIsTurret 0x257800 //E8 ? ? ? ? 84 C0 74 56 48 8B 06 Sub sopra a tutto
#define fIsMissile 0x2576E0 //E8 ? ? ? ? 84 C0 74 0C 48 8B 17 Sub sopra a tutto
#define fIsInhibitor 0x257590 //E8 ? ? ? ? 48 8B CB 0F B6 F8 Sub sopra a tutto
#define fIsNexus 0x2575D0 //E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? F3 0F 10 44 24 ? 48 8D 54 24 ? Sub sopra a tutto
#define fIsObjectType 0x207D00 //0F B6 41 48 4C 8B C9 Sub sopra a tutto
#define fIsDead 0x206F50 // E8 ? ? ? ? 0F B6 F0 EB 17 Sub sopra a tutto è un int
#define fIsCanSee 0x24FE70 // 40 53 48 83 EC 20 48 8B 01 48 8B D9 FF 90 ? ? ? ? 84 C0 75 19 Sub sopra a tutto è un int
#define fIsNotWall 0xD76400 // E8 ? ? ? ? 44 3A E8 Sub sopra a tutto
#define fGetPing 0x49BF00 // E8 ? ? ? ? 8B F8 39 03Sub sopra a tutto
#define fRenderUsualSuspects 0x50BAA0 // 48 8B C4 48 89 58 18 89
#define fIsTargetable 0x9F6AF0 // 40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 84 C0 74 41
#define fIsVisible 0x2091E0 //40 56 48 83 EC 20 48 8B F1 48 81 C1 ? ? ? ? E8 ? ? ? ?
#define fGetSpellState 0x6EA580 // E8 ? ? ? ? 48 8B CE 44 8B F8
#define fGetOwnerPlayer 0x1FF6F0 //40 53 48 83 EC 20 48 81 C1 E8 02 00 00 48 8B DA 48 8B

// Events not needed now
#define fOnStartSpellCast 0x7065D0 // 40 53 48 83 EC 30 4C 8B 0A DWORD64
#define fOnProcessSpell 0x162FD0 // 48 89 5C 24 ? 55 56 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 44 8B 05 ? ? ? ? 
#define fOnProcessSpellIndex 0x1188 // 48 81 C1 ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 48 83 7B ? ? 72 03 48 8B 1B 4C 8B C3 https://www.unknowncheats.me/forum/3750580-post9420.html
#define fOnStopSpellCast 0x706930 // 44 88 4C 24 20 4C 89 44 24 18 88 DWORD64
#define fOnSpellImpact 0x6FF7C0 //48 89 5C 24 10 48 89 6C 24 18 57 41 54 41 55 41 56 41 57 48 81 QWORD* __fastcall Spell::SpellbookClient::OnSpellImpact(Spell::SpellbookClient* a1, const Spell::SpellCastInfo *a2) https://streamable.com/x4e7ib https://streamable.com/xuvq05 https://www.unknowncheats.me/forum/3755876-post9525.html
#define fGetRespawnTimeRemaining 0x201420 //40 53 48 83 EC 20 48 8B D9 48 81 C1 ? ? ? ? E8 ? ? ? ? 0F B6 83 ? ? ? ?
#define fShopActions 0x0 //40 55 53 56 41 56 41 57 48 8D 6C
#define fGetCircleIconPortrait 0x1FBEE0 //E8 ? ? ? ? 4C 3B F8 0F 94 C0
#define fBuildNavPath 0xCD0460 // 48 8B C4 48 89 58 10 55 56 57 41 54 41 55 41 56 41 57 48 8D
#define fSmoothPath 0x8D8B70 //40 53 55 41 56 48 83 EC 50 48
#define fBuyItem 0xE8E030 //40 55 53 56 41 56 41 57 48 8D 6C 24 ? 48 81 EC
#define fSellItem 0x3E6640 //40 57 48 83 EC 20 8B FA E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 89 5C 24
#define fUndoItem 0x242CC0 //48 89 5C 24 ? 57 48 83 EC 30 48 8B F9 48 8B 49 08 48 8B 01 FF 90 ? ? ? ? 48 85 C0 74 15 48 8B 4F 08 48 8B 01

// Variables
#define oSpoofGadget 0x1033100 // E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 80 3B 00 oppure 48 8B 41 08 48 85 C0 74 04 48 8B 00
#define oGameTime 0x21A1F48 // F3 0F 5C 35 ? ? ? ? 0F 28 F8 Stessa qword della sig OK
#define oLocalPlayer 0x21AD080 // 48 8B 05 ? ? ? ? 4C 8B D2 4C 8B C1 Stessa qword della sig OK
#define oHudInstance 0x2192E18 // 48 8B 0D ? ? ? ? 8B 57 10 OK
#define oZoomInstance 0x219F1D0 // 48 8B 05 ? ? ? ? 48 8B 5C 24 ? 48 89 47 70 OK
#define oChatClient 0x21ADE68 // 41 FF D1 48 8B 0D ? ? ? ? OK
#define oChatInstance 0x2195F20 // 75 78 48 8B 1D ? ? ? ? 
#define oViewport 0x2195CC8 // 48 8B 3D ? ? ? ? FF 90 ? ? ? ? OK
#define oObjManager 0x2192E08 // 48 8B 0D ? ? ? ? E8 ? ? ? ? 33 ED 48 8B F8 OK
#define oGetObjectFromNetIdParam 0x2192E08 // 48 8B 0D ? ? ? ? E8 ? ? ? ? 33 ED 48 8B F8 stesso di objmanager OK
#define oBuildingsManager 0x218F2F8 // 4C 8B 05 ?? ?? ?? ?? 49 8B 78 08 41 8B 40 10 48 8D 0C C7 48 3B F9 73 49
#define oMinimapObject 0x21A1FB0 // 48 8B 1D ? ? ? ? 48 85 DB 74 22 OK
#define oMissilesList 0x21ADD78 // 48 8B 0D ? ? ? ? 48 8D 54 24 ? E8 ? ? ? ? 48 8B 7C 24 ? https://www.unknowncheats.me/forum/league-of-legends/310587-league-legends-reversal-structs-offsets-478.html https://www.unknowncheats.me/forum/league-of-legends/434211-reading-missiles-externally.html
#define oAttackableList 0x2192F68 // EB CE 48 8B 82 ? ? ? ?  Stessa qword FORSE TROVATO
#define oHeroList 0x2192F28 // 48 8B 05 ? ? ? ? 45 33 E4 0F 57 C0
#define oMinionsList 0x2195D08 // 48 89 0D ? ? ? ? 48 8D 05 ? ? ? ? 33 D2 OK
#define oTurretsList 0x219C340 // 48 89 0D ? ? ? ? 33 C9 OK
#define oInhibitorList 0x21AF0C0 // 48 8B 05 ? ? ? ? 48 89 7C 24 50

// Structs
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
#define oObjIssueOrderFloatCheck1 0x84
#define oObjIssueOrderFloatCheck2 0x88
#define oObjIssueOrderCheck 0xD4
#define oObjPosition 0x220
#define oObjVisible 0x310
#define oObjAlive 0x328
#define oObjLevel 0x3FA0

#define oObjMana 0x340
#define oObjMaxMana 0x358
#define oObjTargetable 0xEB0
#define oObjHealth 0x1058
#define oObjMaxHealth 0x1070
#define oObjActionState 0x13C0
#define oObjBonusAttackDamage 0x15D8
#define oObjAbilityPower 0x15E8
#define oObjBaseAttackDamage 0x1670
#define oObjScale 0x1684
#define oObjLethality 0x15A8

#define oObjArmor 0x1694
#define oObjBonusArmor 0x16A8
#define oObjArmorPen 0X1DE0
#define oObjMagicResist 0x169C
#define oObjMagicPen 0x1584
#define oObjMagicPenMulti 0x11C8
#define oObjMovementSpeed 0x16AC
#define oObjAttackRange 0x16B4
#define oObjBuffManager 0x27C8
#define oObjSpellBook 0x29E8
#define oObjActiveSpellCast 0x2A20
#define oObjCharData 0x3528
#define oObjName 0x35D8
#define oObjCharacterDataStack 0x35C0
#define oObjAiManager 0x36F0 // https://www.unknowncheats.me/forum/3735791-post9262.html


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

#define oSpellInputTargetNetId 0x14
#define oSpellInputStartPos 0x18
#define oSpellInputEndPos 0x24

#define oSpellInfoSpellData 0x60

#define oSpellDataSpellName 0x80
#define oSpellDataManaCost 0x5F4

#define oActiveSpellCastSpellInfo 0x8
#define oActiveSpellCastSpellType 0x10
#define oActiveSpellCastSpellId 0x124

#define oObjCharDataData 0x28

#define oObjCharDataDataSize 0xC4
#define oObjCharDataDataObjType 0x770 

#define oObjCharDataDataObjTypeDetailed 0x20

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
#define oMissileList 0x2196118
#define oMissileName 0x60
#define oMissileSpellName 0xB0

#define oMissileMapCount 0X10
#define oMissileMapRoot 0x8
#define oMissileMapKey 0x20
#define oMissileMapVal 0x28

#define oMissileSpellInfo 0x2E8
#define oMissileSrcIdx 0x370
#define oMissileDestIdx 0x3C8
#define oMissileDestCheck 0x320
#define oMissileStartPos 0x38C
#define oMissileEndPos 0x398
#define oObjMissileSpellCast 0x250

#define MinimapObjectHud 0x320
#define MinimapHudPos 0x60
#define MinimapHudSize 0x68
//https://www.unknowncheats.me/forum/3727406-post9100.html i need this shit

//CharacterStateIntermediate
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
#define oCrit 0x2A0
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
#define oObjItemManager 0x3FF8

#define oItemManagerList 0x20
 
#define itemListObjectSize 0x20

#define oInventorySlotWrapper 0x10

#define oInventorySlot 0x38

#define oInventorySlotItemId 0x9C
#define oInventorySlotItemTexturePath 0x4E8
#define oInventorySlotItemName 0x528