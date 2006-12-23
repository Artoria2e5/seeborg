/*
    This file is part of SeeBorg.
	Copyright (C) 2003, 2006 Eugene Bujak.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

*/


#ifndef __SEEBORG_IRC_H__
#define __SEEBORG_IRC_H__

using namespace std;

// Bot commands
// ---------------------------------------------------------------------------
string CMD_Shutup_f (class SeeBorg* self, const string command);
string CMD_Wakeup_f (class SeeBorg* self, const string command);

string CMD_Replyrate_f (class SeeBorg* self, const string command);
string CMD_Replynick_f (class SeeBorg* self, const string command);
string CMD_Replyword_f (class SeeBorg* self, const string command);

string CMD_Join_f (class SeeBorg* self, const string command);
string CMD_Part_f (class SeeBorg* self, const string command);
string CMD_Quit_f (class SeeBorg* self, const string command);
string CMD_Save_f (class SeeBorg* self, const string command);
string CMD_Stealth_f(class SeeBorg* self, const string command);
string CMD_Learning_f (class SeeBorg* self, const string command);
string CMD_ircHelp_f (class SeeBorg* self, const string command);

string ircParseCommands(const string command, const char* who);

static botcommand_t ircbotcmds[] = {
  {"help", "Show this command list", CMD_ircHelp_f},
  {"shutup", "As the name says", CMD_Shutup_f},
  {"wakeup", "As the name says", CMD_Wakeup_f},
  {"join", "Join channel", CMD_Join_f},
  {"part", "Part channel", CMD_Part_f},

  {"replyrate", "Show/set reply rate", CMD_Replyrate_f},
  {"replynick", "Show/set nick reply rate", CMD_Replynick_f},
  {"replymagic", "Show/set magic word reply rate", CMD_Replyword_f},

  {"quit", "As the name implies", CMD_Quit_f},
  {"save", "Immedeately save dictionary and settings", CMD_Save_f},
  
  {"learning", "Enable/disable bot's learning ability, should be enabled", CMD_Learning_f},
  {"stealth", "CTCP version stealth", CMD_Stealth_f},

  {NULL, NULL, NULL}
};
static int numircbotcmds = sizeof(ircbotcmds) / sizeof(ircbotcmds[0]) - 1;


// Bot Settings
// ---------------------------------------------------------------------------
void LoadBotSettings();
void SaveBotSettings();


#endif
