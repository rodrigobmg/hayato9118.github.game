// ���@ ���� (Magic.cpp)

//==============================================================
// �C���N���[�h
//==============================================================
#include <nn/gd.h>
#include <nn/gx.h>
#include "Magic.h"
#include "Texture.h"
#include "Main.h"
#include "GxCustm.h"
#include "Sound.h"

//--------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------
#define MAX_MAGIC		200		// �ő�\����
#define LIFE_MAGIC0		120		// ����
#define SPEED_MAGIC0	2.0f	// ���x
#define MAGIC0_W		12.0f	// ��
#define MAGIC0_H		12.0f	// ����

//--------------------------------------------------------------
// �\����
//--------------------------------------------------------------
typedef struct _ANIM_PAT {
    s32 frame;
    s32 count;
} ANIM_PAT;

typedef struct _TMagic {// ���@
	nn::math::VEC2	pos;	// �ʒu
	nn::math::VEC2	vel;	// ���x
	s32				timer;	// �^�C�}�[
	s32				kind;	// ���(0:�t�@�C���{�[��)
    s32				stat;   // ���
    s32				pat;    // �t���[��No.
    s32				cnt;    // �t���[���J�E���^
} TMagic;

//--------------------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------------------

//--------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------
static ANIM_PAT g_animPat0[] =
{
	{15, 1}, {-1, -1}
};
static ANIM_PAT* g_animPat[] = {
	g_animPat0,
};
static TMagic g_magic[MAX_MAGIC];

//--------------------------------------------------------------
// ���@�`��
//--------------------------------------------------------------
void DrawMagic()
{
	TMagic* pEffect = g_magic;
	for (int i = 0; i < MAX_MAGIC; ++i, ++pEffect) {
		if (pEffect->stat == 0)
			continue;
		nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);
        nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texEnemy2);
        CreateMatrix(pEffect->pos.x, pEffect->pos.y, 0.0f, 16.0f, 16.0f);	// �|���S���\���ʒu�Z�b�g
        SetFrame(g_animPat[pEffect->kind][pEffect->pat].frame);
        DrawPolygon(4, 0);							// �|���S���\���i���_��,���_�f�[�^�z��̗v�f�ԍ��j
    }
}

//--------------------------------------------------------------
// ���@�X�V
//--------------------------------------------------------------
void UpdateMagic()
{
    for (int i = 0; i < MAX_MAGIC; ++i) {
		// �ҋ@���H
        if (g_magic[i].stat == 0) {
			continue;	// ����
        }
		// �����H
		--g_magic[i].timer;
		if (g_magic[i].timer <= 0) {
			g_magic[i].stat = 0;
			continue;
		}
		// �ړ�
		g_magic[i].pos += g_magic[i].vel;
		if (g_magic[i].pos.x <=
nn::gx::DISPLAY0_HEIGHT * -0.5f - 16 ||
			g_magic[i].pos.x >=
nn::gx::DISPLAY0_HEIGHT * 0.5f + 16 ||
			g_magic[i].pos.y <=
nn::gx::DISPLAY0_WIDTH * -0.5f - 16 ||
			g_magic[i].pos.y >=
nn::gx::DISPLAY0_WIDTH * 0.5f + 16) {
			g_magic[i].stat = 0;// �ҋ@
			continue;
		}
		// �J�E���^�����Z
        --g_magic[i].cnt;
		// �[���ɂȂ�����
        if (g_magic[i].cnt <= 0) {
			// ���̊G��
            ++g_magic[i].pat;
			// �G�̔ԍ���-1�Ȃ�
            if (g_animPat
[g_magic[i].kind][g_magic[i].pat].frame
< 0) {
				switch (g_magic[i].kind)
				{
				case 0:
					// �擪�ɖ߂�
					g_magic[i].pat = 0;
					break;
				default:
					// �A�j���[�V�����I��
					g_magic[i].stat = 0;
					continue;
				}
            }
			// �J�E���^��������
			g_magic[i].cnt = g_animPat
[g_magic[i].kind][g_magic[i].pat].count;
        }
    }
}

//--------------------------------------------------------------
// ���@������
//--------------------------------------------------------------
void InitMagic()
{
	// �S�āu�ҋ@���v�ɐݒ�
    for (int i = 0; i < MAX_MAGIC; ++i) {
        g_magic[i].stat = 0;
    }
}

//--------------------------------------------------------------
// ���@�I������
//--------------------------------------------------------------
void FinMagic()
{
	// �S�āu�ҋ@���v�ɐݒ�
	for (int i = 0; i < MAX_MAGIC; ++i) {
		g_magic[i].stat = 0;
	}
}

//--------------------------------------------------------------
// ���@�U��
//--------------------------------------------------------------
void AttackMagic(nn::math::VEC2* pos,
	nn::math::VEC2* dir, s32 kind)
{
	for (int i = 0; i < MAX_MAGIC; ++i) {
		// �Đ����H
		if (g_magic[i].stat > 0) {
			continue;	// ����
		}
		PlaySE(SE_FIREBALL);
		// �����l��ݒ�
		g_magic[i].stat = 1;
		g_magic[i].pos = *pos;
		g_magic[i].pat = 0;
		g_magic[i].cnt =
			g_animPat[kind][0].count;
		nn::math::VEC2Normalize(
			&g_magic[i].vel, dir);
		g_magic[i].kind = kind;
		switch (kind) {
		case 0:
			g_magic[i].vel *=
				SPEED_MAGIC0;
			g_magic[i].timer =
				LIFE_MAGIC0;
			break;
		}
		break;	// �J��Ԃ����I��
	}
}

// �����蔻��
int CollisionMagic(nn::math::VEC2* pos,
	nn::math::VEC2* rect,
	nn::math::VEC2* posMagic)
{
	// �S�Ă̖��@�Ƃ̓����蔻��
	for (int i = 0; i < MAX_MAGIC; ++i)
	{
		// �ҋ@��Ԃ̖��@�̓X�L�b�v
		if (g_magic[i].stat == 0)
			continue;
		// ��`���m�̓����蔻��
		if (pos->x - rect->x <=
			g_magic[i].pos.x + MAGIC0_W &&
			g_magic[i].pos.x - MAGIC0_W <=
			pos->x + rect->x &&
			pos->y - rect->y <=
			g_magic[i].pos.y + MAGIC0_H &&
			g_magic[i].pos.y - MAGIC0_H <=
			pos->y + rect->y) {
			// �������Ă���
			g_magic[i].stat = 0;
			*posMagic = g_magic[i].pos;
			return i;
		}
	}
	return -1;
}
