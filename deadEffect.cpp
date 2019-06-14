// ���ŃG�t�F�N�g ���� (DeadEffect.cpp)

//==============================================================
// �C���N���[�h
//==============================================================
#include <nn/gd.h>
#include <nn/gx.h>
#include "DeadEffect.h"
#include "Sound.h"
#include "GxCustm.h"
#include "Main.h"

//--------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------
#define MAX_DEADEFFECT	200

//--------------------------------------------------------------
// �\����
//--------------------------------------------------------------
typedef struct _ANIM_PAT {
    s32 frame;
    s32 count;
} ANIM_PAT;

typedef struct _TDeadEffect {   // ���ŃG�t�F�N�g
    f32 x;      // X���W
    f32 y;      // Y���W
    s32 stat;   // ���
    s32 pat;    // �t���[��No.
    s32 cnt;    // �t���[���J�E���^
} TDeadEffect;

extern nn::gd::Texture2D* s_texDeadEffect;
extern nn::gd::SamplerState* s_samplerDefault;

//--------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------
static ANIM_PAT g_animPat[] =
{
	{12, 4}, {13, 4}, {14, 4}, {15, 4},
	{ 8, 4}, { 9, 4}, {10, 4}, {11, 4},
	{ 4, 4}, { 5, 4}, { 6, 4}, { 7, 4},
	{ 0, 4}, { 1, 4}, { 2, 4}, { 3, 4},
	{-1, -1}
};
static TDeadEffect g_deadEffect[MAX_DEADEFFECT];

//--------------------------------------------------------------
// ���ŃG�t�F�N�g�`��
//--------------------------------------------------------------
void DrawDeadEffect()
{
	TDeadEffect* pEffect = g_deadEffect;
	for (int i = 0; i < MAX_DEADEFFECT; ++i, ++pEffect) {
		if (pEffect->stat == 0)
			continue;
		nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);
        nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texDeadEffect);
        CreateMatrix(pEffect->x, pEffect->y, 0.0f, 16.0f, 16.0f);	// �|���S���\���ʒu�Z�b�g
        SetFrame(g_animPat[pEffect->pat].frame);
        DrawPolygon(4, 0);							// �|���S���\���i���_��,���_�f�[�^�z��̗v�f�ԍ��j
    }
}

//--------------------------------------------------------------
// ���ŃG�t�F�N�g�X�V
//--------------------------------------------------------------
void UpdateDeadEffect()
{
    for (int i = 0; i < MAX_DEADEFFECT; ++i) {
		// �ҋ@���H
        if (g_deadEffect[i].stat == 0) {
			continue;	// ����
        }
		// �J�E���^�����Z
        --g_deadEffect[i].cnt;
		// �[���ɂȂ�����
        if (g_deadEffect[i].cnt <= 0) {
			// ���̊G��
            ++g_deadEffect[i].pat;
			// �G�̔ԍ���-1�Ȃ�
            if (g_animPat
[g_deadEffect[i].pat].frame < 0) {
				// �A�j���[�V�����I��
				g_deadEffect[i].stat = 0;
				continue;
            }
			// �J�E���^��������
			g_deadEffect[i].cnt =
g_animPat[g_deadEffect[i].pat].count;
        }
    }
}

//--------------------------------------------------------------
// ���ŃG�t�F�N�g������
//--------------------------------------------------------------
void InitDeadEffect()
{
	// �S�āu�ҋ@���v�ɐݒ�
    for (int i = 0; i < MAX_DEADEFFECT; ++i) {
        g_deadEffect[i].stat = 0;
    }
}

//--------------------------------------------------------------
// ���ŃG�t�F�N�g�I������
//--------------------------------------------------------------
void FinDeadEffect()
{
	ClearDeadEffect();	// �S�G�t�F�N�g����
}

//--------------------------------------------------------------
// ���ŃG�t�F�N�g�J�n
//--------------------------------------------------------------
void StartDeadEffect(nn::math::VEC2* pos)
{
	for (int i = 0; i < MAX_DEADEFFECT; ++i) {
		// �Đ����H
		if (g_deadEffect[i].stat > 0) {
			continue;	// ����
		}
		PlaySE(SE_DEADEFFECT);
		// �����l��ݒ�
		g_deadEffect[i].x = pos->x;
		g_deadEffect[i].y = pos->y;
		g_deadEffect[i].pat = 0;
		g_deadEffect[i].cnt =
			g_animPat[0].count;
		g_deadEffect[i].stat = 1;
		break;	// �J��Ԃ����I��
	}
}

//--------------------------------------------------------------
// �S�G�t�F�N�g����
//--------------------------------------------------------------
void ClearDeadEffect()
{
	// �S�āu�ҋ@���v�ɐݒ�
	for (int i = 0; i < MAX_DEADEFFECT; ++i) {
		g_deadEffect[i].stat = 0;
	}
}
