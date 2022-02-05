#pragma once

/*-----------
	IocpObject
------------*/

class IocpObject : public enable_shared_from_this<IocpObject>	
{
public:
	
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;
	//일감의 종류를 iocpEvent를 통해서 받아서 확인한다.


};


/*-----------
	IocpCore
------------*/
class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE GetHandle() { return _iocpHandle; }
	bool Register(IocpObjectRef iocpObject); //등록하는 함수
	bool Dispatch(uint32 timeoutMs = INFINITE); //일감이 있나없나 확인하는 함수


private:
	HANDLE _iocpHandle;
};




