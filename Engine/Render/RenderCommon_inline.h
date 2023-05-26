#pragma once
#include "RenderCommon.h"

namespace S2DE::Render::Api
{ 
    template<typename RenderType, typename APIType>
    inline RenderTypeToAPIWrapper<RenderType, APIType>::RenderTypeToAPIWrapper(APIType type) noexcept
    {
        m_type = cast(type);
    }

    template<typename RenderType, typename APIType>
    inline RenderTypeToAPIWrapper<RenderType, APIType>::RenderTypeToAPIWrapper(RenderType type) noexcept
    {
        m_type = type;
    }

    template<typename RenderType, typename APIType> 
    inline RenderTypeToAPIWrapper<RenderType, APIType>::~RenderTypeToAPIWrapper() noexcept
    {
        m_type = static_cast<RenderType>(0); // Will be unknown
    }

    template<typename RenderType, typename APIType>
    inline APIType& RenderTypeToAPIWrapper<RenderType, APIType>::cast(const RenderType & type)
    {
        auto _type = static_cast<APIType>(type);
        return _type;
    }

    // Casted to API type
    template<typename RenderType, typename APIType>
    inline APIType& RenderTypeToAPIWrapper<RenderType, APIType>::operator=(const RenderType& type) 
    {
        auto _type = static_cast<APIType>(type);
        return _type;
    }

    template<typename RenderType, typename APIType>
    inline RenderType& RenderTypeToAPIWrapper<RenderType, APIType>::operator=(const APIType& type) 
    {
        m_type = static_cast<RenderType>(type);
        return *this;
    }
}