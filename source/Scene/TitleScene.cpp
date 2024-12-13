#include"Transition/TileTransitor.h"
#include"../Object/Time/DeltaTime.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SceneMng.h"

#pragma region 初期化

const std::string PATH_SOUND_CURSOR = PATH_SOUND + "Cursor.mp3";
const std::string PATH_SOUND_PUSH = PATH_SOUND + "Push.mp3";
const std::string PATH_IMG_PLAYER_NUM_SELECT = PATH_UI + "Select.png";
const std::string PATH_IMG_TITLE = PATH_UI + "Title.png";
const std::string PATH_IMG_START = PATH_UI + "Start.png";


#pragma endregion


#pragma region パラメータ

//画像読み込み
// プレイヤー人数選択画像
//分割枚数
const int IMG_P_SELECT_ALLNUM = 4;
//分割画像の横の枚数
const int IMG_P_SELECT_XNUM = 1;
//分割画像の縦の枚数
const int IMG_P_SELECT_YNUM = 4;
//横サイズ
const int IMG_P_SELECT_XSIZE = 266;
//縦サイズ
const int IMG_P_SELECT_YSIZE = 36;


//プレイヤー人数選択画像パラメータ
// 
//画像の座標
const Vector2DFloat POS_P_SELECT = { 800.0f, 200.0f };

const float OFFSET_P_SELECT = 166.0f;
//画像の中心座標
const Vector2DFloat CENTER_POS_P_SELECT = { 133.0f, 20.0f };
//拡大率
const double EXRATE_P_SELECT = 3.0;
//回転
const double ANGLE_P_SELECT = 0.0;

//スタート画像パラメータ
// 
//画像の座標
const Vector2DFloat POS_START = { 800.0f, 800.0f };

//画像の中心座標
const Vector2DFloat CENTER_POS_START = { 350.0f, 20.0f };
//拡大率
const double EXRATE_START = 1.0;
//回転
const double ANGLE_START = 0.0;


//最小プレイ人数
constexpr int PLAY_MODE_MIN = 1;

#pragma endregion



TitleScene::TitleScene(SceneMng& manager, int number_of_Players, Transitor& transit) :Scene(manager, number_of_Players,transit),
playModeNum_(1),startFlag_(false)
{
	
	//画像の読み込み
	img_.emplace(IMG_TYPE::TITLE, LoadGraph(PATH_IMG_TITLE.c_str()));
	img_.emplace(IMG_TYPE::START, LoadGraph(PATH_IMG_START.c_str()));
	LoadDivGraph(PATH_IMG_PLAYER_NUM_SELECT.c_str(),
		IMG_P_SELECT_ALLNUM,
		IMG_P_SELECT_XNUM,
		IMG_P_SELECT_YNUM,
		IMG_P_SELECT_XSIZE,
		IMG_P_SELECT_YSIZE,
		selectImg_);

	//音の読み込み
	sounds_.emplace(SOUND_TYPE::CURSOR,LoadSoundMem(PATH_SOUND_CURSOR.c_str()));
	sounds_.emplace(SOUND_TYPE::PUSH,LoadSoundMem(PATH_SOUND_PUSH.c_str()));
	//トランジションの開始
	sceneTransitor_.Start();

}

TitleScene::~TitleScene()
{
	//
	for (int num = 0; num < 4; num++)
	{
		DeleteGraph(selectImg_[num]);
	}
	for (auto& sound : sounds_)
	{
		DeleteSoundMem(sound.second);
	}
	sounds_.clear();
}

void TitleScene::Update(Input& input)
{	
	//タイトル画面でボタンが押されたらセレクト画面へ行く
	if (startFlag_)
	{
		//下ボタンが押されたら
		if (input.IsTriggerd("down"))
		{
			if (playModeNum_ < GetJoypadNum())
			{
				//音を出して、プレイモードのカーソルを動かす
				playModeNum_++;
				PlaySoundMem(sounds_[SOUND_TYPE::CURSOR], DX_PLAYTYPE_BACK);
			}
		}
		//上ボタンが押されたら
		if (input.IsTriggerd("up"))
		{
			//音を出して、プレイモードのカーソルを動かす
			if (playModeNum_ > PLAY_MODE_MIN)
			{
				playModeNum_--;
				PlaySoundMem(sounds_[SOUND_TYPE::CURSOR], DX_PLAYTYPE_BACK);

			}
		}
		if (input.IsTriggerd("c"))
		{
			//決定ボタンが押されたら、ゲームシーンに移行
			PlaySoundMem(sounds_[SOUND_TYPE::PUSH], DX_PLAYTYPE_BACK);
			sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, playModeNum_, sceneTransitor_));
			return;
			
		}
	}	
	else
	{
		if (input.IsTriggerd("c"))
		{
			//モードセレクト画面に切り替える
			startFlag_ = true;
			PlaySoundMem(sounds_[SOUND_TYPE::PUSH], DX_PLAYTYPE_BACK);

		}
	}
	sceneTransitor_.Update();
}

void TitleScene::Draw()
{	
	ClearDrawScreen();
	//タイトル画面でボタンが押されていなければ
	if (!startFlag_)
	{	//タイトル画面の描画
		DrawGraph(0, 0, img_[IMG_TYPE::TITLE], true);
	}
	if (startFlag_)
	{	
		//スタートボタンの描画
		DrawRotaGraph2F(
			POS_START.x,
			POS_START.y,
			CENTER_POS_START.x,
			CENTER_POS_START.y,
			EXRATE_START,
			ANGLE_START,
			img_[IMG_TYPE::START], true);

		//現在接続されているコントローラの数
		int joyPadNum = GetJoypadNum();

		//接続されているコントローラの数分、対戦モードのプレイ人数の最大数を増やす
		for (int playModeType=0;playModeType< joyPadNum ;playModeType++)
		{
			//カーソルで選択されているモードの画像を光らせる
			if ((playModeType+1 == playModeNum_))
			{						
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			//それ以外は暗くする
			else
			{			
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
			}
			//モード選択画像
			DrawRotaGraph2F(
				POS_P_SELECT.x,
				POS_P_SELECT.y+(OFFSET_P_SELECT * playModeType ),
				CENTER_POS_P_SELECT.x,
				CENTER_POS_P_SELECT.y,
				EXRATE_P_SELECT,
				ANGLE_P_SELECT,
				selectImg_[playModeType],
				true);	
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	sceneTransitor_.Draw();

}

