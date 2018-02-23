#pragma once

#include "RHIResources.h"

#include <Foundation.h>

#include "OpenGL.h"

#include <string>

namespace tc
{

static const char* ShaderEnumToString(GLenum shader)
{
    static const char* vs = "GL_VERTEX_SHADER";
    static const char* fs = "GL_FRAGMENT_SHADER";
    switch (shader)
    {
    case GL_VERTEX_SHADER:
        return vs;
    case GL_FRAGMENT_SHADER:
        return fs;
    default:
        return nullptr;
    }
}

template <unsigned int Kind, typename Super>
class TGLSLShader : public Super
{
public:
    TGLSLShader(const char* source, int length)
    {
        ShaderName = glCreateShader(Kind);
        glShaderSource(ShaderName, 1, &source, &length);
    }

    ~TGLSLShader()
    {
        glDeleteShader(ShaderName);
    }

    bool TryCompile()
    {
        glCompileShader(ShaderName);

        GLint compiled = 0;
        glGetShaderiv(ShaderName, GL_COMPILE_STATUS, &compiled);

        if (!compiled)
        {
            LOGINFO("Error compiling shader %d, target %s\n", ShaderName, ShaderEnumToString(Kind));
            GLint infoLen = 0;
            glGetShaderiv(ShaderName, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen)
            {
                char* buf = new char[infoLen];
                if (buf)
                {
                    glGetShaderInfoLog(ShaderName, infoLen, NULL, buf);
                    LOGINFO("Shader log:\n%s\n", buf);
                    delete[] buf;
                }
            }
            return false;
        }

        Compiled = true;
        return true;
    }

    bool IsCompiled() const
    {
        return Compiled;
    }

    void AttachTo(GLuint program) const
    {
        glAttachShader(program, ShaderName);
    }

private:
    GLuint ShaderName;
    bool Compiled = false;
};

typedef TGLSLShader<GL_VERTEX_SHADER, FRHIVertexShader> FGLSLVertexShader;
typedef TGLSLShader<GL_FRAGMENT_SHADER, FRHIPixelShader> FGLSLPixelShader;

class FGLSLProgram
{
    GLuint GLProgram;
public:
    static std::string ShaderHeader;

    FGLSLProgram();

    ~FGLSLProgram();

    static FGLSLProgram* CreateFromFiles(const string& vertFilename, const string& fragFilename)
    {
        return CreateFromFiles(vertFilename.c_str(), fragFilename.c_str());
    }

    static FGLSLProgram*
    CreateFromFiles(const string& vertFilename, const string& fragFilename, const string& geomFilename)
    {
        return CreateFromFiles(vertFilename.c_str(), fragFilename.c_str(), geomFilename.c_str());
    }

    static FGLSLProgram* CreateFromFiles(const char* vertFilename, const char* fragFilename);

    static FGLSLProgram* CreateFromFiles(const char* vertFilename, const char* fragFilename, const char* geomFilename);

    static FGLSLProgram* CreateFromStrings(const char* vertSrc, const char* fragSrc);

    static FGLSLProgram* CreateFromStrings(const char** vertSrcArray, int32_t vertSrcCount,
                                           const char** fragSrcArray, int32_t fragSrcCount);

    static FGLSLProgram* CreateFromShaders(FGLSLVertexShader* vs, FGLSLPixelShader* ps);

    static std::string GetShaderHeader()
    {
        return ShaderHeader;
    }

    static void SetShaderHeader(const std::string& s)
    {
        ShaderHeader = s;
    }

    // Copyright (c) 2016, Toby Chen. All rights reserved.
    // Copyright (c) 2014-2015, NVIDIA CORPORATION. All rights reserved.
    //
    // Redistribution and use in source and binary forms, with or without
    // modification, are permitted provided that the following conditions
    // are met:
    //  * Redistributions of source code must retain the above copyright
    //    notice, this list of conditions and the following disclaimer.
    //  * Redistributions in binary form must reproduce the above copyright
    //    notice, this list of conditions and the following disclaimer in the
    //    documentation and/or other materials provided with the distribution.
    //  * Neither the name of NVIDIA CORPORATION nor the names of its
    //    contributors may be used to endorse or promote products derived
    //    from this software without specific prior written permission.
    //
    // THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
    // EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    // IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    // PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
    // CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    // EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    // PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    // PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
    // OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    // (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    // OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    bool SetSourceFromFiles(const char* vertFilename, const char* fragFilename);

    bool SetSourceFromFiles(const char* vertFilename, const char* fragFilename, const char* geomFilename);

    bool SetSourceFromStrings(const char* vertSrc, const char* fragSrc);

    bool SetSourceFromStrings(const char** vertSrcArray, int32_t vertSrcCount,
                              const char** fragSrcArray, int32_t fragSrcCount);

    void SetAdditionalShader(const char* source, GLenum type);

    bool SetFromShaders(FGLSLVertexShader* vs, FGLSLPixelShader* ps);

    void Enable();

    void Disable();

    GLint GetAttribLocation(const char* attribute, bool isOptional = false);

    GLint GetUniformLocation(const char* uniform, bool isOptional = false);

    void BindTexture2D(const char* name, int32_t unit, GLuint tex);

    void BindTexture2D(GLint index, int32_t unit, GLuint tex);

    void BindTexture2DMultisample(const char* name, int32_t unit, GLuint tex);

    void BindTexture2DMultisample(GLint index, int32_t unit, GLuint tex);

    void BindTextureRect(const char* name, int32_t unit, GLuint tex);

    void BindTextureRect(GLint index, int32_t unit, GLuint tex);

    void BindTextureArray(const char* name, int32_t unit, GLuint tex);

    void BindTextureArray(GLint index, int32_t unit, GLuint tex);

    void SetUniform1i(const char* name, int32_t value);

    void SetUniform1f(const char* name, float value);

    void SetUniform2i(const char* name, int32_t x, int32_t y);

    void SetUniform2f(const char* name, float x, float y);

    void SetUniform3i(const char* name, int32_t x, int32_t y, int32_t z);

    void SetUniform3f(const char* name, float x, float y, float z);

    void SetUniform4f(const char* name, float x, float y, float z, float w);

    void SetUniform3fv(const char* name, const float* value, int32_t count = 1);

    void SetUniform4fv(const char* name, const float* value, int32_t count = 1);

    void SetUniform1i(GLint index, int32_t value);

    void SetUniform1f(GLint index, float value);

    void SetUniform2i(GLint index, int32_t x, int32_t y);

    void SetUniform2f(GLint index, float x, float y);

    void SetUniform3i(GLint index, int32_t x, int32_t y, int32_t z);

    void SetUniform3f(GLint index, float x, float y, float z);

    void SetUniform4f(GLint index, float x, float y, float z, float w);

    void SetUniform3fv(GLint index, const float* value, int32_t count = 1);

    void SetUniform4fv(GLint index, const float* value, int32_t count = 1);

    void SetUniformMatrix3fv(const GLchar* name, const GLfloat* m, int32_t count = 1, bool transpose = false);

    void SetUniformMatrix3fv(GLint index, const GLfloat* m, int32_t count = 1, bool transpose = false);

    void SetUniformMatrix4fv(const GLchar* name, const GLfloat* m, int32_t count = 1, bool transpose = false);

    void SetUniformMatrix4fv(GLint index, const GLfloat* m, int32_t count = 1, bool transpose = false);

    GLuint GetProgram() const { return GLProgram; }

    bool Relink();

protected:
    bool CheckCompileError(GLuint shader, int32_t target);

    GLuint CompileProgram(const char* vsource, const char* fsource);

    GLuint CompileProgram(
            const char** vertSrcArray, int32_t vertSrcCount,
            const char** fragSrcArray, int32_t fragSrcCount);
};

}
