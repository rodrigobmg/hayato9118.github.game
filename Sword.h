// ���ɂ��U����`(Sword.h)
#ifndef ___SWORD_H
#define ___SWORD_H

#include <nn/math.h>

void InitSword();	// ������
void UpdateSword();	// �X�V
void DrawSword();	// �`��
void FinSword();	// �I������
void AttackSword(nn::math::VEC2* pos,
	s32 dir, s32 beam);	// �U��

#endif	// ___SWORD_H
