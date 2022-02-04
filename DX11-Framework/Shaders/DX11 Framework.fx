//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

Texture2D txDiffuse : register( t0 );
Texture2D txNrms : register( t1 );
SamplerState samLinear : register( s0 );

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
struct SurfaceInfo
{
    float4 AmbientMtrl;
    float4 DiffsueMtrl;
    float4 SpecularMtrl;
};

struct Light
{
    float4 AmbientLight;
    float4 DiffuseLight;
    float4 SpecularLight;

    float SpecularPower;
    float3 LightVecW;
};

cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
    SurfaceInfo surfaceInfo;
    Light light;
    float3 EyePosW; // camera position in world space
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos  : SV_POSITION;
    float3 Norm : NORMAL;
    float3 PosW : POSITION;
};

struct PS_INPUT
{
    float4 Pos  : SV_POSITION;
    float3 Norm : NORMAL;
    float3 PosW : POSITION;
    float2 Tex  : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
// Vertex Shader - Transforms the position of the object to projection space and transforms
// the normals to world space
//------------------------------------------------------------------------------------

PS_INPUT VS(float4 Pos : POSITION, float3 NormalL : NORMAL, float2 Tex : TEXCOORD0)
{
    PS_INPUT output = (PS_INPUT) 0;
    
    // output.Pos is currently the position in world space
    output.Pos = mul(Pos, World);
    output.PosW = output.Pos;
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);

    // Convert from local space to world space
    // W component of vector is 0 as vectors cannot be translated
    float3 normalW = mul(float4(NormalL, 0.0f), World).xyz;
    output.Norm = normalize(normalW);
    
    output.Tex = Tex;
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader - Calculate the color of the object per pixel using local illumiation model
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input ) : SV_Target
{  
    input.Norm = normalize(input.Norm);

    // Compute the vector from the vertex to the eye position. 
    float3 toEye = normalize(EyePosW - input.PosW);
    
    // Compute Colour
    float diffuseAmount = max(dot(light.LightVecW, input.Norm), 0.0f);

    // Compute the reflection vector
    float3 r = reflect(-light.LightVecW, input.Norm);
    
    float4 textureColour = txDiffuse.Sample(samLinear, input.Tex);

    // Determine how much specular light makes it into the eye
    float specularAmount = pow(max(dot(r, toEye), 0.0f), light.SpecularPower);

    // Compute diffuse and ambient lighting
    float ambient = light.AmbientLight * surfaceInfo.AmbientMtrl;
    float3 diffuse = diffuseAmount * (surfaceInfo.DiffsueMtrl * light.DiffuseLight).rgb;

    // Calculate specular lighting
    float3 specular = specularAmount * (surfaceInfo.SpecularMtrl * light.SpecularLight).rgb;

    // Sum all the terms together
    float4 Color;
    Color.rgb = ambient + specular + diffuse;
    Color.a = surfaceInfo.DiffsueMtrl.a;

    // return the color
    return textureColour * Color;
}
