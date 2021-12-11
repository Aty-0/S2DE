#include "Buffers.h"


namespace S2DE::Render
{
	VertexBuffer::VertexBuffer() : 
		m_buffer(nullptr),
		m_array()
	{

	}

	VertexBuffer::~VertexBuffer()
	{
		m_array.clear();
		Core::Release(m_buffer);
	}

	bool VertexBuffer::Create(D3D11_USAGE usage)
	{
		if (m_array.size() == 0)
			return false;

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = usage;
		bufferDesc.ByteWidth = sizeof(Vertex) * m_array.size();
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = m_array.data();
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

	
		return SUCCEEDED(Core::Engine::GetRenderer()->GetDevice()->CreateBuffer(&bufferDesc, &InitData, &m_buffer));
	}

	void VertexBuffer::Bind()
	{
		//FIX ME 
		//is that right ?
		std::uint32_t stride = sizeof(Vertex);	
		std::uint32_t offset = 0;

		Core::Engine::GetRenderer()->GetContext()->IASetVertexBuffers(0, 1, &m_buffer, &stride, &offset);
	}

	void VertexBuffer::Lock()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Core::Engine::GetRenderer()->GetContext()->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	}

	void VertexBuffer::Unlock()
	{
		Core::Engine::GetRenderer()->GetContext()->Unmap(m_buffer, 0);
	}


	IndexBuffer::IndexBuffer() :
		m_buffer(nullptr),
		m_array()
	{

	}

	IndexBuffer::~IndexBuffer()
	{
		m_array.clear();
		Core::Release(m_buffer);
	}

	bool IndexBuffer::Create(D3D11_USAGE usage)
	{
		if (m_array.size() == 0)
			return false;

		D3D11_BUFFER_DESC bufferDesc;

		bufferDesc.Usage = usage;
		bufferDesc.ByteWidth = sizeof(std::uint32_t) * m_array.size();
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = m_array.data();
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;


		return SUCCEEDED(Core::Engine::GetRenderer()->GetDevice()->CreateBuffer(&bufferDesc, &InitData, &m_buffer));
	}

	void IndexBuffer::Bind()
	{
		Core::Engine::GetRenderer()->GetContext()->IASetIndexBuffer(m_buffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void IndexBuffer::Lock()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Core::Engine::GetRenderer()->GetContext()->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	}

	void IndexBuffer::Unlock()
	{
		Core::Engine::GetRenderer()->GetContext()->Unmap(m_buffer, 0);
	}
}