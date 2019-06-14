// �A�C�e����` (Item.h)
#ifndef ___H_ITEM
#define ___H_ITEM

#include <nn/math.h>

// �}�N����`
#define ITEM_POTION		0	// �񕜖�
#define ITEM_KEY		1	// ��

// �֐��v���g�^�C�v
void InitItem();	// ������
void UpdateItem();	// �X�V
void DrawItem();	// �`��
void FinItem();		// �I������
void StartItem(nn::math::VEC2* pos, s32 item);
					// �J�n
int CollisionItem(
	nn::math::VEC2* pos,
	nn::math::VEC2* rect);
					// �����蔻��

#endif	// ___H_ITEM
