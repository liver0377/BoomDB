#pragma once

#include <vector>
// 基本数据类型
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

// page相关
typedef uint64 pgid;
typedef std::vector<pgid> pgids;

// TODO : 实现bucket
// bucket相关
typedef int bucket; // 待实现

// TODO : 实现事务
// 事务相关
typedef int txid; // 待实现