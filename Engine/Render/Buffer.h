#pragma once
#include "Base/Main/Common.h"

namespace S2DE::Render
{
	class S2DE_API Buffer
	{
	public:
		Buffer() = default;
		virtual ~Buffer() { }
		virtual bool				  Create(D3D11_USAGE buffer_usage = D3D11_USAGE_DEFAULT) = 0;
		virtual bool				  Lock() = 0;
		virtual void				  Unlock() = 0;
		virtual void				  Update() { Lock(); Unlock(); }
		virtual void				  Bind(std::int32_t startSlot = 0, std::int32_t num_buffers = 1) = 0;
		virtual void				  Unbind() = 0;
		inline virtual ID3D11Buffer*& GetBuffer() { return m_buffer; }

		inline virtual D3D11_BUFFER_DESC GetBufferDesc()
		{
			m_buffer->GetDesc(&m_buffer_desc);
			return m_buffer_desc;
		}

	protected:
		ID3D11Buffer* m_buffer;
		D3D11_BUFFER_DESC m_buffer_desc;
	};
}
