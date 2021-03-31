#pragma once

//每边的长度
#ifndef NumberofColorBlocks
#define NumberofColorBlocks 51
#endif // !NumberofColorBlocks

//数据量
#ifndef DataContain
#define DataContain (NumberofColorBlocks * NumberofColorBlocks - 192 - 30 - (NumberofColorBlocks - 16) * 2) //可以储存的数据量，217为定位点占用，30为版本信息
#endif // !DataContain

#ifndef DEBUG
//#define DEBUG
#endif