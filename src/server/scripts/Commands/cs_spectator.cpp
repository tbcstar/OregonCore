#include "ScriptMgr.h"
#include "Chat.h"
#include "ChannelMgr.h"
#include "Language.h"
#include "Player.h"

class spectator_commandscript : public CommandScript
{
public:
    spectator_commandscript() : CommandScript("spectator_commandscript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> commandTable =
        {
            { "spstop",         SEC_PLAYER,         true,  &HandleSpectatorStopCommand,        "" },

        };
        return commandTable;
    }

static bool HandleSpectatorStopCommand(ChatHandler* handler, const char* args)
{
	Player *plr = handler->GetSession()->GetPlayer();

	if (plr->isSpectator())
	{
		plr->TeleportTo(plr->GetBattlegroundEntryPoint());
		plr->SetSpectator(false);
	}
	return true;
}
};

void AddSC_spectator_commandscript()
{
    new spectator_commandscript();
}