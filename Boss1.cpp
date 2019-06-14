// ボス1処理 (Boss1.cpp)
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

typedef struct _TBoss1 {	// ボス1
	s32				stat;	// 状態
	nn::math::VEC2	pos;	// 座標
	f32				width;	// 幅
	f32				height;	// 高さ
	s32				life;	// 体力
	s32				timer;	// 移動パターン生成用タイマー
	f32				angle;	// 移動パターン生成用角度
	s32				damage;	// ダメージ表示
} TBoss1;
static TBoss1 boss1;

// 初期化処理
void InitBoss1()
{
	boss1.stat = BOSS1_STAT_PAUSE;		// 一時停止
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

// 表示処理
void DrawBoss1()
{
	if (boss1.stat <= BOSS1_STAT_HIDE) return;	// 待機中ならスキップ

	// テクスチャ設定
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

// 移動処理
void UpdateBoss1()
{
	if (boss1.stat > BOSS1_STAT_HIDE) {
		if (boss1.damage > 0) --boss1.damage;
		// 移動中
		switch (boss1.stat) {
		case BOSS1_STAT_PAUSE:	// 一時停止
			boss1.timer--;
			if (boss1.timer <= 0) {
				boss1.stat = BOSS1_STAT_MOVE;
				boss1.timer = 300;
			}
			break;
		case BOSS1_STAT_MOVE:	// 8の字に移動
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
		// 爆発中
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
		// 待機中
	}
}

// 当たり判定
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

// 表示開始
void ShowBoss1()
{
	// 非表示中なら
	if (boss1.stat == BOSS1_STAT_HIDE_MOVE ||
		boss1.stat == BOSS1_STAT_HIDE_PAUSE) {
		// 表示再開
		boss1.stat += 999;
	}
}

// 表示終了
void HideBoss1()
{
	// 爆発中なら
	if (boss1.stat < BOSS1_STAT_HIDE && boss1.stat > BOSS1_STAT_EXPLOSION) {
		// 強制的にクリア
		boss1.stat = BOSS1_STAT_HIDE;
	}
	// 移動中(or一時停止中)なら
	if (boss1.stat > BOSS1_STAT_HIDE) {
		// 非表示に
		boss1.stat -= 999;
	}
}

// ボス1初期化
void ResetBoss1()
{
	boss1.stat = BOSS1_STAT_UNINIT;
}

// ボス1座標取得
nn::math::VEC2 GetBoss1Pos()
{
	return boss1.pos;
}

