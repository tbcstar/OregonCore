/*
 * This file is part of the OregonCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Orgrimmar
SD%Complete: 100
SDComment: Quest support: 2460, 5727, 6566
SDCategory: Orgrimmar
EndScriptData */

/* ContentData
npc_neeru_fireblade     npc_text + gossip options text missing
npc_shenthul
npc_thrall_warchief
npc_eitrigg
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"

/*######
## npc_neeru_fireblade
######*/

#define QUEST_5727  5727

#define GOSSIP_HNF "你可以坦率地说，尼尔鲁..."
#define GOSSIP_SNF "[PH] ..."




/*######
## npc_shenthul
######*/

enum eShenthul
{
    QUEST_SHATTERED_SALUTE  = 2460
};







/*######
## npc_thrall_warchief
######*/

#define QUEST_6566              6566

#define SPELL_CHAIN_LIGHTNING   16033
#define SPELL_SHOCK             16034

#define GOSSIP_HTW "请和我分享你的智慧，酋长。"
#define GOSSIP_STW1 发现什么?"
#define GOSSIP_STW2 "篡位者？"
#define GOSSIP_STW3 "恕我直言，酋长 - 为什么不让他们被摧毁？ 这不会巩固我们的地位吗？"
#define GOSSIP_STW4 "我...我没想到那样，酋长。"
#define GOSSIP_STW5 "我活着就是为了服务，酋长!没有你的指引，我的生命是空虚的，毫无意义的。"
#define GOSSIP_STW6 "当然,酋长!"

//@todo verify abilities/timers







/*######
## npc_eitrigg
######*/

#define GOSSIP_HELLO    "你好,伊崔格。我带来了贝莱德尖塔的消息。"
#define GOSSIP_SELECT1  "这里只有一个酋长，伊崔格！"
#define GOSSIP_SELECT2  "你是什么意思?"
#define GOSSIP_SELECT3  "壁炉谷?但是…"
#define GOSSIP_SELECT4  "我会接受你的提议的，伊崔格。"
#define GOSSIP_SELECT5  "啊，原来他们就是这样把黑铁部族推到塔尖下面的。"
#define GOSSIP_SELECT6  "也许有办法?"
#define GOSSIP_SELECT7  "如你所愿，伊崔格。"








class npc_neeru_fireblade : public CreatureScript
{
public: 
    npc_neeru_fireblade() : CreatureScript("npc_neeru_fireblade") { }
    

    

    bool OnGossipHello(Player* pPlayer, Creature* pCreature) override
    {
        if (pCreature->IsQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());
    
        if (pPlayer->GetQuestStatus(QUEST_5727) == QUEST_STATUS_INCOMPLETE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HNF, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    
        pPlayer->SEND_GOSSIP_MENU(4513, pCreature->GetGUID());
        return true;
    }
    

    

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction) override
    {
        switch (uiAction)
        {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SNF, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(4513, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(QUEST_5727);
            break;
        }
        return true;
    }
    

    

    
};

class npc_shenthul : public CreatureScript
{
public: 
    npc_shenthul() : CreatureScript("npc_shenthul") { }
    struct npc_shenthulAI : public ScriptedAI
    {
        npc_shenthulAI(Creature* c) : ScriptedAI(c) {}
    
        bool CanTalk;
        bool CanEmote;
        uint32 Salute_Timer;
        uint32 Reset_Timer;
        uint64 PlayerGUID;
    
        void Reset()
        {
            CanTalk = false;
            CanEmote = false;
            Salute_Timer = 6000;
            Reset_Timer = 0;
            PlayerGUID = 0;
        }
    
        void EnterCombat(Unit* /*who*/) {}
    
        void UpdateAI(const uint32 diff)
        {
            if (CanEmote)
            {
                if (Reset_Timer <= diff)
                {
                    if (Player* pPlayer = Unit::GetPlayer(*me, PlayerGUID))
                    {
                        if (pPlayer->GetTypeId() == TYPEID_PLAYER && pPlayer->GetQuestStatus(QUEST_SHATTERED_SALUTE) == QUEST_STATUS_INCOMPLETE)
                            pPlayer->FailQuest(QUEST_SHATTERED_SALUTE);
                    }
                    Reset();
                }
                else Reset_Timer -= diff;
            }
    
            if (CanTalk && !CanEmote)
            {
                if (Salute_Timer <= diff)
                {
                    me->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
                    CanEmote = true;
                    Reset_Timer = 60000;
                }
                else Salute_Timer -= diff;
            }
    
            if (!UpdateVictim())
                return;
    
            DoMeleeAttackIfReady();
        }
    
        void ReceiveEmote(Player* pPlayer, uint32 emote)
        {
            if (emote == TEXT_EMOTE_SALUTE && pPlayer->GetQuestStatus(QUEST_SHATTERED_SALUTE) == QUEST_STATUS_INCOMPLETE)
            {
                if (CanEmote)
                {
                    pPlayer->AreaExploredOrEventHappens(QUEST_SHATTERED_SALUTE);
                    Reset();
                }
            }
        }
    };

    

     CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_shenthulAI (pCreature);
    }

    bool OnQuestAccept(Player* pPlayer, Creature* pCreature, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_SHATTERED_SALUTE)
        {
            CAST_AI(npc_shenthulAI, pCreature->AI())->CanTalk = true;
            CAST_AI(npc_shenthulAI, pCreature->AI())->PlayerGUID = pPlayer->GetGUID();
        }
        return true;
    }
    

    

    
};

class npc_thrall_warchief : public CreatureScript
{
public: 
    npc_thrall_warchief() : CreatureScript("npc_thrall_warchief") { }
    struct npc_thrall_warchiefAI : public ScriptedAI
    {
        npc_thrall_warchiefAI(Creature* c) : ScriptedAI(c) {}
    
        uint32 ChainLightning_Timer;
        uint32 Shock_Timer;
    
        void Reset()
        {
            ChainLightning_Timer = 2000;
            Shock_Timer = 8000;
        }
    
        void EnterCombat(Unit* /*who*/) {}
    
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
    
            if (ChainLightning_Timer <= diff)
            {
                DoCastVictim( SPELL_CHAIN_LIGHTNING);
                ChainLightning_Timer = 9000;
            }
            else ChainLightning_Timer -= diff;
    
            if (Shock_Timer <= diff)
            {
                DoCastVictim( SPELL_SHOCK);
                Shock_Timer = 15000;
            }
            else Shock_Timer -= diff;
    
            DoMeleeAttackIfReady();
        }
    };

    

     CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_thrall_warchiefAI (pCreature);
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature) override
    {
        if (pCreature->IsQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());
    
        if (pPlayer->GetQuestStatus(QUEST_6566) == QUEST_STATUS_INCOMPLETE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HTW, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction) override
    {
        switch (uiAction)
        {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_STW1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(5733, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_STW2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            pPlayer->SEND_GOSSIP_MENU(5734, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_STW3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            pPlayer->SEND_GOSSIP_MENU(5735, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_STW4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            pPlayer->SEND_GOSSIP_MENU(5736, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_STW5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
            pPlayer->SEND_GOSSIP_MENU(5737, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_STW6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
            pPlayer->SEND_GOSSIP_MENU(5738, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+7:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(QUEST_6566);
            break;
        }
        return true;
    }
    

    

    
};

class npc_eitrigg : public CreatureScript
{
public: 
    npc_eitrigg() : CreatureScript("npc_eitrigg") { }
    

    

    bool OnGossipHello(Player* pPlayer, Creature* pCreature) override
    {
        if (pCreature->IsQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());
    
        if (pPlayer->GetQuestStatus(4941) == QUEST_STATUS_INCOMPLETE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
    
        pPlayer->SEND_GOSSIP_MENU(3573, pCreature->GetGUID());
        return true;
    }
    
    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction) override
    {
        switch (uiAction)
        {
        case GOSSIP_ACTION_INFO_DEF:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SELECT1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            pPlayer->SEND_GOSSIP_MENU(3574, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SELECT2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(3575, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SELECT3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            pPlayer->SEND_GOSSIP_MENU(3576, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SELECT4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            pPlayer->SEND_GOSSIP_MENU(3577, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SELECT5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            pPlayer->SEND_GOSSIP_MENU(3578, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SELECT6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
            pPlayer->SEND_GOSSIP_MENU(3579, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+6:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SELECT7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
            pPlayer->SEND_GOSSIP_MENU(3580, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+7:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(4941);
            break;
        }
        return true;
    }
    

    

    
};


void AddSC_orgrimmar()
{
    new npc_neeru_fireblade();
    new npc_shenthul();
    new npc_thrall_warchief();
    new npc_eitrigg();

}

