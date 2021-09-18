#pragma once
#include "GameObjects\GameObject.h"

namespace S2DE
{
	class S2DE_API TestObject : public GameObject
	{
	public:
		TestObject();
		~TestObject();

	private:
		virtual void OnRender() override;



		Shader* m_shader;
		VertexBuffer* m_vbuffer;
		IndexBuffer* m_ibuffer;
	};
}