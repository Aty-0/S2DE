#pragma once
#include "GameObjects/Components/Component.h"

namespace S2DE::GameObjects::Components
{
	class S2DE_API DrawableComponent : public Component
	{
	public:
		DrawableComponent() = default;
		virtual ~DrawableComponent() { }

		virtual void UpdateTexture() { }
		virtual void UpdateShader() { }				 
		virtual bool LoadTexture(std::string name) { return false; }
		virtual bool LoadShader(std::string name)  { return false; }
		virtual bool LoadMesh(std::string name)    { return false; }

	};
}