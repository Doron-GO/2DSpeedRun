#include "TitleScene.h"
#include "GameScene.h"
#include "SceneMng.h"
#include"Transition/TileTransitor.h"
#include"../Object/Time/DeltaTime.h"

#pragma region MyRegion

const std::string PATH_SOUND_CURSOR = PATH_SOUND + "Cursor.mp3";
const std::string PATH_SOUND_PUSH = PATH_SOUND + "Push.mp3";
const std::string PATH_IMG_PLAYER_NUM_SELECT = PATH_UI + "Select.png";
const std::string PATH_IMG_TITLE = PATH_UI + "Title.png";
const std::string PATH_IMG_START = PATH_UI + "Start.png";

#pragma endregion



TitleScene::TitleScene(SceneMng& manager, int number_of_Players, Transitor& transit) :Scene(manager, number_of_Players,transit),
playModeNum_(1),startFlag_(false)
{
	sceneTransitor_.Start();
	titleImg_ = LoadGraph(PATH_IMG_TITLE.c_str());
	startImg_ = LoadGraph(PATH_IMG_START.c_str());
	LoadDivGraph(PATH_IMG_PLAYER_NUM_SELECT.c_str(), 4, 1, 4, 266, 36, selectImg_);

	sounds_.emplace(SOUND_TYPE::CURSOR,LoadSoundMem(PATH_SOUND_CURSOR.c_str()));
	sounds_.emplace(SOUND_TYPE::PUSH,LoadSoundMem(PATH_SOUND_PUSH.c_str()));

}

TitleScene::~TitleScene()
{
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

	if (startFlag_)
	{
		if (input.IsTriggerd("down"))
		{
			if (playModeNum_ < GetJoypadNum())
			{
				playModeNum_++;
				PlaySoundMem(sounds_[SOUND_TYPE::CURSOR], DX_PLAYTYPE_BACK);
			}
		}
		if (input.IsTriggerd("up"))
		{
			PlaySoundMem(sounds_[SOUND_TYPE::CURSOR], DX_PLAYTYPE_BACK);
			if (playModeNum_ > 1)
			{
				playModeNum_--;
			}
		}
		if (input.IsTriggerd("c"))
		{
			if (startFlag_)
			{
				PlaySoundMem(sounds_[SOUND_TYPE::PUSH], DX_PLAYTYPE_BACK);
				sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, playModeNum_, sceneTransitor_));
				return;
			}
		}
	}	
	if (!startFlag_)
	{
		if (input.IsTriggerd("c"))
		{
			startFlag_ = true;
			PlaySoundMem(sounds_[SOUND_TYPE::PUSH], DX_PLAYTYPE_BACK);

		}
	}
	sceneTransitor_.Update();
}

void TitleScene::Draw()
{	
	ClearDrawScreen();
	if (!startFlag_)
	{
		DrawGraph(0, 0, titleImg_, true);
	}

	if (startFlag_)
	{
		DrawRotaGraph2F(800.0f, 800.0f, 350.0f, 20.0f, 1.0, 0.0, startImg_, true);

		for (int num=0;num<GetJoypadNum();num++)
		{

			if ((num+1 == playModeNum_))
			{						
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
			}
			else
			{			
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
			}
			DrawRotaGraph2F(800.0f, 200.0f+((screenSize_.y/6))*num, 133.0f, 20.0f, 3.0, 0.0, selectImg_[num], true);	
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	sceneTransitor_.Draw();

}

