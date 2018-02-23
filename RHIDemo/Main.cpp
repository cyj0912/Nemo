#include <Platform.h>
#include <RHI.h>
#include <SDL.h>

using namespace tc;

const char demoVertexShader[] = "#version 410\n"
        "layout (location = 0) in vec3 VertexPosition;\n"
        "layout (location = 1) in vec3 VertexColor;\n"
        "out vec3 interpColor;\n"
        "void main(void)\n"
        "{\n"
        "    interpColor = VertexColor;\n"
        "    gl_Position = vec4(VertexPosition, 1.0);\n"
        "}";

const char demoFragmentShader[] = "#version 410\n"
        "layout (location = 0) out vec4 FragColor;\n"
        "in vec3 interpColor;\n"
        "void main(void)\n"
        "{\n"
        "    FragColor = vec4(interpColor, 1);\n"
        "}";

const char blueFragmentShader[] = "#version 410\n"
        "layout (location = 0) out vec4 FragColor;\n"
        "void main(void)\n"
        "{\n"
        "    FragColor = vec4(0, 0, 1, 1);\n"
        "}";

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    auto* window = SDL_CreateWindow("RHI Demo",
                                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    1600, 900, SDL_WINDOW_OPENGL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

#if TC_OS == TC_OS_WINDOWS_NT
    gladLoadGL();
#endif

    printf("%s", GRHI->GetRHIVersionString());

    //No swapchain on opengl, need to modify when using dx11
    //GRHI->CreateViewport(nullptr, 1600, 900, false, EPixelFormat::Unknown);

    auto demoVS = GRHI->CreateVertexShader(demoVertexShader, sizeof(demoVertexShader));
    auto demoPS = GRHI->CreatePixelShader(demoFragmentShader, sizeof(demoFragmentShader));
    auto bluePS = GRHI->CreatePixelShader(blueFragmentShader, sizeof(blueFragmentShader));

    float triangleVertbuffer[] = {0, 0, 0, 1, 0, 0, 1, 1, 0,
                                  0, 0, 0, 1, 1, 0, 0, 1, 0};
    float triangleColorBuffer[] = {0, 1, 0, 1, 0, 0, 0, 0, 1,
                                  0, 0, 0, 1, 1, 0, 0, 1, 0};
    FSubresourceData createData{};
    createData.BufferDataPtr = triangleVertbuffer;
    auto vertBuffer = GRHI->CreateVertexBuffer(sizeof(triangleVertbuffer), EBufferUsageFlags::Static, createData);
    createData.BufferDataPtr = triangleColorBuffer;
    auto colorBuffer = GRHI->CreateVertexBuffer(sizeof(triangleColorBuffer), EBufferUsageFlags::Static, createData);

    FInputElementDesc inputElementDesc[2] = {
            {SN_POSITION, 0, IEF_Float3, 0, 0, false, 0},
            {SN_POSITION, 1, IEF_Float3, 1, 0, false, 0}
    };

    auto inputLayout = GRHI->CreateInputLayout(inputElementDesc, 2);

    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = true;
        }
        GRHI->BeginFrame();

        GRHI->SetVertexShader(demoVS);
        GRHI->SetPixelShader(demoPS);
        GRHI->SetInputLayout(inputLayout);
        GRHI->SetVertexBuffer(0, vertBuffer, sizeof(float) * 3, 0);
        GRHI->SetVertexBuffer(1, colorBuffer, sizeof(float) * 3, 0);
        GRHI->DrawPrimitive(EPrimitiveType::TriangleList, 0, 1, 0);

        GRHI->SetPixelShader(bluePS);
        GRHI->DrawPrimitive(EPrimitiveType::TriangleList, 3, 1, 0);

        GRHI->EndFrame();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);

    return 0;
}
