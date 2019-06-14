// 敵2 実装 (Enemy2.cpp)

//==============================================================
// インクルード
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
// マクロ定義
//--------------------------------------------------------------
#define SPEED			0.8f        // 敵移動速度
#define WIDTH0			nn::gx::DISPLAY0_HEIGHT
#define HEIGHT0			nn::gx::DISPLAY0_WIDTH
#define WIDTH1			nn::gx::DISPLAY1_HEIGHT
#define HEIGHT1			nn::gx::DISPLAY1_WIDTH
#define MAX_ENEMY2		2
#define ENEMY2_W		12.0f	// 幅(の半分)
#define ENEMY2_H		12.0f	// 高さ(の半分)
#define ENEMY2_TURN_MIN	100	//向きを変える時間(最小)
#define ENEMY2_TURN_MAX	200	//       〃       (最大)
#define ENEMY2_ATTACK_MIN	240	//攻撃開始時間(最小)
#define ENEMY2_ATTACK_MAX	300	//     〃     (最大)
#define ENEMY2_DROP_POTION	30	// 回復薬ドロップ率(%)

//--------------------------------------------------------------
// 構造体
//--------------------------------------------------------------
typedef struct _ANIM_PAT {
    s32 frame;
    s32 count;
} ANIM_PAT;

typedef struct _TEnemy2 {   // 敵1
	nn::math::VEC2	pos;	// 座標
    s32				stat;   // 状態
	nn::math::VEC2	vel;	// 移動量
    s32				timer;	// 向き変更までの時間
    s32				dir;	// 向き
    s32				pat;	// アニメーションパターンNo.
    s32				cnt;	// アニメーションパターンカウンタ
	s32				timer2;	// 次に攻撃が可能になるまでの時間
} TEnemy2;

//--------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------

//--------------------------------------------------------------
// グローバル変数
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
// 敵2描画
//--------------------------------------------------------------
void DrawEnemy2()
{
	TEnemy2* pEnemyArray[MAX_ENEMY2];
	TEnemy2* pEnemy;
	TEnemy2* pTemp;
	TEnemy2** ppEnemy;
	s32 nEnemy = 0;
	s32 i, j;

	// ポインタ配列に有効な要素へのポインタをコピー
	for (i = 0; i < MAX_ENEMY2; ++i) {
		if (enemy2[i].stat <= 0) continue;
		pEnemyArray[nEnemy] = &enemy2[i];
		++nEnemy;
	}
	// Y座標の降順でソート
	for (i = 0; i < nEnemy - 1; ++i) {
		for (j = i + 1; j < nEnemy; ++j) {
			if (pEnemyArray[i]->pos.y < pEnemyArray[j]->pos.y) {
				pTemp = pEnemyArray[i];
				pEnemyArray[i] = pEnemyArray[j];
				pEnemyArray[j] = pTemp;
			}
		}
	}
	// ソート後の順序で(上から順に)表示
	ppEnemy = pEnemyArray;
	for (i = 0; i < nEnemy; ++i) {
		pEnemy = *ppEnemy;
		nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);
        nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texEnemy2);
        CreateMatrix(pEnemy->pos.x, pEnemy->pos.y, 0.0f, 16.0f, 16.0f);   // ポリゴン表示位置セット
        SetFrame(g_animPat[pEnemy->dir][pEnemy->pat].frame);
        DrawPolygon(4, 0);	                     // ポリゴン表示（頂点数,頂点データ配列の要素番号）
		++ppEnemy;
    }
}

//--------------------------------------------------------------
// 敵2更新
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
				// 向きを変える処理
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
				// プレイヤーを攻撃する処理
				--enemy2[i].timer2;
				if (enemy2[i].timer2 <= 0) {
					// プレイヤーが前にいるか？
					nn::math::VEC2 vDir =
						GetPlayerPos() -
						enemy2[i].pos;
					if (nn::math::VEC2Dot(&vDir,
						&enemy2[i].vel) > 0.0f) {
						// 魔法攻撃
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
            case 0: // 上
                enemy2[i].vel.x = 0.0f;
                enemy2[i].vel.y = 1.0f;
                enemy2[i].pos.x = random(WIDTH0 - 64) - WIDTH0 / 2 - 32.0f;
                enemy2[i].pos.y = HEIGHT0 / -2 - 32.0f;
                break;
            case 1: // 右
                enemy2[i].vel.x = 1.0f;
                enemy2[i].vel.y = 0.0f;
                enemy2[i].pos.x = WIDTH0 / -2 - 32.0f;
                enemy2[i].pos.y = random(HEIGHT0 - 64) - HEIGHT0 / 2 - 32.0f;
                break;
            case 2: // 左
                enemy2[i].vel.x = -1.0f;
                enemy2[i].vel.y = 0.0f;
                enemy2[i].pos.x = WIDTH0 / 2 + 32.0f;
                enemy2[i].pos.y = random(HEIGHT0 - 64) - HEIGHT0 / 2 - 32.0f;
                break;
            case 3: // 下
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
// 敵2初期化
//--------------------------------------------------------------
void InitEnemy2()
{
    for (int i = 0; i < MAX_ENEMY2; ++i) {
        enemy2[i].stat = 0;
    }
}

//--------------------------------------------------------------
// 敵2終了処理
//--------------------------------------------------------------
void FinEnemy2()
{
}

// 当たり判定
int CollisionEnemy2(nn::math::VEC2* pos,
	nn::math::VEC2* rect, s32 damage)
{
	// 全ての敵との当たり判定
	for (int i = 0; i < MAX_ENEMY2; ++i)
	{
		// 待機状態の敵はスキップ
		if (enemy2[i].stat == 0)
			continue;
		// 矩形同士の当たり判定
		if (pos->x - rect->x <=
			enemy2[i].pos.x + ENEMY2_W &&
			enemy2[i].pos.x - ENEMY2_W <=
			pos->x + rect->x &&
			pos->y - rect->y <=
			enemy2[i].pos.y + ENEMY2_H &&
			enemy2[i].pos.y - ENEMY2_H <=
			pos->y + rect->y) {
			// 当たっている
			if (damage > 0) {
				// スコア加算
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

// 敵2位置取得
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
