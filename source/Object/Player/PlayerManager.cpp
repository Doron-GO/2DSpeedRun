#include"../../Object/Item/ItemBase.h"
#include"../../Object/Stage/Blocks.h"
#include "PlayerManager.h"
#include "Player.h"

#pragma region �p�X

const std::string IMG_WIN_PASS = "Src/Img/UIimage/Win.png";
const std::string IMG_RESTART_PASS = "Src/Img/UIimage/Restart.png";

#pragma endregion

PlayerManager::PlayerManager(bool& conclusion, Blocks& blocks):conclusion_(conclusion),blocks_(blocks)
{
	//�����ݒ�͑ΐ탂�[�h
	singlePlay_ = false;
	//�ŏ��̓S�[���͂��Ă��Ȃ�
	goalFlag_ = false;
	//�ŏ��͂ǂ���Ƃ��v���C���[1�ɐݒ�
	newFirstPlayerNum_ = PLAYER_TYPE::P_1;
	oldFirstPlayerNum_ = PLAYER_TYPE::P_1;
	//�����ݒ�̏��҂̓v���C���[1�ɐݒ�
	winner_ = static_cast<int>(PLAYER_TYPE::P_1);
	InitImage();

}

PlayerManager::~PlayerManager()
{
	DeleteGraph(winImg_);
	DeleteGraph(restertImg_);
}

void PlayerManager::Init(int playerNum, ColList gruound, ColList Wall, ColList wire)
{
	//�v���C���[�𐶐����z��Ɋi�[
	for (int playerType = 1; playerType <= playerNum; playerType++)
	{	
		std::shared_ptr<Player> player;
		player = std::make_shared<Player>(playerType, blocks_);
		player->Init(gruound, Wall, wire);
		players_.push_back(player);
	}
	goalFlag_ = false;
}

void PlayerManager::Update(Input& input)
{
	//�V���O���v���C����Ȃ����
	if (!singlePlay_)
	{
		//�v���C���[�Ɉ�ԋ߂����v���C���[����������
		SearchHormingPlayer();
	}

	//�A�C�e���̓����蔻��
	CollisionItem(); 

	//���s�����܂������ǂ����𔻒�
	BattleConclusion();

	//�v���C���[�̃A�b�v�f�[�g
	for (const auto& player : players_)
	{
		if (player->IsAlive())
		{
			player->Update(input);
		}
	}
	
}

void PlayerManager::Draw(Vector2DFloat cameraPos)
{
	//�v���C���[�̕`��
	for (const auto& player : players_)
	{
		if (player->IsAlive())
		{
			player->Draw(cameraPos);
		}
	}
	//�����摜�`��
	if (conclusion_)
	{
		DrawRotaGraph2F(800.0f, 300.0f, 200.0f, 20.0f, 2.0, 0.0, img_[IMG_TYPE::WIN], true);
	}
}

const Vector2DFloat& PlayerManager::GetPlayerPos(int playerNum)
{
	return players_[playerNum]->pos_;
}

const Players PlayerManager::GetPlayers()
{
	return players_;
}

void PlayerManager::SearchFirstPlayer(std::pair<CheckPoint::DIRCTION, Vector2DFloat>checkPoint)
{
	//
	checkPointToPlayerDistance_.clear();

	//�v���C���[�ƃ`�F�b�N�|�C���g�Ƃ̋������i�[���Ă���B
	for (auto& p : players_)
	{
		//first:�v���C���[�^�C�v�Asecond:����
		std::pair<int, float> info;
		//�v���C���[�^�C�v
		int playerType = (p->padNum_) - 1;
		//�v���C���[�^�C�v���i�[
		info.first = playerType;
		//�`�F�b�N�|�C���g�̍��W�̏�����
		Vector2DFloat checkPointPos= {0.0f, 0.0f};

		//
		if (checkPoint.first == CheckPoint::DIRCTION::HORIZONTAL)
		{
			checkPointPos = { players_[(p->padNum_) - 1]->GetPos().x,checkPoint.second.y };
		}
		else
		{
			checkPointPos = {checkPoint.second.x, players_[(p->padNum_) - 1]->GetPos().y}; 
		}
		info.second = players_[(p->padNum_) - 1]->GetPos().distance(checkPointPos);
		checkPointToPlayerDistance_.push_back(info);
	}

	for (auto& p1 : players_)
	{
		for (auto& p2 : players_)
		{
			if (p1->IsAlive() && p2->IsAlive())
			{
				auto num1 = (p1->padNum_) - 1;
				auto num2 = (p2->padNum_) - 1;
				//�D�܂����Ȃ��z�񌟍�
				//�v���C���[N���v���C���[N+1���O��������A�v���C���[N��擪�ɂ���B 
				if (checkPointToPlayerDistance_[num1].second < checkPointToPlayerDistance_[num2].second)
				{
					if (checkPointToPlayerDistance_[num1].second < checkPointToPlayerDistance_[(int)newFirstPlayerNum_].second)
					{
						newFirstPlayerNum_ = static_cast<PLAYER_TYPE>(checkPointToPlayerDistance_[num1].first);
					}
				}
			}
		}
	}

}

void PlayerManager::SearchHormingPlayer()
{
	for (auto& player1 : players_)
	{
		//�v���C���[�Ƒ��v���C���[�Ԃ̋���
		std::vector< std::pair<int, float>> playerToPlayerDistance;

		std::pair<int, float> info;

		if (!player1->IsAlive())
		{
			checkPointToPlayerDistance_.clear();
			continue;
		}
		for (auto& player2 : players_)
		{
			if (player1->padNum_ == player2->padNum_) { continue; }
			info.second = static_cast<float>(player2->padNum_ - 1);

			int num = player1->padNum_ - 1;
			size_t size = players_.size() - 1;
			if (num > size)
			{
				num = static_cast<int>(size);
			}
			info.first = static_cast<int>(
				players_[num]->GetPos().distance(player2->GetPos()));
			playerToPlayerDistance.push_back(info);
		}
		auto pp = std::min_element(playerToPlayerDistance.begin(), playerToPlayerDistance.end());
		player1->SetTarget(players_[pp->second]->GetPos());
		playerToPlayerDistance.clear();
	}	 
}

void PlayerManager::BattleConclusion()
{
	if (conclusion_|| goalFlag_)
	{
		for (auto& player : players_)
		{
			if (player->IsAlive())
			{
				winner_ = player->padNum_ -1;
			}
			else
			{
				continue;
			}
		}
		if (!players_[winner_ ]->IsWin())
		{
			players_[winner_]->Conclusion();
		}
	}
}

void PlayerManager::SetGoalSingleMode()
{
	goalFlag_ = true;
}

void PlayerManager::CollisionItem()
{
	for (auto& player1 :players_)
	{
		//�A�C�e���g�p��ԂłȂ���Δ�����
		if (!(player1->GetItem()->IsActivate()))
		{ 
			continue;
		}
		for (auto& player2 : players_)
		{
			if (player1->padNum_ == player2->padNum_) { continue; }
			auto item = player1->GetItem();
			auto iCol = item->col_;
			auto pCol = player2->colRect_;
			if (IsRectCollision(pCol.min_, pCol.max_, iCol.min_, iCol.max_))
			{
				item->End();
				player2->Damage(item->type_);
 			}
		}
	}
}

const PlayerManager:: PLAYER_TYPE PlayerManager::GetOldFirstPlayerNum()
{
	return oldFirstPlayerNum_;
}

const PlayerManager:: PLAYER_TYPE PlayerManager::GetNewFirstPlayerNum()
{
	return newFirstPlayerNum_;
}

void PlayerManager::UpdateFirstPlayerType()
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

void PlayerManager::InitImage(void)
{
	//�摜�̓ǂݍ���

	img_.emplace(IMG_TYPE::WIN, LoadGraph(IMG_WIN_PASS.c_str()));
	img_.emplace(IMG_TYPE::RESTART, LoadGraph(IMG_RESTART_PASS.c_str()));
}

