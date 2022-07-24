// Defaults for number of lights.
#ifndef NUM_DIR_LIGHTS
    #define NUM_DIR_LIGHTS 3
#endif

#ifndef NUM_POINT_LIGHTS
    #define NUM_POINT_LIGHTS 0
#endif

#ifndef NUM_SPOT_LIGHTS
    #define NUM_SPOT_LIGHTS 0
#endif

//包括光照所需的结构体与函数
#include "LightingUtil.hlsl"

Texture2DArray gTreeMapArray : register(t0);

SamplerState gsamPointWrap : register(s0);
SamplerState gsamPointClamp : register(s1);
SamplerState gsamLinearWrap : register(s2);
SamplerState gsamLinearClamp : register(s3);
SamplerState gsamAnisotropicWrap : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
    float4x4 gTexTransform;
};

//绘制过程中所用的杂项常量数据
cbuffer cbPass : register(b1)
{
    float4x4 gView;
    float4x4 gInvView;
    float4x4 gProj;
    float4x4 gInvProj;
    float4x4 gViewProj;
    float4x4 gInvViewProj;
    float3 gEyePosW;
    float cbPerObjectPad1;
    float2 gRenderTargetSize;
    float2 gInvRenderTargetSize;
    float gNearZ;
    float gFarZ;
    float gTotalTime;
    float gDeltaTime;
    float4 gAmbientLight;
    float4 gFogColor;
    float gFogStart;
    float gFogRange;
    float2 cbPerObjectPad2;

    //对于每个以MaxLights为光源数量最大值的对象来说,索引[0,NUM_DIR_LIGHTS)表示的是方向光源
    //..
    Light gLights[MaxLights];
};

//每个材质都各有区别的常量数据
cbuffer cbMaterial : register(b2)
{
    float4 gDiffuseAlbedo;
    float3 gFresnelR0;
    float gRoughness;
    float4x4 gMatTransform;
};

struct VertexIn
{
    float3 PosW : POSITION;
    float2 SizeW : SIZE;
};

struct VertexOut
{
    float3 CenterW : POSITION;
    float2 SizeW : SIZE;
};

struct GeoOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 TexC : TEXCOORD;
    uint PrimID : SV_PrimitiveID;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    //直接将数据传入几何着色器
    vout.CenterW = vin.PosW;
    vout.SizeW = vin.SizeW;

    return vout;
}

//由于我们要将每个点都扩展为一个四边形(即4个顶点)，因此每次调用几个着色器最多输出4个顶点
[maxvertexcount(4)]
void GS(point VertexOut gin[1],
        uint primID : SV_PrimitiveID,
        inout TriangleStream<GeoOut> triStream)
{
    //
    // 计算精灵的局部坐标系与世界空间的相对关系，以使公交牌与y轴对齐且面向观察者
    //
    float3 up = float3(0.0f,1.0f,0.0f);
    float3 look = gEyePosW - gin[0].CenterW;
    look.y = 0.0f;  //与y轴对齐,以此使公告牌立于xz平面
    look = normalize(look);
    float3 right = cross(up,look);

    //
    //计算世界空间中三角形带的顶点(即四边形)
    //
    float halfWidth = 0.5f*gin[0].SizeW.x;
    float halfHeight = 0.5f*gin[0].SizeW.y;

    float4 v[4];
    v[0] = float4(gin[0].CenterW + halfWidth * right - halfHeight * up,1.0f);
    v[1] = float4(gin[0].CenterW + halfWidth * right + halfHeight * up,1.0f);
    v[2] = float4(gin[0].CenterW - halfWidth * right - halfHeight * up,1.0f);
    v[3] = float4(gin[0].CenterW - halfWidth * right + halfHeight * up,1.0f);

    //
    // 将四边形的顶点变换到世界空间,并将它们以三角形带的形式输出
    //
    float2 texC[4] = {
        float2(0.0f,1.0f),
        float2(0.0f,0.0f),
        float2(1.0f,1.0f),
        float2(1.0f,0.0f),
    };

    GeoOut gout;
    [unroll]
    for(int i = 0;i<4;++i){
        gout.PosH = mul(v[i],gViewProj);
        gout.PosW = v[i].xyz;
        gout.NormalW = look;
        gout.TexC = texC[i];
        gout.PrimID = primID;

        triStream.Append(gout);
    }
}

float4 PS(GeoOut pin) : SV_Target
{
    float3 uvw = float3(pin.TexC,pin.PrimID%3);
    float4 diffuseAlbedo = gTreeMapArray.Sample(gsamAnisotropicWrap,uvw) * gDiffuseAlbedo;

    #ifdef ALPHA_TEST
        // 忽略纹理alpha值 < 0.1的像素.这个测试要尽早完成
        clip(diffuseAlbedo.a - 0.1f);
    #endif

    //对法线插值可能导致其非规范化,因此需再次对它进行规范化处理
    pin.NormalW = normalize(pin.NormalW);

    //光线经表面上一点反射到观察点这一方向上的向量
    float3 toEyeW = gEyePosW - pin.PosW;
    float distToEye = length(toEyeW);
    toEyeW /= distToEye;    //规范化处理

    //光照项
    float4 ambient = gAmbientLight*diffuseAlbedo;

    const float shininess = 1.0f - gRoughness;
    Material mat = {diffuseAlbedo,gFresnelR0,shininess};
    float3 shadowFactor = 1.0f;
    float4 directLight = ComputeLighting(gLights,mat,pin.PosW,pin.NormalW,toEyeW,shadowFactor);
    float4 litColor = ambient + directLight;

    #ifdef FOG
        float fogAmount = saturate((distToEye - gFogStart) / gFogRange);
        litColor = lerp(litColor,gFogColor,fogAmount);
    #endif

    //从漫射反射率中获取alpha值的常见手段
    litColor.a = diffuseAlbedo.a;
    return litColor;
}





