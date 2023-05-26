#pragma once
#include "Base/Main/Common.h"
#include "Render/Buffer.h"
#include "Base/Engine.h"

namespace S2DE::Render
{
	template<typename T = std::int32_t>
	class S2DE_API IndexBuffer : public Buffer
	{
	public:
		IndexBuffer()
		{

		}

		~IndexBuffer() final
		{
			Core::Release(m_buffer);
			m_array.clear();
			m_array.shrink_to_fit();
		}

		bool Create(D3D11_USAGE buffer_usage = D3D11_USAGE_DEFAULT) final
		{
			if (m_array.size() == 0)
				return false;

			D3D11_BUFFER_DESC bufferDesc = { };

			bufferDesc.ByteWidth = (T)(sizeof(T) * m_array.size());
			bufferDesc.Usage = buffer_usage;
			bufferDesc.CPUAccessFlags = buffer_usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA subData = { };
			subData.pSysMem = m_array.data();
			subData.SysMemPitch = 0;
			subData.SysMemSlicePitch = 0;

			const auto renderer = Render::Renderer::GetInstance();
			Verify_HR(renderer->GetDevice()->CreateBuffer(&bufferDesc, &subData, &m_buffer), "Can't create buffer!");

			return true;
		}

		bool Lock(Render::Renderer* renderer) final
		{
			if (m_buffer_desc.Usage == D3D11_USAGE_DYNAMIC)
			{
				D3D11_MAPPED_SUBRESOURCE mappedResource = { };
				Verify_HR(renderer->GetContext()->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), "Can't map resource in index buffer");
			}

			return true;
		}

		void Unlock(Render::Renderer* renderer) final
		{
			if (m_buffer_desc.Usage == D3D11_USAGE_DYNAMIC)
			{
				renderer->GetContext()->Unmap(m_buffer, 0);
			}
		}

		void Bind(Render::Renderer* renderer, std::int32_t startSlot = 0, std::int32_t num_buffers = 1) final
		{
			renderer->GetContext()->IASetIndexBuffer(m_buffer, DXGI_FORMAT_R32_UINT, 0);
		}

		void Unbind(Render::Renderer* renderer) final
		{
			renderer->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		}

		[[nodiscard]] std::vector<T>& GetArray() { return m_array; }

	private:
		std::vector<T> m_array;

	};
}