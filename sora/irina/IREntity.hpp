#pragma once

namespace irina
{

	template<typename T>
	void Entity::addComponent(T *comp)
	{
		const ComponentType &compType = comp->getComponentType();
		//같은 타입이 이미 존재하면 존재하는거 삭제
		compDict_[compType] = comp;
	}
	template<typename T>
	void Entity::removeComponent()
	{
		const ComponentType &compType = T::getComponentType();
		ComponentDict::iterator it = compDict_.find(compType);
		//free memory
		delete(it->second);
		compDict_.erase(it);
	}
		
	template<typename T>
	bool Entity::hasComponent() const
	{
		const ComponentType &compType = T::getComponentType();
		ComponentDict::const_iterator it = compDict_.find(compType);
		return (it != compDict_.end());
	}
		
	template<typename T>
	T* Entity::getComponent() const
	{
		const ComponentType &compType = T::getComponentType();
		ComponentDict::const_iterator it = compDict_.find(compType);
		if(it == compDict_.end())
		{
			return NULL;
		}
		//else...
		return static_cast<T*>(it->second);
	}

}
