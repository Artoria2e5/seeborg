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


#ifndef __SEEUTIL_H__
#define __SEEUTIL_H__

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

inline int randInt(int min, int max) {
  return (int)(min+((max-min)*(float)rand()/((float)RAND_MAX)));
}

inline float randFloat(float min, float max) {
#if 0
  float retval = (float)rand()/(float)RAND_MAX;
  retval *= (max - min);
  retval += min;
  return retval;
#else
  return (min+((max-min)*(float)rand()/((float)RAND_MAX)));
#endif
}


int tokenizeString(string &str, vector<string> &tokens);
int splitString(string &str, vector<string> &tokens, const char* needle);
string joinString(vector<string> &tokens);
int lowerString(string &str);
int lowerString(char* str);
int trimString(string &str);

int fReadStringLine(FILE* f, string &);

// Arguments from string
void CMA_TokenizeString (const char* str);
int CMA_Argc(void);
const char* CMA_Argv(unsigned int c);

#endif
