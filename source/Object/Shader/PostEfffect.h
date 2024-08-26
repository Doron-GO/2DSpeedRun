#pragma once
#include<DxLib.h>
class PostEfffect
{
public:

	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	// 頂点数
	static constexpr int NUM_VERTEX = 4;

	// 頂点インデックス数
	static constexpr int NUM_VERTEX_IDX = 6;

	// ポリゴン数
	static constexpr int NUM_POLYGON = 2;


	PostEfffect(int &screenID);
	~PostEfffect();

	void Update();
	void Draw();

	VERTEX2DSHADER vertexs_[NUM_VERTEX];
	// 頂点インデックス
	WORD indexes_[NUM_VERTEX_IDX];

private:

	void MakeSquereVertex(void);
	int& mainScreen_;
	int screen_;
	int ShaderHandle_;
	int constBuf_;

};

