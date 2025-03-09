#include "SpaceSys.h"

int SpaceSys::blockCnt;// 盘块数
int SpaceSys::blockSize;// 盘块大小
queue<int> SpaceSys::blockQ;// 空闲盘块链