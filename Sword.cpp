// ���ɂ��U������ (Sword.cpp)
#include <nn/gd.h>
#include "Sword.h"
#include "GxCustm.h"
#include "Main.h"
#include "Texture.h"
#include "GxInit.h"
#include "Enemy.h"
#include "Enemy2.h"
#include "deadEffect.h"
#include "Sound.h"
#include "Boss1.h"

// �}�N����`
#define SPEED	10.0f	// ����
#define LIFE	30		// ��������(�P��:�t���[��(1/60s))

// ���̏��
typedef struct _SWORD {
	s32				stat;
// ���(0:���������A1:�ҋ@�A2:���A3:�r�[��)
	s32				dir;
// ����(0:��A1:�E�A2:���A3:��)
	nn::math::VEC2	pos;	// �ʒu
	nn::math::VEC2	vel;	// ���x
	s32				timer;	// �^�C�}�[
	s32				str;	// �U����
} SWORD;

static SWORD	g_sword;	// �����

// �֐��v���g�^�C�v
static void CheckCollision();// �����蔻��

// ������
void InitSword() 
{
	g_sword.stat = 1;	// ���=�ҋ@
}

// �X�V
void UpdateSword()
{
	switch (g_sword.stat) {
	case 0:	// ��������
		InitSword();
		// THRU
	case 1:	// �ҋ@
		break;
	case 2:	// ��
		--g_sword.timer;
		if (g_sword.timer <= 0) {
			g_sword.stat = 1;
			break;
		}
		// �����蔻��
		CheckCollision();
		break;
	case 3:	// �r�[��
		g_sword.pos += g_sword.vel;
		if (g_sword.pos.x <= nn::gx::DISPLAY0_HEIGHT * -0.5f + 16 ||
			g_sword.pos.x >= nn::gx::DISPLAY0_HEIGHT * 0.5f - 16 ||
			g_sword.pos.y <= nn::gx::DISPLAY0_WIDTH * -0.5f + 16 ||
			g_sword.pos.y >= nn::gx::DISPLAY0_WIDTH * 0.5f - 16) {
			g_sword.stat = 1;	// �ҋ@��
			// TODO: �����ŏ��ŃG�t�F�N�g����.
			StartDeadEffect(&g_sword.pos);
			break;
		}
		// �����蔻��
		CheckCollision();
		break;
	}
}

// �`��
void DrawSword()
{
	switch (g_sword.stat) {
	case 2:	// ��
	case 3:	// �r�[��
		nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);
		nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texture2D_TGA);
		CreateMatrix(g_sword.pos.x, g_sword.pos.y, 0.0f, 16.0f, 16.0f);
		SetFrame(g_sword.dir * 4 + 3);
		DrawPolygon(4, 0);
		break;
	}
}

// �I������
void FinSword()
{
}

// �U��
void AttackSword(nn::math::VEC2* pos, s32 dir, s32 beam)
{
	if (g_sword.stat == 2 || g_sword.stat == 3) {
		return;	// �U����
	}
	g_sword.pos = *pos;
	g_sword.dir = dir;
	switch (dir) {
	case 0:	// ��
		g_sword.vel.x = 0.0f;
		g_sword.vel.y = 1.0f;
		break;
	case 1:	// �E
		g_sword.vel.x = 1.0f;
		g_sword.vel.y = 0.0f;
		break;
	case 2:	// ��
		g_sword.vel.x = -1.0f;
		g_sword.vel.y = 0.0f;
		break;
	case 3:	// ��
		g_sword.vel.x = 0.0f;
		g_sword.vel.y = -1.0f;
		break;
	}
	// �L�����N�^�̌����Ă�������ɂ��炷.
	g_sword.pos += g_sword.vel * 32;
	if (beam) {
		PlaySE(SE_BEAM);
		// �����Ă�������ɑ��x��ݒ�.
		g_sword.vel *= SPEED;
		// �������Ԃ̓[��.
		g_sword.timer = 0;
		// ��Ԃ�"�r�[��"��.
		g_sword.stat = 3;
		// �U���͂�ݒ�.
		g_sword.str = 1;
	} else {
		PlaySE(SE_SWORD);
		// ���x�̓[��.
		g_sword.vel *= 0.0f;
		// �������Ԃ�ݒ�.
		g_sword.timer = LIFE;
		// ��Ԃ�"��"��.
		g_sword.stat = 2;
		// �U���͂�ݒ�.
		g_sword.str = 5;
	}
}

// �����蔻��
void CheckCollision()
{
	// ���̌����ŋ�`�T�C�Y��ύX
	nn::math::VEC2 rect;
	switch (g_sword.dir) {
	case 0:	// ��
	case 3:	// ��
		rect.x = 4.0f;
		rect.y = 16.0f;
		break;
	default:// �E�A��
		rect.x = 16.0f;
		rect.y = 4.0f;
		break;
	}
	// �G�Ƃ̓����蔻��
	if (CollisionEnemy(&g_sword.pos, &rect, g_sword.str) >= 0) {
		// �r�[�����������Ă�����u�ҋ@�v��
		if (g_sword.stat == 3)
			g_sword.stat = 1;
	} else {
		// �G2�Ƃ̓����蔻��
		if (CollisionEnemy2(&g_sword.pos, &rect, g_sword.str) >= 0) {
			// �r�[�����������Ă�����u�ҋ@�v��
			if (g_sword.stat == 3)
				g_sword.stat = 1;
		} else {
			// �{�X1�Ƃ̓����蔻��
			if (CollisionBoss1(g_sword.pos.x, g_sword.pos.y, rect.x, rect.y, g_sword.str)) {
				g_sword.str = 0;
				// �r�[�����������Ă�����u�ҋ@�v��
				if (g_sword.stat == 3)
					g_sword.stat = 1;
			}
		}
	}
}
