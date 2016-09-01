#pragma once

#include "EightEngine\Actor\Components\DynamicPhysicsComponent.h"
#include "EightEngine\Xml\XmlNode.h"

class SpherePhysicsComponent : public DynamicPhysicsComponent
{
public:
	SpherePhysicsComponent() { }
	~SpherePhysicsComponent() { }

	// Because physx can not stop sphere, only slow it on minimum, we have to do it yourselfs
	// This function check whether this spheres moving is under 0.25f(in x and z direction)
	// and if it is, slow it linearly to minimum speed from which you can not recognised
	// moving and stop it
	void LinearStopSphere()
	{
		auto velocity = GetVelocity();
		float speed = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

		if (speed < 0.25f && speed > 0.f)
		{
			if (speed < 0.09f)
			{
				PutToSleep();
			}
			else
			{
				// Add force which acts in the opposite direction(this mean it's slowing down sphere)
				AddForce(DirectX::XMFLOAT3(velocity.x * -0.15f, 0.f, velocity.z * -0.15f));
			}
		}
	}

	void VSaveToXML(XmlNode &configNode) override
	{
		XmlNode componentNode = configNode.CreateChildNode("SpherePhysicsComponent");
		componentNode.CreateAttribute("type", "PhysicsComponent");

		PhysicsComponent::VSaveToXML(componentNode);
	}
};