Texture2D textures : register(t0);
Texture2D textures2 : register(t1);
SamplerState colorSampler : register(s0);

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projMatrix;
	float4 valores;
};

cbuffer cbChangesOccasionally : register(b1)
{
    float Factor;
};
struct VS_Input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);

	vsOut.tex0 = vertex.tex0;

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
	
    float3 lightDir;
    float lightIntensity = 1.0f;
    float4 color;
	
    color = float4(0.2, 0.2, 0.2, 1); // ambient color
	
    lightDir = -(float3(0.5f, -1.0f, 0.0f)); // lightDirection
	
    color += (float4(1.0f, 1.f, 1.f, 1.0f) /*diffuse color*/ * lightIntensity * (Factor + .2f));

		// Saturate the ambient and diffuse color.
    color = saturate(color);
	
	float4 Textura1 = textures.Sample(colorSampler, pix.tex0);
    float4 Textura2 = textures2.Sample(colorSampler, pix.tex0);
	
    float4 FinalColor = lerp(Textura2, Textura1, Factor);

    return FinalColor * color;
}