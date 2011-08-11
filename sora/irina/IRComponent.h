// Ŭnicode please
#pragma once

#include "../SoraStdAfx.h"

#include "matsu/MTType.h"
#include "matsu/MTSingleton.h"

namespace irina
{
	enum {
		UseComponentMgr = 0x01,
	};

	typedef std::string ComponentType;

	///@brief 컴포넌트의 속성 관련 정보를 찍어내는 매크로이다. 컴포넌트를 정의후에 반드시
	///이 매크로를 사용해야된다
#define COMPONENT_ATTRIBUTE(ComponentTypename, ComponentFlag) \
	public:													  \
		static const ComponentType &getComponentType()		  \
	{														  \
		static ComponentType name(ComponentTypename);		  \
		return name;										  \
	}														  \
	const ComponentType &getType()							  \
	{														  \
		return getComponentType();							  \
	}														  \
	static matsu::u32 getComponentFlag()					  \
	{														  \
		return ComponentFlag;								  \
	}														  
	
	class Component {
	public:
		Component();
		virtual ~Component();
		virtual const ComponentType &getType() = 0;

		virtual void update(m::f32) { }

	protected:
		///@brief component flag를 통해서 컴포넌트에 특수속성을 줄 경우, 
		///관련 연산을 알아서 수행하기 위해서 사용되는 템플릿 메소드이다. 생성자에서
		///이것을 실행시켜줘야 적절히 초기화된다
		///@param T 해당 클래스 자체. Component를 상속받는 클래스 이름
		template<typename T>
			void initComponent();
		///@brief component flag에 의해서 설정된 특수속성을 클래스 해제할때
		///정리해주는 메소드이다. 소멸자에서 이것을 싱행시켜줘야 적절히 초기화된다
		///@param T 해당 클래스 자체. Component를 상속받는 클래스 이름
		template<typename T>
			void cleanupComponent();
	};

	class NullComponent : public Component, public matsu::Singleton<NullComponent> {
		COMPONENT_ATTRIBUTE("null", 0);
	public:
	};



	template<typename T>
		class ComponentManager : public matsu::Singleton< ComponentManager<T> > {
	public:
		typedef std::set<T*> ComponentList;

		void add(T *comp);
		void remove(T *comp);
		static m::u32 size();
		ComponentList &getComponentList() { static ComponentList l; return l; }

		///@brief 관리하는 컴포넌트에 대해서 update를 호출
		static void updateAll(m::f32 dt);

		///@brief 같은 타입의 모든 컴포넌트에 대해서 특정 계산을 수행해야되는 일이 존재한다.
		///(예를 들면 중력컴포넌트는 매 프레임마다 모든 컴포넌트에 대해서 낙하계산을 해야한다)
		///이것을 각각의 컴포넌트에서 따로 구현하는 방법도 있지만 그것은 인터페이스의 통일성을
		///박살낼수 있기때문에 컴포넌트 매니저의 doAll이라는 메소드에 아래의 functor를 넘겨주면
		///적절히 수행한다
		template<typename _Thandler>
		static void doAll(_Thandler &handler);
	};
}

#include "IRComponent.hpp"

