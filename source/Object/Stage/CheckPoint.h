#pragma once
#include"../../Common/Vector2D.h"
#include"../../Common/Collision.h"
#include"../../Common/Raycast.h"
#include<vector>
#include<memory>
#include<list>

class Player;
constexpr int CHECKPOINT_MAX =6;

class CheckPoint
{
public:

	CheckPoint(std::vector<std::shared_ptr<Player>> players, PointColList checkpoint);
	~CheckPoint();

	void Update();
	std::pair<bool, Vector2DFloat> GetCheckPoint2()const;
	const bool IsGoal();
	void SetSingleMode();

	void SinglePlay();
	void MultiPlay();
private:

	void (CheckPoint::* _work)();

	std::vector<std::shared_ptr<Player>> players_;

	//�y�A�\�̓������z��
	//first:�v���C���[���������������㉺�Ȃ̂����E�Ȃ̂��𔻒� true:�㉺�@false:���E
	//second: �`�F�b�N�|�C���g���W
	std::vector < std::pair<bool, Vector2DFloat>> checkPoints_;

	//�����蔻��N���X
	Raycast rayCast_;

	//�`�F�b�N�|�C���g�����蔻�胊�X�g
	PointColList checkPointColList_;

	//���Ɍ������`�F�b�N�|�C���g�i���o�[
	int currentCheckPoint_;

	//�v���C���[���S�[���������ǂ���
	int goalFlag_;

};

