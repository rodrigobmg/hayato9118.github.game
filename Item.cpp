// �A�C�e������ (Item.cpp)

//==============================================================
// �C���N���[�h
//==============================================================
#include <nn/gd.h>
#include <nn/gx.h>
#include "Item.h"
#include "Texture.h"
#include "Main.h"
#include "GxCustm.h"

//--------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------
#define MAX_ITEM	200		// �ő�A�C�e����
#define BLINK_TIME	180		// ���őO�̓_�Ŏ���
#define ERASE_TIME	60		// �����_�Ŏ���
#define POTION_LIFE	600		// �񕜖�\������
#define ITEM0_W		8.0f	// ��
#define ITEM0_H		12.0f	// ����

//--------------------------------------------------------------
// �\����
//--------------------------------------------------------------
typedef struct _ANIM_PAT {
    s32 frame;
    s32 count;
} ANIM_PAT;

typedef struct _TItem {	// �A�C�e��
	nn::math::VEC2	pos;	// ���W
	s32				stat;	// ���
	s32				pat;	// �t���[��No.
	s32				cnt;	// �t���[���J�E���^
	s32				item;	// �A�C�e�����
	s32				life;	// ������܂ł̎���
} TItem;

//--------------------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------------------

//--------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------
static ANIM_PAT g_animPat[] =
{
	{ 3, 20}, { 7, 10}, {11, 20}, { 7, 10},
	{-1, -1}
};
static TItem g_item[MAX_ITEM];

//--------------------------------------------------------------
// �A�C�e���`��
//--------------------------------------------------------------
void DrawItem()
{
	TItem* pItem = g_item;
	for (int i = 0; i < MAX_ITEM; ++i, ++pItem) {
		if (pItem->stat == 0)
			continue;
		// �_�ŏ���
		if (pItem->life > 0 && pItem->life < BLINK_TIME) {
			if (pItem->life < ERASE_TIME) {
				if (pItem->life & 2) continue;
			} else {
				if (pItem->life & 4) continue;
			}
		}
		// �`��
		nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);
		if (pItem->item == ITEM_POTION)
	        nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texEnemy2);
		else
			nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texEnemy);
        CreateMatrix(pItem->pos.x, pItem->pos.y, 0.0f, 16.0f, 16.0f);	// �|���S���\���ʒu�Z�b�g
        SetFrame(g_animPat[pItem->pat].frame);
        DrawPolygon(4, 0);							// �|���S���\���i���_��,���_�f�[�^�z��̗v�f�ԍ��j
    }
}

//--------------------------------------------------------------
// �A�C�e���X�V
//--------------------------------------------------------------
void UpdateItem()
{
    for (int i = 0; i < MAX_ITEM; ++i) {
		// �ҋ@���H
        if (g_item[i].stat == 0) {
			continue;	// ����
        }
		// ���������A�C�e���̏ꍇ
		if (g_item[i].life > 0) {
			// �������Ԃ����Z
			--g_item[i].life;
			// �����Ȃ����
			if (g_item[i].life <= 0) {
				g_item[i].stat = 0;
				continue;
			}
		}
		// �J�E���^�����Z
        --g_item[i].cnt;
		// �[���ɂȂ�����
        if (g_item[i].cnt <= 0) {
			// ���̊G��
            ++g_item[i].pat;
			// �G�̔ԍ���-1�Ȃ�
            if (g_animPat[g_item[i].pat].frame < 0) {
				// �擪�̊G�ɖ߂�
				g_item[i].pat = 0;
            }
			// �J�E���^��������
			g_item[i].cnt =
				g_animPat[g_item[i].pat].count;
        }
    }
}

//--------------------------------------------------------------
// �A�C�e��������
//--------------------------------------------------------------
void InitItem()
{
	// �S�āu�ҋ@���v�ɐݒ�
    for (int i = 0; i < MAX_ITEM; ++i) {
        g_item[i].stat = 0;
    }
}

//--------------------------------------------------------------
// �A�C�e���I������
//--------------------------------------------------------------
void FinItem()
{
	// �S�āu�ҋ@���v�ɐݒ�
	for (int i = 0; i < MAX_ITEM; ++i) {
		g_item[i].stat = 0;
	}
}

//--------------------------------------------------------------
// �A�C�e���J�n
//--------------------------------------------------------------
void StartItem(nn::math::VEC2* pos, s32 item)
{
	for (int i = 0; i < MAX_ITEM; ++i) {
		// �Đ����H
		if (g_item[i].stat > 0) {
			continue;	// ����
		}
		// �����l��ݒ�
		g_item[i].pos = *pos;
		g_item[i].pat = 0;
		g_item[i].cnt = g_animPat[0].count;
		g_item[i].stat = 1;
		g_item[i].item = item;
		switch (item) {
		case ITEM_POTION:
			g_item[i].life = POTION_LIFE;
			break;
		default:
			g_item[i].life = 0;	// �����������Ȃ�
			break;
		}
		break;	// �J��Ԃ����I��
	}
}

//--------------------------------------------------------------
// �����蔻��
//--------------------------------------------------------------
int CollisionItem(nn::math::VEC2* pos, nn::math::VEC2* rect)
{
	// �S�ẴA�C�e���Ƃ̓����蔻��
	for (int i = 0; i < MAX_ITEM; ++i)
	{
		// �ҋ@��Ԃ̃A�C�e���̓X�L�b�v
		if (g_item[i].stat == 0)
			continue;
		// ��`���m�̓����蔻��
		if (pos->x - rect->x <= g_item[i].pos.x + ITEM0_W &&
			g_item[i].pos.x - ITEM0_W <= pos->x + rect->x &&
			pos->y - rect->y <= g_item[i].pos.y + ITEM0_H &&
			g_item[i].pos.y - ITEM0_H <= pos->y + rect->y) {
			// �������Ă���
			g_item[i].stat = 0;

			return g_item[i].item;
		}
	}
	return -1;
}
