// �G2 ��` (Enemy2.h)
#ifndef ___H_ENEMY2
#define ___H_ENEMY2

#include <nn/math.h>

void InitEnemy2();	// ������
void UpdateEnemy2();// �X�V
void DrawEnemy2();	// �`��
void FinEnemy2();	// �I������
int CollisionEnemy2(nn::math::VEC2* pos,
	nn::math::VEC2* rect, s32 damage);
					// �����蔻��
int GetEnemy2Pos(int nEnemy2,
	nn::math::VEC2* pos);
					// �G�ʒu�擾

#endif	// ___H_ENEMY2
