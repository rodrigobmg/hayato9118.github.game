// ボス1定義 (Boss1.h)

// 定数定義
#define BOSS1_STAT_HIDE			0		// 非表示(0以下全て)
#define BOSS1_STAT_PAUSE		1		// 一時停止
#define BOSS1_STAT_MOVE			2		// 8の字に移動
#define BOSS1_STAT_EXPLOSION	-997	// 爆発中(～-1)
#define BOSS1_STAT_HIDE_PAUSE	-997	// 一時停止(非表示)
#define BOSS1_STAT_HIDE_MOVE	-998	// 8の字に移動(非表示)
#define BOSS1_STAT_UNINIT		-999	// 未初期化

// 関数プロトタイプ
void InitBoss1();		// 初期化処理
void DrawBoss1();		// 表示処理
void DrawBoss1HP(f32 hp_x, f32 hp_y);
void UpdateBoss1();		// 移動処理
s32 CollisionBoss1(f32 x, f32 y, f32 w, f32 h, s32 str);	// 当たり判定
s32 GetBoss1Stat();		// ボス1ステータス取得
void ShowBoss1();		// 表示開始
void HideBoss1();		// 表示終了
void ResetBoss1();		// ボス1初期化
nn::math::VEC2 GetBoss1Pos();	// ボス1座標取得
