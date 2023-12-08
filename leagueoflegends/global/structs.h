#pragma once

#include "../stdafx.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#define M_PI_F (float)M_PI
#endif

struct Vector2
{
    float x, y;

    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float _x, float _y) : x(_x), y(_y) {}

    Vector2 operator+(const Vector2& rhs) const
    {
        return Vector2(x + rhs.x, y + rhs.y);
    }

    Vector2 operator-(const Vector2& rhs) const
    {
        return Vector2(x - rhs.x, y - rhs.y);
    }

    Vector2 operator*(float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 operator/(float scalar) const
    {
        return Vector2(x / scalar, y / scalar);
    }

    bool operator==(const Vector2& rhs) const
    {
        return x == rhs.x && y == rhs.y;
	}

    ImVec2 ToImVec() const
    {
        return ImVec2(x, y);
    }

    Vector2 Multiply(float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }
    Vector2 Normalized()
    {
        auto const length = this->Length();
        if (length != 0)
        {
            auto const inv = 1.0f / length;
            return { this->x * inv, this->y * inv };
        }
        return *this;
    }

    float Length() const
    {
        return sqrtf(this->x * this->x + this->y * this->y);
    }

    bool IsValid() const
    {
        return !(this->x == 0.0f && this->y == 0.0f) && (this->x > (-1000000)) && (this->y > (-1000000)) && (this->x < (1000000)) && (this->y < (1000000));
    }
};

struct Vector3
{
    float x, y, z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}


    Vector3 operator+(const Vector3& rhs) const
    {
        return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    Vector3 operator-(const Vector3& rhs) const
    {
        return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    Vector3 operator*(float scalar) const
    {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    Vector3 operator/(float scalar) const
    {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }



    Vector3 Extend(const Vector3& to, float distance) const
    {
        const auto from = *this;
        const auto result = from + (to - from).Normalized() * distance;
        return result;
    }

    Vector3 Normalized() const
    {
        auto const length = this->Length();
        if (length != 0)
        {
            auto const inv = 1.0f / length;
            return { this->x * inv, this->y * inv, this->z * inv };
        }

        return *this;
    }

    Vector3 Center(Vector3 const& other) const
    {
        return (*this + other) / 2;
    }


    Vector3 ToGround() const
    {
        Vector3 result(this->x, 0, this->z);
        return result;
    }

    float Polar() const
    {
        if (this->Close(x, 0.f, 0.f))
        {
            if (z > 0.f)
            {
                return 90.f;
            }
            return z < 0.f ? 270.f : 0.f;
        }

        auto theta = atan(z / x) * 180.f / M_PI;
        if (x < 0.f)
        {
            theta = theta + 180.f;
        }
        if (theta < 0.f)
        {
            theta = theta + 360.f;
        }
        return theta;
    }

    float AngleBetween(Vector3 const& other) const
    {
        auto theta = Polar() - other.Polar();
        if (theta < 0.f)
        {
            theta = theta + 360.f;
        }
        if (theta > 180.f)
        {
            theta = 360.f - theta;
        }
        return theta;
    }

    bool Close(float a, float b, float eps) const
    {
        if (abs(eps) < FLT_EPSILON)
        {
            eps = static_cast<float>(1e-9);
        }
        return abs(a - b) <= eps;

    }

    Vector3 Rotated(float angle) const
    {
        auto const c = cos(angle);
        auto const s = sin(angle);

        return { static_cast<float>(x * c - z * s), y, static_cast<float>(z * c + x * s) };
    }

    Vector3 Perpendicular() const
    {
        return { -z,y,x };
    }

    Vector3 Perpendicular2() const
    {
        return { z,y,-x };
    }

    Vector3 Perpendicular3() const
    {
        return { z,-y,x };
    }

    Vector3 Rotate(Vector3 startPos, float theta)
    {
        float dx = this->x - startPos.x;
        float dz = this->z - startPos.z;

        float px = dx * cos(theta) - dz * sin(theta);
        float pz = dx * sin(theta) + dz * cos(theta);
        return { px + startPos.x, this->y, pz + startPos.z };
    }

    Vector3 Scale(float s)
    {
        return Vector3(x * s, y * s, z * s);
    }

    bool IsValid() const
    {
        return !(this->x == 0.0f && this->y == 0.0f && this->z == 0.0f) && (this->x > (-1000000)) && (this->y > (-1000000)) && (this->z > (-1000000)) && (this->x < (1000000)) && (this->y < (1000000)) && (this->z < (1000000));
    }

    bool operator==(const Vector3& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    float Distance(const Vector3& to) const
    {
        return sqrtf(powf(to.x - x, 2) + powf(to.z - z, 2) + powf(to.y - y, 2));
    }

    float Length() const
    {
        return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
    }

    float DotProduct(const Vector3& other) const
    {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }

    float distanceTo(const Vector3& v) const
    {
        return (float)sqrt(pow(v.x - x, 2) + pow(v.z - z, 2) + pow(v.y - y, 2));
    }

    std::string ToString() {
        return std::to_string(this->x) + "\n" + std::to_string(this->y) + "\n" + std::to_string(this->z);
    }

    bool IsUnderEnemyTower();
};

struct Vector4 {
    float x, y, z, w;

    Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    Vector4(float _x, float _y, float _z, float _w) {}


    float length() {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    float distance(const Vector4& o) {
        return sqrt(pow(x - o.x, 2) + pow(y - o.y, 2) + pow(z - o.z, 2) + pow(w - o.w, 2));
    }

    Vector4 vscale(const Vector4& s) {
        return Vector4(x * s.x, y * s.y, z * s.z, w * s.w);
    }

    Vector4 scale(float s) {
        return Vector4(x * s, y * s, z * s, w * s);
    }

    Vector4 normalize() {
        float l = length();
        return Vector4(x / l, y / l, z / l, w / l);
    }

    Vector4 add(const Vector4& o) {
        return Vector4(x + o.x, y + o.y, z + o.z, w + o.w);
    }

    Vector4 sub(const Vector4& o) {
        return Vector4(x - o.x, y - o.y, z - o.z, w - o.w);
    }

    Vector4 clone() {
        return Vector4(x, y, z, w);
    }
};

struct ProjectionInfo
{
    bool IsOnSegment;
    Vector3 LinePoint;
    Vector3 SegmentPoint;

    ProjectionInfo(bool is_on_segment, Vector3 const& segment_point, Vector3 const& line_point);
};

struct IntersectionResult
{
    bool Intersects;
    Vector3 Point;

    IntersectionResult(bool intersects = false, Vector3 const& point = Vector3());
};

template<typename T>
struct SEntityList
{
    char pad[0x4];
    T** entities;
    size_t size;
    size_t max_size;
};

enum OrbwalkState
{
    Off,
    Attack,
    Clear,
	FastClear,
    Harass,
    Lasthit,
    Flee
};

enum DamageType
{
	Physical,
	Magical,
	True
};

enum SpellIndex
{
	Q,
	W,
	E,
	R,
	D,
	F,
	Item1,
	Item2,
	Item3,
	Item4,
	Item5,
	Item6,
	Trinket,
	Recall
};

enum SkillshotType
{
	SkillshotNone,
	SkillshotLine,
	SkillshotCircle,
	SkillshotCone
};

enum SpellCastMode {
	NormalCast = 1,
	SmartCast,
	SelfCast,
	SmartCastWithIndicator,
	SmartPlusSelfCast,
	SmartPlusSelfCastWithIndicator,
};

enum SpellState : int
{
    IsReady = 0,
    IsDoesNotExist = 2,
    IsNotAvailable = 4,
    IsSupressed = 8,
    IsNotLearned = 12,
    IsDisabled = 16,
    IsCooldown = 32,
    IsProcessing = 48,
    IsNoMana = 64
};

enum ActionState : unsigned short {
	CanAttack = 1,
	CanCast = 2,
	CanMove = 4,
	Immovable = 8,
	Unknownz = 16,
	IsStealth = 32,
	Taunted = 64,
	Feared = 128,
	Fleeing = 256,
	Supressed = 512,
	Asleep = 1024,
	NearSight = 2048,
	Ghosted = 4096,
	HasGhost = 8192,
	Charmed = 16384,
	NoRender = 32768
};

enum ObjectType : unsigned long long
{
    Champion = 7957694998179309635,
    Special = 2336349411965497427,
    Ward = 5989923982968774999,
    Minion_Lane = 8944270284747073869,
    Minion_Lane_Siege = 7306920423476651374,
    Minion_Lane_Ranged = 7306930284704785774,
    Minion_Lane_Melee = 7306365152824092014,
    Minion_Lane_Super = 8243118342183806318,
    Monster = 2338042707385937741,
    Monster_Epic = 2340781521963538015,
    Monster_Dragon = 2336927746459059295,
    Special_Void = 2340781521963538015,
    Structure_Turret = 4294967297,
    UNKNOWN = 5980780305148018688,
    RespawnMarker = 30506402751803475,
    Plants = 5773751200854991191,
	SennaSoul = 8028075772544512841,
    Less = 126939457912832
};

enum CollisionFlags
{
    None = 0,
    Grass = 1,
    Wall = 2,
    Building = 64,
    Tower = 70,
    Prop = 128,
    GlobalVision = 256
};

enum CollidableObjects
{
    Objects,
    Walls
};

enum class PerkID : int
{
	Absolute_Focus = 8233,
	Aftershock = 8439,
	Approach_Velocity = 8410,
	Arcane_Comet = 8229,
	Biscuit_Delivery = 8345,
	Bone_Plating = 8473,
	Celerity = 8234,
	Cheap_Shot = 8126,
	Chrysalis = 8472,
	Conditioning = 8429,
	Conqueror = 8010,
	Cosmic_Insight = 8347,
	Coup_de_Grace = 8014,
	Cut_Down = 8017,
	Dark_Harvest = 8128,
	Demolish = 8446,
	Electrocute = 8112,
	Eyeball_Collection = 8138,
	Fleet_Footwork = 8021,
	Font_of_Life = 8463,
	Futures_Market = 8321,
	Gathering_Storm = 8236,
	Ghost_Poro = 8120,
	Glacial_Augment = 8351,
	Grasp_of_the_Undying = 8437,
	Guardian = 8465,
	Hail_of_Blades = 9923,
	Hextech_Flashtraption = 8306,
	Ingenious_Hunter = 8134,
	Kleptomancy = 8359,
	Last_Stand = 8299,
	Legend_Alacrity = 9104,
	Legend_Bloodline = 9103,
	Legend_Tenacity = 9105,
	Lethal_Tempo = 8008,
	Magical_Footwear = 8304,
	Manaflow_Band = 8226,
	Minion_Dematerializer = 8316,
	Nimbus_Cloak = 8275,
	Nullifying_Orb = 8224,
	Overgrowth = 8451,
	Overheal = 9101,
	Perfect_Timing = 8313,
	Phase_Rush = 8230,
	Predator = 8124,
	Presence_of_Mind = 8009,
	Press_the_Attack = 8005,
	Ravenous_Hunter = 8135,
	Relentless_Hunter = 8105,
	Revitalize = 8453,
	Scorch = 8237,
	Second_Wind = 8444,
	Sudden_Impact = 8143,
	Summon_Aery = 8214,
	Taste_of_Blood = 8139,
	Time_Warp_Tonic = 8352,
	Transcendence = 8210,
	Triumph = 9111,
	Ultimate_Hunter = 8106,
	Unflinching = 8242,
	Unsealed_Spellbook = 8360,
	Waterwalking = 8232,
	Zombie_Ward = 8136,
};

enum class AugmentsId : std::uint32_t
{
    CourageoftheColossus = NameInHash("Augment_CourageoftheColossus"),
    TankItOrLeaveIt = NameInHash("Augment_TankItOrLeaveIt"),
    Goliath = NameInHash("Augment_Goliath"),
    LightningStrikes = NameInHash("Augment_LightningStrikes"),
    Evocation = NameInHash("Augment_Evocation"),
    InfernalConduit = NameInHash("Augment_InfernalConduit"),
    WillingSacrifice = NameInHash("Augment_WillingSacrifice"),
    SonicBoom = NameInHash("Augment_SonicBoom"),
    OutlawsGrit = NameInHash("Augment_OutlawsGrit"),
    Castle = NameInHash("Augment_Castle"),
    Tormentor = NameInHash("Augment_Tormentor"),
    EtherealWeapon = NameInHash("Augment_EtherealWeapon"),
    WithHaste = NameInHash("Augment_WithHaste"),
    ThreadtheNeedle = NameInHash("Augment_ThreadtheNeedle"),
    Dashing = NameInHash("Augment_Dashing"),
    SpiritLink = NameInHash("Augment_SpiritLink"),
    MysticPunch = NameInHash("Augment_MysticPunch"),
    Chauffeur = NameInHash("Augment_Chauffeur"),
    Vulnerability = NameInHash("Augment_Vulnerability"),
    Mythical = NameInHash("Augment_Mythical"),
    SoulSiphon = NameInHash("Augment_SoulSiphon"),
    BluntForce = NameInHash("Augment_BluntForce"),
    Perseverance = NameInHash("Augment_Perseverance"),
    ZhonyasEpiphany = NameInHash("Augment_ZhonyasEpiphany"),
    DiveBomber = NameInHash("Augment_DiveBomber"),
    ShadowRunner = NameInHash("Augment_ShadowRunner"),
    DontBlink = NameInHash("Augment_DontBlink"),
    OmniSoul = NameInHash("Augment_OmniSoul"),
    GuiltyPleasure = NameInHash("Augment_GuiltyPleasure"),
    BacktoBasics = NameInHash("Augment_BacktoBasics"),
    KeystoneConjurer = NameInHash("Augment_KeystoneConjurer"),
    JuiceBox = NameInHash("Augment_JuiceBox"),
    ContractKiller = NameInHash("Augment_ContractKiller"),
    MadScientist = NameInHash("Augment_MadScientist"),
    JeweledGauntlet = NameInHash("Augment_JeweledGauntlet"),
    OceanSoul = NameInHash("Augment_OceanSoul"),
    MindtoMatter = NameInHash("Augment_MindtoMatter"),
    DieAnotherDay = NameInHash("Augment_DieAnotherDay"),
    CannonFodder = NameInHash("Augment_CannonFodder"),
    FrostWraith = NameInHash("Augment_FrostWraith"),
    PhenomenalEvil = NameInHash("Augment_PhenomenalEvil"),
    ComboMaster = NameInHash("Augment_ComboMaster"),
    WarmupRoutine = NameInHash("Augment_WarmupRoutine"),
    NowYouSeeMe = NameInHash("Augment_NowYouSeeMe"),
    Earthwake = NameInHash("Augment_Earthwake"),
    AcceleratingSorcery = NameInHash("Augment_AcceleratingSorcery"),
    WindspeakersBlessing = NameInHash("Augment_WindspeakersBlessing"),
    ApexInventor = NameInHash("Augment_ApexInventor"),
    ExtendoArm = NameInHash("Augment_ExtendoArm"),
    Firebrand = NameInHash("Augment_Firebrand"),
    DemonsDance = NameInHash("Augment_DemonsDance"),
    MountainSoul = NameInHash("Augment_MountainSoul"),
    BannerofCommand = NameInHash("Augment_BannerofCommand"),
    Eureka = NameInHash("Augment_Eureka"),
    BuckleUp = NameInHash("Augment_BuckleUp"),
    CantTouchThis = NameInHash("Augment_CantTouchThis"),
    CircleofDeath = NameInHash("Augment_CircleofDeath"),
    LightemUp = NameInHash("Augment_LightemUp"),
    ScopedWeapons = NameInHash("Augment_ScopedWeapons"),
    Executioner = NameInHash("Augment_Executioner"),
    MasterofDuality = NameInHash("Augment_MasterofDuality"),
    RestlessRestoration = NameInHash("Augment_RestlessRestoration"),
    RabbleRousing = NameInHash("Augment_RabbleRousing"),
    OkBoomerang = NameInHash("Augment_OkBoomerang"),
    ShrinkRay = NameInHash("Augment_ShrinkRay"),
    MidnightExpress = NameInHash("Augment_MidnightExpress"),
    WitchfulThinking = NameInHash("Augment_WitchfulThinking"),
    Repulsor = NameInHash("Augment_Repulsor"),
    TheBrutalizer = NameInHash("Augment_TheBrutalizer"),
    ThiefsGloves = NameInHash("Augment_ThiefsGloves"),
    Vanish = NameInHash("Augment_Vanish"),
    SymphonyofWar = NameInHash("Augment_SymphonyofWar"),
    InfernalSoul = NameInHash("Augment_InfernalSoul"),
    FromBeginningToEnd = NameInHash("Augment_FromBeginningToEnd"),
    FeeltheBurn = NameInHash("Augment_FeeltheBurn"),
    ItsCritical = NameInHash("Augment_ItsCritical"),
    UltimateRevolution = NameInHash("Augment_UltimateRevolution"),
    BladeWaltz = NameInHash("Augment_BladeWaltz"),
    TrueshotProdigy = NameInHash("Augment_TrueshotProdigy"),
    FirstAidKit = NameInHash("Augment_FirstAidKit"),
    CelestialBody = NameInHash("Augment_CelestialBody"),
    TapDancer = NameInHash("Augment_TapDancer"),
    DawnbringersResolve = NameInHash("Augment_DawnbringersResolve"),
    Erosion = NameInHash("Augment_Erosion"),
    Vengeance = NameInHash("Augment_Vengeance"),
    DefensiveManeuvers = NameInHash("Augment_DefensiveManeuvers"),
    SearingDawn = NameInHash("Augment_SearingDawn"),
    QuantumComputing = NameInHash("Augment_QuantumComputing"),
    SlowCooker = NameInHash("Augment_SlowCooker"),
    Typhoon = NameInHash("Augment_Typhoon"),
    IceCold = NameInHash("Augment_IceCold"),
    BuffBuddies = NameInHash("Augment_BuffBuddies"),
    WisdomofAges = NameInHash("Augment_WisdomOfAges"),
    FallenAegis = NameInHash("Augment_FallenAegis"),
    FrozenFoundations = NameInHash("Augment_FrozenFoundations"),
    Recursion = NameInHash("Augment_Recursion"),
    Deft = NameInHash("Augment_Deft"),
};

enum class ItemsDatabase : int
{
	Boots = 1001,
	Faerie_Charm = 1004,
	Rejuvenation_Bead = 1006,
	Giants_Belt = 1011,
	Cloak_of_Agility = 1018,
	Blasting_Wand = 1026,
	Sapphire_Crystal = 1027,
	Ruby_Crystal = 1028,
	Cloth_Armor = 1029,
	Chain_Vest = 1031,
	Null_Magic_Mantle = 1033,
	Emberknife = 1035,
	Long_Sword = 1036,
	Pickaxe = 1037,
	B_F_Sword = 1038,
	Hailblade = 1039,
	Obsidian_Edge = 1040,
	Dagger = 1042,
	Recurve_Bow = 1043,
	Amplifying_Tome = 1052,
	Vampiric_Scepter = 1053,
	Dorans_Shield = 1054,
	Dorans_Blade = 1055,
	Dorans_Ring = 1056,
	Negatron_Cloak = 1057,
	Needlessly_Large_Rod = 1058,
	Dark_Seal = 1082,
	Cull = 1083,
	Scorchclaw_Pup = 1101,
	Gustwalker_Hatchling = 1102,
	Mosstomper_Seedling = 1103,
	Eye_of_the_Herald = 1104,
	Penetrating_Bullets = 1500,
	Fortification = 1501,
	Reinforced_Armor = 1502,
	Wardens_Eye = 1503,
	Vanguard = 1504,
	Lightning_Rod = 1505,
	Reinforced_Armor_2 = 1506,
	Overcharged = 1507,
	Anti_tower_Socks = 1508,
	Gusto = 1509,
	Phreakish_Gusto = 1510,
	Super_Mech_Armor = 1511,
	Super_Mech_Power_Field = 1512,
	Turret_Plating = 1515,
	Structure_Bounty = 1516,
	Structure_Bounty_2 = 1517,
	Structure_Bounty_3 = 1518,
	Structure_Bounty_4 = 1519,
	OvererchargedHA = 1520,
	Fortification_2 = 1521,
	Tower_Power_Up = 1522,
	Health_Potion = 2003,
	Total_Biscuit_of_Everlasting_Will = 2010,
	Kircheis_Shard = 2015,
	Steel_Sigil = 2019,
	Refillable_Potion = 2031,
	Corrupting_Potion = 2033,
	Guardians_Amulet = 2049,
	Guardians_Shroud = 2050,
	Guardians_Horn = 2051,
	Poro_Snax = 2052,
	Control_Ward = 2055,
	Shurelyas_Battlesong = 2065,
	Elixir_of_Iron = 2138,
	Elixir_of_Sorcery = 2139,
	Elixir_of_Wrath = 2140,
	Cappa_Juice = 2141,
	Juice_of_Power = 2142,
	Juice_of_Vitality = 2143,
	Juice_of_Haste = 2144,
	Guardians_Horn_Arena = 222051,
	Shurelyas_Battlesong_Arena = 222065,
	Evenshroud_Arena = 223001,
	Archangels_Staff_Arena = 223003,
	Manamune_Arena = 223004,
	Berserkers_Greaves_Arena = 223006,
	Boots_of_Swiftness_Arena = 223009,
	Chemtech_Putrifier_Arena = 223011,
	Sorcerers_Shoes_Arena = 223020,
	Guardian_Angel_Arena = 223026,
	Infinity_Edge_Arena = 223031,
	Mortal_Reminder_Arena = 223033,
	Lord_Dominiks_Regards_Arena = 223036,
	Seraphs_Embrace_Arena = 223040,
	Muramana_Arena = 223042,
	Phantom_Dancer_Arena = 223046,
	Plated_Steelcaps_Arena = 223047,
	Zekes_Convergence_Arena = 223050,
	Steraks_Gage_Arena = 223053,
	Spirit_Visage_Arena = 223065,
	Sunfire_Aegis_Arena = 223068,
	Black_Cleaver_Arena = 223071,
	Bloodthirster_Arena = 223072,
	Ravenous_Hydra_Arena = 223074,
	Thornmail_Arena = 223075,
	Trinity_Force_Arena = 223078,
	Heartsteel_Arena = 223084,
	Runaans_Hurricane_Arena = 223085,
	Statikk_Shiv_Arena = 223087,
	Rabadons_Deathcap_Arena = 223089,
	Wits_End_Arena = 223091,
	Rapid_Firecannon_Arena = 223094,
	Stormrazor_Arena = 223095,
	Lich_Bane_Arena = 223100,
	Banshees_Veil_Arena = 223102,
	Redemption_Arena = 223107,
	Knights_Vow_Arena = 223109,
	Frozen_Heart_Arena = 223110,
	Mercurys_Treads_Arena = 223111,
	Guardians_Orb_Arena = 223112,
	Nashors_Tooth_Arena = 223115,
	Rylais_Crystal_Scepter_Arena = 223116,
	Winters_Approach_Arena = 223119,
	Fimbulwinter_Arena = 223121,
	Guinsoos_Rageblade_Arena = 223124,
	Void_Staff_Arena = 223135,
	Mercurial_Scimitar_Arena = 223139,
	Youmuus_Ghostblade_Arena = 223142,
	Randuins_Omen_Arena = 223143,
	Hextech_Rocketbelt_Arena = 223152,
	Blade_of_The_Ruined_King_Arena = 223153,
	Maw_of_Malmortius_Arena = 223156,
	Zhonyas_Hourglass_Arena = 223157,
	Ionian_Boots_of_Lucidity_Arena = 223158,
	Spear_Of_Shojin_Arena = 223161,
	Morellonomicon_Arena = 223165,
	Guardians_Blade_Arena = 223177,
	Hullbreaker_Arena = 223181,
	Guardians_Hammer_Arena = 223184,
	Guardians_Dirk_Arena = 223185,
	Locket_of_the_Iron_Solari_Arena = 223190,
	Gargoyle_Stoneplate_Arena = 223193,
	Mikaels_Blessing_Arena = 223222,
	Ardent_Censer_Arena = 223504,
	Essence_Reaver_Arena = 223508,
	Dead_Mans_Plate_Arena = 223742,
	Titanic_Hydra_Arena = 223748,
	Edge_of_Night_Arena = 223814,
	Imperial_Mandate_Arena = 224005,
	Force_of_Nature_Arena = 224401,
	Horizon_Focus_Arena = 224628,
	Cosmic_Drive_Arena = 224629,
	Riftmaker_Arena = 224633,
	Night_Harvester_Arena = 224636,
	Demonic_Embrace_Arena = 224637,
	Crown_of_the_Shattered_Queen_Arena = 224644,
	Shadowflame_Arena = 224645,
	Silvermere_Dawn_Arena = 226035,
	Deaths_Dance_Arena = 226333,
	Chempunk_Chainsword_Arena = 226609,
	Staff_of_Flowing_Water_Arena = 226616,
	Moonstone_Renewer_Arena = 226617,
	Echoes_of_Helia_Arena = 226620,
	Goredrinker_Arena = 226630,
	Stridebreaker_Arena = 226631,
	Divine_Sunderer_Arena = 226632,
	Liandrys_Anguish_Arena = 226653,
	Ludens_Tempest_Arena = 226655,
	Everfrost_Arena = 226656,
	Rod_of_Ages_Arena = 226657,
	Iceborn_Gauntlet_Arena = 226662,
	Turbo_Chemtank_Arena = 226664,
	JakSho_The_Protean_Arena = 226665,
	Radiant_Virtue_Arena = 226667,
	Galeforce_Arena = 226671,
	Kraken_Slayer_Arena = 226672,
	Immortal_Shieldbow_Arena = 226673,
	Navori_Quickblades_Arena = 226675,
	The_Collector_Arena = 226676,
	Duskblade_of_Draktharr_Arena = 226691,
	Eclipse_Arena = 226692,
	Prowlers_Claw_Arena = 226693,
	Seryldas_Grudge_Arena = 226694,
	Serpents_Fang_Arena = 226695,
	Axiom_Arc_Arena = 226696,
	Syzygy_Arena = 227001,
	Draktharrs_Shadowcarver_Arena = 227002,
	Frozen_Fist_Arena = 227005,
	Typhoon_Arena = 227006,
	Icathias_Curse_Arena = 227009,
	Vespertide_Arena = 227010,
	Upgraded_Aeropack_Arena = 227011,
	Liandrys_Lament_Arena = 227012,
	Eye_of_Luden_Arena = 227013,
	Eternal_Winter_Arena = 227014,
	Ceaseless_Hunger_Arena = 227015,
	Dreamshatter_Arena = 227016,
	Deicide_Arena = 227017,
	Infinity_Force_Arena = 227018,
	Reliquary_of_the_Golden_Dawn_Arena = 227019,
	Shurelyas_Requiem_Arena = 227020,
	Starcaster_Arena = 227021,
	Equinox_Arena = 227023,
	Caesura_Arena = 227024,
	Leviathan_Arena = 227025,
	The_Unspoken_Parasite_Arena = 227026,
	Primordial_Dawn_Arena = 227027,
	Infinite_Convergence_Arena = 227028,
	Youmuus_Wake_Arena = 227029,
	Seething_Sorrow_Arena = 227030,
	Edge_of_Finality_Arena = 227031,
	Flicker_Arena = 227032,
	Cry_of_the_Shrieking_City_Arena = 227033,
	Anathemas_Chains_Arena = 228001,
	Abyssal_Mask_Arena = 228020,
	Minion_Dematerializer = 2403,
	Commencing_Stopwatch = 2419,
	Stopwatch = 2420,
	Broken_Stopwatch = 2421,
	Slightly_Magical_Footwear = 2422,
	Slightly_Magical_Footware = 2422,
	Perfectly_Timed_Stopwatch = 2423,
	Broken_Stopwatch_2 = 2424,
	Evenshroud = 3001,
	Archangels_Staff = 3003,
	Manamune = 3004,
	Ghostcrawlers = 3005,
	Berserkers_Greaves = 3006,
	Boots_of_Swiftness = 3009,
	Chemtech_Putrifier = 3011,
	Chalice_of_Blessing = 3012,
	Sorcerers_Shoes = 3020,
	Lifewell_Pendant = 3023,
	Glacial_Buckler = 3024,
	Guardian_Angel = 3026,
	Infinity_Edge = 3031,
	Mortal_Reminder = 3033,
	Last_Whisper = 3035,
	Lord_Dominiks_Regards = 3036,
	Atmas_Reckoning = 3039,
	Seraphs_Embrace = 3040,
	Mejais_Soulstealer = 3041,
	Muramana = 3042,
	Phage = 3044,
	Phantom_Dancer = 3046,
	Plated_Steelcaps = 3047,
	Zekes_Convergence = 3050,
	Hearthbound_Axe = 3051,
	Steraks_Gage = 3053,
	Sheen = 3057,
	Spirit_Visage = 3065,
	Winged_Moonplate = 3066,
	Kindlegem = 3067,
	Sunfire_Aegis = 3068,
	Tear_of_the_Goddess = 3070,
	Black_Cleaver = 3071,
	Bloodthirster = 3072,
	Ravenous_Hydra = 3074,
	Thornmail = 3075,
	Bramble_Vest = 3076,
	Tiamat = 3077,
	Trinity_Force = 3078,
	Wardens_Mail = 3082,
	Warmogs_Armor = 3083,
	Heartsteel = 3084,
	Runaans_Hurricane = 3085,
	Zeal = 3086,
	Statikk_Shiv = 3087,
	Rabadons_Deathcap = 3089,
	Wits_End = 3091,
	Rapid_Firecannon = 3094,
	Stormrazor = 3095,
	Lich_Bane = 3100,
	Banshees_Veil = 3102,
	Aegis_of_the_Legion = 3105,
	Redemption = 3107,
	Fiendish_Codex = 3108,
	Knights_Vow = 3109,
	Frozen_Heart = 3110,
	Mercurys_Treads = 3111,
	Guardians_Orb = 3112,
	Aether_Wisp = 3113,
	Forbidden_Idol = 3114,
	Nashors_Tooth = 3115,
	Rylais_Crystal_Scepter = 3116,
	Mobility_Boots = 3117,
	Winters_Approach = 3119,
	Fimbulwinter = 3121,
	Executioners_Calling = 3123,
	Guinsoos_Rageblade = 3124,
	Deathfire_Grasp = 3128,
	Caulfields_Warhammer = 3133,
	Serrated_Dirk = 3134,
	Void_Staff = 3135,
	Mercurial_Scimitar = 3139,
	Quicksilver_Sash = 3140,
	Youmuus_Ghostblade = 3142,
	Randuins_Omen = 3143,
	Hextech_Alternator = 3145,
	Hextech_Gunblade = 3146,
	Hextech_Rocketbelt = 3152,
	Blade_of_the_Ruined_King = 3153,
	Hexdrinker = 3155,
	Maw_of_Malmortius = 3156,
	Zhonyas_Hourglass = 3157,
	Ionian_Boots_of_Lucidity = 3158,
	Spear_Of_Shojin = 3161,
	Morellonomicon = 3165,
	Zephyr = 3172,
	Guardians_Blade = 3177,
	Umbral_Glaive = 3179,
	Hullbreaker = 3181,
	Guardians_Hammer = 3184,
	Locket_of_the_Iron_Solari = 3190,
	Seekers_Armguard = 3191,
	Gargoyle_Stoneplate = 3193,
	Spectres_Cowl = 3211,
	Mikaels_Blessing = 3222,
	Scarecrow_Effigy = 3330,
	Stealth_Ward = 3340,
	Arcane_Sweeper = 3348,
	Lucent_Singularity = 3349,
	Farsight_Alteration = 3363,
	Oracle_Lens = 3364,
	Your_Cut = 3400,
	Rite_Of_Ruin = 3430,
	Ardent_Censer = 3504,
	Essence_Reaver = 3508,
	Eye_of_the_Herald_2 = 3513,
	Kalistas_Black_Spear = 3599,
	Kalistas_Black_Spear_2 = 3600,
	Dead_Mans_Plate = 3742,
	Titanic_Hydra = 3748,
	Crystalline_Bracer = 3801,
	Lost_Chapter = 3802,
	Catalyst_of_Aeons = 3803,
	Edge_of_Night = 3814,
	Spellthiefs_Edge = 3850,
	Frostfang = 3851,
	Shard_of_True_Ice = 3853,
	Steel_Shoulderguards = 3854,
	Runesteel_Spaulders = 3855,
	Pauldrons_of_Whiterock = 3857,
	Relic_Shield = 3858,
	Targons_Buckler = 3859,
	Bulwark_of_the_Mountain = 3860,
	Spectral_Sickle = 3862,
	Harrowing_Crescent = 3863,
	Black_Mist_Scythe = 3864,
	Fire_at_Will = 3901,
	Deaths_Daughter = 3902,
	Raise_Morale = 3903,
	Oblivion_Orb = 3916,
	Spectral_Cutlass = 4004,
	Imperial_Mandate = 4005,
	Bloodletters_Curse = 4010,
	Force_of_Nature = 4401,
	The_Golden_Spatula = 4403,
	Horizon_Focus = 4628,
	Cosmic_Drive = 4629,
	Blighting_Jewel = 4630,
	Verdant_Barrier = 4632,
	Riftmaker = 4633,
	Leeching_Leer = 4635,
	Night_Harvester = 4636,
	Demonic_Embrace = 4637,
	Watchful_Wardstone = 4638,
	Stirring_Wardstone = 4641,
	Bandleglass_Mirror = 4642,
	Vigilant_Wardstone = 4643,
	Crown_of_the_Shattered_Queen = 4644,
	Shadowflame = 4645,
	Ironspike_Whip = 6029,
	Silvermere_Dawn = 6035,
	Deaths_Dance = 6333,
	Chempunk_Chainsword = 6609,
	Staff_of_Flowing_Water = 6616,
	Moonstone_Renewer = 6617,
	Echoes_of_Helia = 6620,
	Goredrinker = 6630,
	Stridebreaker = 6631,
	Divine_Sunderer = 6632,
	Liandrys_Anguish = 6653,
	Ludens_Tempest = 6655,
	Everfrost = 6656,
	Rod_of_Ages = 6657,
	Bamis_Cinder = 6660,
	Iceborn_Gauntlet = 6662,
	Turbo_Chemtank = 6664,
	JakSho_The_Protean = 6665,
	Radiant_Virtue = 6667,
	Noonquiver = 6670,
	Galeforce = 6671,
	Kraken_Slayer = 6672,
	Immortal_Shieldbow = 6673,
	Navori_Quickblades = 6675,
	The_Collector = 6676,
	Rageknife = 6677,
	Duskblade_of_Draktharr = 6691,
	Eclipse = 6692,
	Prowlers_Claw = 6693,
	Seryldas_Grudge = 6694,
	Serpents_Fang = 6695,
	Axiom_Arc = 6696,
	Sandshrikes_Claw = 7000,
	Syzygy = 7001,
	Draktharrs_Shadowcarver = 7002,
	Frozen_Fist = 7005,
	Typhoon = 7006,
	Icathias_Curse = 7009,
	Vespertide = 7010,
	Upgraded_Aeropack = 7011,
	Liandrys_Lament = 7012,
	Eye_of_Luden = 7013,
	Eternal_Winter = 7014,
	Ceaseless_Hunger = 7015,
	Dreamshatter = 7016,
	Deicide = 7017,
	Infinity_Force = 7018,
	Reliquary_of_the_Golden_Dawn = 7019,
	Shurelyas_Requiem = 7020,
	Starcaster = 7021,
	Equinox = 7023,
	Caesura = 7024,
	Leviathan = 7025,
	The_Unspoken_Parasite = 7026,
	Primordial_Dawn = 7027,
	Infinite_Convergence = 7028,
	Youmuus_Wake = 7029,
	Seething_Sorrow = 7030,
	Edge_of_Finality = 7031,
	Flicker = 7032,
	Cry_of_the_Shrieking_City = 7033,
	Gangplank_Placeholder = 7050,
	Anathemas_Chains = 8001,
	Abyssal_Mask = 8020,
};

enum class BuffType : unsigned char
{
	Internal = 0,
	Aura = 1,
	CombatEnchancer = 2,
	CombatDehancer = 3,
	SpellShield = 4,
	Stun = 5,
	Invisibility = 6,
	Silence = 7,
	Taunt = 8,
	Berserk = 9,
	Polymorph = 10,
	Slow = 11,
	Snare = 12,
	Damage = 13,
	Heal = 14,
	Haste = 15,
	SpellImmunity = 16,
	PhysicalImmunity = 17,
	Invulnerability = 18,
	AttackSpeedSlow = 19,
	NearSight = 20,
	Fear = 22,
	Charm = 23,
	Poison = 24,
	Suppression = 25,
	Blind = 26,
	Counter = 27,
	Currency = 21,
	Shred = 28,
	Flee = 29,
	Knockup = 30,
	Knockback = 31,
	Disarm = 32,
	Grounded = 33,
	Drowsy = 34,
	Asleep = 35,
	Obscured = 36,
	ClickproofToEnemies = 37,
	UnKillable = 38
};

enum class ChampionID
{
	Aatrox = 266,
	Ahri = 103,
	Akali = 84,
	Alistar = 12,
	Amumu = 32,
	Anivia = 34,
	Annie = 1,
	Aphelios = 523,
	Ashe = 22,
	AurelionSol = 136,
	Azir = 268,
	Bard = 432,
	Blitzcrank = 53,
	Brand = 63,
	Braum = 201,
	Caitlyn = 51,
	Camille = 164,
	Cassiopeia = 69,
	Chogath = 31,
	Corki = 42,
	Darius = 122,
	Diana = 131,
	DrMundo = 36,
	Draven = 119,
	Ekko = 245,
	Elise = 60,
	Evelynn = 28,
	Ezreal = 81,
	FiddleSticks = 9,
	Fiora = 114,
	Fizz = 105,
	Galio = 3,
	Gangplank = 41,
	Garen = 86,
	Gnar = 150,
	Gragas = 79,
	Graves = 104,
	Hecarim = 120,
	Heimerdinger = 74,
	Illaoi = 420,
	Irelia = 39,
	Ivern = 427,
	Janna = 40,
	JarvanIV = 59,
	Jax = 24,
	Jayce = 126,
	Jhin = 202,
	Jinx = 222,
	Kaisa = 145,
	Kalista = 429,
	Karma = 43,
	Karthus = 30,
	Kassadin = 38,
	Katarina = 55,
	Kayle = 10,
	Kayn = 141,
	Kennen = 85,
	Khazix = 121,
	Kindred = 203,
	Kled = 240,
	KogMaw = 96,
	Leblanc = 7,
	LeeSin = 64,
	Leona = 89,
	Lillia = 876,
	Lissandra = 127,
	Lucian = 236,
	Lulu = 117,
	Lux = 99,
	Malphite = 54,
	Malzahar = 90,
	Maokai = 57,
	MasterYi = 11,
	MissFortune = 21,
	Mordekaiser = 82,
	Morgana = 25,
	Nami = 267,
	Nasus = 75,
	Nautilus = 111,
	Neeko = 518,
	Nidalee = 76,
	Nocturne = 56,
	Nunu = 20,
	Olaf = 2,
	Orianna = 61,
	Ornn = 516,
	Pantheon = 80,
	Poppy = 78,
	Pyke = 555,
	Qiyana = 246,
	Quinn = 133,
	Rakan = 497,
	Rammus = 33,
	RekSai = 421,
	Rell = 526,
	Renekton = 58,
	Rengar = 107,
	Riven = 92,
	Rumble = 68,
	Ryze = 13,
	Samira = 360,
	Sejuani = 113,
	Senna = 235,
	Seraphine = 147,
	Sett = 875,
	Shaco = 35,
	Shen = 98,
	Shyvana = 102,
	Singed = 27,
	Sion = 14,
	Sivir = 15,
	Skarner = 72,
	Sona = 37,
	Soraka = 16,
	Swain = 50,
	Sylas = 517,
	Syndra = 134,
	TahmKench = 223,
	Taliyah = 163,
	Talon = 91,
	Taric = 44,
	Teemo = 17,
	Thresh = 412,
	Tristana = 18,
	Trundle = 48,
	Tryndamere = 23,
	TwistedFate = 4,
	Twitch = 29,
	Udyr = 77,
	Urgot = 6,
	Varus = 110,
	Vayne = 67,
	Veigar = 45,
	Velkoz = 161,
	Vi = 254,
	Viego = 234,
	Viktor = 112,
	Vladimir = 8,
	Volibear = 106,
	Warwick = 19,
	MonkeyKing = 62,
	Xayah = 498,
	Xerath = 101,
	XinZhao = 5,
	Yasuo = 157,
	Yone = 777,
	Yorick = 83,
	Yuumi = 350,
	Zac = 154,
	Zed = 238,
	Ziggs = 115,
	Zilean = 26,
	Zoe = 142,
	Zyra = 143,
	Gwen = 887,
	Akshan = 166,
	Vex = 711,
	Zeri = 221,
	Renata = 888,
	Belveth = 200,
	Nilah = 895,
	KSante = 897,
	Milio = 902,
	Naafiri = 950,
	Briar = 233,
	Unknown = 5000,
	TFTChampion,
	SG_Skarner,
	SG_VelKoz,
	SG_RekSai,
	SG_KogMaw,
	SG_KhaZix,
	SG_ChoGath,
};

enum class GameMapID
{
	CrystalScar = 8,
	TwistedTreeline = 10,
	SummonersRift,
	HowlingAbyss,
	TFT = 22,
	CherryArena = 30
};

enum class JungleMonsterID
{
		Blue_Order = 1,
		Wolves_Order,
		Raptors_Order,
		Red_Order,
		Krugs_Order,
		Dragon,
		Blue_Chaos,
		Wolves_Chaos,
		Raptors_Chaos,
		Red_Chaos,
		Krugs_Chaos,
		Baron,
		Gromp_Order,
		Gromp_Chaos,
		Crab_Bottom,
		Crab_Top,
		Herlad,
		Max_Camps
};

enum class DragonID : std::int32_t
{
	Elder,
	Chemtech,
	Mountain,
	Hextech,
	Infernal,
	Cloud,
	Ocean,
	Unknown
};

enum class CombatType
{
	Melee = 1,
	Ranged = 2
};

enum GameState {
	Loading,
	Connecting,
	Running,
	Paused,
	Finished,
	Exiting
};

enum class StatEnum : unsigned
{
	//48 89 6C 24 ? 56 48 83 EC 40 48 8B E9 E8 ? ? ? ? 
	AbilityPower = 0,
	Armor = 1,
	AttackDamage = 2,
	AttackSpeed = 3,
	MagicResist = 5,
	MoveSpeed = 6,
	CritChance = 7,
	CritDamageMultiplier = 8,
	AbilityHaste = 10,
	Health = 11,
	HealthPercent = 12,
	HealthMissingPercent = 13,
	LifeSteal = 15,
	MagicFlatPenetration = 19,
	MagicBonusPercentPenetration = 20,
	ArmorBonusPercentPenetration = 24,
	Lethality = 26,
	Tenacity = 27,
	AttackRange = 28,
	HealthRegen = 29,
	ManaRegen = 30,
};

enum class StatType : uint8_t
{
	TOTAL = 0,
	BASE = 1,
	BONUS = 2,
};

enum class TeamType : int
{
	Ally,
	Enemy,
	Neutral,
};

enum class SummonerSpells
{
	Barrier,
	Flash,
	Cleanse,
	Exhaust,
	Ghost,
	Heal,
	Ignite,
	Smite,
	Teleport,
};

enum class Alliance {
	Ally,
	Enemy,
	Neutral,
};