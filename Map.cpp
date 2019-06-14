// マップ処理 (Map.cpp)
#include <nn/gd.h>
#include <nn/gx.h>
#include <string.h>
#include "Map.h"
#include "DeadEffect.h"
#include "GxInit.h"
#include "GxCustm.h"
#include "Enemy.h"
#include "Enemy2.h"
#include "Magic.h"
#include "Sword.h"
#include "Item.h"
#include "Boss1.h"
#include "Flare.h"
#include "Sound.h"
#include "Player.h"

// 定数定義
#define BOSS1_MAP	4		// ボス1発生ポイント
#define KEY_MAP		3		// 鍵の場所
#define KEY_X		20.0f	// 鍵のX座標
#define KEY_Y		30.0f	// 鍵のY座標

// マップ データ (マップ数×15×25)
static u8 g_map[MAX_MAP][MAX_MAPDEPTH][MAX_MAPWIDTH];
static const u8 g_map_init[MAX_MAP][MAX_MAPDEPTH][MAX_MAPWIDTH] = {
#include "map_data.c"
};

// マップ属性データ
static u8 g_attr[MAX_MAP][MAX_MAPDEPTH][MAX_MAPWIDTH];
static const u8 g_attr_init[MAX_MAP][MAX_MAPDEPTH][MAX_MAPWIDTH] = {
#include "map_attr.c"
};

static u32 g_mapNo = 1;	// 現在のマップNo.

// Vertex input
static nn::gd::VertexBufferResource* bufMapCoord	= NULL;
static nn::gd::VertexBufferResource* bufMapTexCoord	= NULL;
static nn::gd::VertexBufferResource* bufMapIndex	= NULL;

// 初期化
s32 InitMap()
{
	memcpy(g_map, g_map_init, sizeof(g_map));
	u8* p = &g_map[0][0][0];
	// 縦方向のマップチップ番号入れ替え
	for (u32 u = 0; u < sizeof(g_map); ++u) {
		u8 x = *p & 15;
		u8 y = 15 - (*p >> 4);
		*p = y << 4 | x;
		++p;
	}
	memcpy(g_attr, g_attr_init, sizeof(g_attr));
	u16 idxs0[] = {1, 0, 3, 1, 3, 2};
	u16 idxs[6 * MAX_MAPWIDTH * MAX_MAPDEPTH];
	float vertex_coord0[] = {
		 0.0f,   0.0f, 0.0f, 1.0f,
		16.0f,   0.0f, 0.0f, 1.0f,
		16.0f, -16.0f, 0.0f, 1.0f,
		 0.0f, -16.0f, 0.0f, 1.0f,
	};
	for (s32 y = 0; y < MAX_MAPDEPTH; ++y) {
		for (s32 x = 0; x < MAX_MAPWIDTH; ++x) {
			s32 j = y * MAX_MAPWIDTH + x;
			for (s32 i = 0; i < 6; ++i) {
				idxs[j * 6 + i] = idxs0[i] + j * 4;
			}
		}
	}
	float vertex_coord[4 * 4 * MAX_MAPWIDTH * MAX_MAPDEPTH];
	for (s32 y = 0; y < MAX_MAPDEPTH; ++y) {
		for (s32 x = 0; x < MAX_MAPWIDTH; ++x) {
			s32 j = (y * MAX_MAPWIDTH + x) * 16;
			for (s32 i = 0; i < 4; ++i) {
				vertex_coord[j + i * 4 + 0] = (nn::gx::DISPLAY0_HEIGHT * -0.5f) + (x * 16) + vertex_coord0[i * 4 + 0];
				vertex_coord[j + i * 4 + 1] = (nn::gx::DISPLAY0_WIDTH  *  0.5f) - (y * 16) + vertex_coord0[i * 4 + 1];
				vertex_coord[j + i * 4 + 2] = vertex_coord0[i * 4 + 2];
				vertex_coord[j + i * 4 + 3] = vertex_coord0[i * 4 + 3];
			}
		}
	}

	nn::gd::VertexBufferResourceDescription desc;
	// 頂点
	desc.m_ByteSize = sizeof(vertex_coord);
	desc.m_MemLocation = nn::gd::Memory::FCRAM;
	nnResult res = nn::gd::Resource::CreateVertexBufferResource(&desc, vertex_coord, &bufMapCoord);
	if (GD_NNRESULT_IS_FAILURE(res)) {NN_PANIC("\n");}

	// テクスチャ座標
	SetMap(0);

	// インデックス
	desc.m_ByteSize = sizeof(idxs);
	res = nn::gd::Resource::CreateVertexBufferResource(&desc, idxs, &bufMapIndex);
	if (GD_NNRESULT_IS_FAILURE(res)) {NN_PANIC("\n");}

	return 1;
}

void UpdateMapChip()
{
	float texture_coord0[] = {
		0.0f   , 0.0625f, 0.0f,
		0.0625f, 0.0625f, 0.0f,
		0.0625f, 0.0f   , 0.0f,
		0.0f   , 0.0f   , 0.0f,
	};
	float texture_coord[3 * 4 * MAX_MAPWIDTH * MAX_MAPDEPTH];

	for (s32 y = 0; y < MAX_MAPDEPTH; ++y) {
		for (s32 x = 0; x < MAX_MAPWIDTH; ++x) {
			s32 j = (y * MAX_MAPWIDTH + x) * 12;
			f32 u = (g_map[g_mapNo][y][x] & 15) / 16.0f;
			f32 v = (g_map[g_mapNo][y][x] / 16) / 16.0f;
			for (s32 i = 0; i < 4; ++i) {
				texture_coord[j + i * 3 + 0] = u + texture_coord0[i * 3 + 0];
				texture_coord[j + i * 3 + 1] = v + texture_coord0[i * 3 + 1];
				texture_coord[j + i * 3 + 2] = texture_coord0[i * 3 + 2];
			}
		}
	}

	// テクスチャ
	nn::gd::VertexBufferResourceDescription desc;
	desc.m_ByteSize = sizeof(texture_coord);
	desc.m_MemLocation = nn::gd::Memory::FCRAM;
	nn::gd::VertexBufferResource* newBuffer;
	nnResult res = nn::gd::Resource::CreateVertexBufferResource(&desc, texture_coord, &newBuffer);
	if (GD_NNRESULT_IS_FAILURE(res)) { NN_PANIC("\n"); }

	nn::gd::VertexBufferResource* oldBuffer = bufMapTexCoord;
	bufMapTexCoord = newBuffer;
	if (oldBuffer) nn::gd::Resource::ReleaseVertexBufferResource(oldBuffer);
}

// マップNo.設定
void SetMap(u32 mapNo)
{
	if (g_mapNo == mapNo) return;

	if (g_mapNo == BOSS1_MAP) {
		StopBGM();
		PlayBGM(BGM_STAGE1);
	}
	if (mapNo == BOSS1_MAP) {
		StopBGM();
		PlayBGM(BGM_BOSS1);
	}

	g_mapNo = mapNo;
	UpdateMapChip();

	ClearDeadEffect();	// 浄化エフェクト消去
	ClearFlare();		// フレアエフェクト消去
	FinItem();			// アイテム終了
	FinSword();			// 剣攻撃終了
	FinMagic();			// 魔法終了
	FinEnemy2();		// 敵2終了
	FinEnemy();			// 敵終了
	InitEnemy();		// 敵初期化
	InitEnemy2();		// 敵2初期化
	InitMagic();		// 魔法初期化
	InitSword();		// 剣攻撃初期化
	InitItem();			// アイテム初期化
	if (g_mapNo == BOSS1_MAP) {
		if (GetBoss1Stat() <= BOSS1_STAT_UNINIT) {
			InitBoss1();
		} else {
			ShowBoss1();
		}
	} else {
		HideBoss1();
	}

	if (g_mapNo == KEY_MAP && !GetPlayerKey()) {
		nn::math::VEC2 pos;
		pos.x = KEY_X;
		pos.y = KEY_Y;
		StartItem(&pos, ITEM_KEY);
	}
}

// 描画
void DrawMap()
{
	Combiner();

	nn::gd::VertexBufferResource* buffers[] = {bufMapCoord, bufMapTexCoord};
	u32 offsets[] = {0, 0};
	nn::gd::VertexInputStage::SetVertexBuffers(0, 2, buffers, offsets);

	nn::gd::VertexInputStage::SetIndexBuffer(bufMapIndex, nn::gd::VertexInputStage::INDEX_FORMAT_USHORT, 0);

	nn::gd::VertexInputStage::SetPrimitiveTopology(nn::gd::VertexInputStage::PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	nn::gd::System::DrawIndexed(MAX_MAPWIDTH * MAX_MAPDEPTH * 6, 0);
}

// 解放
void FinMap()
{
	if (bufMapCoord) nn::gd::Resource::ReleaseVertexBufferResource(bufMapCoord);
	if (bufMapIndex) nn::gd::Resource::ReleaseVertexBufferResource(bufMapIndex);
	if (bufMapTexCoord) nn::gd::Resource::ReleaseVertexBufferResource(bufMapTexCoord);
}

// マップチップNo.取得
u8 GetMapChip(f32 x, f32 y, s32 mapNo)
{
	if (mapNo < 0 || mapNo >= MAX_MAP) mapNo = g_mapNo;
	s32 ix = (s32)((x + nn::gx::DISPLAY0_HEIGHT * 0.5f) / 16.0f);
	if (ix < 0) ix = 0;
	else if (ix > MAX_MAPWIDTH - 1) ix = MAX_MAPWIDTH - 1;
	s32 iy = (s32)((nn::gx::DISPLAY0_WIDTH * 0.5f - y) / 16.0f);
	if (iy < 0) iy = 0;
	else if (iy > MAX_MAPDEPTH - 1) iy = MAX_MAPDEPTH - 1;
	return g_map[mapNo][iy][ix];
}

// マップ属性取得
u8 GetMapAttr(f32 x, f32 y, s32 mapNo)
{
	if (mapNo < 0 || mapNo >= MAX_MAP) mapNo = g_mapNo;
	s32 ix = (s32)((x + nn::gx::DISPLAY0_HEIGHT * 0.5f) / 16.0f);
	if (ix < 0) ix = 0;
	else if (ix > MAX_MAPWIDTH - 1) ix = MAX_MAPWIDTH - 1;
	s32 iy = (s32)((nn::gx::DISPLAY0_WIDTH * 0.5f - y) / 16.0f);
	if (iy < 0) iy = 0;
	else if (iy > MAX_MAPDEPTH - 1) iy = MAX_MAPDEPTH - 1;
	return g_attr[mapNo][iy][ix] & 0x7f;
}

// 現在のマップNo.取得
u32 GetMap()
{
	return g_mapNo;
}

void SetMapChip(int chipNo)
{
	nn::gd::VertexBufferResource* buffers[2];
	buffers[0] = bufferCoord;
	if (chipNo >= 0 && chipNo < 256) {
		buffers[1] = bufferTex256[chipNo];
	} else {
		buffers[1] = bufferTexCoord;
		nn::gd::VertexInputStage::SetIndexBuffer(bufferIndex, nn::gd::VertexInputStage::INDEX_FORMAT_USHORT, 0);
		nn::gd::VertexInputStage::SetPrimitiveTopology(nn::gd::VertexInputStage::PRIMITIVE_TOPOLOGY_TRIANGLEFAN);
	}
	u32 offsets[] = {0, 0};
	nn::gd::VertexInputStage::SetVertexBuffers(0, 2, buffers, offsets);
}

void SetMapDoor(bool open)
{
	for (s32 i = 0; i < MAX_MAP; ++i) {
		for (s32 y = 0; y < MAX_MAPDEPTH; ++y) {
			for (s32 x = 0; x < MAX_MAPWIDTH; ++x) {
				if (open) {
					if (g_attr[i][y][x] == 2) {
						g_attr[i][y][x] = 0x80;
						g_map[i][y][x] = (15 - (227 / 16)) * 16 + (227 % 16);
					}
				} else {
					if (g_attr[i][y][x] == 0x80) {
						g_attr[i][y][x] = 2;
						g_map[i][y][x] = (15 - (31 / 16)) * 16 + (31 % 16);
					}
				}
			}
		}
	}
	UpdateMapChip();
}
