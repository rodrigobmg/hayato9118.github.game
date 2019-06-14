// �G ���� (Enemy.cpp)

//==============================================================
// �C���N���[�h
//==============================================================
#include <nn/gd.h>
#include <nn/gx.h>
#include <nn/math.h>
#include "Main.h"
#include "Enemy.h"
#include "deadEffect.h"
#include "GxCustm.h"
#include "Texture.h"
#include "Item.h"
#include "Map.h"

//--------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------
#define SPEED       0.2f        // �G�ړ����x
#define WIDTH0      nn::gx::DISPLAY0_HEIGHT
#define HEIGHT0     nn::gx::DISPLAY0_WIDTH
#define WIDTH1      nn::gx::DISPLAY1_HEIGHT
#define HEIGHT1     nn::gx::DISPLAY1_WIDTH
#define MAX_ENEMY1  3
#define ENEMY1_W	12.0f	// ��(�̔���)
#define ENEMY1_H	12.0f	// ����(�̔���)
#define ENEMY1_TURN_MIN	100	// ������ς��鎞��(�ŏ�)
#define ENEMY1_TURN_MAX	300	//        �V       (�ő�)
#define ENEMY1_DROP_POTION	30	// �񕜖�h���b�v��(%)

//--------------------------------------------------------------
// �\����
//--------------------------------------------------------------
typedef struct _ANIM_PAT {
    s32 frame;
    s32 count;
} ANIM_PAT;

typedef struct _TEnemy1 {   // �G1
	nn::math::VEC2	pos;	// ���W
    s32				stat;   // ���
	nn::math::VEC2	vel;	// �ړ���
    s32				timer;	// �����ύX�܂ł̎���
    s32				dir;	// ����
    s32				pat;	// 
    s32				cnt;	// 
} TEnemy1;

//--------------------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------------------

//--------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------
static ANIM_PAT g_animPat[4][3] =
{
    {{ 0, 10}, { 2, 10}, {-1, -1}},
    {{ 4, 10}, { 6, 10}, {-1, -1}},
    {{ 8, 10}, {10, 10}, {-1, -1}},
    {{12, 10}, {14, 10}, {-1, -1}},
};
static TEnemy1 enemy1[MAX_ENEMY1];

//--------------------------------------------------------------
// �G�`��
//--------------------------------------------------------------
void DrawEnemy()
{
	TEnemy1* pEnemyArray[MAX_ENEMY1];
	TEnemy1* pEnemy;
	TEnemy1* pTemp;
	TEnemy1** ppEnemy;
	s32 nEnemy = 0;
	s32 i, j;

	// �|�C���^�z��ɗL���ȗv�f�ւ̃|�C���^���R�s�[
	for (i = 0; i < MAX_ENEMY1; ++i) {
		if (enemy1[i].stat <= 0) continue;
		pEnemyArray[nEnemy] = &enemy1[i];
		++nEnemy;
	}
	// Y���W�̍~���Ń\�[�g
	for (i = 0; i < nEnemy - 1; ++i) {
		for (j = i + 1; j < nEnemy; ++j) {
			if (pEnemyArray[i]->pos.y < pEnemyArray[j]->pos.y) {
				pTemp = pEnemyArray[i];
				pEnemyArray[i] = pEnemyArray[j];
				pEnemyArray[j] = pTemp;
			}
		}
	}
	// �\�[�g��̏�����(�ォ�珇��)�\��
	ppEnemy = pEnemyArray;
	for (i = 0; i < nEnemy; ++i) {
		pEnemy = *ppEnemy;
		nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);
        nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texEnemy);
        CreateMatrix(pEnemy->pos.x, pEnemy->pos.y, 0.0f, 16.0f, 16.0f);   // �|���S���\���ʒu�Z�b�g
        SetFrame(g_animPat[pEnemy->dir][pEnemy->pat].frame);
        DrawPolygon(4, 0);	                     // �|���S���\���i���_��,���_�f�[�^�z��̗v�f�ԍ��j
		++ppEnemy;
    }
}

//--------------------------------------------------------------
// �G�X�V
//--------------------------------------------------------------
void UpdateEnemy()
{
    for (int i = 0; i < MAX_ENEMY1; ++i) {
        s32 dir = enemy1[i].dir;
        if (enemy1[i].stat > 0) {
			nn::math::VEC2 pre = enemy1[i].pos;
            enemy1[i].pos.x += enemy1[i].vel.x;
            enemy1[i].pos.y += enemy1[i].vel.y;
            if (enemy1[i].pos.x <  WIDTH0  / -2 - 32.0f ||
                enemy1[i].pos.x >= WIDTH0  /  2 + 32.0f ||
                enemy1[i].pos.y <  HEIGHT0 / -2 - 32.0f ||
                enemy1[i].pos.y >= HEIGHT0 /  2 + 32.0f) {
                enemy1[i].stat = 0;
            } else {
				// �ړ��悪�ǂȂ�
				if (GetMapAttr(enemy1[i].pos.x, enemy1[i].pos.y)) {
					enemy1[i].pos = pre;	// �ʒu��߂�
					enemy1[i].timer = 1;	// ������ς���
				}
                --enemy1[i].timer;
                if (enemy1[i].timer <= 0) {
                    if (enemy1[i].dir == 0 || enemy1[i].dir == 3) {
                        enemy1[i].dir = random(2) + 1;
                        enemy1[i].vel.y = 0.0f;
                        enemy1[i].vel.x = (3 - enemy1[i].dir) * 2.0f - 3.0f;
                    } else {
                        enemy1[i].dir = random(2) * 3;
                        enemy1[i].vel.x = 0.0f;
                        enemy1[i].vel.y = (3 - enemy1[i].dir) * 2.0f / 3.0f - 1.0f;
                    }
					enemy1[i].timer = ENEMY1_TURN_MIN + random(ENEMY1_TURN_MAX - ENEMY1_TURN_MIN);
                }
            }
        } else {
            enemy1[i].stat = 1;
            enemy1[i].dir = random(4);
			enemy1[i].timer = ENEMY1_TURN_MIN + random(ENEMY1_TURN_MAX - ENEMY1_TURN_MIN);
			switch (enemy1[i].dir) {
            case 0: // ��
                enemy1[i].vel.x = 0.0f;
                enemy1[i].vel.y = 1.0f;
                enemy1[i].pos.x = random(WIDTH0 - 64) - WIDTH0 / 2 - 32.0f;
                enemy1[i].pos.y = HEIGHT0 / -2 - 32.0f;
                break;
            case 1: // �E
                enemy1[i].vel.x = 1.0f;
                enemy1[i].vel.y = 0.0f;
                enemy1[i].pos.x = WIDTH0 / -2 - 32.0f;
                enemy1[i].pos.y = random(HEIGHT0 - 64) - HEIGHT0 / 2 - 32.0f;
                break;
            case 2: // ��
                enemy1[i].vel.x = -1.0f;
                enemy1[i].vel.y = 0.0f;
                enemy1[i].pos.x = WIDTH0 / 2 + 32.0f;
                enemy1[i].pos.y = random(HEIGHT0 - 64) - HEIGHT0 / 2 - 32.0f;
                break;
            case 3: // ��
                enemy1[i].vel.x = 0.0f;
                enemy1[i].vel.y = -1.0f;
                enemy1[i].pos.x = random(WIDTH0 - 64) - WIDTH0 / 2 - 32.0f;
                enemy1[i].pos.y = HEIGHT0 / 2 + 32.0f;
                break;
            }
        }

        if (enemy1[i].dir != dir) {
            enemy1[i].pat = enemy1[i].cnt = 0;
        }
        --enemy1[i].cnt;
        if (enemy1[i].cnt <= 0) {
            ++enemy1[i].pat;
            if (g_animPat[enemy1[i].dir][enemy1[i].pat].frame < 0) {
                enemy1[i].pat = 0;
            }
            enemy1[i].cnt = g_animPat[enemy1[i].dir][enemy1[i].pat].count;
        }
    }
}

//--------------------------------------------------------------
// �G������
//--------------------------------------------------------------
void InitEnemy()
{
    for (int i = 0; i < MAX_ENEMY1; ++i) {
        enemy1[i].stat = 0;
    }
}

//--------------------------------------------------------------
// �G�I������
//--------------------------------------------------------------
void FinEnemy()
{
}

// �����蔻��
int CollisionEnemy(nn::math::VEC2* pos,
	nn::math::VEC2* rect, s32 damage)
{
	// �S�Ă̓G�Ƃ̓����蔻��
	for (int i = 0; i < MAX_ENEMY1; ++i)
	{
		// �ҋ@��Ԃ̓G�̓X�L�b�v
		if (enemy1[i].stat == 0)
			continue;
		// ��`���m�̓����蔻��
		if (pos->x - rect->x <= enemy1[i].pos.x + ENEMY1_W &&
			enemy1[i].pos.x - ENEMY1_W <= pos->x + rect->x &&
			pos->y - rect->y <= enemy1[i].pos.y + ENEMY1_H &&
			enemy1[i].pos.y - ENEMY1_H <= pos->y + rect->y) {
			// �������Ă���
			if (damage > 0) {
				// �X�R�A���Z
				AddScore(10);
				enemy1[i].stat = 0;
				StartDeadEffect(&enemy1[i].pos);
				if (random(100) < ENEMY1_DROP_POTION)
					StartItem(&enemy1[i].pos, ITEM_POTION);
			}
			return i;
		}
	}
	return -1;
}

// �G�ʒu�擾
int GetEnemyPos(int nEnemy, nn::math::VEC2* pos)
{
	if (nEnemy >= 0 && nEnemy < MAX_ENEMY1) {
		TEnemy1* pEnemy = &enemy1[nEnemy];
		if (pEnemy->stat > 0) {
			*pos = pEnemy->pos;
			return 1;
		}
	}
	return 0;
}
