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


#include <time.h>
#include <locale.h>

#include "seeborg.h"
#include "seeutil.h"

seeborg_t gSeeBorg;

void PrintReply(string text) {
  printf ("<Seeborg> %s\n", text.c_str());
}


int main (int argc, char* argv[]) {
  setlocale(LC_ALL, "");
  printf ("SeeBorg v" SEEBORGVERSIONSTRING ", copyright (C) 2003 Eugene Bujak.\n\n");
  string body;
  srand(time(NULL));

  printf ("Loading dictionary...\n");
  gSeeBorg.LoadSettings();

#ifndef PROFILE
  printf ("\nSeeBorg offline chat, learning disabled. Press CTRL-C to quit.\n\n");

  while (1) {
	printf ("> ");
	fReadStringLine(stdin, body);
	if (body == "!quit") {
	  break;
	  //string trigreply = gSeeBorg.ParseCommands(body);
	  //if (trigreply != "") PrintReply (trigreply);
	  //continue;
	}
	
	string seeout = gSeeBorg.Reply(body);
	PrintReply (seeout);
  }
#endif

  gSeeBorg.SaveSettings();
  return 0;
}
