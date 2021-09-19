#pragma once
#include "Base/Main/Common.h"
#include "Graphics/Renderer.h"
#include "Base/Utils/Logger.h"
#include "Base/Engine.h"
#include <vector>

//TODO
//Base class

namespace S2DE
{
	class S2DE_API VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();
		
		bool Create(D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
		void Bind();
		void Lock();
		void Unlock();

		inline std::vector<Vertex>&
			GetArray() { return m_array; }
		inline ID3D11Buffer*&
			GetBuffer() { return m_buffer; }

	private:
		ID3D11Buffer*		m_buffer;
		std::vector<Vertex> m_array;

	};

	class S2DE_API IndexBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		bool Create(D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
		void Bind();
		void Lock();
		void Unlock();

		inline std::vector<std::uint32_t>&
			 GetArray() { return m_array; }
		inline ID3D11Buffer*& GetBuffer() { return m_buffer; }
	private:
		ID3D11Buffer* m_buffer;
		std::vector<std::uint32_t> m_array;

	};

	//TODO
	//PS Support ?

	template<typename T>
	class S2DE_API ConstantBuffer
	{
	public:
		ConstantBuffer() :
			m_buffer(nullptr),
			m_buffer_data(nullptr)
		{

		}

		~ConstantBuffer()
		{
			Release(m_buffer);
			Delete(m_buffer_data);
		}

		bool Create()
		{
			m_buffer_data = new T();

			D3D11_BUFFER_DESC bufferDesc;

			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.ByteWidth = sizeof(T);
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;

			return SUCCEEDED(Engine::GetRenderer()->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &m_buffer));
		}

		bool Lock()
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			if (FAILED(Engine::GetRenderer()->GetContext()->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
				return false;

			m_buffer_data = reinterpret_cast<T*>(mappedResource.pData);

			return true;
		}

		void Unlock()
		{
			Engine::GetRenderer()->GetContext()->Unmap(m_buffer, 0);
		}

		void Bind(std::int32_t num)
		{
			Engine::GetRenderer()->GetContext()->VSSetConstantBuffers(num, 1, &m_buffer);
		}

		void Unbind()
		{
			Engine::GetRenderer()->GetContext()->VSSetConstantBuffers(0, 0, nullptr);
		}

		inline T*& GetBufferData() { return m_buffer_data; }
		inline ID3D11Buffer*& GetBuffer() { return m_buffer; }
	private:
		ID3D11Buffer* m_buffer;
		T* m_buffer_data;

	};
}

