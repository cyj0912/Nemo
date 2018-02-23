#pragma once
#include "RHI.h"
#include "OpenGLRHIAPI.h"

namespace tc
{

//Use final keyword to allow the compiler to devirtualize
class OPENGLRHI_API FOpenGLRHI final : public FRHI
{
public:
    const char* GetRHIVersionString() override;

    FRHIRasterizerStateRef CreateRasterizerState(const FRasterizerDesc& desc) override;

    FRHIInputLayoutRef CreateInputLayout(const FInputElementDesc* elements, uint32_t elementCount) override;

    FRHIVertexBufferRef CreateVertexBuffer(uint32_t size, EBufferUsageFlags usage, const FSubresourceData& data) override;
    void* LockVertexBuffer(FRHIVertexBuffer* buffer, uint32_t offset, uint32_t size, EBufferLockOptions options) override;
    void UnlockVertexBuffer(FRHIVertexBuffer* buffer) override;

    FRHIIndexBufferRef CreateIndexBuffer(uint32_t size, EBufferUsageFlags usage, const FSubresourceData& data) override;
    void* LockIndexBuffer(FRHIIndexBuffer* buffer, uint32_t offset, uint32_t size, EBufferLockOptions options) override;
    void UnlockIndexBuffer(FRHIIndexBuffer* buffer) override;

    FRHITexture2DRef CreateTexture2D(uint32_t width, uint32_t height, EPixelFormat format, uint32_t mipLevels, uint32_t numSamples, ETextureUsageFlags flags, const FSubresourceData& data) override;
    FRHITexture2DArrayRef CreateTexture2DArray(uint32_t width, uint32_t height, uint32_t depth, EPixelFormat format, uint32_t mipLevels, ETextureUsageFlags flags, const FSubresourceData& data) override;
    FRHITexture3DRef CreateTexture3D(uint32_t width, uint32_t height, uint32_t depth, EPixelFormat format, uint32_t mipLevels, ETextureUsageFlags flags, const FSubresourceData& data) override;

    FRHIVertexShaderRef CreateVertexShader(const char* code, uint32_t length) override;
    FRHIPixelShaderRef CreatePixelShader(const char* code, uint32_t length) override;

    FRHIViewportRef CreateViewport(void* nativeWindow, uint32_t width, uint32_t height, bool isFullScreen, EPixelFormat format) override;
    void ResizeViewport(FRHIViewport* viewport, uint32_t width, uint32_t height, bool isFullScreen) override;

    //Drawing context
    void BeginFrame() override;

    void EndFrame() override;

    void SetInputLayout(FRHIInputLayout* layout) override;
    void SetVertexBuffer(uint32_t slot, FRHIVertexBuffer* buffer, uint32_t stride, uint32_t offset) override;

    void SetVertexShader(FRHIVertexShader* shader) override;
    void SetPixelShader(FRHIPixelShader* shader) override;

    //RS
    void SetScissorRect() override;
    void SetRasterizerState() override;
    void SetViewport(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth) override;

    void DrawPrimitive(EPrimitiveType primitiveType, uint32_t baseVertexIndex, uint32_t primitiveCount, uint32_t instanceCount) override;
};

#ifdef RHI_STATIC_USE_OPENGL
extern FOpenGLRHI* GRHI;
#endif

} /* namespace tc */
