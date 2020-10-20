#include "ScriptPCH.h"
#include "BattlegroundMgr.h"
#include "Chat.h"

class npc_arena_spectator : public CreatureScript
{
public:
    npc_arena_spectator() : CreatureScript("npc_arena_spectator") {}

bool OnGossipHello(Player *pPlayer, Creature *pCreature)
{
	pPlayer->ADD_GOSSIP_ITEM(0, "显示高于1800评分的比赛列表", GOSSIP_SENDER_MAIN, 1000);
	pPlayer->ADD_GOSSIP_ITEM(0, "显示低于1800评分的比赛列表", GOSSIP_SENDER_MAIN, 2000);
	
	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
	return true;
}

std::string GetClassNameById(uint8 id)
{
    std::string sClass = "";
    switch (id)
    {
        case CLASS_WARRIOR:         sClass = "战士";        break;
        case CLASS_PALADIN:         sClass = "圣骑士";           break;
        case CLASS_HUNTER:          sClass = "猎人";           break;
        case CLASS_ROGUE:           sClass = "盗贼";          break;
        case CLASS_PRIEST:          sClass = "牧师";         break;
        case CLASS_DEATH_KNIGHT:    sClass = "死亡骑士";             break;
        case CLASS_SHAMAN:          sClass = "萨满";          break;
        case CLASS_MAGE:            sClass = "法师";           break;
        case CLASS_WARLOCK:         sClass = "术士";        break;
        case CLASS_DRUID:           sClass = "德鲁伊";          break;
    }
    return sClass;
}

std::string GetGamesStringData(Battleground *bg, uint32 rating)
{
	std::string teamsMember[BG_TEAMS_COUNT];
    uint32 firstTeamId = 0;
    for (Battleground::BattlegroundPlayerMap::const_iterator itr = bg->GetPlayers().begin(); itr != bg->GetPlayers().end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            {
                uint32 team = itr->second.Team;
                if (!firstTeamId)
                    firstTeamId = team;

                teamsMember[firstTeamId == team] += GetClassNameById(player->getClass());
            }

    std::string data = teamsMember[0] + " - ";
    std::stringstream ss;
    ss << rating;
    data += ss.str();
    data += " - " + teamsMember[1];
    return data;
}

uint64 GetFirstPlayerGuid(Battleground *bg)
{
    for (Battleground::BattlegroundPlayerMap::const_iterator itr = bg->GetPlayers().begin(); itr != bg->GetPlayers().end(); ++itr)
	{
        if (Player* pPlayer = ObjectAccessor::FindPlayer(itr->first))
			return pPlayer->GetGUID();
		else
			return 0;
	}
	return 0;
}

void ShowPage (Player *pPlayer, Creature* pCreature, uint32 pages, bool isTop)
{
	uint32 highGames = 0;
	uint32 lowGames = 0;
	uint32 gamesOnPage = 20;
	bool nextPage = false;
	for (BattlegroundSet::iterator itr = sBattlegroundMgr.m_Battlegrounds.begin(); itr != sBattlegroundMgr.m_Battlegrounds.end(); ++itr)
	{
		Battleground * bg = itr->second;
		if (bg->isArena() && bg->isRated())
		{
			if (Player * aPlayer = ObjectAccessor::FindPlayer(GetFirstPlayerGuid(bg)))
			{
				uint32 rating = aPlayer->GetMaxPersonalArenaRatingRequirement();
				if (isTop && rating >= 1800)
				{
					++highGames;
					if (highGames > (pages + 1) * gamesOnPage)
						nextPage = true;
					if (highGames >= pages * gamesOnPage)
						pPlayer->ADD_GOSSIP_ITEM(0, GetGamesStringData(bg, rating), GOSSIP_SENDER_MAIN, (3000 + GetFirstPlayerGuid(bg)));
				}
				else if (!isTop && rating < 1800)
				{
					++lowGames;
					if (lowGames > (pages + 1) * gamesOnPage)
						nextPage = true;
					if (lowGames >= pages * gamesOnPage)
						pPlayer->ADD_GOSSIP_ITEM(0, GetGamesStringData(bg, rating), GOSSIP_SENDER_MAIN, (3000 + GetFirstPlayerGuid(bg)));
				}
			}
			/*else
			{
				pPlayer->ADD_GOSSIP_ITEM(0, "No Rated Arena Games Now!!!", GOSSIP_SENDER_MAIN, (3000 + GetFirstPlayerGuid(bg)));
			}*/
		}
	}
	if (pages > 0)
		pPlayer->ADD_GOSSIP_ITEM(0, "上一页...", GOSSIP_SENDER_MAIN, 1000 + pages - 1);
	if (nextPage)
		pPlayer->ADD_GOSSIP_ITEM(0, "下一页...", GOSSIP_SENDER_MAIN, 1000 + pages + 1);
}

bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	pPlayer->PlayerTalkClass->ClearMenus();
	if (uiAction >= 1000 && uiAction < 2000)
	{
		ShowPage(pPlayer, pCreature, uiAction - 1000, true);
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
	}
	else if (uiAction >= 2000 && uiAction < 3000)
	{
		ShowPage(pPlayer, pCreature, uiAction - 2000, false);
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
	}
	else
	{
		pPlayer->CLOSE_GOSSIP_MENU();

		// ����� ������ ��� ���������
		uint64 guid = (uiAction - 3000);
		if (Player *pTarget = ObjectAccessor::FindPlayer(guid))
			{
			if (pTarget->GetMap()->IsBattlegroundOrArena())
			{
				// ���������� ������������� ����� � ������� ��������� ����������.
				uint32 bgInstId = pTarget->GetBattleground()->GetInstanceID();
				if (pPlayer->IsInvitedForBattlegroundInstance(bgInstId))
				{
					pPlayer->CLOSE_GOSSIP_MENU();
					pCreature->MonsterWhisper("You have INVITED to this Arena match! Can not spectater yourself.", pPlayer->GetGUID());
					return true;
				}

				pPlayer->SetBattlegroundId(pTarget->GetBattlegroundId());
				// remember current position as entry point for return at bg end teleportation
				if (!pPlayer->GetMap()->IsBattlegroundOrArena())
					pPlayer->SetBattlegroundEntryPoint();

				// stop flight if need
				if (pPlayer->IsInFlight())
				{
					pPlayer->GetMotionMaster()->MovementExpired();
					pPlayer->CleanupAfterTaxiFlight();
				}
				// save only in non-flight case
				else
					pPlayer->SaveRecallPosition();

				// to point to see at pTarget with same orientation
				float x,y,z;
				pTarget->GetContactPoint(pPlayer,x,y,z);

				if (pPlayer->GetPet())
				{
					if (pPlayer->GetPet()->getPetType() == SUMMON_PET || pPlayer->GetPet()->getPetType() == HUNTER_PET)
					{
						pPlayer->SetTemporaryUnsummonedPetNumber(pPlayer->GetPet()->GetCharmInfo()->GetPetNumber());
						pPlayer->SetOldPetSpell(pPlayer->GetPet()->GetUInt32Value(UNIT_CREATED_BY_SPELL));
					}
					pPlayer->RemovePet(NULL,PET_SAVE_NOT_IN_SLOT);
				}
				else
					pPlayer->SetTemporaryUnsummonedPetNumber(0);

				pPlayer->TeleportTo(pTarget->GetMapId(), x, y, z, pPlayer->GetAngle(pTarget), TELE_TO_GM_MODE);

				pPlayer->SetSpectator(true);

				pCreature->MonsterWhisper("You can Cancel Spectate! Use command .SPSTOP", pPlayer->GetGUID());
				return true;
			}
			else
			{
				pPlayer->CLOSE_GOSSIP_MENU();
				pCreature->MonsterWhisper("Players not in Arena Now!!!", pPlayer->GetGUID());
				return true;
			}
		}
		else
		{
			pPlayer->CLOSE_GOSSIP_MENU();
			pCreature->MonsterWhisper("Can not Find Target Players!!!", pPlayer->GetGUID());
			return true;
		}
	}
	return true;
}
};

void AddSC_npc_arena_spectator() // ����������� �������
{
    new npc_arena_spectator();
}