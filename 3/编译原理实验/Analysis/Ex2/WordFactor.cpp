#include "WordFactor.h"

using Pro = function<State(char, string &)>;
map<State, Pro>WordFactor::m;
set<char> WordFactor::symbol;
set<State> WordFactor::endState;
set<State> WordFactor::backState;
set<string> WordFactor::keyword;