// �v���C���[��` (Player.h)
#ifndef ___H_PLAYER_
#define ___H_PLAYER_

#include <nn/math.h>

// �֐��v���g�^�C�v
void InitPlayer();	// ������
void UpdatePlayer();// �X�V
void DrawPlayer();	// �`��
void FinPlayer();	// �I������
nn::math::VEC2 GetPlayerPos();
					// �ʒu�擾
s32 GetPlayerHP();	// HP�擾
bool GetPlayerKey();// ��������Ԏ擾

#endif
