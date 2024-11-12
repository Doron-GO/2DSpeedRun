#include<DxLib.h>
#include"../Manager/ImageMng.h"
#include"Transition/TileTransitor.h"
#include"../Object/Time/TimeCount.h"
#include"../Object/Time/DeltaTime.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "SceneMng.h"
#include"../Config.h"

#pragma region �p�X

const std::string PATH_SOUND_READY = PATH_SOUND + "Ready.mp4";
const std::string PATH_SOUND_CURSOR = PATH_SOUND + "Cursor.mp3";
const std::string PATH_SOUND_GO = PATH_SOUND + "Go.mp3";
const std::string PATH_SOUND_PUSH = PATH_SOUND + "Push.mp3";
const std::string PATH_IMG_GO = PATH_UI+ "Go.png";
const std::string PATH_IMG_READY = PATH_UI+ "Ready.png";
const std::string PATH_IMG_MODESELECT = PATH_UI+ "Title_or_OneMore.png";

#pragma endregion

#pragma region �p�����[�^


//�摜�ǂݍ���
// �I���摜
//��������
const int IMG_SELECT_MODE_ALLNUM = 2;
//�����摜�̉��̖���
const int IMG_SELECT_MODE_XNUM = 1;
//�����摜�̏c�̖���
const int IMG_SELECT_MODE_YNUM = 2;
//���T�C�Y
const int IMG_SELECT_MODE_XSIZE = 420;
//�c�T�C�Y
const int IMG_SELECT_MODE_YSIZE = 40;


//�V���O���v���C���[�h
constexpr int SINGLE_PLAY_MODE = 1;

constexpr int SELECT_PLAY_MODE_MAX = 2;

//���f�B�摜�\���J�n����
constexpr float TIME_READY_MIN = 0.6f;
//���f�B�摜�\���I������
constexpr float TIME_READY_MAX = 1.5f;

//�S�[�摜�\���I������
constexpr float TIME_GO_MIN = 1.5f;
//�S�[�摜�\���I������
constexpr float TIME_GO_MAX = 2.0f;


//���f�B�摜
//�摜�̍��W
const Vector2DFloat POS_READY = { 800.0f, 333.3f };
//�摜�̒��S���W
const Vector2DFloat CENTER_POS_READY = { 288.0f, 33.0f };
//�g�嗦
const double EXRATE_READY = 2.0;
//��]
const double ANGLE_READY = 0.0;
//�J�E���g�ɂ����鐔��
const float RATE_COUNT_READY = 40.0f;

//�S�[�摜
//�摜�̍��W
const Vector2DFloat POS_GO = { 800.0f, 333.3f };
//�摜�̒��S���W
const Vector2DFloat CENTER_POS_GO = { 225.0f, 80.0f };
//�g�嗦
const double EXRATE_GO = 1.0;
//��]
const double ANGLE_GO = 0.0;


//���[�h�Z���N�g�摜
//�摜�̍��W
const Vector2DFloat POS_SELECT_MODE = { 800.0f, 800.0f };
//�I�t�Z�b�g�l
const float OFFSET_SELECT_MODE = 100.0f;
//�摜�̒��S���W
const Vector2DFloat CENTER_POS_SELECT_MODE = { 210.0f, 40.0f };
//�g�嗦
const double EXRATE_SELECT_MODE = 2.0;
//��]
const double ANGLE_SELECT_MODE = 0.0;

//�X�^�[�g�^�C���I������Ɏg���萔
const float PLUS_TIME = 2.0f;


#pragma endregion


GameScene::GameScene(SceneMng& manager, int number_of_Players, Transitor& transit):Scene(manager, number_of_Players, transit),
playType_(number_of_Players),_update(&GameScene::UpdateBattleMode)
{	
	//�g�����W�V�����̃X�^�[�g
	sceneTransitor_.Start();
	
	//�I�u�W�F�N�g�̐���
	CreateObject();

	//�v���C���[�h���V���O���v���C�ɕύX
	if (playType_ == SINGLE_PLAY_MODE)
	{
		ChangeSinglePlayMode();
	}
	isReStart_ = false;

	//���֌W�̓ǂݍ���
	InitSound();

	//�摜�̓ǂݍ���
	InitImage();

	//�����J�[�\���ʒu�́A�^�C�g���֖߂�
	selectModeType_ = static_cast<int>(SELECT_MODE::TITLE) ;
	//�J���������A�b�v�f�[�g����
	camera_->Update();
	//�f���^�^�C���̃��Z�b�g
	deltaTime.Reset();
	//�Q�[���X�^�[�g�J�n�v���p�ϐ��̏�����
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
	//�V���O���v���C���[�h�ɂ���
	outSide_->SetSinglePlayMode();
	playerManager_->SetSinglePlayMode();
	checkPoint_->SetSinglePlayMode();
	timeCount_->SetSinglePlayMode();
	_update = &GameScene::UpdateSingleMode;

}

void GameScene::CreateObject(void)
{
	//�J�����̐���
	camera_ = std::make_unique<Camera>();
	//�X�e�[�W�̐���
	stage_ = std::make_unique<Stage>();
	//���S�͈̓N���X�̐���
	outSide_ = std::make_unique<OutSide>(*camera_, playType_);
	//�v���C��[�}�l�[�W���[�̐���
	playerManager_ = std::make_unique<PlayerManager>(outSide_->conclusion_, *stage_->GetBlocks());
	//�v���C��[�}�l�[�W���[�̏�����
	playerManager_->Init(playType_, stage_->GetColList(), stage_->GetWallColList(), stage_->GetWireColList());
	//�X�e�[�W�̏�����
	stage_->Init(playerManager_->GetPlayers());
	//�`�F�b�N�|�C���g�̐���
	checkPoint_ = std::make_unique<CheckPoint>(playerManager_->GetPlayers(), stage_->CheckPointGetColList());
	//���Ԍv���̐���
	timeCount_ = std::make_unique<TimeCount>(*checkPoint_);
	//�J�������v���C���[�ɐڑ�
	camera_->ReConnect(playerManager_->GetPlayers()[(int)playerManager_->GetNewFirstPlayerNum()]);
}

void GameScene::DrawStartUI(void)
{
	auto count = deltaTime.GetElapsedTime();

	//���f�B�摜�`��J�E���g���J�n���Ԃ𒴂��Ă��āA���I�����Ԉȓ��Ȃ�`��
	if ((startTime_ + TIME_READY_MIN < count) && count <= startTime_ + TIME_READY_MAX)
	{
		//���f�B���ʉ�������Ă��Ȃ������痬��
		if (!CheckSoundMem(sounds_[SOUND_TYPE::READY]))
		{
			PlaySoundMem(sounds_[SOUND_TYPE::READY], DX_PLAYTYPE_BACK, true);
		}
		//�摜�`��
		DrawRotaGraph2F(
			POS_READY.x,
			POS_READY.y -( count * RATE_COUNT_READY),
			CENTER_POS_READY.x,
			CENTER_POS_READY.y,
			EXRATE_READY,
			ANGLE_READY, 
			img_[IMG_TYPE::READY], true);
	}

	//�S�[�摜�`��J�E���g���J�n���Ԃ𒴂��Ă��āA���I�����Ԉȓ��Ȃ�`��
	if (!(startTime_ + TIME_GO_MIN > count) && count <= startTime_ + TIME_GO_MAX)
	{
		//�S�[���ʉ�������Ă��Ȃ������痬��
		if (!CheckSoundMem(sounds_[SOUND_TYPE::GO]))
		{
			PlaySoundMem(sounds_[SOUND_TYPE::GO], DX_PLAYTYPE_BACK, true);
		}
		//�摜�`��
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
	//���ʉ��̓ǂݍ���
	sounds_.emplace(SOUND_TYPE::CURSOR, LoadSoundMem(PATH_SOUND_CURSOR.c_str()));
	sounds_.emplace(SOUND_TYPE::PUSH, LoadSoundMem(PATH_SOUND_PUSH.c_str()));
	sounds_.emplace(SOUND_TYPE::GO, LoadSoundMem(PATH_SOUND_GO.c_str()));
	sounds_.emplace(SOUND_TYPE::READY, LoadSoundMem(PATH_SOUND_READY.c_str()));
}

void GameScene::InitImage(void)
{
	//�摜�̓ǂݍ���
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
	//���s���t�����A�������̓S�[��������
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
	//�J�������W
	auto& cameraPos = camera_->GetPos();
	ClearDrawScreen();
	//�X�e�[�W�`��
	stage_->Draw(cameraPos);
	//���S�͈͕`��
	outSide_->Draw(cameraPos);
	//�v���C���[�̕`��
	playerManager_->Draw(cameraPos);
	//���Ԍv���̕`��
	timeCount_->Draw();
	//�Q�[���J�n��UI�̕`��
	DrawStartUI();
	//�Q�[���I����UI�̕`��
	DrawSelectModeUI();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	sceneTransitor_.Draw();
}

void GameScene::DecideOnTheBeginning()
{
	//�擪�̃v���C���[������
	playerManager_->SearchFirstPlayer(checkPoint_->GetCheckPoint());
	//���݂̐擪
	auto newLeder= playerManager_->GetNewFirstPlayerNum();
	//1�t���[���O�̐擪
	auto oldLeder =playerManager_->GetOldFirstPlayerNum();
	//�O�̃t���[���̐擪�v���C���[�ƍ��̐擪�v���C���[������Ă�����A
	//�J�����Ǐ]�Ώۂ�ύX����B
	if (oldLeder != newLeder)
	{
		//�J������擪�v���C���[�ɐڑ�
		camera_->ReConnect(playerManager_->GetPlayers()[static_cast<int>(newLeder)]);
		//�J�����̃X�e�[�g��؂�ւ���Ԃɂ���
		camera_->StateChanging(static_cast<int>(newLeder));
		//���S�͈͂̓X�e�[�g��؂�ւ���Ԃɂ���
		outSide_->StateChanging();
	}
	//�擪�v���C���[�̔ԍ��̍X�V
	playerManager_->UpdateFirstPlayerType();
}

void GameScene::SelectTitleOrGame(Input& input)
{
	int titleMode = static_cast<int>(SELECT_MODE::TITLE);
	int gameMode = static_cast<int>(SELECT_MODE::GAME);
	//���{�^���������ꂽ�Ƃ��A���ɃJ�[�\���𓮂�����Ȃ�
	if (input.IsTriggerd("down") && selectModeType_ < gameMode)
	{
		//���ʉ���炵�āA�J�[�\���𓮂���
		selectModeType_++;
		PlaySoundMem(sounds_[SOUND_TYPE::CURSOR], DX_PLAYTYPE_BACK);
	}
	//��{�^���������ꂽ�Ƃ��A��ɃJ�[�\���𓮂�����Ȃ�
	else if (input.IsTriggerd("up") && selectModeType_ > titleMode)
	{
		//���ʉ���炵�āA�J�[�\���𓮂���
		selectModeType_--;
		PlaySoundMem(sounds_[SOUND_TYPE::CURSOR], DX_PLAYTYPE_BACK);

	}
	//����{�^���������ꂽ�Ƃ��A
	else if (input.IsTriggerd("c"))
	{
		//�J�[�\�����^�C�g���ւȂ���ʉ���炵�āA�^�C�g���V�[���Ɉڍs
		if (selectModeType_ == titleMode)
		{
			isReStart_ = true;
			PlaySoundMem(sounds_[SOUND_TYPE::PUSH], DX_PLAYTYPE_BACK);
			sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, playType_, sceneTransitor_));
			return;
		}
		//�Ⴄ�Ȃ���ʉ���炵�āA�������
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
	//�ΐ킪�I�����Ă���A�������́A�S�[�����Ă�����
	if (outSide_->conclusion_ || checkPoint_->IsGoal())
	{
		//�Z���N�g���[�h�̍ő�l
		int selectModeMax = static_cast<int>(SELECT_MODE::MAX);
		
		//�^�C�g���O�ʂցA�������摜��`��
		for (int selectModeType = 0; selectModeType < selectModeMax; selectModeType++)
		{
			//���ݑI������Ă��郂�[�h�摜�̂݌��点��
			if ((selectModeType == selectModeType_))
			{
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			//��������Ȃ���ΈÂ�����
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
	//�J�����̃A�b�v�f�[�g
	camera_->Update();
	//���S�͈͂̃A�b�v�f�[�g
	outSide_->Update(playerManager_->GetPlayers());
	//�X�e�[�W�̃A�b�v�f�[�g
	stage_->Update();
	//�X�^�[�g���o���I�������A�b�v�f�[�g���J�n����
	if (elapsedTime >= startTime_+ PLUS_TIME)
	{		
		//�^�C���J�E���g�̃A�b�v�f�[�g
		timeCount_->Update(elapsedTime);
		//�`�F�b�N�|�C���g�̃A�b�v�f�[�g
		checkPoint_->Update();
		//�v���C���[�̃A�b�v�f�[�g
		playerManager_->Update(input);
		//�擪�̃v���C���[�����߂鏈��
		DecideOnTheBeginning();
	}
}

void GameScene::UpdateSingleMode(Input& input, float elapsedTime)
{
	//�J�����̃A�b�v�f�[�g
	camera_->Update();
	//���S�͈͂̃A�b�v�f�[�g
	outSide_->Update(playerManager_->GetPlayers());
	//�X�e�[�W�̃A�b�v�f�[�g
	stage_->Update();
	//�X�e�[�g�����o���I����Ă�����A�b�v�f�[�g���J�n
	if (elapsedTime >= startTime_+ PLUS_TIME)
	{		
		//�^�C���J�E���g�̃A�b�v�f�[�g
		timeCount_->Update(elapsedTime);
		//�S�[���ɓ��B������
		if (checkPoint_->IsGoal())
		{
			//�v���C���[���S�[����ԂɕύX
			playerManager_->SetGoalSingleMode();
		}
		//�`�F�b�N�|�C���g�̃A�b�v�f�[�g
		checkPoint_->Update();
		//�v���C���[�̃A�b�v�f�[�g
		playerManager_->Update(input);
		//�擪��T������
		DecideOnTheBeginning();	
	}
}


