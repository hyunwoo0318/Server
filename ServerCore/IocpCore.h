#pragma once

/*-----------
	IocpObject
------------*/

class IocpObject : public enable_shared_from_this<IocpObject>	
{
public:
	weak_ptr<IocpObject> _Wptr;
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;
	//�ϰ��� ������ iocpEvent�� ���ؼ� �޾Ƽ� Ȯ���Ѵ�.


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
	bool Register(IocpObjectRef iocpObject); //����ϴ� �Լ�
	bool Dispatch(uint32 timeoutMs = INFINITE); //�ϰ��� �ֳ����� Ȯ���ϴ� �Լ�


private:
	HANDLE _iocpHandle;
};



