#include<DxLib.h>
#include"../Manager/ImageMng.h"
#include"Transition/TileTransitor.h"
#include"../Object/Time/TimeCount.h"
#include"../Object/Time/DeltaTime.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "SceneMng.h"
#include"../Config.h"


#pragma region 定数宣言

const std::string PATH_SOUND_READY = PATH_SOUND + "Ready.mp4";
const std::string PATH_SOUND_CURSOR = PATH_SOUND + "Cursor.mp3";
const std::string PATH_SOUND_GO = PATH_SOUND + "Go.mp3";
const std::string PATH_SOUND_PUSH = PATH_SOUND + "Push.mp4";
const std::string PATH_IMG_GO = PATH_UI+ "Go.png";
const std::string PATH_IMG_READY = PATH_UI+ "Ready.png";
const std::string PATH_IMG_MODESELECT = PATH_UI+ "Title_or_OneMore.png";

#pragma endregion

GameScene::GameScene(SceneMng& manager, int number_of_Players, Transitor& transit):Scene(manager, number_of_Players, transit),
playerNum_(number_of_Players),_update(&GameScene::MultiPlayUpdate)
{	
	sceneTransitor_.Start();
	StandardInit();
	if (playerNum_ ==1)
	{
		SinglePlayModeChange();
	}
	SoundInit();
	ImageInit();
	selectModeNum_ = 0;
	camera_->Update();
	deltaTime.Reset();
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

void GameScene::SinglePlayModeChange(void)
{
	outSide_->SinglePlay();
	playerManager_->SetSinglePlayMode();
	checkPoint_->SetSingleMode();
	_update = &GameScene::SinglePlayUpdate;
	timeCount_->SinglePlay();
}

void GameScene::StandardInit(void)
{
	camera_ = std::make_unique<Camera>();
	stage_ = std::make_unique<Stage>();
	outSide_ = std::make_unique<OutSide>(*camera_, playerNum_);
	playerManager_ = std::make_unique<PlayerManager>(outSide_->conclusion_, *stage_->GetBlocks());
	playerManager_->Init(playerNum_, stage_->GetColList(), stage_->GetWallColList(), stage_->GetWireColList());
	stage_->Init(playerManager_->GetPlayers());
	checkPoint_ = std::make_unique<CheckPoint>(playerManager_->GetPlayers(), stage_->CheckPointGetColList());
	timeCount_ = std::make_unique<TimeCount>(*checkPoint_);
	camera_->ReConnect(playerManager_->GetPlayers()[(int)playerManager_->GetNewFirstPlayerNum()]);
}

void GameScene::ReadyGoDraw(void)
{
	auto elapsed = deltaTime.GetElapsedTime();

	// ※※コメントとマジックナンバー要修正

	// READY中
	if (!(startTime_ + 0.6f > elapsed) && elapsed <= startTime_ + 1.5f)
	{
		if (!CheckSoundMem(sounds_[SOUND_TYPE::READY]))
		{
			PlaySoundMem(sounds_[SOUND_TYPE::READY], DX_PLAYTYPE_BACK, true);
		}

		DrawRotaGraph2F(screenSize_.x / 2.0f, 0.0f + (screenSize_.y / 3.0f) - elapsed * 40,
			288.0f, 33.0f, 2.0f, 0.0f, img_[IMG_TYPE::READY], true);
	}

	// GO
	if (!(startTime_ + 1.5f > elapsed) && elapsed <= startTime_ + 2.0f)
	{
		if (!CheckSoundMem(sounds_[SOUND_TYPE::GO]))
		{
			PlaySoundMem(sounds_[SOUND_TYPE::GO], DX_PLAYTYPE_BACK, true);
		}
		DrawRotaGraph2F(screenSize_.x / 2.0f, 0.0f + (screenSize_.y / 3.0f),
			225.0f, 80.0f, 1.0f, 0.0f, img_[IMG_TYPE::GO], true);
	}

}

void GameScene::SoundInit(void)
{
	sounds_.emplace(SOUND_TYPE::CURSOR, LoadSoundMem(PATH_SOUND_CURSOR.c_str()));
	sounds_.emplace(SOUND_TYPE::PUSH, LoadSoundMem(PATH_SOUND_PUSH.c_str()));
	sounds_.emplace(SOUND_TYPE::GO, LoadSoundMem(PATH_SOUND_GO.c_str()));
	sounds_.emplace(SOUND_TYPE::READY, LoadSoundMem(PATH_SOUND_READY.c_str()));
}

void GameScene::ImageInit(void)
{
	img_.emplace(IMG_TYPE::GO, LoadGraph(PATH_IMG_GO.c_str()));
	img_.emplace(IMG_TYPE::READY, LoadGraph(PATH_IMG_READY.c_str()));
	LoadDivGraph(PATH_IMG_MODESELECT.c_str(), 2, 1, 2, 420, 40, selectImg_);

}

void GameScene::Update(Input& input)
{
	deltaTime.update();
	auto elapsed = deltaTime.GetElapsedTime();
	(this->*_update)(input, elapsed);
	//勝敗が付いた、もしくはゴールしたら
	if (outSide_->conclusion_ || checkPoint_->IsGoal())
	{
		if (Select(input))
		{
			return;
		}
	}
	
	sceneTransitor_.Update();
}

void GameScene::Draw()
{

	// ※※コメント要必要

	auto& cameraPos = camera_->GetPos();
	ClearDrawScreen();
	stage_->Draw(cameraPos);
	outSide_->Draw(cameraPos);
	playerManager_->Draw(cameraPos);
	auto newLeder = playerManager_->GetNewFirstPlayerNum();

	// タイマーUI描画(1人モード用)
	timeCount_->Draw();

	// ゲーム進行UI描画
	ReadyGoDraw();

	// リプレイ選択描画
	SelectDraw();

	// シーン遷移時のトランジッション描画
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	sceneTransitor_.Draw();

}


void GameScene::DecideOnTheBeginning()
{

	// ※※コメント必要、static_cast<int>

	playerManager_->SearchFirstPlayer(checkPoint_->GetCheckPoint2());
	auto newLeder= playerManager_->GetNewFirstPlayerNum();
	auto oldLeder =playerManager_->GetOldFirstPlayerNum();
	
	//前のフレームの先頭プレイヤーと今の先頭プレイヤーが違っていたら、
	//カメラ追従対象を変更する。
	if (oldLeder != newLeder)
	{
		// カメラ追従対象変更処理
		camera_->ReConnect(playerManager_->GetPlayers()[static_cast<int>(newLeder)]);
		// ・・・・・・
		camera_->StateChanging((int)newLeder);
		// ・・・・・・
		outSide_->StateChanging();
	}

	// ・・・・・・
	playerManager_->UpdateFirstPlayerNum();

}

bool GameScene::Select(Input& input)
{

	// ※※コメント必要

	if (input.IsTriggerd("down"))
	{
		if (selectModeNum_ < 1)
		{
			selectModeNum_++;
			PlaySoundMem(sounds_[SOUND_TYPE::CURSOR], DX_PLAYTYPE_BACK);
		}
	}
	else if (input.IsTriggerd("up"))
	{
		if (selectModeNum_ > 0)
		{
			selectModeNum_--;
			PlaySoundMem(sounds_[SOUND_TYPE::CURSOR], DX_PLAYTYPE_BACK);
		}
	}
	else if (input.IsTriggerd("c"))
	{
		if (selectModeNum_ == 0)
		{
			PlaySoundMem(sounds_[SOUND_TYPE::PUSH], DX_PLAYTYPE_BACK);
			sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, playerNum_, sceneTransitor_));
			return true;
		}
		else 
		{
			PlaySoundMem(sounds_[SOUND_TYPE::PUSH], DX_PLAYTYPE_BACK);
			sceneManager_.ChangeScene(std::make_shared<TitleScene>(sceneManager_, 0, sceneTransitor_));
			return true;
		}
	}
	return false;	
}

void GameScene::SelectDraw(void)
{

	// ※※コメント必要

	if (outSide_->conclusion_ || checkPoint_->IsGoal())
	{
		for (int num = 0; num < 2; num++)
		{

			if ((num == selectModeNum_))
			{
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			else
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
			}
			DrawRotaGraph2F(800.0f, 800.0f + ((screenSize_.y / 10)) * num, 210.0f, 40.0f, 2.0, 0.0, selectImg_[num], true);
		}
	}

}

void GameScene::MultiPlayUpdate(Input& input, float elapsedTime)
{
	camera_->Update();
	outSide_->Update(playerManager_->GetPlayers());
	stage_->Update();
	if (elapsedTime >= startTime_+2.0f)
	{		
		timeCount_->Update(elapsedTime);
		checkPoint_->Update();
		playerManager_->Update(input);

		// ※※コメント必要
		DecideOnTheBeginning();
	}
}

void GameScene::SinglePlayUpdate(Input& input, float elapsedTime)
{
	camera_->Update();
	outSide_->Update(playerManager_->GetPlayers());
	stage_->Update();
	if (elapsedTime >= startTime_+ 2.0f)
	{
		timeCount_->Update(elapsedTime);
		if (checkPoint_->IsGoal())
		{
			playerManager_->SetGoalSingleMode();
		}
		checkPoint_->Update();
		playerManager_->Update(input);
		DecideOnTheBeginning();	
	}
}


