// Ŭnicode please
#include "../SoraStdAfx.h"
#include "IREntity.h"
#include "sora/SRTemplateLib.h"

using namespace sora;

namespace irina
{
	Entity::Entity()
		: entityId_(EntityManager::getInstance().getNextEntityId())
	{
		EntityManager::getInstance().add(this);
	}
	Entity::~Entity() 
	{
		destroyDict(compDict_);
		EntityManager::getInstance().remove(this);
	}

	EntityId Entity::getId() const
	{
		return entityId_;
	}


	EntityManager::EntityManager()
		: nextEntityId_(1)
	{
	}
	EntityId EntityManager::getNextEntityId()
	{
		EntityId next = nextEntityId_;
		nextEntityId_++;
		return next;
	}
	Entity* EntityManager::get(EntityId entityId) const
	{
		EntityDict::const_iterator it = dict_.find(entityId);
		if(it == dict_.end())
		{
			return NULL;
		}
		//else...
		return it->second;
	}
	void EntityManager::add(Entity *e)
	{
		dict_[e->getId()] = e;
	}
	void EntityManager::remove(Entity *e)
	{
		//메모리 해제의 책임은 각각의 객체가 알아서 적절히 수행한다
		EntityDict::iterator it = dict_.find(e->getId());
		dict_.erase(it);
	}
}
