#include "UVSphere.h"

namespace tc
{

FStaticMesh* FUVSphereModelGenerator::Generate(int segments, int rings, float radius, bool generateUV)
{
    auto sphericalToCartesian = [](float r, float theta, float phi) -> Vector3
    {
        auto rSinPhi = r * sin(phi);
        return {cos(theta) * rSinPhi, sin(theta) * rSinPhi, r * cos(phi)};
    };

    float segmentGap = M_PI * 2.0f / (float)segments;
    float ringGap = M_PI / (float)(rings + 1);

    float phi = 0.0f;
    for (int i = 1; i <= rings; i++)
    {
        phi += ringGap;
        float theta = 0.0f;
        for (int j = 0; j < segments; j++)
        {
            auto position = sphericalToCartesian(1.0f, phi, theta);
            float u = theta / (M_PI * 2.0f);
            float v = phi / M_PI;
            theta += segmentGap;
        }
    }
}

} /* namespace tc */
