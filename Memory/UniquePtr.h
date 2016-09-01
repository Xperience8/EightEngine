#pragma once

template <typename _Type, typename _Allocator>
class UniquePtr
{
private:
	_Type *m_pObject;

	UniquePtr(const UniquePtr<_Type, _Allocator> &other) { }
	UniquePtr &operator=(const UniquePtr &other) { }

public:
	template <typename... _Parameters>
	UniquePtr(_Parameters&&... _Args)
	{
		m_pObject = reinterpret_cast<_Type*>(_Allocator::Alloc(sizeof(_Type)));
		::new (m_pObject)_Type(std::forward<_Parameters>(_Args)...);
	}
	~UniquePtr()
	{
		Reset();
	}

	void Reset()
	{
		if (m_pObject)
		{
			m_pObject->~_Type();
			_Allocator::Dealloc(m_pObject);
		}
	}

	_Type *operator->() { return m_pObject; }
	_Type &operator*() { return *m_pObject; }
	const _Type &operator*() const { return *m_pObject; }
};