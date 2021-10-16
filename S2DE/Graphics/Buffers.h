#pragma once
#include "Base/Main/Common.h"
#include "Base/Engine.h"
#include "Graphics/Renderer.h"


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
			//Delete(m_buffer_data);
			Release(m_buffer);
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
			

			D3D11_SUBRESOURCE_DATA subresource;
			subresource.pSysMem = m_buffer_data;
			subresource.SysMemPitch = 0;
			subresource.SysMemSlicePitch = 0;

			return SUCCEEDED(Engine::GetRenderer()->GetDevice()->CreateBuffer(&bufferDesc, &subresource, &m_buffer));
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

		void Bind(std::int32_t num = 0)
		{
			Engine::GetRenderer()->GetContext()->VSSetConstantBuffers(num, 1, &m_buffer);
		}

		inline T*&			  GetBufferData() { return reinterpret_cast<T*&>(m_buffer_data); }
		inline ID3D11Buffer*& GetBuffer() { return m_buffer; }
	private:
		ID3D11Buffer* m_buffer;
		void*			  m_buffer_data;

	};
}

