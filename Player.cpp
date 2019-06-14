// プレイヤー処理 (Player.cpp)
#include <nn/hid.h>
#include <nn/gx.h>
#include <nn/gd.h>
#include <math.h>
#include "Main.h"
#include "Player.h"
#include "deadEffect.h"
#include "Enemy.h"
#include "Enemy2.h"
#include "Sword.h"
#include "Texture.h"
#include "GxCustm.h"
#include "Magic.h"
#include "Item.h"
#include "Sound.h"
#include "Fade.h"
#include "Map.h"
#include "Boss1.h"

//--------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------
#define SPEED			2.0f	// プレイヤー移動速度
#define HP_MAX			9		// 体力最大
#define PLAYER_WIDTH	32
#define PLAYER_HEIGHT	32

//--------------------------------------------------------------
// 構造体
//--------------------------------------------------------------
typedef struct _ANIM_PAT {
	s32 frame;
	s32 count;
} ANIM_PAT;

//--------------------------------------------------------------
// 関数プロトタイプ
//--------------------------------------------------------------
static void AdjustPlayerPos(f32 bx, f32 by);

//--------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------
static ANIM_PAT g_animPat[8][3] =
{
	{ { 1,  1 },{ -1, -1 } },
	{ { 0, 10 },{ 2, 10 },{ -1, -1 } },
	{ { 5,  1 },{ -1, -1 } },
	{ { 4, 10 },{ 6, 10 },{ -1, -1 } },
	{ { 9,  1 },{ -1, -1 } },
	{ { 8, 10 },{ 10, 10 },{ -1, -1 } },
	{ { 13,  1 },{ -1, -1 } },
	{ { 12, 10 },{ 14, 10 },{ -1, -1 } },
};
static s32  g_anim = 0;
static s32  g_pat = 0;
static s32  g_cnt = 0;

static nn::math::VEC2   g_pos;  // プレイヤー位置
static nn::math::VEC2	g_vel;	// プレイヤー速度
static s32	g_stat = 0;			// プレイヤー状態
static s32	g_timer = 0;		// プレイヤータイマー
static s32	g_hp = HP_MAX;		// プレイヤー体力

static s32 mapNo = 0;
static s32 newMapNo;
static s32 oldMapX;
static s32 oldMapY;

static bool g_key = false;

//--------------------------------------------------------------
// 初期化
//--------------------------------------------------------------
void InitPlayer()
{
	g_hp = 9;					// 体力
	g_stat = 0;					// 状態
	g_pos.x = g_pos.y = 0.0f;	// 位置
	g_vel.x = g_vel.y = 0.0f;	// 速度

	g_anim = 0;	// アニメーション番号
	g_pat = 0;	// パターン番号
	g_cnt = 0;	// フレーム数

	g_key = false;				// 鍵非所持

	if (mapNo != 0) {
		SetMap(0);
		mapNo = 0;
	}
}

//--------------------------------------------------------------
// プレイヤー位置補正
//--------------------------------------------------------------
void AdjustPlayerPos(f32 bx, f32 by)
{
	if (g_pos.x < nn::gx::DISPLAY0_HEIGHT * -0.5f + 16)
		g_pos.x = nn::gx::DISPLAY0_HEIGHT * -0.5f + 16;
	if (g_pos.x > nn::gx::DISPLAY0_HEIGHT * 0.5 - 16)
		g_pos.x = nn::gx::DISPLAY0_HEIGHT * 0.5 - 16;
	if (g_pos.y > nn::gx::DISPLAY0_WIDTH * 0.5 - 16)
		g_pos.y = nn::gx::DISPLAY0_WIDTH * 0.5 - 16;
	if (g_pos.y < nn::gx::DISPLAY0_WIDTH * -0.5 + 16)
		g_pos.y = nn::gx::DISPLAY0_WIDTH * -0.5 + 16;

	// 足元のマップを調査
	u8 attr0 = GetMapAttr(g_pos.x - 8.0f, g_pos.y - 8.0f, newMapNo);
	u8 attr1 = GetMapAttr(g_pos.x + 8.0f, g_pos.y - 8.0f, newMapNo);
	if (attr0 || attr1) {
		attr0 = GetMapAttr(bx - 8.0f, g_pos.y - 8.0f, oldMapX);
		attr1 = GetMapAttr(bx + 8.0f, g_pos.y - 8.0f, oldMapX);
		if (attr0 || attr1) {
			attr0 = GetMapAttr(g_pos.x - 8.0f, by - 8.0f, oldMapY);
			attr1 = GetMapAttr(g_pos.x + 8.0f, by - 8.0f, oldMapY);
			if (attr0 || attr1) {
				g_pos.x = bx;
				newMapNo = mapNo;
			} else {
				newMapNo = oldMapY;
			}
			g_pos.y = by;
		} else {
			g_pos.x = bx;
			newMapNo = oldMapX;
		}
	}
}

// 当たり判定&位置取得
static int CollisionPosition(int n,
	nn::math::VEC2* pos,
	nn::math::VEC2* rect,
	nn::math::VEC2* posCol)
{
	int hit = 0;
	int no;
	switch (n) {
	case 0:	// 敵1
		no = CollisionEnemy(pos, rect, 0);
		if (no >= 0)
			hit = GetEnemyPos(no, posCol);
		break;
	case 1:	// 敵2
		no = CollisionEnemy2(pos, rect, 0);
		if (no >= 0)
			hit = GetEnemy2Pos(no, posCol);
		break;
	case 2:	// 魔法
		no = CollisionMagic(pos, rect, posCol);
		if (no >= 0)
			hit = 1;
		break;
	case 3:	// アイテム
		switch (CollisionItem(pos, rect)) {
		case ITEM_POTION:
			if (g_hp < HP_MAX) {
				PlaySE(SE_ITEMOK);
				++g_hp;
			} else {
				PlaySE(SE_ITEMNG);
				g_hp = HP_MAX;
				AddScore(10);
			}
			break;
		case ITEM_KEY:
			g_key = true;
			PlaySE(SE_DOOROPEN);
			SetMapDoor();
			break;
		}
		break;
	default:
		break;
	}
	return hit;
}

//--------------------------------------------------------------
// 更新
//--------------------------------------------------------------
void UpdatePlayer()
{
	static s32 dir = 3; // 0:上、1:右、2:左、3:下
	s32 walk = 0;       // 0:待機、1:歩行

	// 現在位置保存
	f32 bx, by;
	bx = g_pos.x;
	by = g_pos.y;
	// マップNo.設定
	newMapNo = mapNo;
	oldMapX = mapNo;
	oldMapY = mapNo;
	// ダメージを受けた
	if (g_stat == 2) {
		--g_timer;
		if (g_timer > 0) {
			g_pos += g_vel;
			g_vel *= 0.9f;
			AdjustPlayerPos(bx, by);
			return;
		} else {
			g_stat = 1;	// 通常状態に復帰
		}
	}

	// プレイヤーが倒された
	if (g_stat == 3) {
		--g_timer;
		if (g_timer <= 0) {
			if (!GetFadeOutFlag()) {
				FadeOut(1.0f);
				SetFadeOutFlag();
			}
		}
		return;
	}

	// スライドパッドによる移動
	f32 dx = padStatus.stick.x / (f32)nn::hid::CTR::LIMIT_OF_STICK_CLAMP_MAX;
	f32 dy = padStatus.stick.y / (f32)nn::hid::CTR::LIMIT_OF_STICK_CLAMP_MAX;
	f32 len = (f32)sqrt(dx * dx + dy * dy);
	if (len > 0.1f) {
		g_pos.x += dx * SPEED * 2.0f;
		g_pos.y += dy * SPEED * 2.0f;
		walk = 1;
		if (fabs(dx) >= fabs(dy)) {
			if (dx < 0.0f) {
				dir = 2;
			} else {
				dir = 1;
			}
		} else {
			if (dy < 0.0f) {
				dir = 3;
			} else {
				dir = 0;
			}
		}
	}

	// 十字キーによる移動
	if (padStatus.hold & PAD_UP) {
		dir = 0;
		walk = 1;
		g_pos.y += SPEED;
	}
	if (padStatus.hold & PAD_DOWN) {
		dir = 3;
		walk = 1;
		g_pos.y -= SPEED;
	}
	if (padStatus.hold & PAD_LEFT) {
		dir = 2;
		walk = 1;
		g_pos.x -= SPEED;
	}
	if (padStatus.hold & PAD_RIGHT) {
		dir = 1;
		walk = 1;
		g_pos.x += SPEED;
	}

	s32 w = mapNo % MAX_WORLDWIDTH;
	s32 h = mapNo / MAX_WORLDWIDTH;
	if (g_pos.x < nn::gx::DISPLAY0_HEIGHT / -2 + PLAYER_WIDTH / 2) {
		w = (w + MAX_WORLDWIDTH - 1) % MAX_WORLDWIDTH;
		newMapNo = h * MAX_WORLDWIDTH + w;
		oldMapY = newMapNo;
		g_pos.x = nn::gx::DISPLAY0_HEIGHT / 2 - PLAYER_WIDTH / 2;
	} else if (g_pos.x > nn::gx::DISPLAY0_HEIGHT / 2 - PLAYER_WIDTH / 2) {
		w = (w + 1) % MAX_WORLDWIDTH;
		newMapNo = h * MAX_WORLDWIDTH + w;
		oldMapY = newMapNo;
		g_pos.x = nn::gx::DISPLAY0_HEIGHT / -2 + PLAYER_WIDTH / 2;
	} else if (g_pos.y > nn::gx::DISPLAY0_WIDTH / 2 - PLAYER_HEIGHT / 2) {
		h = (h + MAX_WORLDDEPTH - 1) % MAX_WORLDDEPTH;
		newMapNo = h * MAX_WORLDWIDTH + w;
		oldMapX = newMapNo;
		g_pos.y = nn::gx::DISPLAY0_WIDTH / -2 + PLAYER_HEIGHT / 2;
	} else if (g_pos.y < nn::gx::DISPLAY0_WIDTH / -2 + PLAYER_HEIGHT / 2) {
		h = (h + 1) % MAX_WORLDDEPTH;
		newMapNo = h * MAX_WORLDWIDTH + w;
		oldMapX = newMapNo;
		g_pos.y = nn::gx::DISPLAY0_WIDTH / 2 - PLAYER_HEIGHT / 2;
	}

	// プレイヤー位置補正
	AdjustPlayerPos(bx, by);

	// マップ切替
	if (mapNo != newMapNo) {
		mapNo = newMapNo;
		SetMap(mapNo);
	}

	// アニメーション更新
	s32 anim = dir * 2 + walk;
	if (g_anim != anim) {
		g_anim = anim;
		g_pat = g_cnt = 0;
	}
	--g_cnt;
	if (g_cnt <= 0) {
		++g_pat;
		if (g_animPat[g_anim][g_pat].frame < 0) {
			g_pat = 0;
		}
		g_cnt = g_animPat[g_anim][g_pat].count;
	}

	// 剣による攻撃
	if (g_stat < 2) {
		if (padStatus.trigger & PAD_A) {
			AttackSword(&g_pos, dir, 0);
		}
		if (padStatus.trigger & PAD_X) {
			AttackSword(&g_pos, dir, 1);
		}
	}

	// 敵との当たり判定
	if (g_stat < 2) {
		nn::math::VEC2 rect, posEnemy;
		rect.x = 12.0f;
		rect.y = 12.0f;
		for (int i = 0; i < 4; ++i) {
			if (CollisionPosition(i, &g_pos, &rect, &posEnemy)) {
				// 当たっている
				--g_hp;
				if (g_hp > 0) {
					g_stat = 2;	// ダメージを受けた
					PlaySE(SE_PLAYERDAMAGE);
					g_timer = 60;	// 硬直時間
					g_vel = g_pos - posEnemy;
					nn::math::VEC2Normalize(&g_vel, &g_vel);
					g_vel *= (SPEED * 2.0f);
				} else {
					g_stat = 3;	// 倒された
					PlaySE(SE_PLAYERDEAD);
					g_timer = 180;	// 消滅時間
					StartDeadEffect(&g_pos);
				}
				break;
			}
		}
		if (g_stat < 2) {
			s32 bs = GetBoss1Stat();
			if (bs == BOSS1_STAT_MOVE || bs == BOSS1_STAT_PAUSE) {
				if (CollisionBoss1(g_pos.x, g_pos.y, rect.x, rect.y, 0)) {
					// 当たっている
					g_hp -= 3;
					if (g_hp > 0) {
						g_stat = 2;	// ダメージを受けた
						PlaySE(SE_PLAYERDAMAGE);
						g_timer = 60;	// 硬直時間
						g_vel = g_pos - GetBoss1Pos();
						nn::math::VEC2Normalize(&g_vel, &g_vel);
						g_vel *= (SPEED * 2.0f);
					} else {
						g_stat = 3;	// 倒された
						PlaySE(SE_PLAYERDEAD);
						g_timer = 180;	// 消滅時間
						StartDeadEffect(&g_pos);
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------
// 描画
//--------------------------------------------------------------
void DrawPlayer()
{
	if (g_stat < 2 || g_stat == 2 && (g_timer & 2) != 0) {
		nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texture2D_TGA);
		CreateMatrix(g_pos.x, g_pos.y, 0.0f, 16.0f, 16.0f);	// ポリゴン表示位置セット
		SetFrame(g_animPat[g_anim][g_pat].frame);
		DrawPolygon(4, 0);                      // ポリゴン表示（頂点数,頂点データ配列の要素番号）
	}
}

//--------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------
void FinPlayer()
{

}

//--------------------------------------------------------------
// 位置取得
//--------------------------------------------------------------
nn::math::VEC2 GetPlayerPos()
{
	return g_pos;
}

//--------------------------------------------------------------
// HP取得
//--------------------------------------------------------------
s32 GetPlayerHP()
{
	if (g_hp < 0) return 0;
	return g_hp;
}

//--------------------------------------------------------------
// 鍵所持状態取得
//--------------------------------------------------------------
bool GetPlayerKey()
{
	return g_key;
}
