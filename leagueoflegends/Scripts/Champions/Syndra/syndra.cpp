#include "../stdafx.h"
#include "../Orbwalker.h"

class SyndraModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Syndra");

    Skillshot q = SkillshotManager::RegisterSpell(name, SpellIndex::Q, Skillshot(800.0f, 105.0f, 0.0f, 0.6f, SkillshotType::SkillshotCircle));
    Skillshot w = SkillshotManager::RegisterSpell(name, SpellIndex::W, Skillshot(950.0f, 110.0f, 1900.0f, 0.4f, SkillshotType::SkillshotCircle));
    Skillshot e = SkillshotManager::RegisterSpell(name, SpellIndex::E, Skillshot(1300.0f, 90.0f, 2200.0f, 0.25f, SkillshotType::SkillshotLine));

private:
    float gameTime = 0.0f;

    float lastQForECastTime = 0.0f;
    float lastW1CastTime = 0.0f;
    float lastECastTime = 0.0f;

    Object* lastESphere = nullptr;

    Vector3 lastEObjPos = {};
    Vector3 lastEProjectionPos = {};
    Vector3 lastETargetPos = {};

public:
    SyndraModule()
    {
        ChampionModuleManager::RegisterModule(name, this);
    }

    bool CanCastW1()
    {
        return gameTime > lastW1CastTime + 0.1f;
    }

    bool SphereForEExists(Vector3 targetPos)
    {
        auto playerPos = globals::localPlayer->GetPosition();
        playerPos.y = 0.0f;
        targetPos.y = 0.0f;

        std::vector<QWORD> includeFilterTypeHashes = { ObjectType::Special, ObjectType::Less };

        for (Object* obj : *globals::minionManager)
        {
            float dist = obj->GetPosition().Distance(playerPos);
            if (dist > 800.0f || dist < 150.0f) continue;
            
            bool includeHashMatched = false;
            for (auto hash : includeFilterTypeHashes)
            {
                if (obj->GetCharacterData()->GetObjectTypeHash() == hash)
                {
                    includeHashMatched = true;
                    break;
                }
            }
            if (includeFilterTypeHashes.size() && !includeHashMatched) continue;

            if (obj->GetName() != SP_STRING("SyndraSphere") &&
                (obj->GetName() != SP_STRING("TestCubeRender10Vision") || gameTime - lastQForECastTime > 0.5f)) continue;
            if (!obj->IsAlive()) continue;
            if ((obj->GetActionState() & 0x400) != 0) continue;

            auto objPos = obj->GetPosition();
            objPos.y = 0.0f;

            auto playerToObj = objPos - playerPos;
            auto playerToTarget = targetPos - playerPos;

            if (playerToObj.Length() >= playerToTarget.Length() + 500.0f) continue;

            float dot1 = playerToTarget.DotProduct(playerToObj);
            float dot2 = playerToObj.DotProduct(playerToObj);

            if (dot1 < 0.0f) continue;

            float t = dot1 / dot2;

            Vector3 projection = playerPos + (playerToObj * t);
            projection.y = 0.0;

            Vector3 distVector = targetPos - projection;
            distVector.y = 0.0;

            lastEObjPos = objPos;
            lastEProjectionPos = projection;
            lastETargetPos = targetPos;

            float objY = obj->GetPosition().y;
            lastEObjPos.y = objY;
            lastEProjectionPos.y = objY;
            lastETargetPos.y = objY;

            if (distVector.Length() <= e.GetRadius())
            {
                lastESphere = obj;
                return true;
            }
        }
        lastESphere = nullptr;
        return false;
    }

    void Initialize() override
    {

    }

    void Update() override
    {
        gameTime = Engine::GetGameTime();
    }

    void Combo() override
    {
        Modules::prediction::PredictionOutput qPrediction;
        Modules::prediction::PredictionOutput wPrediction;
        Modules::prediction::PredictionOutput ePrediction;

        auto spellCast = globals::localPlayer->GetActiveSpellCast();
        if (spellCast && spellCast->GetSpellId() == SpellIndex::E) return;

        if (e.IsCastable() && Modules::prediction::GetPrediction(e, ePrediction))
        {
            if (q.IsCastable() && (q.GetStacks() > 0 || q.GetName().size() == 7) &&
                globals::localPlayer->GetSpellBySlotId(SpellIndex::E)->GetManaCost() +
                globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetManaCost() <= globals::localPlayer->GetMana())
            {
                Engine::CastToPosition(SpellIndex::Q, ePrediction.position);
                lastQForECastTime = gameTime;
                return;
            }

            if (lastQForECastTime > gameTime + 0.1f ||  SphereForEExists(ePrediction.position))
            {
                Engine::CastToPosition(SpellIndex::E, ePrediction.position);
                lastECastTime = gameTime;
                return;
            }
        }
        else
        {
            lastEObjPos = {};
            lastEProjectionPos = {};
            lastETargetPos = {};
        }
        
        if (w.IsCastable() && (int)w.GetName().size() == 11 && Modules::prediction::GetPrediction(w, wPrediction))
        {
            Engine::CastToPosition(SpellIndex::W, wPrediction.position);
            return;
        }

        if (q.IsCastable() && (q.GetStacks() > 0 || q.GetName().size() == 7) && Modules::prediction::GetPrediction(q, qPrediction))
        {
            Engine::CastToPosition(SpellIndex::Q, qPrediction.position);
            return;
        }

        if (CanCastW1() && w.IsCastable() && (int)w.GetName().size() == 7 && Modules::prediction::GetPrediction(w, wPrediction))
        {
            auto monster = ObjectManager::GetObjectInRange(w.GetRange(), "",
                { ObjectType::Minion_Lane, ObjectType::Monster },
                { ObjectType::Monster_Epic, ObjectType::Monster_Dragon }, false);
            if (monster && monster == Engine::GetSelectedObject())
            {
                Engine::CastToPosition(SpellIndex::W, monster->GetPosition());
                lastW1CastTime = gameTime;
                return;
            }

            auto sphere = ObjectManager::GetObjectInRange(w.GetRange(), SP_STRING("SyndraSphere"), { ObjectType::Special }, {}, true);
            if (sphere && (sphere != lastESphere || gameTime > lastECastTime + 0.4f))
            {
                Engine::CastToPosition(SpellIndex::W, sphere->GetPosition());
                lastW1CastTime = gameTime;
                return;
            }

            if (monster)
            {
                Engine::CastToPosition(SpellIndex::W, monster->GetPosition());
                lastW1CastTime = gameTime;
                return;
            }
        }
    }

    void Clear() override {
        return;
    }

    void Harass() override {
        return;
    }

    void Lasthit() override {
        return;
    }

    void Killsteal() {
        return;
    }

    void Flee() override {
        return;
    }

    //Events
    void OnCreateMissile() override {
        return;
    }

    void OnDeleteMissile() override {
        return;
    }

    void OnBeforeAttack() override {
        return;
    }

    void OnAfterAttack() override {
        return;
    }
    void Render() override
    {
        if (e.IsCastable())
        {
            render::RenderCircleWorld(globals::localPlayer->GetPosition(), 40, e.GetRange(), COLOR_PURPLE, 1.0f);
        }

        if (lastEObjPos.IsValid() && lastEProjectionPos.IsValid() && lastETargetPos.IsValid() )
        {
            render::RenderCircleWorld(lastEObjPos, 20, 30.0f, COLOR_BLUE, 3.0f);
            render::RenderCircleWorld(lastETargetPos, 20, 30.0f, COLOR_RED, 3.0f);

            auto projectionDirection = lastEProjectionPos - lastETargetPos;
            auto projectionDirectionN = projectionDirection.Normalized();
            auto projectionSuccess = lastETargetPos + ((projectionDirection.Length() > e.GetRadius()) ? projectionDirectionN * e.GetRadius() : projectionDirection);

            auto objScreen = Engine::WorldToScreen(lastEObjPos).ToImVec();
            auto projectionScreen = Engine::WorldToScreen(lastEProjectionPos).ToImVec();
            auto targetScreen = Engine::WorldToScreen(lastETargetPos).ToImVec();
            auto projectionSuccessScreen = Engine::WorldToScreen(projectionSuccess).ToImVec();

            render::RenderLine(objScreen, projectionScreen, COLOR_WHITE, 3.0f);
            render::RenderLine(projectionSuccessScreen, projectionScreen, COLOR_RED, 3.0f);
            render::RenderLine(targetScreen, projectionSuccessScreen, COLOR_GREEN, 3.0f);
        }
    }
};

SyndraModule module;