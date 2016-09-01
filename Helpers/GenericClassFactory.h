# pragma once

template <typename BaseClass, typename DerivedClass>
inline std::shared_ptr<BaseClass> GenericContructorFunc()
{
	return std::static_pointer_cast<BaseClass>(std::make_shared<DerivedClass>());
}

template <typename BaseClass>
class GenericClassFactory
{
private:
	typedef std::shared_ptr<BaseClass>(*GenericContructor)();
	std::unordered_map<std::string, GenericContructor> m_ConstructorsMap;
public:

	template <typename DerivedClass>
	void Register(const std::string &name)
	{
		m_ConstructorsMap.emplace(name, GenericContructorFunc<BaseClass, DerivedClass>);
	}

	std::shared_ptr<BaseClass> Construct(const std::string &name)
	{
		auto constructor = m_ConstructorsMap.find(name);
		if (constructor == m_ConstructorsMap.end())
		{
			EIGHT_ASSERT(false, "You are trying to contruct object in GenericClassFactory, but object which you want to create is not registered in this class");
			return std::shared_ptr<BaseClass>();
		}
		return constructor->second();
	}
};