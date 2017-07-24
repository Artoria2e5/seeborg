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


#include <ctype.h>

#include "seeutil.h"
#include <cppjieba/Jieba.hpp>

static const wchar_t *tokenizer_nullstring = L"";

// ================
// String functions

size_t splitString(IN const wstring &str, IN OUT vector<wstring> &tokens, IN const wchar_t* needle) {
  // TODO: undefined behaviour when input string contains only needles,
  //       but should return zero.
  vector<size_t> splitpos;
  size_t nstart, n;

  if (str.empty()) return 0;

  n = str.find(needle);
  for (; n != str.npos; n = str.find(needle, n)) {
	splitpos.push_back(n);
	n += wcslen(needle);
  }
  splitpos.push_back(str.npos);

  nstart = 0;
  size_t sz = splitpos.size();
  for (size_t i = 0; i < sz; i++) {
	size_t nend = splitpos[i];
	if (nend == nstart) {
	  nstart = nend+wcslen(needle);
	  continue;
	}
	if (nend == str.npos) {
	  tokens.push_back(str.substr(nstart));
	} else {
	  tokens.push_back(str.substr(nstart, nend - nstart));
	}
	nstart = nend+wcslen(needle);
  }
  return sz;
}

size_t tokenizeString(const wstring &str, vector<wstring> &tokens) {
	const char* const DICT_PATH = JIEBA_DATA_DIR "/jieba.dict.utf8";
	const char* const HMM_PATH = JIEBA_DATA_DIR "/hmm_model.utf8";
	const char* const USER_DICT_PATH = JIEBA_DATA_DIR "/user.dict.utf8";
	const char* const IDF_PATH = JIEBA_DATA_DIR "/idf.utf8";
	const char* const STOP_WORD_PATH = JIEBA_DATA_DIR "/stop_words.utf8";
	static cppjieba::Jieba jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH);

	if (str.empty()) return 0;
	char* u8str = utf8_wstringtombs(str);
	size_t added_tokens;

	vector<string> jieba_out;
	jieba.Cut(u8str, jieba_out, true);
	safe_free(u8str);

	wstring wtokoid;
	for (string tokoid : jieba_out) {
		utf8_mbstowstring(tokoid.c_str(), wtokoid);
		added_tokens += splitString(wtokoid, tokens, L" ");
	}

	return added_tokens;
}

wstring joinString(vector<wstring> &tokens, wstring separator) {
  wstring str;
  
  int sz = tokens.size();
  for (int i = 0; i < sz; i++) {
	str += tokens[i];
	if (i + 1 != sz) str += separator;
  }
  
  return str;
}


int lowerString(wstring &str) {
  wchar_t* cmess = wcsdup(str.c_str());
  lowerString (cmess);
  str = cmess;
  safe_free(cmess);
  
  return true;
}

int lowerString(wchar_t* str) {
//  wcslwr(str);
  int sz = wcslen(str);
  for (int i = 0; i < sz; i++) {
    str[i] = towlower(str[i]);
  }
  
  return true;
}


int trimString(wstring &str, bool punct) {
  // TODO: Optimize
  while (1) {
	if (str.empty()) break;
	if (iswspace(str[0])) str.erase(0, 1);
	else if (punct && iswpunct(str[0])) str.erase(0, 1);
	else if (iswcntrl(str[0])) str.erase(0, 1);
	else break;
  }
  
  while (1) {
	if (str.empty()) break;
	if (iswspace(str[str.size() - 1])) str.erase(str.size() - 1);
	else if (punct && iswpunct(str[str.size() - 1])) str.erase(str.size() - 1);
	else if (iswcntrl(str[str.size() - 1])) str.erase(str.size() - 1);
	else break;
  }
  return true;
}


// Filter message before tokenising and other parsing
// ---
int FilterMessage(wstring &message) {
  size_t n;	// MSVC doesn't like 'for' locality
  NormalizeZHMessage(message);
  for (n = message.find(L'\t'); n != message.npos; n = message.find(L'\t', n)) {
	message.replace(n, 1, 1, L' ');
  }

  for (n = message.find(L'\n'); n != message.npos; n = message.find(L'\n', n)) {
	message.erase(n, 1);
  }
  
  for (n = message.find(L'\r'); n != message.npos; n = message.find(L'\r', n)) {
	message.erase(n, 1);
  }
  
  for (n = message.find(L'\"'); n != message.npos; n = message.find(L'\"', n)) {
	message.erase(n, 1);
  }
  
  for (n = message.find(L"? "); n != message.npos; n = message.find(L"? ", n)) {
	message.replace(n, 2, L"?. ");
  }
  
  for (n = message.find(L"! "); n != message.npos; n = message.find(L"! ", n)) {
	message.replace(n, 2, L"!. ");
  }
  
  lowerString (message);
  return true;
}

// Perform normalization on Chinese input.
int NormalizeZHMessage(wstring &message) {
	size_t n;
	static opencc::SimpleConverter occ_conv("tw2s");
#define MSG_REPLACE(s,l,x)\
	for (n = message.find(s); n != message.npos; n = message.find(s, n)) { \
		message.replace(n, l, x);\
	}
	char * u8message = utf8_wstringtombs(message); 
	string conv = occ_conv.Convert(u8message);
	safe_free(u8message);
	wstring wconv;
	utf8_mbstowstring(conv.c_str(), wconv);
	message.assign(std::move(wconv));
	MSG_REPLACE(L"？", 1, L"?");
	MSG_REPLACE(L"！", 1, L"!");
	MSG_REPLACE(L"。", 1, L".");
	MSG_REPLACE(L"「", 1, L"“");
	MSG_REPLACE(L"」", 1, L"”");
	MSG_REPLACE(L"『", 1, L"‘");
	MSG_REPLACE(L"』", 1, L"’");
	MSG_REPLACE(L"“", 1, L" \"");
	MSG_REPLACE(L"”", 1, L"\" ");
	MSG_REPLACE(L"‘", 1, L"' ");
	MSG_REPLACE(L"’", 1, L" '");
#undef MSG_REPLACE
	return true;
}

// Reads string from file until we reach EOF or newline
// ---
int fReadStringLine(IN FILE *f, OUT wstring &str) {
  size_t blocksize = 65536;
  size_t blockoffset = 0;
  wchar_t *wcstr = NULL;
  char *s = NULL;
  int retval;
  
  if (f == NULL) return false;
//  str.erase(0, str.npos);
  str.clear();
//  str.resize(0);
  
  s = (char*)malloc(blocksize);
  if (s == NULL) return false; // error;
  
  s[blocksize-1] = 104; // if touched by fgets(), we may need more
  
  while (1) {
	if (fgets (s + blockoffset, blocksize - blockoffset, f) == NULL) {
	  // An error
	  if (!feof(f)) perror("error reading lines database");
	  safe_free(s);
	  return false;
	}
	
	if (s[blocksize-1] == 104) break;
	else {
	  // was touched, probably requires another take
	  blockoffset = blocksize - 1;
	  blocksize *= 2;
	  s = (char*)realloc(s, (blocksize));
	  s[blocksize-1] = 104;
	}
  }
  
  // convert from UTF8 to wchar_t
  retval = utf8_mbstowcs(NULL, s, 0);
  if (retval < 0) {
	safe_free(s);
	return false;
  }
  wcstr = (wchar_t*)malloc((retval+1)*sizeof(wchar_t));
  retval = utf8_mbstowcs(wcstr, s, retval+1);
  if (retval < 0) {
	safe_free (wcstr);
	safe_free (s);
	return false;
  }
  
  str = wcstr;
  
  safe_free(wcstr);
  safe_free(s);
  return true;
}


// ========================
// argument-style tokenizer
// ========================
tokenizer_t* tokenizer_init(void) {
  tokenizer_t* tokenizer = (tokenizer_t*)malloc(sizeof(tokenizer_t));
  if (tokenizer == NULL) return NULL;
  tokenizer->argc = 0;
  tokenizer->argv = NULL;
  
  return tokenizer;
}

// all tokens become empty if you pass NULL string into it
size_t tokenizer_tokenize(tokenizer_t* tokenizer, const wchar_t* wcstr) {
  const wchar_t* tokenstart = wcstr;
  const wchar_t* tokenend = wcstr;
  size_t len;
  
  // Get rid of old tokens, since we're going to use new ones
  for (size_t i = 0; i < tokenizer->argc; i++) {
	if (tokenizer->argv[i] == NULL) continue;
	safe_free(tokenizer->argv[i]);
	tokenizer->argv[i] = NULL;
  }
  
  tokenizer->argc = 0;
  
  if (wcstr == NULL) return 0;
  
  while (1) {
	// find start, skip space, everything else is token start
	while (*tokenstart && iswspace(*tokenstart)) tokenstart++;
	
	// if we reach end of string before new token starts, return
	if (*tokenstart == L'\0') return tokenizer->argc;
	
	tokenend = tokenstart;
	while (*tokenend && !iswspace(*tokenend)) tokenend++;
	// ignore end of string, treat as token end position
	
	len = tokenend - tokenstart;
#ifdef _DEBUG
//	fprintf (stderr, "%i\n", len);
#endif
	
	// resize/create argv if necessary
	if ((tokenizer->argc % tokenizer_sizestep) == 0) {
	  tokenizer->argv = 
		(wchar_t**)realloc(tokenizer->argv, tokenizer->argc+tokenizer_sizestep);
	}

	// allocate memory for argument
	tokenizer->argv[tokenizer->argc] = (wchar_t*)malloc((len+1)*sizeof(wchar_t));
	wchar_t* str = tokenizer->argv[tokenizer->argc];

	// fill argv with the token
	for (size_t i = 0; i < len; i++) str[i] = tokenstart[i];
	str[len] = L'\0';

	// increment argument count
	tokenizer->argc++;
	tokenstart = tokenend;
  }

  return tokenizer->argc; // just in case, this code should be unreachable
}

size_t tokenizer_argc(tokenizer_t* tokenizer) {
  if (tokenizer == NULL) return 0;
  return tokenizer->argc;
}

const wchar_t* tokenizer_argv(tokenizer_t* tokenizer, const size_t index) {
  if (tokenizer == NULL) return tokenizer_nullstring;
  if (index > tokenizer->argc) return tokenizer_nullstring;
  return tokenizer->argv[index];
}

void tokenizer_free(tokenizer_t* tokenizer) {
  if (tokenizer == NULL) return;
  if (tokenizer->argv == NULL) return;
  for (size_t i = 0; i < tokenizer->argc; i++) {
	safe_free(tokenizer->argv[i]);
  }
  safe_free (tokenizer->argv);
  safe_free (tokenizer);
}

// ==============================
// utf8/wstring handling routines
// ==============================
bool utf8_mbstowstring(const char *mbstr, wstring &wstr) {
  ssize_t retval = utf8_mbstowcs(NULL, mbstr, 0);
  if (retval == -1) return false;

  wchar_t *tempwcstr = (wchar_t*)malloc(sizeof(wchar_t)*(retval+1));
  retval = utf8_mbstowcs(tempwcstr, mbstr, retval);
  if (retval == -1) return false;

  tempwcstr[retval] = L'\0';

  wstr = tempwcstr;
  safe_free(tempwcstr);

  return true;
}

// Don't forget to safe_free() the return value
char* utf8_wstringtombs(const wstring &wstr) {
  size_t retval = utf8_wcstombs(NULL, wstr.c_str(), 0);
  char* temputf8str = (char*)malloc(retval+1);
  retval = utf8_wcstombs(temputf8str, wstr.c_str(), retval);
  temputf8str[retval] = 0;

  return temputf8str;
}
