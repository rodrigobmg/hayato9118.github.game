// �G ��` (Enemy.h)
#ifndef ___H_ENEMY
#define ___H_ENEMY

#include <nn/math.h>

void InitEnemy();	// ������
void UpdateEnemy();	// �X�V
void DrawEnemy();	// �`��
void FinEnemy();	// �I������
int CollisionEnemy(nn::math::VEC2* pos,
	nn::math::VEC2* rect, s32 damage);
					// �����蔻��
int GetEnemyPos(int nEnemy,
	nn::math::VEC2* pos);
					// �G�ʒu�擾

#endif	// ___H_ENEMY
