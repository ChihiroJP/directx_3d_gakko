// ============================================================================
// 3D cube [cube.cpp]
// ==========================================================================

#include "header/cube.h"
#include <DirectXMath.h>
#include "header/direct3d.h"
#include "header/shader3d.h"

static constexpr int NUM_VERTEX = 3 * 2 * 6; // Vertex

static ID3D11Buffer* g_pVertexBuffer = nullptr; // VertexBuffer

// Release は不要
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

static float g_x = 0.0f;
static float g_angle = 0.0f;
static float g_scale = 1.0f;
static float g_AccumulatedTime = 0.0f;
static float total = 5.0f;


struct Vertex3D
{
    DirectX::XMFLOAT3 position; // 座標
    DirectX::XMFLOAT4 color; // 色
   // DirectX::XMFLOAT2 uv;
};

static Vertex3D g_CubeVertex[NUM_VERTEX]
{
    // Front face (Red)
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},

    // Back face (Green)
    {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},

    // Top face (Cyan)
    {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}},
    {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}},
    {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}},
    {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}},
    {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}},

    // Bottom face (Magenta)
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 1.0f}},
    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 1.0f}},
    {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 1.0f}},
    {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 1.0f}},
    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 1.0f}},
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 1.0f}},

    // Left face (Blue)
    {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}},

    // Right face (Yellow)
    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}},
    {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}},
    {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}},
    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}},
};


void Cube_Update(double elapsed_time)
{
	g_AccumulatedTime = g_AccumulatedTime + static_cast<float>(elapsed_time);
	g_x = sinf(g_AccumulatedTime) * 4.5f; // Move left and right
	g_scale = (sinf(g_AccumulatedTime) + 1.0f) * 0.5f + 0.5f; // Scale between 0.0 and 1.5
	g_angle = g_AccumulatedTime * 0.5f; // Rotate over time
}


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

    
    g_pDevice->CreateBuffer(&bd, &sd, &g_pVertexBuffer); 
}

void Cube_Finalize(void)
{
    SAFE_RELEASE(g_pVertexBuffer);
}

void Cube_Draw(DirectX::XMMATRIX mtxWorld)
{
    Shader3d_Begin();

    

    // 頂点バッファを描画パイプライン処理
    UINT stride = sizeof(Vertex3D);
    UINT offset = 0;
    g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // ===========================================================================
    // 座標変換開始
    // =======================================================================================================================================================
    
    // ワールド座標変換行列
    // DirectX::XMMATRIX mtxWorld = DirectX::XMMatrixIdentity(); // 単位行列の作成
    // ==================================================== If want rotate use below 3 line ==================================================================
    // DirectX::XMMATRIX mtxRotateY = DirectX::XMMatrixRotationY(XMConvertToRadians());
    // DirectX::XMMATRIX mtxRotateY = DirectX::XMMatrixRotationY(g_angle);
    // DirectX::XMMATRIX mtxRotateX = DirectX::XMMatrixRotationX(g_angle * 0.5f);

    // ==================================================== If want rotate use above 3 line ==================================================================
    // 
    
    // ==================================================== move position (平行移動) =========================================================================
	// DirectX::XMMATRIX mtxPosition = DirectX::XMMatrixTranslation(0.5f, 0.0f, 0.0f); // move position (平行移動)

    // ==================================================== move position (平行移動) =========================================================================

	// ==================================================== If want scale (拡大縮小) use below line ==========================================================
	// DirectX::XMMATRIX mtxScale = DirectX::XMMatrixScaling(3.0f, 1.0f, 1.0f); // scale (拡大縮小)
    // ==================================================== If want scale (拡大縮小) use above line ==========================================================
  
    
    // DirectX::XMMATRIX mtxWorld = mtxPosition * mtxScale * mtxRotateY; // Combine ( Remember need to follow (Rotate -> Scaling -> Move)
    // DirectX::XMMATRIX mtxPosition = DirectX::XMMatrixTranslation(1.0f, 0.0f, 0.0f); // move position (平行移動)
   
    
    // 頂点シェーダにワールド座標変換行列を設定
    Shader3d_SetWorldMatrix(mtxWorld);
	

    //                                    変換終わり
    // =======================================================================================================================================================
    
    g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); 
   
    // Texture 
    // g_pContext -> PSSetShaderResources(0, 1, &g_pTexture);

    
    g_pContext->Draw(NUM_VERTEX, 0); // 面を増やすなら、vertex count (今は6)の部分を増やす 1面 = 6, 2面 = 12 etc



    
}