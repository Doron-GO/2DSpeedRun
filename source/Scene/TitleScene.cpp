#include"Transition/TileTransitor.h"
#include"../Object/Time/DeltaTime.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SceneMng.h"

#pragma region ������

const std::string PATH_SOUND_CURSOR = PATH_SOUND + "Cursor.mp3";
const std::string PATH_SOUND_PUSH = PATH_SOUND + "Push.mp3";
const std::string PATH_IMG_PLAYER_NUM_SELECT = PATH_UI + "Select.png";
const std::string PATH_IMG_TITLE = PATH_UI + "Title.png";
const std::string PATH_IMG_START = PATH_UI + "Start.png";


#pragma endregion


#pragma region �p�����[�^

//�摜�ǂݍ���
// �v���C���[�l���I���摜
//��������
const int IMG_P_SELECT_ALLNUM = 4;
//�����摜�̉��̖���
const int IMG_P_SELECT_XNUM = 1;
//�����摜�̏c�̖���
const int IMG_P_SELECT_YNUM = 4;
//���T�C�Y
const int IMG_P_SELECT_XSIZE = 266;
//�c�T�C�Y
const int IMG_P_SELECT_YSIZE = 36;


//�v���C���[�l���I���摜�p�����[�^
// 
//�摜�̍��W
const Vector2DFloat POS_P_SELECT = { 800.0f, 200.0f };

const float OFFSET_P_SELECT = 166.0f;
//�摜�̒��S���W
const Vector2DFloat CENTER_POS_P_SELECT = { 133.0f, 20.0f };
//�g�嗦
const double EXRATE_P_SELECT = 3.0;
//��]
const double ANGLE_P_SELECT = 0.0;

//�X�^�[�g�摜�p�����[�^
// 
//�摜�̍��W
const Vector2DFloat POS_START = { 800.0f, 800.0f };

//�摜�̒��S���W
const Vector2DFloat CENTER_POS_START = { 350.0f, 20.0f };
//�g�嗦
const double EXRATE_START = 1.0;
//��]
const double ANGLE_START = 0.0;


//�ŏ��v���C�l��
constexpr int PLAY_MODE_MIN = 1;

#pragma endregion



TitleScene::TitleScene(SceneMng& manager, int number_of_Players, Transitor& transit) :Scene(manager, number_of_Players,transit),
playModeNum_(1),startFlag_(false)
{
	
	//�摜�̓ǂݍ���
	img_.emplace(IMG_TYPE::TITLE, LoadGraph(PATH_IMG_TITLE.c_str()));
	img_.emplace(IMG_TYPE::START, LoadGraph(PATH_IMG_START.c_str()));
	LoadDivGraph(PATH_IMG_PLAYER_NUM_SELECT.c_str(),
		IMG_P_SELECT_ALLNUM,
		IMG_P_SELECT_XNUM,
		IMG_P_SELECT_YNUM,
		IMG_P_SELECT_XSIZE,
		IMG_P_SELECT_YSIZE,
		selectImg_);

	//���̓ǂݍ���
	sounds_.emplace(SOUND_TYPE::CURSOR,LoadSoundMem(PATH_SOUND_CURSOR.c_str()));
	sounds_.emplace(SOUND_TYPE::PUSH,LoadSoundMem(PATH_SOUND_PUSH.c_str()));
	//�g�����W�V�����̊J�n
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
	//�^�C�g����ʂŃ{�^���������ꂽ��Z���N�g��ʂ֍s��
	if (startFlag_)
	{
		//���{�^���������ꂽ��
		if (input.IsTriggerd("down"))
		{
			if (playModeNum_ < GetJoypadNum())
			{
				//�����o���āA�v���C���[�h�̃J�[�\���𓮂���
				playModeNum_++;
				PlaySoundMem(sounds_[SOUND_TYPE::CURSOR], DX_PLAYTYPE_BACK);
			}
		}
		//��{�^���������ꂽ��
		if (input.IsTriggerd("up"))
		{
			//�����o���āA�v���C���[�h�̃J�[�\���𓮂���
			if (playModeNum_ > PLAY_MODE_MIN)
			{
				playModeNum_--;
				PlaySoundMem(sounds_[SOUND_TYPE::CURSOR], DX_PLAYTYPE_BACK);

			}
		}
		if (input.IsTriggerd("c"))
		{
			//����{�^���������ꂽ��A�Q�[���V�[���Ɉڍs
			PlaySoundMem(sounds_[SOUND_TYPE::PUSH], DX_PLAYTYPE_BACK);
			sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, playModeNum_, sceneTransitor_));
			return;
			
		}
	}	
	else
	{
		if (input.IsTriggerd("c"))
		{
			//���[�h�Z���N�g��ʂɐ؂�ւ���
			startFlag_ = true;
			PlaySoundMem(sounds_[SOUND_TYPE::PUSH], DX_PLAYTYPE_BACK);

		}
	}
	sceneTransitor_.Update();
}

void TitleScene::Draw()
{	
	ClearDrawScreen();
	//�^�C�g����ʂŃ{�^����������Ă��Ȃ����
	if (!startFlag_)
	{	//�^�C�g����ʂ̕`��
		DrawGraph(0, 0, img_[IMG_TYPE::TITLE], true);
	}
	if (startFlag_)
	{	
		//�X�^�[�g�{�^���̕`��
		DrawRotaGraph2F(
			POS_START.x,
			POS_START.y,
			CENTER_POS_START.x,
			CENTER_POS_START.y,
			EXRATE_START,
			ANGLE_START,
			img_[IMG_TYPE::START], true);

		//���ݐڑ�����Ă���R���g���[���̐�
		int joyPadNum = GetJoypadNum();

		//�ڑ�����Ă���R���g���[���̐����A�ΐ탂�[�h�̃v���C�l���̍ő吔�𑝂₷
		for (int playModeType=0;playModeType< joyPadNum ;playModeType++)
		{
			//�J�[�\���őI������Ă��郂�[�h�̉摜�����点��
			if ((playModeType+1 == playModeNum_))
			{						
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			//����ȊO�͈Â�����
			else
			{			
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
			}
			//���[�h�I���摜
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

