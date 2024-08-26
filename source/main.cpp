#include<DxLib.h>
#include"Scene/SceneMng.h"
#include"Scene/GameScene.h"
#include"Scene/TitleScene.h"
#include"Scene/Transition/IrisTransitor.h"
#include"Input/Input.h"
using namespace std;
#include"_debug/_DebugConOut.h"
#include"_debug/_DebugDispOut.h"



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SetGraphMode(1600, 1000, 32);			//640×480ドットの32bitカラーに設定
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	_dbgSetup(1200, 800, 256);

	SetDrawScreen(DX_SCREEN_BACK);
	SceneMng sceneManager;
	IrisTransitor irisTransitor;
	sceneManager.ChangeScene(make_shared<TitleScene>(sceneManager,0, irisTransitor));
	Input input;

	while (ProcessMessage() != -1&&CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		input.Update(1);
		sceneManager.Update(input);
		sceneManager.Draw();
		_dbgDraw();

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}



