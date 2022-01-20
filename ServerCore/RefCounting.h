#pragma once

/*-------------
*RefCountable
--------------*/

class RefCountable
{
public:
	RefCountable() : _refCount(1) {}
	virtual ~RefCountable() {}
	//상위 class의 소멸자는 virtual로 선언되어야함

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

	//복사
	TsharedPtr(const TsharedPtr& rhs) { Set(rhs._ptr); }
	//이동
	TsharedPtr(const TsharedPtr&& rhs)
	{
		_ptr = rhs._ptr; rhs._ptr = nullptr;
	}
	//상속 관계 복사
	template <typename U>
	TsharedPtr(const TsharedPtr<U>& rhs) { Set(static_cast<T*>(rhs._ptr)); }

	//복사연산자
	TsharedPtr& operator =(const TsharedPtr& rhs)
	{
		if (_ptr != rhs._ptr)
		{
			Release();
			Set(rhs._ptr);
		}
		return *this;
	}

	//이동 연산자
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
	T* _ptr = nullptr;// reference counting이 지원됨
};