// マップ定義 (Map.h)
#ifndef ___H_MAP
#define ___H_MAP

#include <nn/types.h>

#define MAX_WORLDWIDTH	3			// 横マップ数
#define MAX_WORLDDEPTH	7			// 縦マップ数
#define MAX_MAP			(MAX_WORLDWIDTH*MAX_WORLDDEPTH)	// マップ数
#define MAX_MAPWIDTH	25			// マップ幅
#define MAX_MAPDEPTH	15			// マップ高さ

#define CHIP_WIDTH		16			// マップチップ幅
#define CHIP_DEPTH		16			// マップチップ高さ

s32 InitMap();						// 初期化
void SetMap(u32 mapNo);				// マップNo.設定
void DrawMap();						// 描画
void FinMap();						// 解放
u8 GetMapChip(f32 x, f32 y, s32 mapNo = -1);	// マップチップNo.取得
u8 GetMapAttr(f32 x, f32 y, s32 mapNo = -1);	// マップ属性取得
u32 GetMap();						// マップNo.取得
void SetMapChip(int chipNo = -1);
void SetMapDoor(bool open = true);

#endif // ___H_MAP
