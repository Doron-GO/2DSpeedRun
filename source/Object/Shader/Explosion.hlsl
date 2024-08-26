#include"./Pixel/PixelShader2DHeader.hlsli"

cbuffer cbParam : register(b4)
{
    float4 g_color_;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	//UV座標とテクスチャを参照して、最適な色を取得する
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
	//アルファテスト描画しない
    if (srcCol.a < 0.01f)
    {
        discard;
    }
	//float gray = dot(srcCol.rgb, float3(0.299, 0.587, 0.114));
    float gray = (srcCol.r + srcCol.g + srcCol.b) / 3.0f;
    return gray;
}