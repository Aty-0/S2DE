#include "DirectionalLight.h"
#include "GameObjects/Base/GameObject.h"

namespace S2DE::GameObjects::Components::Light
{
	DirectionalLight::DirectionalLight()
	{

	}

	DirectionalLight::~DirectionalLight()
	{

	}

	void DirectionalLight::InitLight()
	{
		m_attenuation = DirectX::SimpleMath::Vector3(0.0f, 0.1f, 0.0f);
		m_range = 25.0f;
		m_pad = 1.0f;

		// Add callbacks...
		const auto transform = GetOwner()->GetTransform();
		Assert(transform, "transform is null");

		transform->onPositionChanged.AddCallback(std::bind(&DirectionalLight::UpdateCB, this));
		transform->onRotationChanged.AddCallback(std::bind(&DirectionalLight::UpdateCB, this));
		transform->onScaleChanged.AddCallback(std::bind(&DirectionalLight::UpdateCB, this));
		onColorChanged.AddCallback(std::bind(&DirectionalLight::UpdateCB, this));
		onStrengthChanged.AddCallback(std::bind(&DirectionalLight::UpdateCB, this));
	}

	void DirectionalLight::UpdateCB()
	{
		Render::LightGlobals::LightConstantBuffer->Lock();
		auto& str = m_lightStructure;

		str.attenuation = m_attenuation;
		str.pad = m_pad;
		str.light_type = static_cast<std::int32_t>(LightTypes::DirectionalLight);

		const auto transform = GetOwner()->GetTransform();
		Assert(transform, "transform is null");

		auto parentPosition = DirectX::SimpleMath::Vector3::Zero;
		auto parentRotation = DirectX::SimpleMath::Vector3::Zero;

		// TODO: Get global position, rotation, scale 
		//		 This is local p, r, s
		if (transform->GetParent() != nullptr)
		{
			auto transformParent = transform->GetParent()->GetTransform();

			if (transformParent != nullptr)
			{
				parentPosition = transformParent->GetPosition();
				parentRotation = transformParent->GetRotation();
			}
		}

		if (transform != nullptr)
		{
			str.position = DirectX::SimpleMath::Vector4(transform->GetPosition().x + parentPosition.x,
				transform->GetPosition().y + parentPosition.y, transform->GetPosition().z + parentPosition.z, 1);

			str.direction = DirectX::SimpleMath::Vector4(transform->GetRotation().x + parentRotation.x,
				transform->GetRotation().y + parentRotation.y, transform->GetRotation().z + parentRotation.z, 1);
		}

		str.color = DirectX::SimpleMath::Vector4(m_color.r, m_color.g, m_color.b, 1);

		Render::LightGlobals::SetNewLightStructure(str, GetUUID());

		Render::LightGlobals::LightConstantBuffer->Unlock();
		Render::LightGlobals::LightConstantBuffer->Bind(1);
		Render::LightGlobals::LightConstantBuffer->Unbind();

	}
}
