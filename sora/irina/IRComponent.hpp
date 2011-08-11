#pragma once

namespace irina
{
	template<typename T>
	void Component::initComponent()
	{
		if((T::getComponentFlag() & UseComponentMgr) == UseComponentMgr)
		{
			ComponentManager<T>::getInstance().add(static_cast<T*>(this));
		}
	}
	template<typename T>
	void Component::cleanupComponent()
	{
		if((T::getComponentFlag() & UseComponentMgr) == UseComponentMgr)
		{
			ComponentManager<T>::getInstance().remove(static_cast<T*>(this));				
		}
	}

	template<typename T>
	void ComponentManager<T>::add(T *comp)
	{
		getComponentList().insert(comp);
	}

	template<typename T>
	void ComponentManager<T>::remove(T *comp)
	{
		typename ComponentList::iterator it = getComponentList().find(comp);
		getComponentList().erase(it);
	}

	template<typename T>
	m::u32 ComponentManager<T>::size()
	{
		return ComponentManager<T>::getInstance().getComponentList().size();
	}

	template<typename T>
	void ComponentManager<T>::updateAll(m::f32 dt)
	{ 
		BOOST_FOREACH(T* comp, ComponentManager<T>::getInstance().getComponentList())
		{
			comp->update(dt);
		}
	}
	template<typename T>
	template<typename _Thandler>
	void ComponentManager<T>::doAll(_Thandler &handler)
	{
		BOOST_FOREACH(T* comp, ComponentManager<T>::getInstance().getComponentList())
		{
			handler(comp);
		}
	}
}
 
