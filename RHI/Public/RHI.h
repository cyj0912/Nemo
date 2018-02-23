#pragma once
#include "RHIResources.h"
#include <cstdint>

namespace tc
{

/*
 * One more layer of abstraction to make the architecture cleaner
 * And to not include gl.h everywhere which pulls in all sorts of OS dependencies
 * Intended to be statically resolved
 *
 * The api roughly follows directx, this should ease my porting to dx in the future.
 */

enum class ERasterizerFillMode : uint8_t
{
    Point,
    Wireframe,
    Solid
};

enum class ERasterizerCullMode : uint8_t
{
    None,
    CW,
    CCW
};

struct FRasterizerDesc
{
    ERasterizerFillMode FillMode; //Default = Solid
    ERasterizerCullMode CullMode; //Default = CCW, One other dx state merged in here

    //TODO: these states are ignored for now
    int DepthBias; //Default=0
    float DepthBiasClamp; //Default=0
    float SlopeScaledDepthBias; //Default=0
    bool DepthClipEnable; //Default=true
    bool ScissorEnable; //Default=false
    bool MultisampleEnable; //Default=false
    bool AntialiasedLineEnable; //Default=false
};

/*
 * TODO: huge work in progress
enum class EBlendFactor : uint8_t
{
    Zero = 1,
    One = 2,
    SourceColor = 3,
    InverseSourceColor = 4,
    SourceAlpha = 5,
    InverseSourceAlpha = 6,
    DestAlpha = 7,
    InverseDestAlpha = 8,
    DestColor = 9,
    InverseDestColor = 10,
    D3D11_BLEND_SRC_ALPHA_SAT = 11,
    D3D11_BLEND_BLEND_FACTOR = 14,
    D3D11_BLEND_INV_BLEND_FACTOR = 15,
    D3D11_BLEND_SRC1_COLOR = 16,
    D3D11_BLEND_INV_SRC1_COLOR = 17,
    D3D11_BLEND_SRC1_ALPHA = 18,
    D3D11_BLEND_INV_SRC1_ALPHA = 19
};

enum class EBlendOp : uint8_t
{
    Add = 1,
    Subtract = 2,
    ReverseSubtract = 3,
    Min = 4,
    Max = 5
};

struct FRenderTargetBlendDesc
{
    bool BlendEnable;
    EBlendFactor SrcBlend;
    EBlendFactor DestBlend;
    EBlendOp BlendOp;
    EBlendFactor SrcBlendAlpha;
    EBlendFactor DestBlendAlpha;
    EBlendOp BlendOpAlpha;
    uint8_t RenderTargetWriteMask;
};

struct FBlendDesc
{
    bool bEnableAlphaToCoverage;
    bool bUseIndependentBlendStates;
    FRenderTargetBlendDesc RenderTarget[8];
};

enum class EComparisonFunc : uint8_t
{
    Never = 1,
    Less = 2,
    Equal = 3,
    LessEqual = 4,
    Greater = 5,
    NotEqual = 6,
    GreaterEqual = 7,
    Always = 8
};

struct FDepthStencilDesc {
    bool                       DepthEnable;
    D3D11_DEPTH_WRITE_MASK     DepthWriteMask;
    EComparisonFunc      DepthFunc;
    bool                       StencilEnable;
    uint8_t                      StencilReadMask;
    uint8_t                      StencilWriteMask;
    D3D11_DEPTH_STENCILOP_DESC FrontFace;
    D3D11_DEPTH_STENCILOP_DESC BackFace;
}
*/

class RHI_API FRHI
{
public:
    virtual const char* GetRHIVersionString() = 0;

    //Device

    virtual FRHIRasterizerStateRef CreateRasterizerState(const FRasterizerDesc& desc) = 0;

    virtual FRHIInputLayoutRef CreateInputLayout(const FInputElementDesc* elements, uint32_t elementCount) = 0;

    virtual FRHIVertexBufferRef CreateVertexBuffer(uint32_t size, EBufferUsageFlags usage, const FSubresourceData& data) = 0;
    virtual void* LockVertexBuffer(FRHIVertexBuffer* buffer, uint32_t offset, uint32_t size, EBufferLockOptions options) = 0;
    virtual void UnlockVertexBuffer(FRHIVertexBuffer* buffer) = 0;
    virtual FRHIIndexBufferRef CreateIndexBuffer(uint32_t size, EBufferUsageFlags usage, const FSubresourceData& data) = 0;
    virtual void* LockIndexBuffer(FRHIIndexBuffer* buffer, uint32_t offset, uint32_t size, EBufferLockOptions options) = 0;
    virtual void UnlockIndexBuffer(FRHIIndexBuffer* buffer) = 0;

    virtual FRHITexture2DRef CreateTexture2D(uint32_t width, uint32_t height, EPixelFormat format, uint32_t mipLevels, uint32_t numSamples, ETextureUsageFlags flags, const FSubresourceData& data) = 0;
    virtual FRHITexture2DArrayRef CreateTexture2DArray(uint32_t width, uint32_t height, uint32_t depth, EPixelFormat format, uint32_t mipLevels, ETextureUsageFlags flags, const FSubresourceData& data) = 0;
    virtual FRHITexture3DRef CreateTexture3D(uint32_t width, uint32_t height, uint32_t depth, EPixelFormat format, uint32_t mipLevels, ETextureUsageFlags flags, const FSubresourceData& data) = 0;

    virtual FRHIVertexShaderRef CreateVertexShader(const char* code, uint32_t length) = 0;
    virtual FRHIPixelShaderRef CreatePixelShader(const char* code, uint32_t length) = 0;

    virtual FRHIViewportRef CreateViewport(void* nativeWindow, uint32_t width, uint32_t height, bool isFullScreen, EPixelFormat format) = 0;
    virtual void ResizeViewport(FRHIViewport* viewport, uint32_t width, uint32_t height, bool isFullScreen) = 0;

    //Drawing context
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;

    //IA
    //SetIndexBuffer and PrimitiveTopology is folded into Draw calls
    virtual void SetInputLayout(FRHIInputLayout* layout) = 0;
    virtual void SetVertexBuffer(uint32_t slot, FRHIVertexBuffer* buffer, uint32_t stride, uint32_t offset) = 0;
    virtual void SetVertexBuffers(uint32_t beginSlot, uint32_t num, FRHIVertexBuffer** buffers, uint32_t* strides, uint32_t* offsets)
    {
        for (int i = 0; i < num; i++)
        {
            SetVertexBuffer(beginSlot + i, buffers[i], strides[i], offsets[i]);
        }
    }

    virtual void SetVertexShader(FRHIVertexShader* shader) = 0;
    virtual void SetPixelShader(FRHIPixelShader* shader) = 0;

    //RS
    virtual void SetScissorRect() = 0;
    virtual void SetRasterizerState() = 0;
    virtual void SetViewport(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth) = 0;
    //TODO: virtual void SetMultipleViewports() = 0

    /* OM
    virtual void SetBlendState() = 0;
    virtual void SetDepthStencilState() = 0;
    virtual void SetRenderTargets() = 0;
    virtual void SetRenderTargetsAndUnorderedAccessViews() = 0; */

    virtual void DrawPrimitive(EPrimitiveType primitiveType, uint32_t baseVertexIndex, uint32_t primitiveCount, uint32_t instanceCount) = 0;
};

} /* namespace tc */

#ifdef RHI_STATIC_USE_OPENGL
#include "OpenGLRHI.h"
#else
namespace tc
{
extern FRHI* GRHI;
} /* namespace tc */
#endif
