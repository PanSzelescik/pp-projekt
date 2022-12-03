#ifndef consts_cpp
#define consts_cpp

#include <string>
#include "utils.cpp"

using namespace std;

static const string FILE_NAME = "menu.txt";

static const string CONTINUE_CHAR = "X";
static const string CONTINUE_CHAR_LOWER = toLowercase(CONTINUE_CHAR);

static const string EXIT_CHAR = "Q";
static const string EXIT_CHAR_LOWER = toLowercase(EXIT_CHAR);

static const string DELETE_CHAR = "D";
static const string DELETE_CHAR_LOWER = toLowercase(DELETE_CHAR);

static const string BACK_CHAR = "P";
static const string BACK_CHAR_LOWER = toLowercase(BACK_CHAR);

static const string YES_CHAR = "T";
static const string YES_CHAR_LOWER = toLowercase(YES_CHAR);

static const string NO_CHAR = "N";
static const string NO_CHAR_LOWER = toLowercase(NO_CHAR);

#endif