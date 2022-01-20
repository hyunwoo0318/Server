#pragma once
#include<stack>
#include<map>
#include<vector>

/*-------------
	DeadLockProfiler
--------------*/
class DeadLockProfiler
{
public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();

private:
	void Dfs(int32 here);

	unordered_map<const char*, int32> _nameToId;
	unordered_map<int32, const char*> _IdToName;
	stack<int32>					_lockStack;
	map<int32, set<int32>>			_lockHistory;

	Mutex _lock;

private:
	vector<int32> _discoveredOrder; // ��尡 �߰ߵȼ����� ����ϴ� �迭
	int32 _discoveredCount = 0; //��尡 �߰ߵ� ����
	vector<bool>	_finished; //dfs�� ���� �Ǿ����� ����
	vector<int32>	_parent;
};
