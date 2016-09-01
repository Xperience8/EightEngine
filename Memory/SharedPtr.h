#pragma once

#pragma warning(push)
#pragma warning(disable : 4521)
#pragma warning(disable : 4522)

template <typename _Type, typename _Allocator>
class SharedPtr
{
private:
	_declspec(align(16))
	struct ReferenceCounterHeader
	{
		UINT ReferenceCount;
	};

	_Type *m_pObject;

	void AddRef()
	{
		ReferenceCounterHeader *pAllocMem = reinterpret_cast<ReferenceCounterHeader*>(
			reinterpret_cast<BYTE*>(m_pObject)-sizeof(ReferenceCounterHeader));
		++pAllocMem->ReferenceCount;
	}
	void DecRef()
	{
		ReferenceCounterHeader *pAllocMem = reinterpret_cast<ReferenceCounterHeader*>(
			reinterpret_cast<BYTE*>(m_pObject)-sizeof(ReferenceCounterHeader));
		--pAllocMem->ReferenceCount;

		if (pAllocMem->ReferenceCount < 1)
		{
			m_pObject->~_Type();
			_Allocator::Dealloc(pAllocMem);
			m_pObject = nullptr;
		}
	}
public:
	// Empty pointer is constructed
	SharedPtr(nullptr_t) : m_pObject(nullptr) { }

	template <typename... _Parameters>
	explicit SharedPtr(_Parameters&&... _Args)
	{
		ReferenceCounterHeader *pAllocMem = reinterpret_cast<ReferenceCounterHeader*>(
			_Allocator::Alloc(sizeof(_Type) + sizeof(ReferenceCounterHeader)));
		pAllocMem->ReferenceCount = 1;

		m_pObject = reinterpret_cast<_Type*>(reinterpret_cast<BYTE*>(pAllocMem)
			+sizeof(ReferenceCounterHeader));

		::new (m_pObject)_Type(std::forward<_Parameters>(_Args)...);
	}

	SharedPtr(SharedPtr<_Type, _Allocator> &other) : m_pObject(other.m_pObject)
	{
		AddRef();
	}
	SharedPtr(const SharedPtr<_Type, _Allocator> &other) : m_pObject(other.m_pObject)
	{
		AddRef();
	}

	template <typename _DerivedClass>
	SharedPtr(SharedPtr<_DerivedClass, _Allocator> &other) 
		: m_pObject(static_cast<_Type*>(other.Get()))
	{
		AddRef();
	}
	template <typename _DerivedClass>
	SharedPtr(const SharedPtr<_DerivedClass, _Allocator> &other) 
		: m_pObject(static_cast<_Type*>(other.Get()))
	{
		AddRef();
	}
	SharedPtr &operator=(SharedPtr<_Type, _Allocator> &other)
	{
		if (m_pObject)
		{
			DecRef(m_pObject);
		}
		m_pObject = other.m_pObject;
		AddRef();

		return *this;
	}
	SharedPtr &operator=(const SharedPtr<_Type, _Allocator> &other)
	{
		if (m_pObject)
		{
			DecRef(m_pObject);
		}
		m_pObject = other.m_pObject;
		AddRef();

		return *this;
	}
	template <typename _DerivedClass>
	SharedPtr &operator=(SharedPtr<_DerivedClass, _Allocator> &other)
	{
		if (m_pObject)
		{
			DecRef(m_pObject);
		}
		m_pObject = static_cast<_Type*>(other.Get());
		AddRef();

		return *this;
	}
	template <typename _DerivedClass>
	SharedPtr &operator=(const SharedPtr<_DerivedClass, _Allocator> &other) 
	{
		if (m_pObject)
		{
			DecRef(m_pObject);
		}
		m_pObject = static_cast<_Type*>(other.Get());
		AddRef();

		return *this;
	}

	~SharedPtr()
	{
		Reset();
	}

	void Reset()
	{
		if (m_pObject)
		{
			DecRef();
			m_pObject = nullptr;
		}
	}

	_Type *Get() { return m_pObject; }
	_Type *Get() const { return m_pObject; }

	_Type *operator->() { return m_pObject; }
	_Type *operator->() const { return m_pObject; }

	_Type &operator*() { return *m_pObject; }
	const _Type &operator*() const { return *m_pObject; }

	explicit operator bool() const
	{
		return m_pObject != nullptr;
	}
};

template <typename _DerivedType, typename _BaseType, typename _Allocator>
SharedPtr<_DerivedType, _Allocator> StaticPointerCast(const SharedPtr<_BaseType, _Allocator> &ptr)
{
	return SharedPtr<_DerivedType, _Allocator>(ptr);
}


#pragma warning(pop)