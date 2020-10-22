/* Wotlk Dev to fcwow
*
* Copyright (C) 2008-2016 JustACore.
* Copyright (C) 2008-2016 TrinityCore.
* Copyright (C) 2005-2016 MaNGOS.
*/

// 整体需要修复,跑是没问题。

#include "ScriptPCH.h"
#include "TowerDefense.h"

class TD_NPC : public CreatureScript
{
public:
    TD_NPC() : CreatureScript(TD_NPC_SCRIPT_NAME) {}

    bool OnGossipHello(Player *player, Creature *creature) 
    { 
        player->PlayerTalkClass->ClearMenus();

        // 加载脚本
        TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI* instance = static_cast<TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI*>(creature->GetInstanceData());
        if(!instance){
            sLog.outBasic("TowerDefense: 脚本未被加载 Scriptsname为TD_NPC OnGossipHello();!");
            return false;
        }

        if(instance->GetEventStatus() > TD_EVENT_STATUS_NONE){
            ChatHandler(player).SendSysMessage(TD_SYSTEM_MSG_ALREADYRUNNING);
            return false;
        }

        instance->AddSimpleMenuItemToPlayer(player,TD_ICON_START_EVENT,TD_NPC_MENU_MAIN, GOSSIP_ACTION_INFO_DEF + 1,TD_NPC_TEXT_START_EVENT);
        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true; 
    }

    bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 action) 
    {
        player->PlayerTalkClass->ClearMenus();
        // 加载脚本
        TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI* instance = static_cast<TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI*>(creature->GetInstanceData());
        if(!instance){
            sLog.outBasic("TowerDefense: 脚本未被加载 Scriptsname为TD_NPC OnGossipHello();!");
            return false;
        }

        switch(sender)
        {
        case TD_NPC_MENU_MAIN:
            if(instance->GetEventStatus() < TD_EVENT_STATUS_TELEPORT){
                instance->AddSimpleMenuItemToPlayer(player,TD_ICON_START_EVENT,TD_NPC_MENU_EVENT_MODE, GOSSIP_ACTION_INFO_DEF + 1, TD_NPC_TEXT_START_EVENT);
                instance->AddSimpleMenuItemToPlayer(player,TD_ICON_DOT_EVENT,TD_NPC_MENU_SHOW_RECORDS, GOSSIP_ACTION_INFO_DEF + 1, TD_NPC_TEXT_SHOW_RECORDS);
                instance->AddSimpleMenuItemToPlayer(player,TD_ICON_EXIT,TD_NPC_MENU_EXIT, GOSSIP_ACTION_INFO_DEF + 1, TD_TEXT_EXIT);
                player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            }else{
                player->GetSession()->SendNotification(TD_SYSTEM_MSG_STARTED_ALREADY);
                player->CLOSE_GOSSIP_MENU();
            }
            break;
        case TD_NPC_MENU_EVENT_MODE:
            instance->AddSimpleMenuItemToPlayer(player,TD_ICON_DOT_EVENT,TD_NPC_MENU_START_EVENT, GOSSIP_ACTION_INFO_DEF + 51, TD_NPC_TEXT_EVENT_EASY);
            instance->AddSimpleMenuItemToPlayer(player,TD_ICON_START_EVENT,TD_NPC_MENU_START_EVENT, GOSSIP_ACTION_INFO_DEF + 52, TD_NPC_TEXT_EVENT_HARD);
            instance->AddSimpleMenuItemToPlayer(player,TD_ICON_START_EVENT,TD_NPC_MENU_START_EVENT, GOSSIP_ACTION_INFO_DEF + 53, TD_NPC_TEXT_EVENT_EXTREME);
            instance->AddSimpleMenuItemToPlayer(player,TD_ICON_EXIT,TD_NPC_MENU_EXIT, GOSSIP_ACTION_INFO_DEF + 1, TD_TEXT_EXIT);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
            break;
        case TD_NPC_MENU_START_EVENT:
            instance->StartEvent(player, action);
            player->CLOSE_GOSSIP_MENU();
            break;
        case TD_NPC_MENU_SHOW_RECORDS:
            instance->GetRecords(player);
            player->CLOSE_GOSSIP_MENU();
            break;
        case TD_NPC_MENU_EXIT:
            player->CLOSE_GOSSIP_MENU();
            break;
        }
        return true;
    }
};

class TD_ITEM : public ItemScript
{
public:
    TD_ITEM() : ItemScript(TD_ITEM_SCRIPT_NAME) {}

    bool OnUse(Player* player, Item* item, SpellCastTargets const& targets)
    {
        player->PlayerTalkClass->ClearMenus();

        if(player->GetMapId() != TD_INSTANCE_MAP_ID)
            return false;

        // 加载脚本
        //TowerDefenseInstanceScript::TowerDefenseMapInstanceScript* instance = static_cast<TowerDefenseInstanceScript::TowerDefenseMapInstanceScript*>(player->GetInstanceScript());
        TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI* instance = static_cast<TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI*>(player->GetInstanceData());

        if(!instance){
            sLog.outBasic("TowerDefense: 脚本未加载  Item的Scriptname为 TD_ITEM OnUse();!");
            return false;
        }

        if(instance->GetEventStatus() < TD_EVENT_STATUS_TELEPORT){
            ChatHandler(player).SendSysMessage(TD_SYSTEM_MSG_CANNOT_USE_OUTSIDE);
            return false;
        }

        instance->AddSimpleMenuItemToPlayer(player,TD_ICON_DOT_EVENT,TD_ITEM_MENU_MAIN, GOSSIP_ACTION_INFO_DEF + 1, TD_ITEM_TEXT_START_EVENT);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,item->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 Sender, uint32 Action) 
    {
        player->PlayerTalkClass->ClearMenus();

        // 加载脚本
        TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI* instance = static_cast<TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI*>(player->GetInstanceData());
        if(!instance){
            sLog.outBasic("TowerDefense: 脚本未加载  Item的Scriptname为 TD_ITEM OnUse();!");
            return false;
        }

        switch(Sender)
        {
        case TD_ITEM_MENU_MAIN:
            if(instance->GetEventStatus() < TD_EVENT_STATUS_TELEPORT)
                instance->AddSimpleMenuItemToPlayer(player,TD_ICON_START_EVENT,TD_ITEM_MENU_SELECT_EVENT_MODE, GOSSIP_ACTION_INFO_DEF + 1, TD_NPC_TEXT_START_EVENT);
            if(instance && instance->GetEventStatus() >= TD_EVENT_STATUS_TELEPORT)
            {
                instance->AddSimpleMenuItemToPlayer(player,TD_ICON_DOT_EVENT,TD_ITEM_MENU_BASE_HEALTH, GOSSIP_ACTION_INFO_DEF + 1, TD_ITEM_TEXT_BASE_HEALTH,instance->GetBaseHealth());
                instance->AddSimpleMenuItemToPlayer(player,TD_ICON_DOT_EVENT,TD_ITEM_MENU_CURRENT_RESOURCES, GOSSIP_ACTION_INFO_DEF + 1, TD_ITEM_TEXT_CURRENT_RESOURCES,instance->GetResources());
                if(instance->GetEventStatus() < TD_EVENT_STATUS_RUNNING && instance->GetEventMode() == TD_EVENT_MODE_EASY){
                    instance->AddSimpleMenuItemToPlayer(player,TD_ICON_START_EVENT,TD_ITEM_MENU_START_WAVE, GOSSIP_ACTION_INFO_DEF + 1, TD_ITEM_TEXT_START_WAVE,instance->GetCurrentWaveId()+1);
                }
                instance->AddSimpleMenuItemToPlayer(player,TD_ICON_BUY_GUARD,TD_ITEM_MENU_SPAWN_TOWER_MENU, GOSSIP_ACTION_INFO_DEF + 1, TD_ITEM_TEXT_SPAWN_TOWER_MENU);
                instance->AddSimpleMenuItemToPlayer(player,TD_ICON_DECLINE,TD_ITEM_MENU_QUIT_EVENT, GOSSIP_ACTION_INFO_DEF + 1, TD_ITEM_TEXT_QUIT_EVENT);
            }
            else{
                instance->AddSimpleMenuItemToPlayer(player,TD_ICON_DOT_EVENT,TD_ITEM_MENU_MAIN, GOSSIP_ACTION_INFO_DEF + 1, TD_ITEM_TEXT_NO_EVENT_DATA);
            }
            instance->AddSimpleMenuItemToPlayer(player,TD_ICON_EXIT,TD_ITEM_MENU_EXIT, GOSSIP_ACTION_INFO_DEF + 1, TD_TEXT_EXIT);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
            break;
        case TD_ITEM_MENU_SELECT_EVENT_MODE:
            instance->AddSimpleMenuItemToPlayer(player,TD_ICON_START_EVENT,TD_ITEM_MENU_START_EVENT, GOSSIP_ACTION_INFO_DEF + 51, TD_NPC_TEXT_EVENT_EASY);
            instance->AddSimpleMenuItemToPlayer(player,TD_ICON_START_EVENT,TD_ITEM_MENU_START_EVENT, GOSSIP_ACTION_INFO_DEF + 52, TD_NPC_TEXT_EVENT_HARD);
            instance->AddSimpleMenuItemToPlayer(player,TD_ICON_START_EVENT,TD_ITEM_MENU_START_EVENT, GOSSIP_ACTION_INFO_DEF + 53, TD_NPC_TEXT_EVENT_EXTREME);
            instance->AddSimpleMenuItemToPlayer(player,TD_ICON_EXIT,TD_ITEM_MENU_EXIT, GOSSIP_ACTION_INFO_DEF + 1, TD_TEXT_EXIT);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case TD_ITEM_MENU_START_EVENT:
            player->CLOSE_GOSSIP_MENU();
            instance->StartEvent(player, Action);
            break;
        case TD_ITEM_MENU_BASE_HEALTH:
            OnGossipSelect(player, creature, TD_ITEM_MENU_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            break;
        case TD_ITEM_MENU_CURRENT_RESOURCES:
            OnGossipSelect(player, creature, TD_ITEM_MENU_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            break;
        case TD_ITEM_MENU_SPAWN_TOWER_MENU:
            instance->AddSimpleMenuItemToPlayer(player,TD_ICON_DOT_EVENT,TD_ITEM_MENU_SPAWN_TOWER_MENU, GOSSIP_ACTION_INFO_DEF + 1, TD_ITEM_TEXT_CURRENT_RESOURCES,instance->GetResources());
            instance->GetGuardList();
            instance->AddSimpleMenuItemToPlayer(player,TD_ICON_DOT_EVENT,TD_ITEM_MENU_MAIN, GOSSIP_ACTION_INFO_DEF + 1, TD_TEXT_GOBACK);
            instance->AddSimpleMenuItemToPlayer(player,TD_ICON_EXIT,TD_ITEM_MENU_EXIT, GOSSIP_ACTION_INFO_DEF + 1, TD_TEXT_EXIT);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case TD_ITEM_MENU_SPAWN_TOWER_GUARD:
            player->CLOSE_GOSSIP_MENU();
            instance->SpawnFromGuardList(Action);
            break;
        case TD_ITEM_MENU_START_WAVE:
            instance->StartNextWave(1000); // 立刻开始
            player->CLOSE_GOSSIP_MENU();
            break;
        case TD_ITEM_MENU_QUIT_EVENT:
            instance->HandleEventComplete(TD_EVENT_COMPLETE_QUIT);
            player->CLOSE_GOSSIP_MENU();
            break;
        case TD_ITEM_MENU_EXIT:
            player->CLOSE_GOSSIP_MENU();
            break;
        }
        return true;
    }
};

class TD_MOB : public CreatureScript
{
public:
    TD_MOB() : CreatureScript(TD_MOB_SCRIPT_NAME) {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new TD_MOBAI (creature);
    }

    struct TD_MOBAI : public ScriptedAI
    {
        TD_MOBAI(Creature* creature) : instance(static_cast<TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI*>(creature->GetInstanceData())), ScriptedAI(creature) {}

        Player* player;
        uint32 MobSpawnTimer, MobEffectTimer, MobPathId, MobPower, MobBounty, MobHealth, MobDmg, MobAttackSpeed, MobSpellId;
        bool MobStartedPath, MobIsRunning, MobIsAirMob;
        float MobAttackDistance, MobSpeed;
        MonsterInfo* monster;
        TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI* instance;

        void Reset()
        {
            if(!instance)
                return;
            player = instance->GetPlayer();
            if(!player)
                return;
            MobSpawnTimer = 500;
            MobEffectTimer = 1000;
            MobStartedPath = false;
            monster = instance->GetMonsterByEntry(me->GetEntry());
            if(!monster){
                me->DespawnOrUnsummon();
                sLog.outBasic("TowerDefense: 生物的ID为: [%u] 有TowerDefense的脚本名,但却不属于Event[%u]的一部分.", me->GetEntry(), instance->GetCurrentWaveId());
            }else{
                MobPathId = monster->GetPathId();
                MobPower = monster->GetPower();
                MobBounty = monster->GetBounty();
                MobIsRunning = monster->GetIsRunning();
                MobIsAirMob = monster->GetIsAirMob();
                MobHealth = monster->GetHealth();
                MobDmg = monster->GetDamage();
                MobAttackDistance = monster->GetAttackDistance();
                MobAttackSpeed = monster->GetAttSpeed();
                MobSpeed = monster->GetSpeed();
                MobSpellId = monster->GetDefSpell();
            }
        }
        /* 进攻方模板设置 SET RegenHealth = 0  */ 

        void InitOnCreate()
        {
            me->SetMaxHealth(MobHealth);
            me->SetHealth(me->GetMaxHealth());
            SetSpeeds(MobSpeed);
        }

        void SetSpeeds(float speed)
        {
            me->SetSpeed(MOVE_WALK,  speed, true);
            me->SetWalk(true);

            if(MobIsRunning){
                me->SetSpeed(MOVE_RUN,   speed, true);
                me->SetWalk(false);
            }
            if(MobIsAirMob){
                me->SetSpeed(MOVE_FLIGHT,speed, true);
                me->SetWalk(false);
            }
        }

        bool WaveIsRunning()
        {
            if(!instance) return false;

            if(instance->GetEventStatus() >= TD_EVENT_STATUS_RUNNING)
                return true;
            return false;
        }

        void StartWalkingToEndPoint() { 
            (MobPathId) ? me->GetMotionMaster()->MovePath(MobPathId,false) : sLog.outBasic("TowerDefense: 生物ID为: [%u] 无法加载到赋予的路径ID,请检查数据库.", me->GetEntry());
        }

        uint32 GetMobSpellByCastType(TDEventSpellCastType type) {
            if(!monster) return 0;
            return monster->GetSpellIdByCastType(type);
        }

        void CastOnSpawnSpells() {
            if(!player) return;

            if(GetMobSpellByCastType(TD_CAST_ON_SPAWN_CAST_SELF))
                me->CastSpell(me, GetMobSpellByCastType(TD_CAST_ON_SPAWN_CAST_SELF), true);
            if(GetMobSpellByCastType(TD_CAST_ON_SPAWN_CAST_TARGET))
                me->CastSpell(player, GetMobSpellByCastType(TD_CAST_ON_SPAWN_CAST_TARGET), true);
        }

        void DoGuardAOEEffects(Unit* guard) {
            if(!instance) return;

            uint32 AOESpell = instance->GetGuardSpellByCastType(guard->GetGUID(), TD_CAST_AOE_CAST_TARGET);
            guard->CastSpell(me, AOESpell, true);

            switch(AOESpell)
            {
            case 15571: // 这是生物在进入冰霜塔半径区域,它的速度会被减慢
                SetSpeeds(MobSpeed * 0.7f);
                break;
            }
        }

        void StopGuardAOEEffects(Unit* guard){
            SetSpeeds(MobSpeed);
        }

        Unit* GetClosestAOEGuard() 
        {  
            if(!instance || !WaveIsRunning())
                return NULL;

            Unit* target = me->SelectNearestTarget(5.0f); // 冰霜塔的AOE效果
            if(target){
                GuardInfo* guardInfo = instance->GetGuardByGuid(target->GetGUID());
                if(guardInfo && guardInfo->Entry == target->GetEntry())
                    if(IsAOEGuard(target))
                        return target;
            }
            return NULL;
        }

        Unit* GetClosestGuard() 
        {  
            if(!instance || !WaveIsRunning())
                return NULL;

            Unit* target = me->SelectNearestTarget(5.0f); // 同上
            if(target){
                GuardInfo* guardInfo = instance->GetGuardByGuid(target->GetGUID());
                if(guardInfo && guardInfo->Entry == target->GetEntry())
                    return target;
            }
            return NULL;
        }

        bool IsAOEGuard(Unit* guard)
        {
            if(!guard)
                return false;
            if (QueryResult* queryResult = CharacterDatabase.PQuery("SELECT spellAOECastTarget FROM custom_td_base_spells WHERE creatureEntry = '%u'", guard->GetEntry()))
                return true;
            return false;
        }

        void JustDied(Unit* killer) 
        {
            if(!instance || !killer) return;
            instance->UpdateUnits(TD_EVENT_DEC, 1);
            instance->UpdateResources(TD_EVENT_INC,MobBounty);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if(!instance || !MobPathId) {return;}
            if (id == instance->GetLastPointInPath(MobPathId)){
                instance->UpdateHealth(MobPower);
                instance->UpdateUnits(TD_EVENT_DEC, 1);
                me->DespawnOrUnsummon();
            }
            return;
        }

        void CastDefaultSpell(Unit* target)
        {
            if(!target)
                return;
            if(me->GetDistance(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ()) > 8)
                me->SetFacingTo(target->GetOrientation());

            if(MobIsAirMob && instance->GetGuardIsAntiAir(target->GetGUID()) && MobSpellId) // 空中单位可以被反空单位攻击
                me->CastSpell(target, MobSpellId, true);
            else if(!MobIsAirMob && instance->GetGuardIsAntiGround(target->GetGUID()) && MobSpellId) // 地面生物可以被地面生物攻击
                me->CastSpell(target, MobSpellId, true);
        }

        void DamageDealt(Unit* target, uint32& damage, DamageEffectType damageType)
        {
            if (damageType != SPELL_DIRECT_DAMAGE) // 这里原本是法术伤害,改为指定伤害，防止和其他伤害控制系统冲突
                return;
            damage = 0;
        }

        void DamageTaken(Unit* attacker, uint32& damage)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC); // 玩家不能攻击
        }

        bool GuardIsInDistance(Unit* guard)
        {
            if(!guard)
                return false;
            if (WaveIsRunning()){
                if(me->GetDistance(guard->GetPositionX(),guard->GetPositionY(),guard->GetPositionZ()) <= MobAttackDistance)
                    return true;
            }
            return false;
        }

        bool IsCasting()
        {
            if(me->HasUnitState(UNIT_STATE_CASTING))
                return true;
            return false;
        }

        void UpdateAI(const uint32 diff)
        {
            if(!instance) return;

            Player* player = instance->GetPlayer();
            if(!player || instance->GetEventStatus() < TD_EVENT_STATUS_TELEPORT) me->DespawnOrUnsummon();

            if(!WaveIsRunning()) return;

            if (MobSpawnTimer <= diff && !MobStartedPath)
            {
                me->CastSpell(me, instance->GetSpellIdByUniqueId(1), true);
                InitOnCreate();
                StartWalkingToEndPoint();
                CastOnSpawnSpells();
                MobStartedPath = true;
            } else MobSpawnTimer -= diff;

            if (MobEffectTimer <= diff)
            {
                Unit* guard = GetClosestAOEGuard();
                // 防守的脚本都点丑陋,看天意，自己动手修吧
                if(guard && me->GetDistance(guard->GetPositionX(),guard->GetPositionY(),guard->GetPositionZ()) > 8)
                    StopGuardAOEEffects(guard);

                if(guard && me->GetDistance(guard->GetPositionX(),guard->GetPositionY(),guard->GetPositionZ()) < 8)
                    DoGuardAOEEffects(guard);

                if (!WaveIsRunning() || IsCasting())
                    return;

                if(GuardIsInDistance(GetClosestGuard()))
                    CastDefaultSpell(GetClosestGuard());

                MobEffectTimer = 1000;
            } else MobEffectTimer -= diff;
        }
    };
};

class TD_GUARD : public CreatureScript
{
public:
    TD_GUARD() : CreatureScript(TD_GUARD_SCRIPT_NAME) {}

    bool OnGossipHello(Player* player, Creature* creature)
    {
        player->PlayerTalkClass->ClearMenus();

        TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI* instance = static_cast<TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI*>(creature->GetInstanceData());
        if(!instance)
            return false;

        if(instance->GetEventStatus() < TD_EVENT_STATUS_TELEPORT){
            ChatHandler(player).SendSysMessage(TD_SYSTEM_MSG_CANNOT_USE_OUTSIDE);
            return false;
        }

        instance->AddSimpleMenuItemToPlayer(player,TD_ICON_DOT_EVENT,TD_GUARD_MENU_MAIN, GOSSIP_ACTION_INFO_DEF + 1,TD_GUARD_TEXT_TOWER_NAME,creature->GetName());
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI* instance = static_cast<TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI*>(creature->GetInstanceData());
        if(!instance)
            return false;

        switch(sender)
        {
        case TD_GUARD_MENU_MAIN:
            if(instance->GetEventStatus() >= TD_EVENT_STATUS_TELEPORT)
            {
                instance->AddSimpleMenuItemToPlayer(player,TD_ICON_DOT_EVENT,TD_GUARD_MENU_NAME, GOSSIP_ACTION_INFO_DEF + 1,TD_GUARD_TEXT_TOWER_NAME,creature->GetName());
                instance->AddSimpleMenuItemToPlayer(player,TD_ICON_DOT_EVENT,TD_GUARD_MENU_CURRENT_RESOURCES, GOSSIP_ACTION_INFO_DEF + 1,TD_GUARD_TEXT_CURRENT_RESOURCES,instance->GetResources());
                instance->UpgradeGuardMenu(creature->GetGUID());
                instance->AddSimpleMenuItemToPlayer(player,TD_ICON_BUY_GUARD,TD_GUARD_MENU_SELL, GOSSIP_ACTION_INFO_DEF + 1,TD_GUARD_TEXT_SELL_GUARD,instance->GetGuardSellPrice(creature->GetGUID()));
            }
            instance->AddSimpleMenuItemToPlayer(player,TD_ICON_EXIT,TD_GUARD_MENU_EXIT, GOSSIP_ACTION_INFO_DEF + 1,TD_TEXT_EXIT);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
            break;
        case TD_GUARD_MENU_NAME:
            OnGossipSelect(player, creature, TD_GUARD_MENU_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            break;
        case TD_GUARD_MENU_CURRENT_RESOURCES:
            OnGossipSelect(player, creature, TD_GUARD_MENU_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            break;
        case TD_GUARD_MENU_UPGRADE: 
            instance->UpgradeGuard(creature->GetGUID());
            player->CLOSE_GOSSIP_MENU();
            break;
        case TD_GUARD_MENU_UPGRADE_MAX:
            OnGossipSelect(player, creature, TD_GUARD_MENU_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            break;
        case TD_GUARD_MENU_SELL: 
            instance->SellGuard(creature->GetGUID());
            player->CLOSE_GOSSIP_MENU();
            break;
        case TD_GUARD_MENU_EXIT:
            player->CLOSE_GOSSIP_MENU();
            break;
        }
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new TD_GUARDAI (creature);
    }

    struct TD_GUARDAI : public ScriptedAI
    {
        TD_GUARDAI(Creature* creature) : instance(static_cast<TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI*>(creature->GetInstanceData())), ScriptedAI(creature) {}

        uint32 SpawnTimer;
        uint32 SpellTimer;
        Unit* Target;
        bool Spawned;
        MonsterInfo* monster;
        TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI* instance;

        void Reset()
        {
            if(!instance){
                sLog.outBasic("TowerDefense: 未加载脚本 Scriptname为 On TD_GUARDAI Reset();!");
                return;
            }
            Spawned = false;
            SpawnTimer = 200;
            SpellTimer = 1000;
            InitOnCreate();
        }

        void InitOnCreate()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISARMED);
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            me->SetFaction(TD_FACTION_GUARD); // 将生物的阵营设置为对玩家中立绿色,对生物红色  自己查阵营ID
            me->SetReactState(REACT_AGGRESSIVE);
        }

        bool WaveIsRunning()
        {
            if(instance){
                if (instance->GetEventStatus() >= TD_EVENT_STATUS_RUNNING)
                    return true;
            }
            return false;
        }

        bool AttackerIsInWave(Unit* attacker)
        {
            if(!attacker)
                return false;
            if(instance->IsCreatureInWave(instance->GetCurrentWaveId(),attacker->GetEntry()))
                return true;
            return false;
        }

        bool AttackerIsInDistance(Unit* attacker)
        {
            if(!attacker)
                return false;
            if (WaveIsRunning()){
                if(me->GetDistance(attacker->GetPositionX(),attacker->GetPositionY(),attacker->GetPositionZ()) <= instance->GetGuardAttackDistance(me->GetGUID()))
                    return true;
            }
            return false;
        }

        bool IsCasting()
        {
            if(me->HasUnitState(UNIT_STATE_CASTING))
                return true;
            return false;
        }

        Unit* GetClosestAttacker() 
        {  
            if(!instance || !WaveIsRunning())
                return NULL;
            Unit* target = NULL;

            target = SelectTarget(SELECT_TARGET_TOPAGGRO,0,instance->GetGuardAttackDistance(me->GetGUID()),false);
            if(!target)
                target = me->SelectNearestTarget(instance->GetGuardAttackDistance(me->GetGUID()));
            if(target && AttackerIsInWave(target)){
                me->AddThreat(target,100);
                return target;
            }
            return NULL;
        }

        uint32 GetDefaultDmg()
        {
            if(!instance)
                return 0;
            uint32 defDmg = instance->GetGuardDamage(me->GetGUID());
            if(defDmg)
                return defDmg;
            else
                instance->RecordLog("TowerDefense: 守卫的默认伤害值未确定: [%u].", me->GetEntry());
            return 0;
        }

        uint32 GetDefaultSpell()
        {
            if(!instance)
                return 0;
            GuardInfo* guard = instance->GetGuardByGuid(me->GetGUID());
            if(guard)
                return guard->GetDefSpell();
            else
                instance->RecordLog("TowerDefense: 守卫的默认技能未找到: [%u].", me->GetEntry());
            return 0;
        }

        void CastDefaultSpell(Unit* target)
        {
            if(!target)
                return;
            if(me->GetDistance(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ()) > 8)
                me->SetFacingTo(target->GetOrientation());

            if(!GetDefaultSpell())
                return;

            monster = instance->GetMonsterByEntry(target->GetEntry());
            if(!monster){
                sLog.outBasic("TowerDefense: 生物的ID为: [%u] 虽然设置了TowerDefense的脚本名称, 但却不是Event[%u]的一部分.", me->GetEntry(), instance->GetCurrentWaveId());
            }else{
                if(instance->GetGuardIsAntiAir(me->GetGUID()) && monster->GetIsAirMob()) // 如果守卫是空中单位,攻击方是地面单位，则施放默认技能
                me->CastSpell(target, GetDefaultSpell(), true);
                else if(instance->GetGuardIsAntiGround(me->GetGUID()) && !monster->GetIsAirMob()) // 如果守卫是反地面单位，攻击者是地面单位，则施放默认技能
                me->CastSpell(target, GetDefaultSpell(), true);
            }
        }

        void DamageDealt(Unit* target, uint32& damage, DamageEffectType damageType)
        {
            if (damageType != SPELL_DIRECT_DAMAGE) //  这里原本是法术伤害,改为指定伤害，防止和其他伤害控制系统冲突
                return;
            damage = 0;
        }

        void DamageTaken(Unit* attacker, uint32& damage)
        {
            if(!instance || !WaveIsRunning())
                return;
            if (!AttackerIsInWave(attacker))
                damage = 0;
        }

        void SpellHitTarget(Unit* target, const SpellEntry* spell)
        {
            if(!target || !WaveIsRunning() || !spell)
                return;
            if(target->GetTypeId() == TYPEID_UNIT && AttackerIsInWave(target)){ 
                me->DealDamage(target,GetDefaultDmg()); 
                me->CastSpell(target,instance->GetGuardSpellByCastType(me->GetGUID(), TD_CAST_ON_HIT_TARGET_CAST_TARGET),true);
            }
        }

        void CastOnSpawnSpells()
        {
            if(!instance)
                return;
            Player* player = instance->GetPlayer();
            if(!player)
                return;
            me->CastSpell(me, instance->GetGuardSpellByCastType(me->GetGUID(), TD_CAST_ON_SPAWN_CAST_SELF), true);
        }

        void UpdateAI(const uint32 diff){
            if(!instance)
                return;

            Player* player = instance->GetPlayer();
            if(!player || instance->GetEventStatus() < TD_EVENT_STATUS_TELEPORT)
                me->DespawnOrUnsummon();

            if (SpawnTimer <= diff && !Spawned)
            {
                me->CastSpell(me, instance->GetSpellIdByUniqueId(1), true);
                CastOnSpawnSpells();
                Spawned = true;
            } else SpawnTimer -= diff;

            if (SpellTimer <= diff)
            {
                if (!WaveIsRunning() || IsCasting())
                    return;

                if(AttackerIsInDistance(GetClosestAttacker()))
                    CastDefaultSpell(GetClosestAttacker());

                SpellTimer = instance->GetGuardAttSpeed(me->GetGUID());
            } else SpellTimer -= diff;
        }
    };
};

class TD_UPDATER_TRIGGER : public CreatureScript
{
public:
    TD_UPDATER_TRIGGER() : CreatureScript(TD_UPDATER_SCRIPT_NAME) { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new TD_UPDATER_TRIGGERAI(creature);
    }

    struct TD_UPDATER_TRIGGERAI : public ScriptedAI
    {
        TD_UPDATER_TRIGGERAI(Creature* creature) : instance(static_cast<TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI*>(creature->GetInstanceData())), ScriptedAI(creature) {}

        TowerDefenseInstanceScript::TowerDefenseInstanceScriptAI* instance;

        uint32 SpawnTimer;
        uint32 UnitsCounter;
        Player* player;

        void Reset()
        {
            SpawnTimer = 1000;
            UnitsCounter = 0;
            InitOnCreate();
        }

        void InitOnCreate()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_IMMUNE_TO_PC);
            me->SetWalk(false);
            me->SetFaction(35);
            me->SetReactState(REACT_PASSIVE);
        }

        bool WaveIsLoaded()
        {
            if (!instance)
                return false;
            if (instance->GetEventStatus() == TD_EVENT_STATUS_LOADED)
                return true;
            return false;
        }

        void SpawnWaveCreatures()
        {
            if (!instance)
                return;
            WaveInfo* wave = instance->GetWaveById(instance->GetCurrentWaveId());
            if(UnitsCounter < wave->GetNumCreatures()) 
            {
                    instance->SpawnNextCreature(me);
                    instance->UpdateUnits(TD_EVENT_INC,1); 
                    UnitsCounter++;
            }else{
                instance->SetEventStatus(TD_EVENT_STATUS_LOADED);
                instance->RecordLog("TowerDefense: 事件 [%u] 开始加载,它的Id是: [%u].", instance->GetCurrentWaveId(), instance->GetEventId());
                UnitsCounter = 0;
            }
        }

        void UpdateAI(uint32 const diff)
        {
            if (!instance)
                return;
            player = instance->GetPlayer();
            if(!player)
                return;

            instance->OnUpdate(diff); 
            if (instance->GetEventStatus() < TD_EVENT_STATUS_LOADING)
                return;

            if(instance->GetUnits() == 0 && WaveIsLoaded()){
                instance->SendMessageToPlayer(TD_SYSTEM_MSG_WAVE_HAS_ENDED, instance->GetCurrentWaveId());
                instance->SetEventStatus(TD_EVENT_STATUS_TELEPORT);
                instance->RecordLog("事件Id: [%u] 会话中未刷新单位 [%u], 停止运行.",instance->GetEventId(),instance->GetCurrentWaveId());

                WaveInfo* wave = instance->GetWaveById(instance->GetCurrentWaveId()+1);  
                if(!wave) // 如果没有会话产生,则事件完成
                    instance->HandleEventComplete(TD_EVENT_COMPLETE_WON);

                switch(instance->GetEventMode())
                {
                case TD_EVENT_MODE_HARD:
                    instance->SendMessageToPlayer(TD_SYSTEM_MSG_NEXT_WAVE_STARTS_IN_HARD, 30);
                    instance->StartNextWave(30000); // 30秒后启动会话
                    break;
                case TD_EVENT_MODE_EXTREME:
                    instance->StartNextWave(1000); // 立刻启动会话
                    break;
                }
            }

            if (!WaveIsLoaded()){ 
                if (SpawnTimer <= diff)
                {
                    SpawnWaveCreatures();
                    SpawnTimer = 1000;
                } else SpawnTimer -= diff;
            }
        }
    };
};

class TD_VENDOR : public CreatureScript
{
public:
    TD_VENDOR() : CreatureScript(TD_VENDOR_SCRIPT_NAME) {}

    bool OnGossipHello(Player *player, Creature *creature) 
    { 
        player->PlayerTalkClass->ClearMenus();
        //player->PlayerTalkClass->GetGossipMenu()->AddMenuItem(0, TD_ICON_DOT_EVENT, GOSSIP_SENDER_INFO, GOSSIP_ACTION_INFO_DEF, "", 0);
        player->PlayerTalkClass->GetGossipMenu().AddMenuItem(TD_ICON_DOT_EVENT, TD_VENDOR_TEXT_EXCHANGE, TD_VENDOR_MENU_MAIN, GOSSIP_ACTION_INFO_DEF + 1, "", false);
        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true; 
    }

    bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 action) 
    {
        player->PlayerTalkClass->ClearMenus();
        switch(sender)
        {
        case TD_VENDOR_MENU_MAIN:
            player->PlayerTalkClass->GetGossipMenu().AddMenuItem(TD_ICON_DOT_EVENT, TD_VENDOR_TEXT_EXCHANGE, TD_VENDOR_EXCHANGE, GOSSIP_ACTION_INFO_DEF + 1, "", false);
            player->PlayerTalkClass->GetGossipMenu().AddMenuItem(TD_ICON_BUY_GUARD, TD_VENDOR_TEXT_LIST, TD_VENDOR_LIST, GOSSIP_ACTION_INFO_DEF + 1, "", false);
            player->PlayerTalkClass->GetGossipMenu().AddMenuItem(TD_ICON_EXIT, TD_TEXT_EXIT, TD_VENDOR_MENU_EXIT, GOSSIP_ACTION_INFO_DEF + 1, "", false);
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            break;
        case TD_VENDOR_EXCHANGE:
            player->CLOSE_GOSSIP_MENU();
            if(uint32 ResAmount = GetAmountOfResourcesPlayerHas(player))
            {
                UpdatePlayerResources(player, TD_EVENT_DEC, ResAmount);
                player->AddItem(40752, ResAmount);
                ChatHandler(player).SendSysMessage("NO,所有的资源都被置换了");
            }else
                ChatHandler(player).SendSysMessage("NO,你没有任何资源了!");
            break;
        case TD_VENDOR_LIST:
            player->GetSession()->SendListInventory(creature->GetGUID());
            break;
        case TD_VENDOR_MENU_EXIT:
            player->CLOSE_GOSSIP_MENU();
            break;
        }
        return true;
    }

    uint32 GetAmountOfResourcesPlayerHas(Player* player)
    {
        if(!player)
            return 0;
        if(QueryResult* queryResult = CharacterDatabase.PQuery("SELECT currentResources FROM custom_td_players WHERE playerGUID = '%u'", player->GetGUIDLow()))
        {
            Field* Fields = queryResult->Fetch();
            return Fields[0].GetUInt32();
        }
        return 0;
    }

    void UpdatePlayerResources(Player* player, TDEventUpdateType type, uint32 resValue)
    {
        if(!player)
            return;
        if(!type)
            return;
        uint32 newRes = 0;
        if(QueryResult* queryResult = CharacterDatabase.PQuery("SELECT currentResources FROM custom_td_players WHERE playerGUID = '%u'", player->GetGUIDLow()))
        {
            Field* Fields = queryResult->Fetch();
            uint32 oldRes = Fields[0].GetUInt32();

            switch(type)
            {
            case TD_EVENT_INC:
                newRes = oldRes + resValue;
                break;
            case TD_EVENT_DEC:
                newRes = oldRes - resValue;
                break;
            }
            CharacterDatabase.PExecute("UPDATE custom_td_players SET currentResources = '%u' WHERE playerGUID = '%u'", newRes, player->GetGUIDLow());
        }
        return;
    }
};

void AddSC_TowerDefenseMisc()
{
    new TD_NPC();
    new TD_ITEM();
    new TD_MOB();
    new TD_GUARD();
    new TD_VENDOR();
    new TD_UPDATER_TRIGGER();
}