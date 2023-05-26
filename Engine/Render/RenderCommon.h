#pragma once
#include "Base/Main/Common.h"
#include "Math/Color.h"

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/imgui_impl_sdl.h"
#include "Libs/imgui/imgui_impl_dx11.h"

// TODO: Our graphical structures must use smart pointers
//       std or from atlbase
#include "atlbase.h"


// TODO: Somehow we need to switch enums on typedefs
//       This is needed for swithing current render API
//       Currently it is just a scratch and it had nothing with this big task
//       It's just a quick way to get something without checking API

//       A bit info about our problems with using other graphical API
//       Renderer is not universal! It is dependent on D3D11, 
//       and not ready yet to use other render API



namespace S2DE::Render::Api
{    
    template<typename RenderType, typename APIType>
    class RenderTypeToAPIWrapper
    {
        // FIX ME:
        // static_assert(!std::is_enum<RenderType>::value, "RenderType not a enum, can't make wrap!");
        // static_assert(!std::is_enum<APIType>::value, "APIType not a enum, can't make wrap!");

    public:        
        using Type = RenderType;


        RenderTypeToAPIWrapper() = default; 
        explicit RenderTypeToAPIWrapper(APIType type) noexcept;
        explicit RenderTypeToAPIWrapper(RenderType type) noexcept;

        ~RenderTypeToAPIWrapper() noexcept;

        // manual cast
        static APIType& cast(const RenderType& type);


        APIType& operator=(const RenderType& type);
        RenderType& operator=(const APIType& type);

    private:
        RenderType m_type;
    };

    enum class EPrimitiveTopology_D3D11
    {
        Undefined = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED,
        PointList = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
        LineList = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
        LineStrip = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
        TriangleList = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        TriangleStrip = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
        LineListAdj = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
        LineStripAdj = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
        TriangleListAdj = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
        TriangleStripAdj = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ,
    };

    typedef RenderTypeToAPIWrapper<EPrimitiveTopology_D3D11, D3D11_PRIMITIVE_TOPOLOGY> PrimitiveTopology;

    enum class ERasterizerMode
    {
        Default, // Without culling
        CullBack,
        CullFront,
        TwoSided,
        Wireframe
    };

    typedef ERasterizerMode RasterizerMode;

    enum class EClearFlag_D3D11
    {
        Depth = D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH,
        Stencil = D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL
    };

    typedef RenderTypeToAPIWrapper<EClearFlag_D3D11, D3D11_CLEAR_FLAG> ClearFlag;

    enum class ECullMode_D3D11
    {
        None = D3D11_CULL_MODE::D3D11_CULL_NONE,
        Front = D3D11_CULL_MODE::D3D11_CULL_FRONT,
        Back = D3D11_CULL_MODE::D3D11_CULL_BACK
    };

    typedef RenderTypeToAPIWrapper<ECullMode_D3D11, D3D11_CULL_MODE> CullMode;

    enum class EFillMode_D3D11
    {
        Solid = D3D11_FILL_MODE::D3D11_FILL_SOLID,
        Wireframe = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME
    };

    typedef RenderTypeToAPIWrapper<EFillMode_D3D11, D3D11_FILL_MODE> FillMode;
}

#include "Render/RenderCommon_inline.h"
