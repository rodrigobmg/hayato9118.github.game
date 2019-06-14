// �G2 ���� (Enemy2.cpp)

//==============================================================
// �C���N���[�h
//==============================================================
#include <nn/gd.h>
#include <nn/gx.h>
#include <nn/math.h>
#include "Main.h"
#include "Enemy2.h"
#include "deadEffect.h"
#include "GxCustm.h"
#include "Texture.h"
#include "Player.h"
#include "Magic.h"
#include "Item.h"

//--------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------
#define SPEED			0.8f        // �G�ړ����x
#define WIDTH0			nn::gx::DISPLAY0_HEIGHT
#define HEIGHT0			nn::gx::DISPLAY0_WIDTH
#define WIDTH1			nn::gx::DISPLAY1_HEIGHT
#define HEIGHT1			nn::gx::DISPLAY1_WIDTH
#define MAX_ENEMY2		2
#define ENEMY2_W		12.0f	// ��(�̔���)
#define ENEMY2_H		12.0f	// ����(�̔���)
#define ENEMY2_TURN_MIN	100	//������ς��鎞��(�ŏ�)
#define ENEMY2_TURN_MAX	200	//       �V       (�ő�)
#define ENEMY2_ATTACK_MIN	240	//�U���J�n����(�ŏ�)
#define ENEMY2_ATTACK_MAX	300	//     �V     (�ő�)
#define ENEMY2_DROP_POTION	30	// �񕜖�h���b�v��(%)

//--------------------------------------------------------------
// �\����
//--------------------------------------------------------------
typedef struct _ANIM_PAT {
    s32 frame;
    s32 count;
} ANIM_PAT;

typedef struct _TEnemy2 {   // �G1
	nn::math::VEC2	pos;	// ���W
    s32				stat;   // ���
	nn::math::VEC2	vel;	// �ړ���
    s32				timer;	// �����ύX�܂ł̎���
    s32				dir;	// ����
    s32				pat;	// �A�j���[�V�����p�^�[��No.
    s32				cnt;	// �A�j���[�V�����p�^�[���J�E���^
	s32				timer2;	// ���ɍU�����\�ɂȂ�܂ł̎���
} TEnemy2;

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
static TEnemy2 enemy2[MAX_ENEMY2];

//--------------------------------------------------------------
// �G2�`��
//--------------------------------------------------------------
void DrawEnemy2()
{
	TEnemy2* pEnemyArray[MAX_ENEMY2];
	TEnemy2* pEnemy;
	TEnemy2* pTemp;
	TEnemy2** ppEnemy;
	s32 nEnemy = 0;
	s32 i, j;

	// �|�C���^�z��ɗL���ȗv�f�ւ̃|�C���^���R�s�[
	for (i = 0; i < MAX_ENEMY2; ++i) {
		if (enemy2[i].stat <= 0) continue;
		pEnemyArray[nEnemy] = &enemy2[i];
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
        nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texEnemy2);
        CreateMatrix(pEnemy->pos.x, pEnemy->pos.y, 0.0f, 16.0f, 16.0f);   // �|���S���\���ʒu�Z�b�g
        SetFrame(g_animPat[pEnemy->dir][pEnemy->pat].frame);
        DrawPolygon(4, 0);	                     // �|���S���\���i���_��,���_�f�[�^�z��̗v�f�ԍ��j
		++ppEnemy;
    }
}

//--------------------------------------------------------------
// �G2�X�V
//--------------------------------------------------------------
void UpdateEnemy2()
{
    for (int i = 0; i < MAX_ENEMY2; ++i) {
        s32 dir = enemy2[i].dir;
        if (enemy2[i].stat > 0) {
            enemy2[i].pos.x += enemy2[i].vel.x;
            enemy2[i].pos.y += enemy2[i].vel.y;
            if (enemy2[i].pos.x <  WIDTH0  / -2 - 32.0f ||
                enemy2[i].pos.x >= WIDTH0  /  2 + 32.0f ||
                enemy2[i].pos.y <  HEIGHT0 / -2 - 32.0f ||
                enemy2[i].pos.y >= HEIGHT0 /  2 + 32.0f) {
                enemy2[i].stat = 0;
            } else {
				// ������ς��鏈��
                --enemy2[i].timer;
                if (enemy2[i].timer <= 0) {
                    if (enemy2[i].dir == 0 || enemy2[i].dir == 3) {
                        enemy2[i].dir = random(2) + 1;
                        enemy2[i].vel.y = 0.0f;
                        enemy2[i].vel.x = (3 - enemy2[i].dir) * 2.0f - 3.0f;
                    } else {
                        enemy2[i].dir = random(2) * 3;
                        enemy2[i].vel.x = 0.0f;
                        enemy2[i].vel.y = (3 - enemy2[i].dir) * 2.0f / 3.0f - 1.0f;
                    }
                    enemy2[i].timer = ENEMY2_TURN_MIN
+ random(ENEMY2_TURN_MAX - ENEMY2_TURN_MIN);
                }
				// �v���C���[���U�����鏈��
				--enemy2[i].timer2;
				if (enemy2[i].timer2 <= 0) {
					// �v���C���[���O�ɂ��邩�H
					nn::math::VEC2 vDir =
						GetPlayerPos() -
						enemy2[i].pos;
					if (nn::math::VEC2Dot(&vDir,
						&enemy2[i].vel) > 0.0f) {
						// ���@�U��
						AttackMagic(&enemy2[i].pos,
							&vDir, 0);
					}
					enemy2[i].timer2 =
ENEMY2_ATTACK_MIN +
random(ENEMY2_ATTACK_MAX - ENEMY2_ATTACK_MIN);
				}
            }
        } else {
            enemy2[i].stat = 1;
            enemy2[i].dir = random(4);
			enemy2[i].timer = ENEMY2_TURN_MIN
+ random(ENEMY2_TURN_MAX - ENEMY2_TURN_MIN);
			enemy2[i].timer2 = ENEMY2_ATTACK_MIN
+ random(ENEMY2_ATTACK_MAX - ENEMY2_ATTACK_MIN);
			switch (enemy2[i].dir) {
            case 0: // ��
                enemy2[i].vel.x = 0.0f;
                enemy2[i].vel.y = 1.0f;
                enemy2[i].pos.x = random(WIDTH0 - 64) - WIDTH0 / 2 - 32.0f;
                enemy2[i].pos.y = HEIGHT0 / -2 - 32.0f;
                break;
            case 1: // �E
                enemy2[i].vel.x = 1.0f;
                enemy2[i].vel.y = 0.0f;
                enemy2[i].pos.x = WIDTH0 / -2 - 32.0f;
                enemy2[i].pos.y = random(HEIGHT0 - 64) - HEIGHT0 / 2 - 32.0f;
                break;
            case 2: // ��
                enemy2[i].vel.x = -1.0f;
                enemy2[i].vel.y = 0.0f;
                enemy2[i].pos.x = WIDTH0 / 2 + 32.0f;
                enemy2[i].pos.y = random(HEIGHT0 - 64) - HEIGHT0 / 2 - 32.0f;
                break;
            case 3: // ��
                enemy2[i].vel.x = 0.0f;
                enemy2[i].vel.y = -1.0f;
                enemy2[i].pos.x = random(WIDTH0 - 64) - WIDTH0 / 2 - 32.0f;
                enemy2[i].pos.y = HEIGHT0 / 2 + 32.0f;
                break;
            }
			enemy2[i].vel *= SPEED;
        }

        if (enemy2[i].dir != dir) {
            enemy2[i].pat = enemy2[i].cnt = 0;
        }
        --enemy2[i].cnt;
        if (enemy2[i].cnt <= 0) {
            ++enemy2[i].pat;
            if (g_animPat[enemy2[i].dir][enemy2[i].pat].frame < 0) {
                enemy2[i].pat = 0;
            }
            enemy2[i].cnt = g_animPat[enemy2[i].dir][enemy2[i].pat].count;
        }
    }
}

//--------------------------------------------------------------
// �G2������
//--------------------------------------------------------------
void InitEnemy2()
{
    for (int i = 0; i < MAX_ENEMY2; ++i) {
        enemy2[i].stat = 0;
    }
}

//--------------------------------------------------------------
// �G2�I������
//--------------------------------------------------------------
void FinEnemy2()
{
}

// �����蔻��
int CollisionEnemy2(nn::math::VEC2* pos,
	nn::math::VEC2* rect, s32 damage)
{
	// �S�Ă̓G�Ƃ̓����蔻��
	for (int i = 0; i < MAX_ENEMY2; ++i)
	{
		// �ҋ@��Ԃ̓G�̓X�L�b�v
		if (enemy2[i].stat == 0)
			continue;
		// ��`���m�̓����蔻��
		if (pos->x - rect->x <=
			enemy2[i].pos.x + ENEMY2_W &&
			enemy2[i].pos.x - ENEMY2_W <=
			pos->x + rect->x &&
			pos->y - rect->y <=
			enemy2[i].pos.y + ENEMY2_H &&
			enemy2[i].pos.y - ENEMY2_H <=
			pos->y + rect->y) {
			// �������Ă���
			if (damage > 0) {
				// �X�R�A���Z
				AddScore(20);
				enemy2[i].stat = 0;
				StartDeadEffect(&enemy2[i].pos);
				if (random(100) < ENEMY2_DROP_POTION)
					StartItem(&enemy2[i].pos, ITEM_POTION);
			}
			return i;
		}
	}
	return -1;
}

// �G2�ʒu�擾
int GetEnemy2Pos(int nEnemy,
	nn::math::VEC2* pos)
{
	if (nEnemy >= 0 &&
		nEnemy < MAX_ENEMY2) {
		TEnemy2* pEnemy = &enemy2[nEnemy];
		if (pEnemy->stat > 0) {
			*pos = pEnemy->pos;
			return 1;
		}
	}
	return 0;
}
