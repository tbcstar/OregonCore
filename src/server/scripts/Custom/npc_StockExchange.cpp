#include "ScriptPCH.h"
#include "Config.h"
#include "ScriptedCreature.h"
#include "Language.h"
#include "time.h"

#define STOCKNUMBER 5
#define GOLDTOCOPPER 10000

class CS_StockExchange : public CreatureScript
{
public:
    CS_StockExchange() : CreatureScript("Creature_StockExchange") { }

    uint32 stock = 0;

    bool OnGossipHello(Player* plr, Creature* creature) override //MAIN MENU
    {
        //StockUpdate every 2 minutes
        QueryResult* result = WorldDatabase.PQuery("SELECT `UpdateTime`   FROM `stocks` WHERE iD = 0");
        if (!result)
            return false;
        Field* field = result->Fetch();
        uint32 stockPrevTime = field[0].GetUInt32();
        uint32 realCurrTime = getMSTime();
        uint32 stockDiff = getMSTimeDiff(stockPrevTime, realCurrTime);
        if (stockDiff >= 120000)
        {
            uint32 timeMult;
            timeMult = stockDiff / 120000;
            if (StockUpdate(timeMult))
                sLog.outError("股票已更新 (%u)", realCurrTime);
            else
            {
                sLog.outError("更新股票时出错...");
            }
            stockPrevTime = realCurrTime;
            WorldDatabase.PExecute("UPDATE `stocks` SET `UpdateTime` = %u WHERE `iD` = 0", getMSTime());
        }
        WorldSession* session = plr->GetSession();
        QueryResult* licencetest = CharacterDatabase.PQuery("SELECT `1`, `2`, `3`, `4`, `5`  FROM `character_stockdata` WHERE iD = %u", plr->GetGUIDLow());
        plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Scroll_08:30:30:-18:0|t现在的汇率是多少？", GOSSIP_SENDER_MAIN, 2);
        plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Book_11:30:30:-18:0|t还有多少未售出的股票?", GOSSIP_SENDER_MAIN, 1);
        if (!licencetest)
        {
            plr->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_MISC_NOTE_02:30:30:-18:0|t我想获得一份证券资格证。", GOSSIP_SENDER_MAIN, 12, "你需要支付足够的金币", 4000000, false);
        }
        else //Player can invest if he has a licence
        {
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Coin_02:30:30:-18:0|t我想管理我的投资组合。", GOSSIP_SENDER_MAIN, 3);
        }
        plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t很高兴和你做生意。", GOSSIP_SENDER_MAIN, 4);
        plr->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        return true;
    }
    bool OnGossipSelect(Player* plr, Creature* creature, uint32 sender, uint32 uiAction) override
    {

        int32 amount = 0;
        plr->PlayerTalkClass->ClearMenus();
        WorldSession* session = plr->GetSession();
        std::ostringstream message;
        
        float ownedratio[5];
        int worth[5];
        for (int iter = 1; iter < 6; iter++)
        {
            QueryResult* result = WorldDatabase.PQuery("SELECT `OwnedRatio`,`Worth`   FROM `stocks` WHERE iD = %u", iter);
            if (!result)
                return false;
            Field* field = result->Fetch();
            ownedratio[iter - 1] = field[0].GetFloat();
            worth[iter - 1] = field[1].GetInt32();
        }
        switch (uiAction)
        {
        case 1: //REMAINING SHARES
            message << "剩余股份(金币)$B太阳之井 : " << worth[0] * (1 - ownedratio[0]) << "$B黑暗神庙 : " << worth[1] * (1 - ownedratio[1]) << "$B盘牙水库 : " << worth[2] * (1 - ownedratio[2]) << "$B海加尔山 : " << worth[3] * (1 - ownedratio[3]) << "$B风暴要塞 : " << worth[4] * (1 - ownedratio[4]);
            creature->Whisper(message.str().c_str(), plr->GetGUID());
            plr->PlayerTalkClass->CloseGossip();
            OnGossipHello(plr, creature);
            break;
        case 2: //CURRENT EXCHANGE RATES
            message << "当前汇率(金币)$B太阳之井 : " << worth[0] << "$B黑暗神庙 : " << worth[1] << "$B盘牙水库 : " << worth[2] << "$B海加尔山 : " << worth[3] << "$B风暴要塞 : " << worth[4];
            creature->Whisper(message.str().c_str(), plr->GetGUID());
            plr->PlayerTalkClass->CloseGossip();
            OnGossipHello(plr, creature);
            break;
        case 3: //MANAGE PORTFOLIO MENU (5-10)
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Helmet_30:30:30:-18:0|t太阳之井", GOSSIP_SENDER_MAIN, 5);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Helmet_66:30:30:-18:0|t黑暗神庙", GOSSIP_SENDER_MAIN, 6);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Bomb_03:30:30:-18:0|t盘牙水库", GOSSIP_SENDER_MAIN, 7);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Chest_Cloth_56:30:30:-18:0|t海加尔山", GOSSIP_SENDER_MAIN, 8);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_FireDancer_01:30:30:-18:0|t风暴要塞", GOSSIP_SENDER_MAIN, 9);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t返回", GOSSIP_SENDER_MAIN, 10);
            plr->SEND_GOSSIP_MENU(50002, creature->GetGUID());
            break;
        case 4: //CLOSE GOSSIP
            plr->PlayerTalkClass->CloseGossip();
            break;
        case 5: //MANAGE RAVENHOLDT STOCK
            stock = 1;
            plr->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Letter_03:30:30:-18:0|t购买股票", stock, 0, "你想投资多少金币？", 0, true);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Coin_16:30:30:-18:0|t出售股票", stock, 11);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t返回...", GOSSIP_SENDER_MAIN, 3);
            plr->SEND_GOSSIP_MENU(50002, creature->GetGUID());
            return true;
            break;
        case 6: //MANAGE BLOODSAIL STOCK
            stock = 2;
            plr->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Letter_03:30:30:-18:0|t购买股票", stock, 0, "你想投资多少金币？", 0, true);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Coin_16:30:30:-18:0|t出售股票", stock, 11);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t返回...", GOSSIP_SENDER_MAIN, 3);
            plr->SEND_GOSSIP_MENU(50002, creature->GetGUID());
            break;
        case 7: //MANAGE GOBLIN STOCK
            stock = 3;
            plr->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Letter_03:30:30:-18:0|t购买股票", stock, 0, "你想投资多少金币？", 0, true);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Coin_16:30:30:-18:0|t出售股票", stock, 11);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t返回...", GOSSIP_SENDER_MAIN, 3);
            plr->SEND_GOSSIP_MENU(50002, creature->GetGUID());
            break;
        case 8: //MANAGE SILKTRADERS STOCK
            stock = 4;
            plr->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Letter_03:30:30:-18:0|t购买股票", stock, 0, "你想投资多少金币？", 0, true);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Coin_16:30:30:-18:0|t出售股票", stock, 11);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t返回...", GOSSIP_SENDER_MAIN, 3);
            plr->SEND_GOSSIP_MENU(50002, creature->GetGUID());
            break;
        case 9: //MANAGE ELDOAR'NORORE STOCK
            stock = 5;
            plr->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Letter_03:30:30:-18:0|t购买股票", stock, 0, "你想投资多少金币？", 0, true);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Coin_16:30:30:-18:0|t出售股票", stock, 11);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t返回...", GOSSIP_SENDER_MAIN, 3);
            plr->SEND_GOSSIP_MENU(50002, creature->GetGUID());
            return true;
            break;
        case 11: //TRANSACTION = SELL
            if (!SellStock(plr, stock, creature))
                plr->GetSession()->SendNotification("出售股票时的错误…请与管理员联系。");
            else
            {

            }
            plr->PlayerTalkClass->CloseGossip();
            OnGossipHello(plr, creature);

            break;
        case 12: //CREATE PLAYER LICENCE ( = ROW IN CHARACTER_STOCKDATA )
            if (plr->GetMoney() >= 40 * GOLDTOCOPPER)
            {
                if (CreateLicence(plr))
                {
                    amount = 40 * GOLDTOCOPPER;
                    plr->ModifyMoney(-amount);
                    message << "您的证券资格证已成功获得，" << plr->GetName() << "!";
                    creature->Say(message.str().c_str(), LANG_UNIVERSAL, plr->GetGUID());
                }
                else
                    plr->GetSession()->SendNotification("注册证券资格证时出错。请与管理员联系。");
            }
            else
            {
                message << "你没有400金币， " << plr->GetName() << "...";
                creature->Say(message.str().c_str(), LANG_UNIVERSAL, plr->GetGUID());
            }
            plr->PlayerTalkClass->CloseGossip();
            OnGossipHello(plr, creature);
            break;
        default:
            OnGossipHello(plr, creature);
            break;
        }
        return true;
    }
    bool OnGossipSelectCode(Player* plr, Creature* creature, uint32 stock, uint32 uiAction, const char* code) override
    {
        plr->PlayerTalkClass->ClearMenus();
        std::ostringstream message;
        int32 amount = 0;
        std::string investment = code;
        static const char* allowedcharacters = "1234567890";
        if (!investment.length() || investment.find_first_not_of(allowedcharacters) != std::string::npos) //If investment was specified incorrectly
        {
            plr->GetSession()->SendNotification("您的投资失败将用数字来宣布。(0123456789)");
        }
        else //If investment was specified properly
        {
            //Statements
            uint32 investmentI = uint32(atol(code));
            if (investmentI*GOLDTOCOPPER <= plr->GetMoney())
            {
                QueryResult* result1 = WorldDatabase.PQuery("SELECT `OwnedRatio`,`Worth`  FROM `stocks` WHERE iD = %u", stock);
                if (!result1)
                    return false;
                Field* field = result1->Fetch();
                float ownedratio = field[0].GetFloat();
                int worth = field[1].GetInt32();
                QueryResult* result2 = CharacterDatabase.PQuery("SELECT `%u` FROM `character_stockdata` WHERE iD = %u", stock, plr->GetGUIDLow());
                if (!result2)
                    return false;
                field = result2->Fetch();
                float initialratio = field[0].GetFloat();
                float ratio = static_cast<float>(investmentI) / worth;
                if (ratio <= (1 - ownedratio)) //If enough stocks left
                {
                    int amount = investmentI * GOLDTOCOPPER;
                    plr->ModifyMoney(-amount); // substract it from player money
                    CharacterDatabase.PExecute("UPDATE `character_stockdata` SET `%u` = %f WHERE `iD` = %u", stock, initialratio + ratio, plr->GetGUIDLow());
                    WorldDatabase.PExecute("UPDATE `stocks` SET `OwnedRatio` = %f WHERE `iD` = %u", ownedratio + ratio, stock);
                }
                else
                {
                    plr->GetSession()->SendNotification("没有那么多的股份供你购买，在尝试投资之前检查剩余的股份。");
                }
            }
            else
            {
                message << "你没有那么多钱，" << plr->GetName() << ".";
                creature->Say(message.str().c_str(), LANG_UNIVERSAL, plr->GetGUID());
            }
        }
        plr->PlayerTalkClass->CloseGossip();
        OnGossipHello(plr, creature);
        return true;
    }

private:
    bool SendMoneyByMail(Player* player, uint64 money, int32 stock)
    {
        MailSender sender(MAIL_CREATURE, 50038, MAIL_STATIONERY_NORMAL);
        std::string stockName;
        switch (stock)
        {
        case 1:
            stockName = "太阳之井";
            break;
        case 2:
            stockName = "黑暗神庙";
            break;
        case 3:
            stockName = "盘牙水库";
            break;
        case 4:
            stockName = "海加尔山";
            break;
        case 5:
            stockName = "风暴要塞";
            break;
        default:
            return false;
        }
        std::ostringstream text;
        text << "您已要求交易大厅出售您的 " << stockName << " 股票。这产生了一些 " << money / GOLDTOCOPPER << " 我已经从中扣除的金币 " << money / 10 / GOLDTOCOPPER << " 考虑到各种销售和会计成本。随信附上的是 " << 9 * money / 10 / GOLDTOCOPPER << " 给你的金币。";
        MailDraft draft("Share Sale", text.str().c_str());
        draft.AddMoney(money * 9 / 10);
        //SQLTransaction letter = CharacterDatabase.BeginTransaction();
        draft.SendMailTo(player, sender);
        //draft.SendMailTo(letter, MailReceiver(NULL, player.GetCounter()), sender);
        //CharacterDatabase.CommitTransaction(letter);
        return true;
    }
    bool SellStock(Player* plr, int32 stock, Creature* creature) /*SELL ALL THE SHARES OF THE SPECIFIED STOCK */
    {
        QueryResult* result2 = WorldDatabase.PQuery("SELECT `OwnedRatio`,`Worth`   FROM `stocks` WHERE iD = %u", stock);
        if (!result2)
            return false;
        QueryResult* result1 = CharacterDatabase.PQuery("SELECT `1`, `2`, `3`, `4`, `5`  FROM `character_stockdata` WHERE iD = %u", plr->GetGUIDLow());
        if (!result1)
            return false;
        Field* field = result1->Fetch();
        float ratio = field[stock - 1].GetFloat();
        if (ratio == 0)
        {
            plr->GetSession()->SendNotification("你没有这支股票的份额...");
        }
        else //ONLY IF PLAYER OWNS SOMETHING
        {
            Field* field = result2->Fetch();
            float newownedratio = field[0].GetFloat() - ratio;
            int worth = field[1].GetInt32();
            if (!SendMoneyByMail(plr, worth*GOLDTOCOPPER*ratio, stock))
                return false;
            std::ostringstream text;
            text << "我将作出售的担保，" << plr->GetName() << ", 您将通过邮件收到您的款项。";
            creature->Say(text.str().c_str(),LANG_UNIVERSAL, plr->GetGUID());
            CharacterDatabase.PExecute("UPDATE `character_stockdata` SET `%u` = 0 WHERE `iD` = %u", stock, plr->GetGUIDLow());
            WorldDatabase.PExecute("UPDATE `stocks` SET `OwnedRatio` = %f WHERE `iD` = %u", newownedratio, stock);
        }
        return true;
    }
    bool CreateLicence(Player* plr)
    {
        CharacterDatabase.BeginTransaction();
        CharacterDatabase.PExecute("REPLACE INTO `character_stockdata` (`iD`, `1`, `2`, `3`, `4`, `5`) VALUES (%u, 0, 0, 0, 0, 0)", plr->GetGUIDLow());
        CharacterDatabase.CommitTransaction();
        return true;
    }
    bool StockUpdate(int timeMult)
    {
        int32 stockWorth[STOCKNUMBER], newStockWorth[STOCKNUMBER];
        int32 stockScale[STOCKNUMBER];
        float stockOwnedRatio[STOCKNUMBER];
        srand(time(NULL));
        // Load stock values from DB
        for (int itr = 0; itr < STOCKNUMBER; itr++)
        {
            QueryResult* result = WorldDatabase.PQuery("SELECT `Scale`,`OwnedRatio`,`Worth`  FROM `stocks` WHERE iD = %u", itr + 1);
            if (!result)
                return false;
            Field* field = result->Fetch();
            stockScale[itr] = field[0].GetInt32();
            stockOwnedRatio[itr] = field[1].GetFloat();
            stockWorth[itr] = field[2].GetInt32();
        }
        // Dicethrows and Upload values to DB
        for (int itr = 0; itr < STOCKNUMBER; itr++)
        {
            char sign;
            if (stockWorth[itr] + stockScale[itr] * timeMult > 200000) //Don't go over 200.000 gold coins
                sign = -1;
            else if (stockWorth[itr] - stockScale[itr] * timeMult < 0) //Nor below 0
                sign = 1;
            else
            {
                int dice = (rand() % 101) + floor(10 * stockOwnedRatio[itr]) - 55;
                sign = dice / abs(dice);
            }
            newStockWorth[itr] = stockWorth[itr] + sign * stockScale[itr] * timeMult;
            WorldDatabase.PExecute("UPDATE `stocks` SET `worth` = %u WHERE `iD` = %u", newStockWorth[itr], itr + 1);
        }
        return true;
    }
};

void AddSC_StockExchange()
{
    new CS_StockExchange();
}