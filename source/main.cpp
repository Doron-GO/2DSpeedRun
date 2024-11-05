#include<DxLib.h>
#include"Scene/Transition/IrisTransitor.h"
#include"_debug/_DebugDispOut.h"
#include"_debug/_DebugConOut.h"
#include"Scene/TitleScene.h"
#include"Scene/GameScene.h"
#include"Scene/SceneMng.h"
#include"Input/Input.h"
using namespace std;

static constexpr int PAD_NUM = 1;


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#pragma region DxLib初期化

	SetGraphMode(1600, 1000, 32);

	//ウィンドウサイズを変更できるようにする
	SetWindowSizeChangeEnableFlag(true);

	ChangeWindowMode(true);

	_dbgSetup(1200, 800, 256);

	SetDrawScreen(DX_SCREEN_BACK);

	if (DxLib_Init() == -1)
	{
		return -1;
	}


#pragma endregion

	//シーンマネージャーの生成
	SceneMng sceneManager;
	
	//トランスジッターの生成
	IrisTransitor irisTransitor;

	//初期シーンはタイトルシーンに設定
	sceneManager.ChangeScene(make_shared<TitleScene>(sceneManager,0, irisTransitor));

	//インプットの生成
	Input input;

	//ゲームループ
	while (ProcessMessage() != -1&&CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		input.Update(PAD_NUM);
		sceneManager.Update(input);
		sceneManager.Draw();
		_dbgDraw();

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}



