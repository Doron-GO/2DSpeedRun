#include"../../Object/Item/ItemBase.h"
#include"../../Object/Stage/Blocks.h"
#include "PlayerManager.h"
#include "Player.h"

#pragma region �萔�錾


const std::string IMG_WIN_PASS = "Src/Img/UIimage/Win.png";
const std::string IMG_RESTART_PASS = "Src/Img/UIimage/Restart.png";

//��������摜�p�����[�^
//���W
const float WIN_IMAGE_POS_X = 800.0f;
const float WIN_IMAGE_POS_Y = 300.0f;
//���S�̍��W
const float WIN_IMAGE_CENTER_X = 200.0f;
const float WIN_IMAGE_CENTER_Y = 20.0f;
//�傫��
const float WIN_IMAGE_SCALE = 2.0;

//�擪�v���C���[�̔ԍ�
constexpr int FIRST_PLAYER_NUM = 0;

#pragma endregion

PlayerManager::PlayerManager(bool& conclusion, Blocks& blocks):conclusion_(conclusion),blocks_(blocks)
{
	singlePlay_ = false;
	winImg_=LoadGraph(IMG_WIN_PASS.c_str());
	restertImg_ =LoadGraph(IMG_RESTART_PASS.c_str());
	goalFlag_ = false;
	newFirstPlayerNum_ = PLAYER_NUM::P_1;
	oldFirstPlayerNum_ = PLAYER_NUM::P_1;
	winner_ = 0;
}

PlayerManager::~PlayerManager()
{
	DeleteGraph(winImg_);
	DeleteGraph(restertImg_);
	
}

void PlayerManager::Init(int playerNum, ColList gruound, ColList Wall, ColList wire)
{
	for (int Num = 1; Num <= playerNum; Num++)
	{	
		std::shared_ptr<Player> player;
		player = std::make_shared<Player>(Num, blocks_);
		player->Init(gruound, Wall, wire);
		players_.push_back(player);
	}
	goalFlag_ = false;
}

void PlayerManager::Update(Input& input)
{

	//�V���O���v���C���[�h�łȂ����
	if (!singlePlay_)
	{
		//�v���C���[�����ԋ߂��̑��v���C���[����������
		SearchHormingPlayer();
		//�A�C�e���Ƃ̓����蔻��
		CollisionItem(); 
	}	
	//�Q�[���Z�b�g����
	BattleConclusion();
	for (const auto& player : players_)
	{
		//�v���C���[�������Ă�����A�b�v�f�[�g������
		if (player->IsAlive())
		{
			player->Update(input);
		}
	}
	
}

void PlayerManager::Draw(Vector2DFloat cameraPos)
{
	for (const auto& player : players_)
	{
		//�v���C���[�������Ă�����v���C���[��`��
		if (player->IsAlive())
		{
			player->Draw(cameraPos);
		}
	}
	//���s�����܂����珟������摜��\��
	if (conclusion_)
	{
		DrawRotaGraph2F(WIN_IMAGE_POS_X, WIN_IMAGE_POS_Y, WIN_IMAGE_CENTER_X, WIN_IMAGE_CENTER_Y, WIN_IMAGE_SCALE, 0.0, winImg_,true);
	}
}

const Vector2DFloat& PlayerManager::GetPlayerPos(int playerNum)
{
	return players_[playerNum]->GetPlayerPos();
}

const Players PlayerManager::GetPlayers()
{
	return players_;
}

void PlayerManager::SearchFirstPlayer(std::pair<bool, Vector2DFloat>checkPoint)
{
	//�v���C���[�ƃ`�F�b�N�|�C���g�Ԃ̋���
	std::vector< std::pair<float,int >> checkPointToPlayerDistance;

	//�v���C���[�ƃ`�F�b�N�|�C���g�Ƃ̋������i�[���Ă���B
	for (auto& p : players_)
	{
		//�����ĂȂ���Ζ߂�
		if (!p->IsAlive())
		{
			continue;
		}
		//�`�F�b�N�|�C���g�̍��W��������
		Vector2DFloat checkPointPos= {0.0f, 0.0f};

		//�v���C���[�̃p�b�h�i���o�[���擾
		auto padNum = p->GetPadNum()-1;

		//first�̓v���C���[����`�F�b�N�|�C���g���W�܂ŋ���
		//srcond�̓p�b�h�i���o�[
		std::pair<float,int > info2;

		//�Z�J���h�Ƀp�b�h�i���o�[���i�[
		info2.second = padNum;

		//first��true�Ȃ�`�F�b�N�|�C���gY���W�Ɉ�ԋ߂��v���C���[��T��
		if (checkPoint.first)
		{
			checkPointPos = { players_[padNum]->GetPos().x,checkPoint.second.y };
		}
		//first��false�Ȃ�`�F�b�N�|�C���gX���W�Ɉ�ԋ߂��v���C���[��T��
		else
		{
			checkPointPos = {checkPoint.second.x, players_[padNum]->GetPos().y};
		}
		//�v���C���[����`�F�b�N�|�C���g���W�܂ŋ������i�[
		info2.first = players_[padNum]->GetPos().distance(checkPointPos);
		//�z��Ɋi�[
		checkPointToPlayerDistance.push_back(info2);
	}
	//�߂����Ƀ\�[�g����
	std::sort(checkPointToPlayerDistance.begin(), checkPointToPlayerDistance.end());
	//��ԋ߂��v���C���[�̔ԍ����i�[
	newFirstPlayerNum_ = static_cast<PLAYER_NUM>(checkPointToPlayerDistance[FIRST_PLAYER_NUM].second);
}

void PlayerManager::SearchHormingPlayer()
{
	//�v���C���[(�߂��ق��v���C���[��T����)
	for (auto& player1 : players_)
	{
		//�v���C���[�Ƒ��v���C���[�Ԃ̋����ƃp�b�h�i���o�[���i�[����z��
		std::vector< std::pair<float, int>> playerToPlayerDistance;
		
		//���C���[�Ƒ��v���C���[�Ԃ̋����ƃp�b�h�i���o�[���
		std::pair<float,int > info;

		//�v���C���[�������Ă��Ȃ���Ζ߂�
		if (!player1->IsAlive())
		{
			continue;
		}
		//�����Ώۃv���C���[�̃p�b�h�i���o�[
		auto padNum1 = player1->GetPadNum()-1;

		//�����Ώۃv���C���[
		for (auto& player2 : players_)
		{
			//�����Ώۃv���C���[�̃p�b�h�i���o�[
			auto padNum2 = player2->GetPadNum()-1;

			//�p�b�h�i���o�[���ꏏ�Ȃ�A�����v���C���[�Ԃ̋����ɂȂ��Ă��܂��̂Ŗ߂�
			if (padNum1 == padNum2)
			{ 
				continue; 
			}
			//�����Ώۃv���C���[�̃p�b�h�i���o�[���i�[
			info.second= padNum2;

			//�v���C���[�ƌ����Ώۃv���C���[�Ԃ̋������i�[
			info.first = player1->GetPos().distance(player2->GetPos());

			//�v���C���[�Ƒ��v���C���[�Ԃ̋����ƃp�b�h�i���o�[����z��Ɋi�[
			playerToPlayerDistance.push_back(info);
		}
		//�v���C���[�Ƃ̋������߂����Ƀ\�[�g����
		std::sort(playerToPlayerDistance.begin(), playerToPlayerDistance.end());
		//��ԋ߂��v���C���[������
		int playerNum = playerToPlayerDistance[FIRST_PLAYER_NUM].second;
		//��ԋ߂��v���C���[�̍��W���i�[
		player1->SetTarget(players_[playerNum]->GetPos());
	}	 
}

void PlayerManager::BattleConclusion()
{
	if (!conclusion_ && !goalFlag_)
	{
		return;
	}
	for (auto& player : players_)
	{
		//�v���C���[�̃p�b�h�i���o�[
		auto padNum = player->GetPadNum()-1;

		//�v���C���[�������Ă��Ȃ���Ζ߂�
		if (!player->IsAlive())
		{
			continue;
		}
		//�����Ă����炻�̃v���C���[������
		else
		{
			winner_ = padNum;
		}
	}
	//(���s�����܂������ɁA��񂾂����̊֐����Ă�)
	if (!players_[winner_ ]->IsWin())
	{
		//�v���C���[��������Ԃɂ���
		players_[winner_]->Conclusion();
	}
	
}

void PlayerManager::SetGoalSingleMode()
{
	goalFlag_ = true;
}

void PlayerManager::CollisionItem()
{
	//�A�C�e���g�p���v���C���[
	for (auto& player1 :players_)
	{
		//�A�C�e���g�p��ԂłȂ���Δ�����
		if (!(player1->GetItem()->IsActivate()))
		{ 
			continue;
		}
		//�A�C�e���g�p���v���C���[�̃p�b�h�i���o�[
		auto& padNum1 = player1->GetPadNum();

		//�A�C�e�������Ă��鑤�v���C���[
		for (auto& player2 : players_)
		{
			//�A�C�e�������Ă��鑤�v���C���[�̃p�b�h�i���o�[
			auto& padNum2 = player2->GetPadNum();

			//�p�b�h�i���o�[���ꏏ�Ȃ�A�����v���C���[�Ȃ̂Ŗ߂�
			if (padNum1 == padNum2)
			{ 
				continue; 
			}
			//�v���C���[�̃A�C�e�����
			auto item = player1->GetItem();
			//�A�C�e���̋�`���
			auto iCol = item->col_;
			//�v���C���[�̋�`���
			auto pCol = player2->GetCol_Rect();

			//�v���C���[�ƃA�C�e���̋�`���d�Ȃ��Ă����瓖�����Ă���
			if (IsRectCollision(pCol.min_, pCol.max_, iCol.min_, iCol.max_))
			{
				//�A�C�e�����A�N�e�B�u��Ԃɂ���
				item->End();
				//���̃A�C�e���ڐG���́A�_���[�W���A�N�V�������s��
				player2->Damage(item->type_);
 			}
		}
	}
}

const PlayerManager:: PLAYER_NUM PlayerManager::GetOldFirstPlayerNum()
{
	return oldFirstPlayerNum_;
}

const PlayerManager:: PLAYER_NUM PlayerManager::GetNewFirstPlayerNum()
{
	return newFirstPlayerNum_;
}

void PlayerManager::UpdateFirstPlayerNum()
{
	oldFirstPlayerNum_ = newFirstPlayerNum_;
}

bool PlayerManager::IsRectCollision(Vec pMin, Vec pMax, Vec iMin, Vec iMax)
{
	if (IsCollisionRightSide(pMax, iMin)&& IsCollisionLeftSide(iMax, pMin)&&
		IsCollisionTopSide(pMax, iMin)&& IsCollisionDownSide(iMax, pMin))
	{	
		return true;
	}
	return false;
}

bool PlayerManager::IsCollisionTopSide(Vec Max, Vec Min)
{
	return (Max.y<=Min.y);
}

bool PlayerManager::IsCollisionDownSide(Vec Max, Vec Min)
{
	return (Max.y <= Min.y);
}

bool PlayerManager::IsCollisionLeftSide(Vec Max, Vec Min)
{
	return (Min.x <= Max.x);
}
 
bool PlayerManager::IsCollisionRightSide(Vec Max, Vec Min)
{
	return (Min.x <= Max.x);
}

void PlayerManager::SetSinglePlayMode()
{
	singlePlay_ = true;
}

