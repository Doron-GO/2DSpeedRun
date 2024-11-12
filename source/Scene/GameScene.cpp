#include<DxLib.h>
#include"../Manager/ImageMng.h"
#include"Transition/TileTransitor.h"
#include"../Object/Time/TimeCount.h"
#include"../Object/Time/DeltaTime.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "SceneMng.h"
#include"../Config.h"

#pragma region パス

const std::string PATH_SOUND_READY = PATH_SOUND + "Ready.mp4";
const std::string PATH_SOUND_CURSOR = PATH_SOUND + "Cursor.mp3";
const std::string PATH_SOUND_GO = PATH_SOUND + "Go.mp3";
const std::string PATH_SOUND_PUSH = PATH_SOUND + "Push.mp3";
const std::string PATH_IMG_GO = PATH_UI+ "Go.png";
const std::string PATH_IMG_READY = PATH_UI+ "Ready.png";
const std::string PATH_IMG_MODESELECT = PATH_UI+ "Title_or_OneMore.png";

#pragma endregion

#pragma region パラメータ


//画像読み込み
// 選択画像
//分割枚数
const int IMG_SELECT_MODE_ALLNUM = 2;
//分割画像の横の枚数
const int IMG_SELECT_MODE_XNUM = 1;
//分割画像の縦の枚数
const int IMG_SELECT_MODE_YNUM = 2;
//横サイズ
const int IMG_SELECT_MODE_XSIZE = 420;
//縦サイズ
const int IMG_SELECT_MODE_YSIZE = 40;


//シングルプレイモード
constexpr int SINGLE_PLAY_MODE = 1;

constexpr int SELECT_PLAY_MODE_MAX = 2;

//レディ画像表示開始時間
constexpr float TIME_READY_MIN = 0.6f;
//レディ画像表示終了時間
constexpr float TIME_READY_MAX = 1.5f;

//ゴー画像表示終了時間
constexpr float TIME_GO_MIN = 1.5f;
//ゴー画像表示終了時間
constexpr float TIME_GO_MAX = 2.0f;


//レディ画像
//画像の座標
const Vector2DFloat POS_READY = { 800.0f, 333.3f };
//画像の中心座標
const Vector2DFloat CENTER_POS_READY = { 288.0f, 33.0f };
//拡大率
const double EXRATE_READY = 2.0;
//回転
const double ANGLE_READY = 0.0;
//カウントにかける数字
const float RATE_COUNT_READY = 40.0f;

//ゴー画像
//画像の座標
const Vector2DFloat POS_GO = { 800.0f, 333.3f };
//画像の中心座標
const Vector2DFloat CENTER_POS_GO = { 225.0f, 80.0f };
//拡大率
const double EXRATE_GO = 1.0;
//回転
const double ANGLE_GO = 0.0;


//モードセレクト画像
//画像の座標
const Vector2DFloat POS_SELECT_MODE = { 800.0f, 800.0f };
//オフセット値
const float OFFSET_SELECT_MODE = 100.0f;
//画像の中心座標
const Vector2DFloat CENTER_POS_SELECT_MODE = { 210.0f, 40.0f };
//拡大率
const double EXRATE_SELECT_MODE = 2.0;
//回転
const double ANGLE_SELECT_MODE = 0.0;

//スタートタイム終了判定に使う定数
const float PLUS_TIME = 2.0f;


#pragma endregion


GameScene::GameScene(SceneMng& manager, int number_of_Players, Transitor& transit):Scene(manager, number_of_Players, transit),
playType_(number_of_Players),_update(&GameScene::UpdateBattleMode)
{	
	//トランジションのスタート
	sceneTransitor_.Start();
	
	//オブジェクトの生成
	CreateObject();

	//プレイモードをシングルプレイに変更
	if (playType_ == SINGLE_PLAY_MODE)
	{
		ChangeSinglePlayMode();
	}
	isReStart_ = false;

	//音関係の読み込み
	InitSound();

	//画像の読み込み
	InitImage();

	//初期カーソル位置は、タイトルへ戻る
	selectModeType_ = static_cast<int>(SELECT_MODE::TITLE) ;
	//カメラを一回アップデートする
	camera_->Update();
	//デルタタイムのリセット
	deltaTime.Reset();
	//ゲームスタート開始計測用変数の初期化
	startTime_ = deltaTime.GetElapsedTime();

}

GameScene::~GameScene()
{
	for (auto& img : img_)
	{
		DeleteGraph(img.second);
	}
	img_.clear();
	for (int num = 0;num< IMG_SELECT_NUM;num++)
	{
		DeleteGraph(selectImg_[num]);
	}
	for (auto& sound : sounds_)
	{
		DeleteSoundMem(sound.second);	
	}
	sounds_.clear();
}

void GameScene::ChangeSinglePlayMode(void)
{
	//シングルプレイモードにする
	outSide_->SetSinglePlayMode();
	playerManager_->SetSinglePlayMode();
	checkPoint_->SetSinglePlayMode();
	timeCount_->SetSinglePlayMode();
	_update = &GameScene::UpdateSingleMode;

}

void GameScene::CreateObject(void)
{
	//カメラの生成
	camera_ = std::make_unique<Camera>();
	//ステージの生成
	stage_ = std::make_unique<Stage>();
	//死亡範囲クラスの生成
	outSide_ = std::make_unique<OutSide>(*camera_, playType_);
	//プレイやーマネージャーの生成
	playerManager_ = std::make_unique<PlayerManager>(outSide_->conclusion_, *stage_->GetBlocks());
	//プレイやーマネージャーの初期化
	playerManager_->Init(playType_, stage_->GetColList(), stage_->GetWallColList(), stage_->GetWireColList());
	//ステージの初期化
	stage_->Init(playerManager_->GetPlayers());
	//チェックポイントの生成
	checkPoint_ = std::make_unique<CheckPoint>(playerManager_->GetPlayers(), stage_->CheckPointGetColList());
	//時間計測の生成
	timeCount_ = std::make_unique<TimeCount>(*checkPoint_);
	//カメラをプレイヤーに接続
	camera_->ReConnect(playerManager_->GetPlayers()[(int)playerManager_->GetNewFirstPlayerNum()]);
}

void GameScene::DrawStartUI(void)
{
	auto count = deltaTime.GetElapsedTime();

	//レディ画像描画カウントが開始時間を超えていて、かつ終了時間以内なら描画
	if ((startTime_ + TIME_READY_MIN < count) && count <= startTime_ + TIME_READY_MAX)
	{
		//レディ効果音が流れていなかったら流す
		if (!CheckSoundMem(sounds_[SOUND_TYPE::READY]))
		{
			PlaySoundMem(sounds_[SOUND_TYPE::READY], DX_PLAYTYPE_BACK, true);
		}
		//画像描画
		DrawRotaGraph2F(
			POS_READY.x,
			POS_READY.y -( count * RATE_COUNT_READY),
			CENTER_POS_READY.x,
			CENTER_POS_READY.y,
			EXRATE_READY,
			ANGLE_READY, 
			img_[IMG_TYPE::READY], true);
	}

	//ゴー画像描画カウントが開始時間を超えていて、かつ終了時間以内なら描画
	if (!(startTime_ + TIME_GO_MIN > count) && count <= startTime_ + TIME_GO_MAX)
	{
		//ゴー効果音が流れていなかったら流す
		if (!CheckSoundMem(sounds_[SOUND_TYPE::GO]))
		{
			PlaySoundMem(sounds_[SOUND_TYPE::GO], DX_PLAYTYPE_BACK, true);
		}
		//画像描画
		DrawRotaGraph2F(
			POS_GO.x,
			POS_GO.y,
			CENTER_POS_GO.x,
			CENTER_POS_GO.y,
			EXRATE_GO,
			ANGLE_GO, 
			img_[IMG_TYPE::GO], true);
	}

}

void GameScene::InitSound(void)
{
	//効果音の読み込み
	sounds_.emplace(SOUND_TYPE::CURSOR, LoadSoundMem(PATH_SOUND_CURSOR.c_str()));
	sounds_.emplace(SOUND_TYPE::PUSH, LoadSoundMem(PATH_SOUND_PUSH.c_str()));
	sounds_.emplace(SOUND_TYPE::GO, LoadSoundMem(PATH_SOUND_GO.c_str()));
	sounds_.emplace(SOUND_TYPE::READY, LoadSoundMem(PATH_SOUND_READY.c_str()));
}

void GameScene::InitImage(void)
{
	//画像の読み込み
	img_.emplace(IMG_TYPE::GO, LoadGraph(PATH_IMG_GO.c_str()));
	img_.emplace(IMG_TYPE::READY, LoadGraph(PATH_IMG_READY.c_str()));
	LoadDivGraph(
		PATH_IMG_MODESELECT.c_str(),
		IMG_SELECT_MODE_ALLNUM,
		IMG_SELECT_MODE_XNUM,
		IMG_SELECT_MODE_YNUM,
		IMG_SELECT_MODE_XSIZE,
		IMG_SELECT_MODE_YSIZE,
		selectImg_);

}

void GameScene::Update(Input& input)
{
	deltaTime.update();
	float elapsed = deltaTime.GetElapsedTime();
	(this->*_update)(input, elapsed);
	//勝敗が付いた、もしくはゴールしたら
	if (outSide_->conclusion_ || checkPoint_->IsGoal())
	{
		SelectTitleOrGame(input);
	}
	if (!isReStart_)
	{	
		sceneTransitor_.Update();
	}
}

void GameScene::Draw()
{
	//カメラ座標
	auto& cameraPos = camera_->GetPos();
	ClearDrawScreen();
	//ステージ描画
	stage_->Draw(cameraPos);
	//死亡範囲描画
	outSide_->Draw(cameraPos);
	//プレイヤーの描画
	playerManager_->Draw(cameraPos);
	//時間計測の描画
	timeCount_->Draw();
	//ゲーム開始時UIの描画
	DrawStartUI();
	//ゲーム終了時UIの描画
	DrawSelectModeUI();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	sceneTransitor_.Draw();
}

void GameScene::DecideOnTheBeginning()
{
	//先頭のプレイヤーを検索
	playerManager_->SearchFirstPlayer(checkPoint_->GetCheckPoint());
	//現在の先頭
	auto newLeder= playerManager_->GetNewFirstPlayerNum();
	//1フレーム前の先頭
	auto oldLeder =playerManager_->GetOldFirstPlayerNum();
	//前のフレームの先頭プレイヤーと今の先頭プレイヤーが違っていたら、
	//カメラ追従対象を変更する。
	if (oldLeder != newLeder)
	{
		//カメラを先頭プレイヤーに接続
		camera_->ReConnect(playerManager_->GetPlayers()[static_cast<int>(newLeder)]);
		//カメラのステートを切り替え状態にする
		camera_->StateChanging(static_cast<int>(newLeder));
		//死亡範囲はステートを切り替え状態にする
		outSide_->StateChanging();
	}
	//先頭プレイヤーの番号の更新
	playerManager_->UpdateFirstPlayerType();
}

void GameScene::SelectTitleOrGame(Input& input)
{
	int titleMode = static_cast<int>(SELECT_MODE::TITLE);
	int gameMode = static_cast<int>(SELECT_MODE::GAME);
	//下ボタンを押されたとき、下にカーソルを動かせるなら
	if (input.IsTriggerd("down") && selectModeType_ < gameMode)
	{
		//効果音を鳴らして、カーソルを動かす
		selectModeType_++;
		PlaySoundMem(sounds_[SOUND_TYPE::CURSOR], DX_PLAYTYPE_BACK);
	}
	//上ボタンを押されたとき、上にカーソルを動かせるなら
	else if (input.IsTriggerd("up") && selectModeType_ > titleMode)
	{
		//効果音を鳴らして、カーソルを動かす
		selectModeType_--;
		PlaySoundMem(sounds_[SOUND_TYPE::CURSOR], DX_PLAYTYPE_BACK);

	}
	//決定ボタンが押されたとき、
	else if (input.IsTriggerd("c"))
	{
		//カーソルがタイトルへなら効果音を鳴らして、タイトルシーンに移行
		if (selectModeType_ == titleMode)
		{
			isReStart_ = true;
			PlaySoundMem(sounds_[SOUND_TYPE::PUSH], DX_PLAYTYPE_BACK);
			sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, playType_, sceneTransitor_));
			return;
		}
		//違うなら効果音を鳴らして、もう一戦
		else
		{
			isReStart_ = true;
			PlaySoundMem(sounds_[SOUND_TYPE::PUSH], DX_PLAYTYPE_BACK);
			sceneManager_.ChangeScene(std::make_shared<TitleScene>(sceneManager_, 0, sceneTransitor_));
			return;
		}
	}
}

void GameScene::DrawSelectModeUI(void)
{
	//対戦が終了している、もしくは、ゴールしていたら
	if (outSide_->conclusion_ || checkPoint_->IsGoal())
	{
		//セレクトモードの最大値
		int selectModeMax = static_cast<int>(SELECT_MODE::MAX);
		
		//タイトル外面へ、もう一戦画像を描画
		for (int selectModeType = 0; selectModeType < selectModeMax; selectModeType++)
		{
			//現在選択されているモード画像のみ光らせる
			if ((selectModeType == selectModeType_))
			{
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			//そうじゃなければ暗くする
			else
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
			}
			DrawRotaGraph2F(
				POS_SELECT_MODE.x,
				POS_SELECT_MODE.y + ( OFFSET_SELECT_MODE * selectModeType) ,
				CENTER_POS_SELECT_MODE.x,
				CENTER_POS_SELECT_MODE.y,
				EXRATE_SELECT_MODE,
				ANGLE_SELECT_MODE,
				selectImg_[selectModeType], true);
		}
	}

}

void GameScene::UpdateBattleMode(Input& input, float elapsedTime)
{
	//カメラのアップデート
	camera_->Update();
	//死亡範囲のアップデート
	outSide_->Update(playerManager_->GetPlayers());
	//ステージのアップデート
	stage_->Update();
	//スタート演出が終わったらアップデートを開始する
	if (elapsedTime >= startTime_+ PLUS_TIME)
	{		
		//タイムカウントのアップデート
		timeCount_->Update(elapsedTime);
		//チェックポイントのアップデート
		checkPoint_->Update();
		//プレイヤーのアップデート
		playerManager_->Update(input);
		//先頭のプレイヤーを決める処理
		DecideOnTheBeginning();
	}
}

void GameScene::UpdateSingleMode(Input& input, float elapsedTime)
{
	//カメラのアップデート
	camera_->Update();
	//死亡範囲のアップデート
	outSide_->Update(playerManager_->GetPlayers());
	//ステージのアップデート
	stage_->Update();
	//ステート時演出が終わっていたらアップデートを開始
	if (elapsedTime >= startTime_+ PLUS_TIME)
	{		
		//タイムカウントのアップデート
		timeCount_->Update(elapsedTime);
		//ゴールに到達したら
		if (checkPoint_->IsGoal())
		{
			//プレイヤーをゴール状態に変更
			playerManager_->SetGoalSingleMode();
		}
		//チェックポイントのアップデート
		checkPoint_->Update();
		//プレイヤーのアップデート
		playerManager_->Update(input);
		//先頭を探す処理
		DecideOnTheBeginning();	
	}
}


