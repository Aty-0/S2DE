#pragma once
#include "Base/Main/Common.h"
#include "Graphics/Buffer.h"
#include "Base/Engine.h"

namespace S2DE::Render
{
	typedef void* ConstantBufferData;

	template<typename T = ConstantBufferData>
	class S2DE_API ConstantBuffer : public Buffer
	{
	public:
		ConstantBuffer() :
			m_data(nullptr)
		{

		}

		~ConstantBuffer()
		{

		}

		virtual bool Create(D3D11_USAGE buffer_usage = D3D11_USAGE_DEFAULT) override
		{
			m_data = new T();

			D3D11_BUFFER_DESC bufferDesc{};
			bufferDesc.Usage = buffer_usage;
			bufferDesc.ByteWidth = sizeof(T);
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = buffer_usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;


			D3D11_SUBRESOURCE_DATA subresource{};
			subresource.pSysMem = m_data;
			subresource.SysMemPitch = 0;
			subresource.SysMemSlicePitch = 0;

			S2DE_CHECK_SAFE(Core::Engine::GetRenderer()->GetDevice()->CreateBuffer(&bufferDesc, &subresource, &m_buffer), "Can't create constant buffer");

			return true;
		}

		virtual bool Lock()	 override
		{
			if (m_buffer_desc.Usage == D3D11_USAGE_DYNAMIC)
			{
				D3D11_MAPPED_SUBRESOURCE mappedResource{};
				S2DE_CHECK_SAFE(Core::Engine::GetRenderer()->GetContext()->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), "Can't map resource in constant buffer");
				m_data = reinterpret_cast<T*>(mappedResource.pData);
			}

			return true;
		}

		virtual void Unlock() override
		{
			if (m_buffer_desc.Usage == D3D11_USAGE_DYNAMIC)
				Core::Engine::GetRenderer()->GetContext()->Unmap(m_buffer, 0);
		}

		virtual void Bind(std::int32_t startSlot = 0, std::int32_t num_buffers = 1)	 override
		{
			if (m_buffer_desc.Usage == D3D11_USAGE_DEFAULT)
				Core::Engine::GetRenderer()->GetContext()->UpdateSubresource(m_buffer, 0, NULL, m_data, 0, 0);

			Core::Engine::GetRenderer()->GetContext()->VSSetConstantBuffers(startSlot, num_buffers, &m_buffer);
		}

		inline T*& GetData() { return reinterpret_cast<T*&>(m_data); }

	private:
		ConstantBufferData			  m_data;

	};
}