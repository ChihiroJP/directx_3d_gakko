/*==============================================================================

   3D描画用ピクセルシェーダー [shader_pixel_3d.hlsl]
														 Author : CheeHow
														 Date   : 2025/09/09
--------------------------------------------------------------------------------

==============================================================================*/
struct PS_IN
{
    float4 posH : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

Texture2D tex; //texture
SamplerState samp; // texture sampler

float4 main(PS_IN pi) : SV_TARGET
{
    return tex.Sample(samp, pi.uv) * pi.color;
}
