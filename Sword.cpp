// 剣による攻撃処理 (Sword.cpp)
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

// マクロ定義
#define SPEED	10.0f	// 速さ
#define LIFE	30		// 生存時間(単位:フレーム(1/60s))

// 剣の情報
typedef struct _SWORD {
	s32				stat;
// 状態(0:未初期化、1:待機、2:剣、3:ビーム)
	s32				dir;
// 方向(0:上、1:右、2:左、3:下)
	nn::math::VEC2	pos;	// 位置
	nn::math::VEC2	vel;	// 速度
	s32				timer;	// タイマー
	s32				str;	// 攻撃力
} SWORD;

static SWORD	g_sword;	// 剣情報

// 関数プロトタイプ
static void CheckCollision();// 当たり判定

// 初期化
void InitSword() 
{
	g_sword.stat = 1;	// 状態=待機
}

// 更新
void UpdateSword()
{
	switch (g_sword.stat) {
	case 0:	// 未初期化
		InitSword();
		// THRU
	case 1:	// 待機
		break;
	case 2:	// 剣
		--g_sword.timer;
		if (g_sword.timer <= 0) {
			g_sword.stat = 1;
			break;
		}
		// 当たり判定
		CheckCollision();
		break;
	case 3:	// ビーム
		g_sword.pos += g_sword.vel;
		if (g_sword.pos.x <= nn::gx::DISPLAY0_HEIGHT * -0.5f + 16 ||
			g_sword.pos.x >= nn::gx::DISPLAY0_HEIGHT * 0.5f - 16 ||
			g_sword.pos.y <= nn::gx::DISPLAY0_WIDTH * -0.5f + 16 ||
			g_sword.pos.y >= nn::gx::DISPLAY0_WIDTH * 0.5f - 16) {
			g_sword.stat = 1;	// 待機へ
			// TODO: ここで消滅エフェクト発生.
			StartDeadEffect(&g_sword.pos);
			break;
		}
		// 当たり判定
		CheckCollision();
		break;
	}
}

// 描画
void DrawSword()
{
	switch (g_sword.stat) {
	case 2:	// 剣
	case 3:	// ビーム
		nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);
		nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texture2D_TGA);
		CreateMatrix(g_sword.pos.x, g_sword.pos.y, 0.0f, 16.0f, 16.0f);
		SetFrame(g_sword.dir * 4 + 3);
		DrawPolygon(4, 0);
		break;
	}
}

// 終了処理
void FinSword()
{
}

// 攻撃
void AttackSword(nn::math::VEC2* pos, s32 dir, s32 beam)
{
	if (g_sword.stat == 2 || g_sword.stat == 3) {
		return;	// 攻撃中
	}
	g_sword.pos = *pos;
	g_sword.dir = dir;
	switch (dir) {
	case 0:	// 上
		g_sword.vel.x = 0.0f;
		g_sword.vel.y = 1.0f;
		break;
	case 1:	// 右
		g_sword.vel.x = 1.0f;
		g_sword.vel.y = 0.0f;
		break;
	case 2:	// 左
		g_sword.vel.x = -1.0f;
		g_sword.vel.y = 0.0f;
		break;
	case 3:	// 下
		g_sword.vel.x = 0.0f;
		g_sword.vel.y = -1.0f;
		break;
	}
	// キャラクタの向いている方向にずらす.
	g_sword.pos += g_sword.vel * 32;
	if (beam) {
		PlaySE(SE_BEAM);
		// 向いている方向に速度を設定.
		g_sword.vel *= SPEED;
		// 持続時間はゼロ.
		g_sword.timer = 0;
		// 状態を"ビーム"に.
		g_sword.stat = 3;
		// 攻撃力を設定.
		g_sword.str = 1;
	} else {
		PlaySE(SE_SWORD);
		// 速度はゼロ.
		g_sword.vel *= 0.0f;
		// 持続時間を設定.
		g_sword.timer = LIFE;
		// 状態を"剣"に.
		g_sword.stat = 2;
		// 攻撃力を設定.
		g_sword.str = 5;
	}
}

// 当たり判定
void CheckCollision()
{
	// 剣の向きで矩形サイズを変更
	nn::math::VEC2 rect;
	switch (g_sword.dir) {
	case 0:	// 上
	case 3:	// 下
		rect.x = 4.0f;
		rect.y = 16.0f;
		break;
	default:// 右、左
		rect.x = 16.0f;
		rect.y = 4.0f;
		break;
	}
	// 敵との当たり判定
	if (CollisionEnemy(&g_sword.pos, &rect, g_sword.str) >= 0) {
		// ビームが当たっていたら「待機」に
		if (g_sword.stat == 3)
			g_sword.stat = 1;
	} else {
		// 敵2との当たり判定
		if (CollisionEnemy2(&g_sword.pos, &rect, g_sword.str) >= 0) {
			// ビームが当たっていたら「待機」に
			if (g_sword.stat == 3)
				g_sword.stat = 1;
		} else {
			// ボス1との当たり判定
			if (CollisionBoss1(g_sword.pos.x, g_sword.pos.y, rect.x, rect.y, g_sword.str)) {
				g_sword.str = 0;
				// ビームが当たっていたら「待機」に
				if (g_sword.stat == 3)
					g_sword.stat = 1;
			}
		}
	}
}
