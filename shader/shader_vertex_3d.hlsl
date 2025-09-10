/*==============================================================================

   3D描画用頂点シェーダー [shader_vertex_3d.hlsl]
														 Author : CheeHow
														 Date   : 2025/09/09
--------------------------------------------------------------------------------

==============================================================================*/

// 定数バッファ
cbuffer VS_CNOSTANT_BUFFER : register(b0)
{
    float4x4 world;
};

cbuffer VS_CNOSTANT_BUFFER : register(b1)
{
    float4x4 view;
};

cbuffer VS_CNOSTANT_BUFFER : register(b2)
{
    float4x4 proj;
};

struct VS_IN
{
    float4 posL: POSITION0;
    float4 color : COLOR0;
};

struct VS_OUT
{
    float4 posH : SV_POSITION;
    float4 color : COLOR0;
};


//=============================================================================
// 頂点シェーダ
//=============================================================================
VS_OUT main(VS_IN vi)
{

    VS_OUT vo;

    // 座標変換
    float4 posW = mul(vi.posL, world);
    float4 posWV = mul(posW, view);
    vo.posH = mul(posWV, proj);
    
    // 上の座標変換のもう一つの書き方
    //float4x4 mtxWV = mul(world, view);
    //float4x4 mtxWVP = mul(mtxWV, proj);
    //vo.posH = mul(vi.posL, mtxWVP);

    // 一ラインで書く方法
    // vo.posH = mul(vi.posL, mul(mul(world, view), proj)); 
    
    vo.color = vi.color;
    
    return vo;
}