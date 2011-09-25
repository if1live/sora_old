// Ŭnicode please
#pragma once

#include "../SoraStdAfx.h"
#include "IRComponent.h"

namespace irina
{
	class EntityManager;

	typedef matsu::u32 EntityId;

	class Entity {
	public:
		typedef std::tr1::unordered_map<ComponentType, Component*> ComponentDict;

		Entity();
		~Entity(); 

		template<typename T>
			void addComponent(T *comp);
		template<typename T>
			void removeComponent();
		template<typename T>
			bool hasComponent() const;
		template<typename T>
			T* getComponent() const;

		EntityId getId() const;
		
	private:
		ComponentDict compDict_;
		const EntityId entityId_;
	};

	class EntityManager : public sora::Singleton<EntityManager> {
	public:
		typedef std::map<EntityId, Entity*> EntityDict;

		EntityManager();

		///@entity마다 고유한 아이디를 부여한다. 이것이 있으면 더욱 쉽게 entity를 관리할수
		///있을것이라고 생각된다. 아니면 말고. 아래의 메소드로 다음 entity가 받을 id를 알수있다
		EntityId getNextEntityId();
		
		Entity *get(EntityId entityId) const;
		
		void add(Entity* e);
		void remove(Entity *e);
	private:
		EntityId nextEntityId_;
		
		///@brief 생성된 entity가 저장되는 객체
		EntityDict dict_;
	};
}

#include "IREntity.hpp"
