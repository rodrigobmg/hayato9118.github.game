// �{�X1���� (Boss1.cpp)
#include <nn/gd.h>
#include "Main.h"
#include "Boss1.h"
#include "DeadEffect.h"
#include "Map.h"
#include "Texture.h"
#include "Flare.h"
#include "GxCustm.h"
#include "Sound.h"
#include "Text.h"

#define BOSS1_LIFE_MAX		25
#define BOSS1_WIDTH			48.0f
#define BOSS1_HEIGHT		54.0f

#define BOSS1_STAT_EXPSTART	-300

typedef struct _TBoss1 {	// �{�X1
	s32				stat;	// ���
	nn::math::VEC2	pos;	// ���W
	f32				width;	// ��
	f32				height;	// ����
	s32				life;	// �̗�
	s32				timer;	// �ړ��p�^�[�������p�^�C�}�[
	f32				angle;	// �ړ��p�^�[�������p�p�x
	s32				damage;	// �_���[�W�\��
} TBoss1;
static TBoss1 boss1;

// ����������
void InitBoss1()
{
	boss1.stat = BOSS1_STAT_PAUSE;		// �ꎞ��~
	boss1.width = BOSS1_WIDTH;
	boss1.height = BOSS1_HEIGHT;
	boss1.timer = 0;
	boss1.angle = 0.0f;
	boss1.timer = 180;
	boss1.pos.x = 0.0f;
	boss1.pos.y = 10.0f;
	boss1.life = BOSS1_LIFE_MAX;
	boss1.damage = 0;
}

// �\������
void DrawBoss1()
{
	if (boss1.stat <= BOSS1_STAT_HIDE) return;	// �ҋ@���Ȃ�X�L�b�v

	// �e�N�X�`���ݒ�
	nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texBoss1);
	nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);

	CreateMatrix(boss1.pos.x, boss1.pos.y, 0.0f, 64.0f, 64.0f);
	SetFrame();
	static const u8 red[] = {255, 0, 0, 255};
	DrawPolygon(4, 0, (boss1.damage) ? red : NULL);
}

void DrawBoss1HP(f32 hp_x, f32 hp_y)
{
	if (boss1.stat <= BOSS1_STAT_HIDE) return;
	f32 hp_max = BOSS1_LIFE_MAX - 20.0f;
	f32 hp = boss1.life;
	nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texNumber);
	hp_x += 4.0f;
	hp_y -= 16.0f;
	CreateMatrix(hp_x + hp_max * 1.5f, hp_y, 0.0f, hp_max * 1.6f, 16.0f);
	SetFrame(11);
	DrawPolygon(4, 0);
	if (hp > 0) {
		CreateMatrix(hp_x + hp * 1.2f, hp_y, 0.0f, hp * 1.6f, 16.0f);
		SetFrame(10);
		DrawPolygon(4, 0);
	}
}

// �ړ�����
void UpdateBoss1()
{
	if (boss1.stat > BOSS1_STAT_HIDE) {
		if (boss1.damage > 0) --boss1.damage;
		// �ړ���
		switch (boss1.stat) {
		case BOSS1_STAT_PAUSE:	// �ꎞ��~
			boss1.timer--;
			if (boss1.timer <= 0) {
				boss1.stat = BOSS1_STAT_MOVE;
				boss1.timer = 300;
			}
			break;
		case BOSS1_STAT_MOVE:	// 8�̎��Ɉړ�
			boss1.pos.x = (nn::gx::DISPLAY0_HEIGHT / 2 - boss1.width) * nn::math::CosDeg(boss1.angle + 90.0f);
			boss1.pos.y = (nn::gx::DISPLAY0_WIDTH / 2 - boss1.height) * nn::math::SinDeg(boss1.angle * 2.0f);
			boss1.angle += 45.0f / 60.0f;
			boss1.timer--;
			if (boss1.timer <= 0) {
				boss1.stat = BOSS1_STAT_PAUSE;
				boss1.timer = 180;
			}
			break;
		case 3:
			break;
		}
	} else if (boss1.stat < BOSS1_STAT_HIDE && boss1.stat > BOSS1_STAT_EXPLOSION) {
		boss1.damage = 0;
		// ������
		if (boss1.stat & 1) {
			s32 x = (s32)boss1.width;
			x = random(x * 2) - x;
			s32 y = (s32)boss1.height;
			y = random(y * 2) - y;
			nn::math::VEC2 pos;
			pos.x = boss1.pos.x + x;
			pos.y = boss1.pos.y + y;
			StartDeadEffect(&pos);
		}
		if ((boss1.stat & 7) == 0) {
			PlaySE(SE_EXPLOSION);
		}
		boss1.stat++;
	} else {
		// �ҋ@��
	}
}

// �����蔻��
s32 CollisionBoss1(f32 x, f32 y, f32 w, f32 h, s32 str)
{
	if (boss1.stat > BOSS1_STAT_HIDE &&
		x - w < boss1.pos.x + boss1.width &&
		y - h < boss1.pos.y + boss1.height &&
		boss1.pos.x - boss1.width < x + w &&
		boss1.pos.y - boss1.height < y + h) {
		boss1.life -= str;
		if (boss1.life <= 0) {
			boss1.stat = BOSS1_STAT_EXPSTART;
			AddScore(10000);
			PlaySE(SE_MONSTER);
			StartDeadEffect(&boss1.pos);
			StartFlare(boss1.pos.x, boss1.pos.y);
		}
		if (str > 0 && boss1.damage < 1) {
			boss1.damage = 6;
			PlaySE(SE_DAMAGE);
		}
		return 1;
	}
	return 0;
}

s32 GetBoss1Stat()
{
	return boss1.stat;
}

// �\���J�n
void ShowBoss1()
{
	// ��\�����Ȃ�
	if (boss1.stat == BOSS1_STAT_HIDE_MOVE ||
		boss1.stat == BOSS1_STAT_HIDE_PAUSE) {
		// �\���ĊJ
		boss1.stat += 999;
	}
}

// �\���I��
void HideBoss1()
{
	// �������Ȃ�
	if (boss1.stat < BOSS1_STAT_HIDE && boss1.stat > BOSS1_STAT_EXPLOSION) {
		// �����I�ɃN���A
		boss1.stat = BOSS1_STAT_HIDE;
	}
	// �ړ���(or�ꎞ��~��)�Ȃ�
	if (boss1.stat > BOSS1_STAT_HIDE) {
		// ��\����
		boss1.stat -= 999;
	}
}

// �{�X1������
void ResetBoss1()
{
	boss1.stat = BOSS1_STAT_UNINIT;
}

// �{�X1���W�擾
nn::math::VEC2 GetBoss1Pos()
{
	return boss1.pos;
}

