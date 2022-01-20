#pragma once
 
#include"Types.h"
#include"Allocator.h"
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

using namespace std;

template<typename Type>
using Vector = vector<Type, StlAllocator<Type>>;

template<typename Type>
using List = vector<Type, StlAllocator<Type>>;

template<typename Key, typename Type, typename Pred = less<Key>>
using Map = map<Type, StlAllocator<Type>>;

template<typename Type>
using Vector = vector<Type, StlAllocator<Type>>;

template<typename Type>
using Vector = vector<Type, StlAllocator<Type>>;

