// 敵 実装 (Enemy.cpp)

//==============================================================
// インクルード
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
// マクロ定義
//--------------------------------------------------------------
#define SPEED       0.2f        // 敵移動速度
#define WIDTH0      nn::gx::DISPLAY0_HEIGHT
#define HEIGHT0     nn::gx::DISPLAY0_WIDTH
#define WIDTH1      nn::gx::DISPLAY1_HEIGHT
#define HEIGHT1     nn::gx::DISPLAY1_WIDTH
#define MAX_ENEMY1  3
#define ENEMY1_W	12.0f	// 幅(の半分)
#define ENEMY1_H	12.0f	// 高さ(の半分)
#define ENEMY1_TURN_MIN	100	// 向きを変える時間(最小)
#define ENEMY1_TURN_MAX	300	//        〃       (最大)
#define ENEMY1_DROP_POTION	30	// 回復薬ドロップ率(%)

//--------------------------------------------------------------
// 構造体
//--------------------------------------------------------------
typedef struct _ANIM_PAT {
    s32 frame;
    s32 count;
} ANIM_PAT;

typedef struct _TEnemy1 {   // 敵1
	nn::math::VEC2	pos;	// 座標
    s32				stat;   // 状態
	nn::math::VEC2	vel;	// 移動量
    s32				timer;	// 向き変更までの時間
    s32				dir;	// 向き
    s32				pat;	// 
    s32				cnt;	// 
} TEnemy1;

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
static TEnemy1 enemy1[MAX_ENEMY1];

//--------------------------------------------------------------
// 敵描画
//--------------------------------------------------------------
void DrawEnemy()
{
	TEnemy1* pEnemyArray[MAX_ENEMY1];
	TEnemy1* pEnemy;
	TEnemy1* pTemp;
	TEnemy1** ppEnemy;
	s32 nEnemy = 0;
	s32 i, j;

	// ポインタ配列に有効な要素へのポインタをコピー
	for (i = 0; i < MAX_ENEMY1; ++i) {
		if (enemy1[i].stat <= 0) continue;
		pEnemyArray[nEnemy] = &enemy1[i];
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
        nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texEnemy);
        CreateMatrix(pEnemy->pos.x, pEnemy->pos.y, 0.0f, 16.0f, 16.0f);   // ポリゴン表示位置セット
        SetFrame(g_animPat[pEnemy->dir][pEnemy->pat].frame);
        DrawPolygon(4, 0);	                     // ポリゴン表示（頂点数,頂点データ配列の要素番号）
		++ppEnemy;
    }
}

//--------------------------------------------------------------
// 敵更新
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
				// 移動先が壁なら
				if (GetMapAttr(enemy1[i].pos.x, enemy1[i].pos.y)) {
					enemy1[i].pos = pre;	// 位置を戻す
					enemy1[i].timer = 1;	// 向きを変える
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
            case 0: // 上
                enemy1[i].vel.x = 0.0f;
                enemy1[i].vel.y = 1.0f;
                enemy1[i].pos.x = random(WIDTH0 - 64) - WIDTH0 / 2 - 32.0f;
                enemy1[i].pos.y = HEIGHT0 / -2 - 32.0f;
                break;
            case 1: // 右
                enemy1[i].vel.x = 1.0f;
                enemy1[i].vel.y = 0.0f;
                enemy1[i].pos.x = WIDTH0 / -2 - 32.0f;
                enemy1[i].pos.y = random(HEIGHT0 - 64) - HEIGHT0 / 2 - 32.0f;
                break;
            case 2: // 左
                enemy1[i].vel.x = -1.0f;
                enemy1[i].vel.y = 0.0f;
                enemy1[i].pos.x = WIDTH0 / 2 + 32.0f;
                enemy1[i].pos.y = random(HEIGHT0 - 64) - HEIGHT0 / 2 - 32.0f;
                break;
            case 3: // 下
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
// 敵初期化
//--------------------------------------------------------------
void InitEnemy()
{
    for (int i = 0; i < MAX_ENEMY1; ++i) {
        enemy1[i].stat = 0;
    }
}

//--------------------------------------------------------------
// 敵終了処理
//--------------------------------------------------------------
void FinEnemy()
{
}

// 当たり判定
int CollisionEnemy(nn::math::VEC2* pos,
	nn::math::VEC2* rect, s32 damage)
{
	// 全ての敵との当たり判定
	for (int i = 0; i < MAX_ENEMY1; ++i)
	{
		// 待機状態の敵はスキップ
		if (enemy1[i].stat == 0)
			continue;
		// 矩形同士の当たり判定
		if (pos->x - rect->x <= enemy1[i].pos.x + ENEMY1_W &&
			enemy1[i].pos.x - ENEMY1_W <= pos->x + rect->x &&
			pos->y - rect->y <= enemy1[i].pos.y + ENEMY1_H &&
			enemy1[i].pos.y - ENEMY1_H <= pos->y + rect->y) {
			// 当たっている
			if (damage > 0) {
				// スコア加算
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

// 敵位置取得
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
