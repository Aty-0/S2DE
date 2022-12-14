#pragma once
#include "GameObjects/Components/Component.h"

namespace S2DE::GameObjects::Components
{
	class S2DE_API DrawableComponent : public Component
	{
	public:
		DrawableComponent() = default;
		virtual ~DrawableComponent() { }

		virtual void				 UpdateTexture() { }
		virtual void				 UpdateShader() { }
		virtual bool				 LoadTexture(std::string name) { return false; }
		virtual bool				 LoadTextureA(std::string name, bool unload_texture = false, bool auto_load_texture = true) { return false; }
		virtual bool				 LoadShader(std::string name) { return false; }
		virtual bool				 LoadMesh(std::string name) { return false; }

	protected:
		virtual void				 CreateVertexBuffer() { }
		virtual void				 CreateIndexBuffer() { }
		virtual void				 SetDefaultShader() { }
		virtual void				 SetDefaultTexture() { }

	private:
		S2DE_SERIALIZE_BEGIN(S2DE::GameObjects::Components::Component)
		S2DE_SERIALIZE_END();
	};
}