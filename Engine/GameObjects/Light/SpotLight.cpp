#include "SpotLight.h"

namespace S2DE::GameObjects::Light
{
	SpotLight::SpotLight()
	{
		//m_attenuation = DirectX::SimpleMath::Vector3(0.04f, 0.1f, 0.06f);
		m_attenuation = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
		m_range = 100.0f;
		m_strength = 1.0f;
		m_spot = 10.0f;
	}

	SpotLight::~SpotLight()
	{

	}

	void SpotLight::SetSpot(float spot)
	{	  
		m_spot = spot;
		OnChangeSpot();
	}	  
		  
	void SpotLight::OnChangeSpot()
	{	  
		UpdateCB();
	}	  
		  
	void SpotLight::UpdateCB() 
	{	  
		// Update light structure
		m_lightStructure.strength = m_strength;
		m_lightStructure.attenuation = m_attenuation;
		m_lightStructure.range = m_range;
		m_lightStructure.pad = m_pad;
		m_lightStructure.spot = m_spot;
		m_lightStructure.light_type = static_cast<std::int32_t>(Render::LightTypes::Spot);
		m_lightStructure.enabled = static_cast<std::int32_t>(isEnabled());
		m_lightStructure.position = DirectX::SimpleMath::Vector4(GetPosition().x, GetPosition().y, GetPosition().z, 1);
		m_lightStructure.direction = DirectX::SimpleMath::Vector4(GetRotation().x, GetRotation().y, GetRotation().z, 1);
		m_lightStructure.color = DirectX::SimpleMath::Vector4(m_color.r, m_color.g, m_color.b, 1);

		// Get light constant buffer
		Render::LightConstBuff buffer = Render::LightGlobals::GlobalLightConstBuffer;

		// Upload new data
		buffer->Lock();
		buffer->GetData()->lights[m_index] = m_lightStructure;
		buffer->Unlock();
		buffer->Bind(1);
		buffer->Unbind();
	}	  
		  
	void SpotLight::OnPositionChanged()  
	{	  
		UpdateCB();		  
	}	  
		  
	void SpotLight::OnRotationChanged()  
	{	  		  
		UpdateCB();		  
	}	  
		  
	void SpotLight::OnChangeStrength() 
	{	  
		UpdateCB();		  		  
	}	  
		  
	void SpotLight::OnChangeColor()	
	{	  
		UpdateCB();		  		  		  
	}	  
		  
	void SpotLight::OnChangePad() 
	{	  
		UpdateCB();		  		  		  		  
	}	  
		  
	void SpotLight::OnChangeRange() 
	{	  
		UpdateCB();		  		  		  		  		  
	}	  

	void SpotLight::OnChangeAttenuation()
	{	  
		UpdateCB();		  		  		  		  		  
	}
}