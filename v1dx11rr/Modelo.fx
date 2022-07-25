Texture2D colorMap : register(t0);
Texture2D specMap  : register(t1);
Texture2D bumpMap  : register(t2);
SamplerState colorSampler : register(s0);

cbuffer cbChangerEveryFrame   : register(b0) { matrix worldMatrix; };

cbuffer cbNeverChanges        : register(b1) { matrix viewMatrix;  };

cbuffer cbChangeOnResize      : register(b2) { matrix projMatrix;  };

cbuffer cbChangesOccasionally : register(b3) { float3 cameraPos;   };

cbuffer cbChangesOccasionally : register(b4) { float  specIntensa; };

cbuffer cbChangesOccasionally : register(b5) { float  Factor;      };

struct VS_Input
{
    float4 pos        : POSITION;
    float2 tex0       : TEXCOORD0;
    float3 norm       : NORMAL0;
};
					  
struct PS_Input
{
    float4 pos        : SV_POSITION;
    float2 tex0       : TEXCOORD0;
    float3 norm       : TEXCOORD1;
    float3 cameraVec  : TEXCOORD2;
    float specIntensa : TEXCOORD3;
    float3 tangente   : TEXCOORD4;
    float3 binormal   : TEXCOORD5;
};

PS_Input VS_Main(VS_Input vertex)
{
    float4 worldPosition;
    PS_Input vsOut    = (PS_Input) 0;
					  
    vsOut.pos         = mul(vertex.pos, worldMatrix);
    vsOut.pos         = mul(vsOut.pos, viewMatrix);
    vsOut.pos         = mul(vsOut.pos, projMatrix);
    vsOut.tex0        = vertex.tex0;
	
	//Normal
    vsOut.norm        = normalize(mul(vertex.norm, (float3x3) worldMatrix));
    vsOut.norm        = normalize(vsOut.norm);

    // Tangente
    float3 tang;
    float3 c1         = cross(vsOut.norm, float3(0.0, 0.0, 1.0));
    float3 c2         = cross(vsOut.norm, float3(0.0, 1.0, 0.0));
    if (length(c1) > length(c2)) 
        tang = c1;
    else
        tang = c2;
    vsOut.tangente    = normalize(tang);
    
   // Binormal
    float3 binorm;
           binorm     = cross(tang, vsOut.norm);
    vsOut.binormal    = normalize(binorm);
					    
    worldPosition     = mul(vertex.pos, worldMatrix);
					    
    vsOut.cameraVec   = cameraPos.xyz - worldPosition.xyz;
    vsOut.cameraVec   = normalize(vsOut.cameraVec);

    vsOut.specIntensa = specIntensa;
    return vsOut;
}


float4 PS_Main(PS_Input pix) : SV_TARGET
{
    
    float4 colorTextura = colorMap.Sample(colorSampler, pix.tex0);
    float4 ambiente     = float4(0.2f, 0.2f, 0.2f, 1.0f);
    float4 difusa       = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 specular     = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 specularMap  = specMap.Sample(colorSampler, pix.tex0);
    float4 specFinal;

    float4 bumpmap      = bumpMap.Sample(colorSampler, pix.tex0);
           bumpmap      = (bumpmap * 2.0f) - 1.0f;
    float3 bumpnorm     = (bumpmap.x * pix.tangente) + (bumpmap.y * pix.binormal) + (bumpmap.z * pix.norm);
           bumpnorm     = normalize(bumpnorm);
	
    float3 luzDir       = -(float3(0.5f, -1.0f, 0.0f));
    float3 luzDirInv    = -luzDir;
//	float luzintensa	= saturate(dot(pix.norm, luzDir));
    float luzintensa    = saturate(dot(bumpnorm, luzDirInv) * 0.5); //bump
    float3 reflejo;    
    
    if (luzintensa > 0)
    {
        ambiente += (difusa * luzintensa);
        ambiente  = saturate(ambiente);
//		reflejo	  = normalize(2 * luzintensa * pix.norm - luzDir);	
        reflejo   = normalize(2 * luzintensa * bumpnorm - luzDirInv); //bump
        specular  = pow(saturate(dot(reflejo, pix.cameraVec)), pix.specIntensa);
        specFinal = specular * specularMap;
    }

    ambiente = ambiente * colorTextura * (Factor + 0.2f);
    float4 luzFinal = saturate(ambiente + specFinal);

    return luzFinal;
}