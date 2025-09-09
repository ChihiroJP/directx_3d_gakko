// ============================================================================
// 3D cube [cube.cpp]
// ==========================================================================

#include "header/cube.h"
#include <DirectXMath.h>
#include "header/direct3d.h"
#include "header/shader.h"

static constexpr int NUM_VERTEX = 3 * 2 * 6; // Vertex

static ID3D11Buffer* g_pVertexBuffer = nullptr; // VertexBuffer

// Release は不要
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

struct Vertex3D
{
    DirectX::XMFLOAT3 position; // 座標
    DirectX::XMFLOAT4 color; // 色
   // DirectX::XMFLOAT2 uv;
};

static Vertex3D g_CubeVertex[NUM_VERTEX]
{
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}}
        
};

void Cube_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    // デバイスとデバイスコンテキストの
    g_pDevice = pDevice;
    g_pContext = pContext;

    // 頂点ばふぁ生成
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(Vertex3D) * NUM_VERTEX;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = g_CubeVertex;

    
    g_pDevice->CreateBuffer(&bd, NULL, &g_pVertexBuffer); 
}

void Cube_Finalize(void)
{
    SAFE_RELEASE(g_pVertexBuffer);
}

void Cube_Draw(void)
{
    Shader_Begin();

    // 頂点バッファを描画パイプライン処理
    UINT stride = sizeof(Vertex3D);
    UINT offset = 0;
    g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // ===========================================================================
    // 座標変換開始（４回）
    // =====================================================================================================================================================
    
    // ワールド座標変換行列
    DirectX::XMMATRIX mtxWorld = DirectX::XMMatrixIdentity(); // 単位行列の作成

    // 頂点シェーダにワールド座標変換行列を設定
    Shader_SetWorldMatrix(mtxWorld);

    // ビュー座標へんかんぎょうれつの作成
    DirectX::XMMATRIX mtxView = DirectX::XMMatrixLookAtLH({2.0f, 2.0f, -5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.1f, 0.0f}); // カメラが見てる場所

    // Shader_SetViewMatrix(mtxView);
    
    // パースペクティブ行列の作成
    float fovAngleY = DirectX::XMConvertToRadians(60.0f);
    float aspectRatio = static_cast<float>(Direct3D_GetBackBufferWidth()) / Direct3D_GetBackBufferHeight();
    float nearZ = 0.01f;
    float farZ = 100.0f;
    DirectX::XMMATRIX mtxPerspective = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ); // cast として計算して、float変換する
    Shader_SetProjectionMatrix(mtxPerspective);

    //                                    変換終わり
    // ==================================================================================================================================================
    
    g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); 

    // Texture 
    // g_pContext -> PSSetShaderResources(0, 1, &g_pTexture);

    
    g_pContext->Draw(6, 0); // 面を増やすなら、vertex count (今は6)の部分を増やす 1面 = 6, 2面 = 12 etc

}