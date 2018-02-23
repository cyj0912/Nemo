#include "OpenGLRHI.h"
#include "Shader.h"
#include "VAO.h"

namespace tc
{

static FOpenGLRHI GRHIStatic;
FOpenGLRHI* GRHI = &GRHIStatic;

static GLenum BufferUsageToGLFlag(EBufferUsageFlags flags)
{
    if (Any(flags, EBufferUsageFlags::Static))
    {
        return GL_STATIC_DRAW;
    } else if (Any(flags, EBufferUsageFlags::Dynamic))
    {
        return GL_DYNAMIC_DRAW;
    }
    //No flag??
    return GL_DYNAMIC_DRAW;
}

template <int Target, typename Super>
class FGLHardwareBuffer : public Super
{
public:
    FGLHardwareBuffer(uint32_t size, EBufferUsageFlags usage, const FSubresourceData& data)
    {
        glGenBuffers(1, &BufferName);
        Bind();
        glBufferData(Target, size, data.BufferDataPtr, BufferUsageToGLFlag(usage));
    }

    ~FGLHardwareBuffer() override
    {
        glDeleteBuffers(1, &BufferName);
    }

    void* Lock(uint32_t offset, uint32_t size, EBufferLockOptions options)
    {
        Bind();

        GLenum access = 0;

        if (options == EBufferLockOptions::ReadOnly)
            access = GL_MAP_READ_BIT;
        else if (options == EBufferLockOptions::WriteOnly)
            access = GL_MAP_WRITE_BIT;
        else if (options == EBufferLockOptions::ReadWrite)
            access = GL_MAP_WRITE_BIT | GL_MAP_READ_BIT;
        else if (options == EBufferLockOptions::WriteDiscard)
            access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT;
        else if (options == EBufferLockOptions::NoOverwrite)
            access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_UNSYNCHRONIZED_BIT;

        void* buffer = glMapBufferRange(Target, offset, size, access);
        return buffer;
    }

    void Unlock()
    {
        glBindBuffer(Target, BufferName);
        glUnmapBuffer(Target);
    }

    void Bind()
    {
        glBindBuffer(Target, BufferName);
    }

    GLuint BufferName;
};

typedef FGLHardwareBuffer<GL_ARRAY_BUFFER, FRHIVertexBuffer> FGL3VertexBuffer;

typedef FGLHardwareBuffer<GL_ELEMENT_ARRAY_BUFFER, FRHIIndexBuffer> FGL3IndexBuffer;

const char* FOpenGLRHI::GetRHIVersionString()
{
    static int numExtensions = 0;
    static string versionString;
    if (numExtensions == 0)
    {
        auto* glver = glGetString(GL_VERSION);
        versionString += (const char*)glver;
        versionString += "\n";
        versionString += "Extensions: ";

        glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
        for (int i = 0; i < numExtensions; i++)
        {
            const GLubyte* ext = glGetStringi(GL_EXTENSIONS, i);
            versionString += (const char*)ext;
            versionString += " ";
        }
        versionString += "\n";
    }
    return versionString.c_str();
}

FRHIVertexBufferRef FOpenGLRHI::CreateVertexBuffer(uint32_t size, EBufferUsageFlags usage, const FSubresourceData& data)
{
    return new FGL3VertexBuffer(size, usage, data);
}

void* FOpenGLRHI::LockVertexBuffer(FRHIVertexBuffer* buffer, uint32_t offset, uint32_t size, EBufferLockOptions options)
{
    return static_cast<FGL3VertexBuffer*>(buffer)->Lock(offset, size, options);
}

void FOpenGLRHI::UnlockVertexBuffer(FRHIVertexBuffer* buffer)
{
    return static_cast<FGL3VertexBuffer*>(buffer)->Unlock();
}

FRHIIndexBufferRef FOpenGLRHI::CreateIndexBuffer(uint32_t size, EBufferUsageFlags usage, const FSubresourceData& data)
{
    return new FGL3IndexBuffer(size, usage, data);
}

void* FOpenGLRHI::LockIndexBuffer(FRHIIndexBuffer* buffer, uint32_t offset, uint32_t size, EBufferLockOptions options)
{
    return static_cast<FGL3IndexBuffer*>(buffer)->Lock(offset, size, options);
}

void FOpenGLRHI::UnlockIndexBuffer(FRHIIndexBuffer* buffer)
{
    return static_cast<FGL3IndexBuffer*>(buffer)->Unlock();
}


FRHITexture2DRef FOpenGLRHI::CreateTexture2D(uint32_t width, uint32_t height, EPixelFormat format, uint32_t mipLevels,
                                             uint32_t numSamples, ETextureUsageFlags flags,
                                             const FSubresourceData& data)
{
    throw "Unimplemented";
    return nullptr;
}

FRHITexture2DArrayRef
FOpenGLRHI::CreateTexture2DArray(uint32_t width, uint32_t height, uint32_t depth, EPixelFormat format,
                                 uint32_t mipLevels, ETextureUsageFlags flags, const FSubresourceData& data)
{
    throw "Unimplemented";
    return nullptr;
}

FRHITexture3DRef
FOpenGLRHI::CreateTexture3D(uint32_t width, uint32_t height, uint32_t depth, EPixelFormat format, uint32_t mipLevels,
                            ETextureUsageFlags flags, const FSubresourceData& data)
{
    throw "Unimplemented";
    return nullptr;
}

FRHIVertexShaderRef FOpenGLRHI::CreateVertexShader(const char* code, uint32_t length)
{
    auto* shader = new FGLSLVertexShader(code, length);
    if (!shader->TryCompile())
    {
        delete shader;
        return nullptr;
    }
    return shader;
}

FRHIPixelShaderRef FOpenGLRHI::CreatePixelShader(const char* code, uint32_t length)
{
    auto* shader = new FGLSLPixelShader(code, length);
    if (!shader->TryCompile())
    {
        delete shader;
        return nullptr;
    }
    return shader;
}

FRHIViewportRef
FOpenGLRHI::CreateViewport(void* nativeWindow, uint32_t width, uint32_t height, bool isFullScreen, EPixelFormat format)
{
    throw "Unimplemented";
    return nullptr;
}

void FOpenGLRHI::ResizeViewport(FRHIViewport* viewport, uint32_t width, uint32_t height, bool isFullScreen)
{
    throw "Unimplemented";
}

//Drawing context

void FOpenGLRHI::BeginFrame()
{

}

void FOpenGLRHI::EndFrame()
{
    FRHIResource::DeletePending();
}

class FVAOCache
{
public:
    void BindInputLayout(const FVAOInputLayout* il)
    {
        BoundInputLayout = *il;
    }

    void BindBuffer(int slot, FGL3VertexBuffer* buffer, uint32_t stride, uint32_t offset)
    {
        BoundVertexBuffers[slot] = buffer;
        Strides[slot] = stride;
        Offsets[slot] = offset;
    }

    GLuint CreateAndBindVAO()
    {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        for (const FVAOAttribute& attr : BoundInputLayout.Attributes)
        {
            BoundVertexBuffers[attr.Slot]->Bind();
            glEnableVertexAttribArray(attr.AttribIndex);
            glVertexAttribPointer(attr.AttribIndex, attr.Size, attr.Type, GL_FALSE, Strides[attr.Slot],
                                  (void*) (attr.Offset + Offsets[attr.Slot]));
        }
        return vao;
    }

    size_t TryHash()
    {
        size_t seed = 0;
        tc_hash_combine(seed, BoundInputLayout);
        for (int i = 0; i < MaxInputSlot; i++)
            hash_combine(seed, BoundVertexBuffers[i]);
        return seed;
    }

private:
    FVAOInputLayout BoundInputLayout;
    FGL3VertexBuffer* BoundVertexBuffers[MaxInputSlot] = {nullptr};
    uint32_t Strides[MaxInputSlot];
    uint32_t Offsets[MaxInputSlot];
};

static FVAOCache vaoCache;

FRHIInputLayoutRef FOpenGLRHI::CreateInputLayout(const FInputElementDesc* elements, uint32_t elementCount)
{
    auto* layout = new FVAOInputLayout();
    for (int i = 0; i < elementCount; i++)
    {
        layout->AddAttribute(elements[i]);
    }
    return layout;
}

void FOpenGLRHI::SetInputLayout(FRHIInputLayout* layout)
{
    vaoCache.BindInputLayout(static_cast<FVAOInputLayout*>(layout));
}

void FOpenGLRHI::SetVertexBuffer(uint32_t slot, FRHIVertexBuffer* buffer, uint32_t stride, uint32_t offset)
{
    vaoCache.BindBuffer(slot, static_cast<FGL3VertexBuffer*>(buffer), stride, offset);
}

FGLSLVertexShader* BoundVS = nullptr;
FGLSLPixelShader* BoundPS = nullptr;

void FOpenGLRHI::SetVertexShader(FRHIVertexShader* shader)
{
    BoundVS = static_cast<FGLSLVertexShader*>(shader);
}

void FOpenGLRHI::SetPixelShader(FRHIPixelShader* shader)
{
    BoundPS = static_cast<FGLSLPixelShader*>(shader);
}

void FOpenGLRHI::SetScissorRect()
{
    throw "Unimplemented";
}

void FOpenGLRHI::SetRasterizerState()
{
    throw "Unimplemented";
}

void FOpenGLRHI::SetViewport(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
{
    throw "Unimplemented";
}

void FOpenGLRHI::DrawPrimitive(EPrimitiveType primitiveType, uint32_t baseVertexIndex, uint32_t primitiveCount,
                               uint32_t instanceCount)
{
    GLuint vao = vaoCache.CreateAndBindVAO();
    auto* program = FGLSLProgram::CreateFromShaders(BoundVS, BoundPS);
    program->Enable();

    if (primitiveType == EPrimitiveType::TriangleList)
    {
        if (instanceCount > 1)
            glDrawArraysInstanced(GL_TRIANGLES, baseVertexIndex, primitiveCount * 3, instanceCount);
        else
            glDrawArrays(GL_TRIANGLES, baseVertexIndex, primitiveCount * 3);
    }

    glDeleteVertexArrays(1, &vao);
    delete program;
}

} /* namespace tc */
