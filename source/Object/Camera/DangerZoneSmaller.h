#pragma once
#include"../../Common/Vector2D.h"

class OutSide;

class DangerZoneSmaller
{

public:

	DangerZoneSmaller(Vector2DFloat& max, Vector2DFloat& min);
	~DangerZoneSmaller();
	
	//���S����̃G���A�����k������֐�
	void Smaller();
	
	// ���S����̃G���A�����k���̃A�b�v�f�[�g
	void UpdateSmaller();

	//�����o�֐��|�C���^���Ăяo���A�b�v�f�[�g�֐�
	void Update();

	//�k���ҋ@���A�b�v�f�[�g
	void UpdateWait();

	//�A�b�v�f�[�g��UpdateSmaller�ɕύX
	void Activated();

	//�J�E���g��0�Ƀ��Z�b�g
	void ResetCounter();

private:
	
	//�k�����n�߂�܂ł̌v���p
	int count_;

	//�A�b�v�f�[�g�Ǘ��p�����o�֐��|�C���^
	void (DangerZoneSmaller::* _update)();


	//���S�G���A�̍��W�̍ő�l(�l�p�̉E��)
	Vector2DFloat& outSideMax_;

	//���S�G���A�̍��W�̍ŏ��l(�l�p�̍���)
	Vector2DFloat& outSideMin_;

};

