#pragma once
#include <irina/IRComponent.h>

using namespace irina;

class Test1Component : public Component {
	COMPONENT_ATTRIBUTE("test1", 0);
};

class Test2Component : public Component {
	COMPONENT_ATTRIBUTE("test2", UseComponentMgr);
public:
	Test2Component() { initComponent<Test2Component>();	}
	virtual ~Test2Component() {	cleanupComponent<Test2Component>();	}	 
};
