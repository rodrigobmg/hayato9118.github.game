// ���ŃG�t�F�N�g ��` (DeadEffect.h)
#ifndef ___H_DEADEFFECT
#define ___H_DEADEFFECT

#include <nn/math.h>

void InitDeadEffect();	// ������
void UpdateDeadEffect();// �X�V
void DrawDeadEffect();	// �`��
void FinDeadEffect();	// �I������
void StartDeadEffect(nn::math::VEC2* pos);
						// �J�n
void ClearDeadEffect();	// �S�G�t�F�N�g����

#endif	// ___H_DEADEFFECT
