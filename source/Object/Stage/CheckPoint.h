#pragma once
#include"../../Common/Vector2D.h"
#include"../../Common/Collision.h"
#include"../../Common/Raycast.h"
#include<vector>
#include<memory>
#include<list>

class Player;
using CHECKPOINT_SAKUJO= std::pair<bool, Vector2DFloat>;

class CheckPoint
{
public:

	//�`�F�b�N�|�C���g�̕���
	enum class DIRCTION
	{	
		//����
		HORIZONTAL,
		//����
		VERTICAL
	};

	CheckPoint(std::vector<std::shared_ptr<Player>> players, PointColList checkpoint);
	~CheckPoint();

	//�A�b�v�f�[�g
	void Update();

	//�`�F�b�N�|�C���g�̎擾
	std::pair<DIRCTION, Vector2DFloat> GetCheckPoint()const;

	//�S�[�����������ǂ���
	const bool IsGoal();

	//�V���O�����[�h�ɐ؂�ւ���
	void SetSinglePlayMode();

	//�V���O�����[�h�A�b�v�f�[�g
	void SinglePlayUpdate();

	//�ΐ탂�[�h�A�b�v�f�[�g
	void MultiPlayUpdate();


private:

	void (CheckPoint::* update_)();

	//�v���C���[���
	std::vector<std::shared_ptr<Player>> players_;

	//pair��z��Ɋi�[
	//first:true�Ȃ牡�����̃`�F�b�N�|�C���g false:�c�����̃`�F�b�N�|�C���g
	// second:�`�F�b�N�|�C���g�̍��W
	std::vector < std::pair<bool, Vector2DFloat>> checkPoints_sakujo;
	std::vector < std::pair<DIRCTION, Vector2DFloat>> checkPoints_;

	//�����蔻��
	Raycast rayCast_;

	//�`�F�b�N�|�C���g�����蔻�胊�X�g
	PointColList checkPointColList_;

	//���Ɍ������`�F�b�N�|�C���g�i���o�[
	int currentCheckPoint_;

	//�v���C���[���S�[���������ǂ���
	int goalFlag_;

};

