#pragma once

/*-------------
*RefCountable
--------------*/

class RefCountable
{
public:
	RefCountable() : _refCount(1) {}
	virtual ~RefCountable() {}
	//���� class�� �Ҹ��ڴ� virtual�� ����Ǿ����

	int32 GetRefCount() { return _refCount; }

	int32 AddRef() { return ++_refCount; }
	int32 ReleaseRef()
	{
		int32 refCount = --_refCount;
		if (refCount == 0)
		{
			delete this;
		}
		return refCount;
	}

protected:
	atomic<int32> _refCount;

};

/*-------------
*SharedPtr
--------------*/

template<typename T>
class TsharedPtr
{
public:
	TsharedPtr() {}
	TsharedPtr(T* ptr) { Set(ptr); }

	//����
	TsharedPtr(const TsharedPtr& rhs) { Set(rhs._ptr); }
	//�̵�
	TsharedPtr(const TsharedPtr&& rhs)
	{
		_ptr = rhs._ptr; rhs._ptr = nullptr;
	}
	//��� ���� ����
	template <typename U>
	TsharedPtr(const TsharedPtr<U>& rhs) { Set(static_cast<T*>(rhs._ptr)); }

	//���翬����
	TsharedPtr& operator =(const TsharedPtr& rhs)
	{
		if (_ptr != rhs._ptr)
		{
			Release();
			Set(rhs._ptr);
		}
		return *this;
	}

	//�̵� ������
	TsharedPtr& operator=(TsharedPtr&& rhs)
	{
		Release();
		_ptr = rhs._ptr;
		rhs._ptr = nullptr;
		return *this;
	}

	bool	 operator ==(const TsharedPtr& rhs) { return rhs._ptr == _ptr; }
	bool	operator ==(T* ptr) { return ptr == _ptr; }
	bool	 operator !=(const TsharedPtr& rhs) { return rhs._ptr != _ptr; }
	bool	 operator !=(T* ptr) { return ptr != _ptr; }
	bool	 operator <(const TsharedPtr& rhs) { return _ptr < rhs._ptr; }
	T*		 operator *() { return _ptr; }
	T*		 operator ->() { return _ptr; }
	const T* operator *() const { return _ptr; }
	const T* operator ->() const { return _ptr; }
	
	

	bool IsNull() { return _ptr == nullptr; }
	


private:
	inline void Set(T* ptr)
	{
		_ptr = ptr;
		if (ptr)
		{
			ptr->AddRef();
		}
	}

	inline void Release()
	{
		if (_ptr != nullptr)
		{
			_ptr->ReleaseRef();
			_ptr = nullptr;
		}
	}

private:
	T* _ptr = nullptr;// reference counting�� ������
};