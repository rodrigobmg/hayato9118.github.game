// ���@ ��` (Magic.h)
#ifndef ___H_MAGIC
#define ___H_MAGIC

#include <nn/math.h>

void InitMagic();	// ������
void UpdateMagic();	// �X�V
void DrawMagic();	// �`��
void FinMagic();	// �I������
void AttackMagic(
	nn::math::VEC2* pos,
	nn::math::VEC2* dir,
	s32 kind);		// �U��
int CollisionMagic(
	nn::math::VEC2* pos,
	nn::math::VEC2* rect,
	nn::math::VEC2* posMagic);
					// �����蔻��

#endif	// ___H_MAGIC
