//==============================================================
//  Polygon.cpp
//
//--------------------------------------------------------------
//                                    Author : Nakagawa Hiroki
//                                             Michiba  Hiroaki
//                                             HAL OSAKA
//==============================================================

//==============================================================
// インクルード
//==============================================================
#include <nn/gx.h>
#include <nn/math.h>
#include <nn/fs.h>
#include <nn/os.h>
#include <nn/init.h>
#include <nn/applet.h>
#include <nn/fnd/fnd_ExpHeap.h>
#include <nn/gd.h>
#include <math.h>

#include "Main.h"
#include "TexUtils.h"
#include "demo.h"
#include "Enemy.h"
#include "Enemy2.h"
#include "Title.h"
#include "GameOver.h"
#include "GameClear.h"
#include "Sword.h"
#include "deadEffect.h"
#include "Player.h"
#include "Magic.h"
#include "Item.h"
#include "Number.h"
#include "Sound.h"
#include "Fade.h"
#include "GxCustm.h"

//Shader
extern nn::gd::ShaderPipeline* shaderPipeline;
extern nn::gd::UniformLocation s_shaderVariable_view;

/* ExpHeap for app. */
extern nn::fnd::ExpHeap s_AppHeap;
extern uptr s_HeapForGx;
extern const u32 s_GxHeapSize;

extern nn::gd::RenderTarget* s_MainRenderTarget;
extern nn::gd::DepthStencilTarget* s_MainDepthStencilTarget;

extern nn::gd::Texture2D* s_texture2D_TGA;
extern nn::gd::Texture2D* s_texture2D_BMP;
extern nn::gd::SamplerState* s_samplerDefault;

//--------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------
#define STAGE_TIMER		(3*60*60)

//--------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------
void DrawFrame(void);
int  Initialize(void);
void Finalize();

//--------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------
nn::hid::PadStatus          padStatus;      // キー情報
nn::hid::TouchPanelStatus   tpStatus;       // タッチパネル情報
nn::hid::AccelerationFloat  acceleration;   // 加速度センサ情報

static nn::math::RandomGenerator	rg;		// 乱数生成器

s32 g_scene = 0;			// シーン

s32 g_score = 0;			// スコア
s32 g_timer = STAGE_TIMER;	// タイマー
s32 g_hiscore[MAX_HISCORE] = {0};

static s32 fadeout;

//==============================================================
// プログラム本編
//==============================================================

// フェードアウトフラグ取得
s32 GetFadeOutFlag()
{
	return fadeout;
}

// フェードアウトフラグ設定
void SetFadeOutFlag()
{
	fadeout = 1;
}

// 乱数生成
s32 random(s32 range)
{
	return (s32)rg.Generate((u32)range);
}

// 乱数初期化
void init_random()
{
	rg.SetSeed((s64)nn::os::Tick::GetSystemCurrent());
}

// スコア加算
void AddScore(s32 score)
{
	g_score += score;
}
// スコア取得
s32 GetScore()
{
	return g_score;
}

// ハイスコア取得
s32 GetHiScore(s32 i)
{
	if (i >= 0 && i < MAX_HISCORE)
		return g_hiscore[i];
	return 0;
}

//--------------------------------------------------------------
// ゲーム初期化
//--------------------------------------------------------------
void InitGame()
{
	PlayBGM(BGM_STAGE1);

	InitEnemy();
	InitEnemy2();
	InitSword();
	InitDeadEffect();
	InitPlayer();
	InitMagic();
	InitItem();
	g_timer = STAGE_TIMER;
	g_score = 0;

	FadeIn(1.0f);
	fadeout = 0;
}

//--------------------------------------------------------------
// ゲーム終了処理
//--------------------------------------------------------------
void FinGame()
{
	FinEnemy();
	FinEnemy2();
	FinSword();
	FinDeadEffect();
	FinPlayer();
	FinMagic();
	FinItem();

	// ハイスコア更新
	for (int i = 0; i < MAX_HISCORE; ++i) {
		if (g_hiscore[i] <= g_score) {
			if (g_hiscore[i] < g_score) {
				for (int j = MAX_HISCORE - 1; j > i; --j)
					g_hiscore[j] = g_hiscore[j - 1];
				g_hiscore[i] = g_score;
			}
			break;
		}
	}

	StopBGM();
}

// シーン設定
void SetScene(s32 scene)
{
	if (g_scene == scene) return;
	// 現在のシーンの終了処理
	switch (g_scene) {
	case 1:	// タイトル
		FinTitle();
		break;
	case 2:	// ゲーム
		FinGame();
		break;
	case 3:	// ゲームオーバー
		FinGameOver();
		break;
	case 4:	// ゲームクリア
		FinGameClear();
		break;
	}
	// シーン切り替え
	g_scene = scene;
	// 次のシーンの初期化処理
	switch (g_scene) {
	case 1:	// タイトル
		InitTitle();
		break;
	case 2:	// ゲーム
		InitGame();
		break;
	case 3:	// ゲームオーバー
		InitGameOver();
		break;
	case 4:	// ゲームクリア
		InitGameClear();
		break;
	}
}

//--------------------------------------------------------------
// マトリクス作成
//--------------------------------------------------------------
void CreateMatrix(f32 vec_tx, f32 vec_ty, f32 vec_tz, f32 scl_x, f32 scl_y, f32 scl_z)
{
    nn::math::MTX34 trans;                      // 平行移動用行列の変数
    nn::math::MTX34 scale;                      // 拡縮用行列の変数
    nn::math::MTX34 eye;                        // カメラ座標系変換行列用
    
    /*
      3DSにおけるLCDの配置方向(正位置)は短辺が横(x)、長辺が縦(y)で設定されている。
      カメラを反時計回りに90°(-1.0f)傾けることで短辺が縦(y)、長辺が横(x)になるように設定している。
    */
    nn::math::VEC3  camPos(0.0f, 0.4f, 9.5f);   // カメラの位置(X,Y,Z)
    nn::math::VEC3  camUp(-1.0f, 0.0f, 0.0f);   // カメラの上方向を決めるベクトル(X,Y,Z)
    nn::math::VEC3  target(0.0f, 0.0f, 0.0f);   // カメラが見ている点(X,Y,Z)
    
    nn::math::VEC3  transVec(vec_tx, vec_ty, vec_tz);       // 移動方向のベクトル(x,y,zの順でセット)
    nn::math::VEC3  scaleVec(scl_x, scl_y, scl_z);          // 拡大縮小のベクトル(x,y,zの順でセット)

    //---------------------------------------------------------------------------   
    nn::math::MTX34Identity(&eye);                              // カメラ座標系に単位行列をセット
    nn::math::MTX34LookAt(&eye, &camPos, &camUp, &target);      // 視野変換行列を作り出す

    nn::math::MTX34Translate(&trans, &transVec);                // 移動方向ベクトルから平行移動行列を作成
    nn::math::MTX34Mult(&eye, &eye, &trans);                    // 視野変換行列と座標変換行列（移動）を合成
    nn::math::MTX34Scale(&scale, &scaleVec);                    // 拡縮ベクトルから拡縮行列を作成
    nn::math::MTX34Mult(&eye, &eye, &scale);                    // 視野変換行列と座標変換行列（拡縮）を合成

    nn::math::MTX44 mv(eye);                                    // 合成した3*4行列を4*4行列にセット

    nn::gd::ShaderStage::SetShaderPipelineConstantFloat(shaderPipeline, s_shaderVariable_view, static_cast<f32*>(mv));
    // 視野変換×ローカル座標変換行列をシェーダパイプラインにセット
}

//--------------------------------------------------------------
// ゲーム描画処理
//--------------------------------------------------------------
void DrawGame(void)
{
    BeforeRender();     // ポリゴン表示前処理

    // Clear the render buffers
    u8 clearColor[] = {25, 25, 122, 255}; // 濃い青
    nn::gd::Memory::ClearTargets(s_MainRenderTarget, s_MainDepthStencilTarget, clearColor, 1.f, 0); // 画面クリア

    nn::gd::TextureStage::SetSamplerState(nn::gd::TextureStage::TEXTURE_UNIT_0, s_samplerDefault);

    nn::gd::TextureStage::SetTexture(nn::gd::TextureStage::TEXTURE_UNIT_0, *s_texture2D_BMP);
    CreateMatrix(0.0f, 0.0f, 0.0f, 16.0f, 8.0f);    // ポリゴン表示位置セット
    SetFrame(-1);
    DrawPolygon(4, 0);                      // ポリゴン表示（頂点数,頂点データ配列の要素番号）

    DrawEnemy();        // 敵描画
	DrawEnemy2();       // 敵2描画
	DrawPlayer();		// プレイヤー描画
	DrawItem();			// アイテム描画
	DrawSword();		// 剣描画
	DrawMagic();		// 魔法描画
	DrawDeadEffect();	// 消滅エフェクト描画

	// 数値表示
	nn::math::VEC2 pos;
	pos.x = nn::gx::DISPLAY0_HEIGHT / -2;
	pos.y = nn::gx::DISPLAY0_WIDTH / 2;
	DrawNumber(&pos, 3, g_timer / 60);
	pos.x = nn::gx::DISPLAY0_HEIGHT / 2 - 24 * 5;
	DrawNumber(&pos, 5, g_score);
	pos.x = nn::gx::DISPLAY0_HEIGHT / 2 - 24;
	pos.y = nn::gx::DISPLAY0_WIDTH / -2 + 32;
	DrawNumber(&pos, 1, GetPlayerHP());

	DrawFade();			// フェードイン・アウト

    AfterRender();		// ポリゴン表示後処理
}

//--------------------------------------------------------------
// 入力処理
//--------------------------------------------------------------
void UpdateKey(void)
{
    // Pad
    static nn::hid::PadReader padReader;
    padReader.ReadLatest(&padStatus);
    // Touch
    static nn::hid::TouchPanelReader tpReader;
    tpReader.ReadLatest(&tpStatus);
    // Acceleration
    static nn::hid::AccelerometerReader accReader;
    nn::hid::AccelerometerStatus accStatus;
    accReader.ReadLatest(&accStatus);
    accReader.ConvertToAcceleration(&acceleration, 1, &accStatus);
}

//--------------------------------------------------------------
// ゲーム更新
//--------------------------------------------------------------
void UpdateGame()
{
	UpdateFade();		// フェードイン・アウト更新
	if (IsFading())
		return;
	if (fadeout) {
		SetScene(fadeout + 2);
		return;
	}
	UpdateEnemy();		// 敵更新処理
	UpdateEnemy2();		// 敵2更新処理
	UpdatePlayer();		// プレイヤー更新処理
	UpdateSword();		// 剣攻撃更新処理
	UpdateDeadEffect();	// 消滅エフェクト更新処理
	UpdateMagic();		// 魔法更新処理
	UpdateItem();		// アイテム更新処理
	--g_timer;
	if (g_timer <= 0) {
		g_timer = 0;
		if (!fadeout) {
			FadeOut(1.0f);	// フェードアウト開始
			fadeout = 2;	// クリアでフェードアウト
		}
	}
}

//--------------------------------------------------------------
// 移動処理
//--------------------------------------------------------------
void PolyMove()
{
	switch (g_scene) {
	case 0:	// 初期化
		InitTitle();		// タイトル初期化
		g_scene = 1;
		// THRU
	case 1:	// タイトル
		UpdateTitle();		// タイトル更新処理
		break;
	case 2:	// ゲーム
		UpdateGame();		// ゲーム更新処理
		break;
	case 3:	// ゲームオーバー
		UpdateGameOver();	// ゲームオーバー更新処理
		break;
	case 4:	// ゲームクリア
		UpdateGameClear();	// ゲームクリア更新処理
		break;
	}
}

//--------------------------------------------------------------
// 上画面描画
//--------------------------------------------------------------
void DrawFrame(void)
{
	switch (g_scene) {
	case 0:	// 初期化
		break;
	case 1:	// タイトル
		DrawTitle();
		break;
	case 2:	// ゲーム
		DrawGame();
		break;
	case 3:	// ゲームオーバー
		DrawGameOver();
		break;
	case 4:	// ゲームクリア
		DrawGameClear();
		break;
	}
}

//--------------------------------------------------------------
// ゲームメイン
//--------------------------------------------------------------
void GameMain(void)
{
    UpdateKey();    // 入力処理
    PolyMove();     // 移動処理
    DrawFrame();    // 描画処理
}

//--------------------------------------------------------------
// エントリポイント
//--------------------------------------------------------------
void nnMain(void)
{
	// nn::applet::Enable() は呼ばない (SimplePlayer::Initialize()にまかせる)
	//nn::applet::Enable();

    nngxStartLcdDisplay();

	InitSound();
	Initialize();

    // hid(入力装置)の初期化
    nn::hid::Initialize();

	init_random();	// 乱数初期化

	StartSound();
	while (1)
    {
        GameMain();
        nngxWaitVSync(NN_GX_DISPLAY_BOTH); // VSyncを待ち
    }
	EndSound();

    Finalize();
	FinSound();

    /* shutdown_display */
    s_AppHeap.Free(reinterpret_cast<void*>(s_HeapForGx));
    s_AppHeap.Finalize();
}
