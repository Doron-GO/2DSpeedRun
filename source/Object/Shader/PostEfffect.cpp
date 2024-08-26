#include "PostEfffect.h"

PostEfffect::PostEfffect(int &screenID):mainScreen_(screenID)
{
	MakeSquereVertex();
	screen_ = MakeScreen(1600, 1000, true);
	ShaderHandle_ = LoadPixelShader("Object/Shader/Explosion.cso");
	constBuf_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * 1);

}

PostEfffect::~PostEfffect()
{
}

void PostEfffect::Update()
{
}

void PostEfffect::Draw()
{

	SetDrawScreen(screen_);

	//ClearDrawScreen();

	SetUsePixelShader(ShaderHandle_);

	SetUseTextureToShader(0, mainScreen_);


	// 定数バッファ
	FLOAT4* constBufsPtr2 =
		(FLOAT4*)GetBufferShaderConstantBuffer(constBuf_);

	constBufsPtr2->x = 1.0f;
	constBufsPtr2->y = 1.0f;
	constBufsPtr2->z = 1.0f;
	constBufsPtr2->w = 1.0f;

	// 定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(constBuf_);//Material_.constBuf_

	//// 定数バッファをピクセルシェーダー用定数バッファレジスタにセット
	SetShaderConstantBuffer(
		constBuf_, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	//// 描画
	DrawPolygonIndexed2DToShader(vertexs_, NUM_VERTEX, indexes_, NUM_POLYGON);

	//// メインに戻す
	SetDrawScreen(mainScreen_);
	DrawGraph(0, 0,screen_, false);



}

void PostEfffect::MakeSquereVertex(void)
{
	int cnt = 0;
	float sX = 0.0f;
	float sY = 0.0f;
	float eX = static_cast<float>(1600.0f);
	float eY = static_cast<float>(1000.0f);

	for (int i = 0; i < 4; i++)
	{
		vertexs_[i].rhw = 1.0f;
		vertexs_[i].dif = GetColorU8(255, 255, 255, 255);
		vertexs_[i].spc = GetColorU8(255, 255, 255, 255);
		vertexs_[i].su = 0.0f;
		vertexs_[i].sv = 0.0f;
	}

	// 左上
	vertexs_[cnt].pos = VGet(sX, sY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	// 右上
	vertexs_[cnt].pos = VGet(eX, sY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;
	// 右下
	vertexs_[cnt].pos = VGet(eX, eY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 1.0f;
	cnt++;
	// 左下
	vertexs_[cnt].pos = VGet(sX, eY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 1.0f;

	// 頂点インデックス
	cnt = 0;
	indexes_[cnt++] = 0;
	indexes_[cnt++] = 1;
	indexes_[cnt++] = 3;

	indexes_[cnt++] = 1;
	indexes_[cnt++] = 2;
	indexes_[cnt++] = 3;

}
