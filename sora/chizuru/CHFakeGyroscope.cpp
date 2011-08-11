// Ŭnicode please
#include "../SoraStdAfx.h"

#include "matsu/MTQuaternion.h"
#include "matsu/MTVector.h"

#include "chizuru/CHInputEvent.h"
#include "chizuru/CHFakeGyroscope.h"

using namespace matsu;
using namespace std;

namespace chizuru
{
	matsu::vec3 FakeGyroscope::calc(const matsu::vec3 &accelVec, const matsu::vec3 &headingVec) const
	{
		vec3 accel = accelVec;
		vec3 heading = headingVec;
		accel.normalized();
		heading.normalized();
		
		//http://www.icondb.com/users/tiff/

		const vec3 &north = headingVec;
		const vec3 &gravity = accelVec;
		const vec3 deviceUp = vec3(0, 1, 0);
		
		vec3 rotationAxis = deviceUp.cross(gravity);
		float rotationAngle = acos(deviceUp.dot(gravity));
		quat localToGravity = quat::createFromAxisAngle(rotationAxis, rotationAngle);
		
		vec3 rightAngleHeading = localToGravity.inverse() * north;
		rightAngleHeading.setY(0);
		rightAngleHeading = localToGravity * rightAngleHeading; 
		
		vec3 worldAxisY = -gravity;
		vec3 worldAxisZ = -rightAngleHeading.normalize();
		vec3 worldAxisX = worldAxisY.cross(worldAxisZ);
		worldAxisX.normalized();
		
		//roll, yaw, pitch뽑기 
		//http://www.codeguru.com/forum/archive/index.php/t-329530.html
		float xx = worldAxisX.getX();
		float xy = worldAxisY.getX();
		float xz = worldAxisZ.getX();
		float yz = worldAxisZ.getY();
		float zz = worldAxisZ.getZ();
		
		float roll = atan2(xy, xx);
		float pitch = -asin(xz);
		float yaw = atan2(yz, zz);

		//원인을 모르겠지만 yaw를 반대로 해야 원래 자이로와 값의 방향이 일치한다 
		yaw = -yaw;
		
		return matsu::vec3(roll, pitch, yaw);
	}
}