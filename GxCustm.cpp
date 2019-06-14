//==============================================================
//  GxCustm.cpp
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
#include "demo.h"

#include "TexUtils.h"
#include "GxInit.h"
#include "GxCustm.h"

#define WAIT_VSYNC
#define PRINT(__msg) NN_LOG __msg

int disp0_Format = 0, disp0_Width = 0, disp0_Height = 0;
int disp1_Format = 0, disp1_Width = 0, disp1_Height = 0;

//--------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------

//--------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------


//==============================================================
// プログラム本編
//==============================================================
//--------------------------------------------------------------
// 頂点データ初期化
//--------------------------------------------------------------
void InitObjects(void)
{
    u16 idxs[] = {0, 1, 2, 3};
    nnResult res;

	/* ポリゴン頂点 */
	float vertex_coord[] = {
		-1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 1.0f,
	};
	/* テクスチャ　*/
	float texture_coord[]={
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};

	/*
	  データフォーマット作成
	  第1引数:配列ID　0:vertex_coord 1:texture_coord
	  第2引数:データ種別
	  第3引数:データ型
	  第4引数:一組のデータ数 4:x,y,z,w  3:u,v,z
	  第5引数:配列のインデックス(どこから使用するか)
	*/
    nn::gd::InputElementDescription descs[] =
    {
        { 0, "aPosition", nn::gd::VertexInputStage::STREAM_TYPE_FLOAT, 4, 0},
		{ 1, "aTexCoord", nn::gd::VertexInputStage::STREAM_TYPE_FLOAT, 3, 0}
	};

	u32 strides[] = { sizeof(float) * 4, sizeof(float) * 3 };   /* ID順 一組のデータサイズ */
	
	/*
	  inputLayoutに対して値をセット
	  第1引数:データフォーマットの配列のポインタ
	  第2引数:フォーマット数
	  第3引数:一組のデータ数の配列を持ったポインタ
	  第4引数:シェーダオブジェクトのポインタ
	  第5引数:上記のデータをセットする構造体のポインタ
	*/
    res = nn::gd::VertexInputStage::CreateInputLayout(descs, 2, strides, vertexShader, &inputLayout);
    if ( GD_NNRESULT_IS_FAILURE(res) ) {NN_PANIC("\n");}

	nn::gd::VertexBufferResourceDescription desc;
    /* 頂点 */
	desc.m_ByteSize = sizeof(vertex_coord);
    desc.m_MemLocation = nn::gd::Memory::FCRAM;
    res = nn::gd::Resource::CreateVertexBufferResource(&desc, vertex_coord, &bufferCoord);
    if ( GD_NNRESULT_IS_FAILURE(res) ) {NN_PANIC("\n");}

	/* テクスチャ */
	desc.m_ByteSize = sizeof(texture_coord);
    desc.m_MemLocation = nn::gd::Memory::FCRAM;
    res = nn::gd::Resource::CreateVertexBufferResource(&desc, texture_coord, &bufferTexCoord);
    if ( GD_NNRESULT_IS_FAILURE(res) ) {NN_PANIC("\n");}
	f32 uv[3 * 4];
	for (s32 y = 0; y < 4; ++y) {
		for (s32 x = 0; x < 4; ++x) {
			for (s32 v = 0; v < 4; ++v) {
				uv[v * 3 + 0] = (x + texture_coord[v * 3 + 0]) * 0.25f;
				uv[v * 3 + 1] = (y + texture_coord[v * 3 + 1]) * 0.25f;
				uv[v * 3 + 2] = 0.0f;
			}
			desc.m_ByteSize = sizeof(uv);
			desc.m_MemLocation = nn::gd::Memory::FCRAM;
			res = nn::gd::Resource::CreateVertexBufferResource(&desc, uv, &bufferTex4x4[y * 4 + x]);
			if ( GD_NNRESULT_IS_FAILURE(res) ) {NN_PANIC("\n");}
		}
	}
	for (int v = 0; v < 16; ++v) {
		for (int u = 0; u < 16; ++u) {
			uv[0] = u * 0.0625f; uv[1] = (v + 1) * 0.0625f; uv[2] = 0.0f;
			uv[3] = (u + 1) * 0.0625f; uv[4] = (v + 1) * 0.0625f; uv[5] = 0.0f;
			uv[6] = (u + 1) * 0.0625f; uv[7] = v * 0.0625f; uv[8] = 0.0f;
			uv[9] = u * 0.0625f; uv[10] = v * 0.0625f; uv[11] = 0.0f;
			res = nn::gd::Resource::CreateVertexBufferResource(&desc, uv, &bufferTex256[v * 16 + u]);
			if (GD_NNRESULT_IS_FAILURE(res)) { NN_PANIC("\n"); }
		}
	}

	/* インデックス */
    desc.m_ByteSize = sizeof(idxs);
    res = nn::gd::Resource::CreateVertexBufferResource(&desc, idxs, &bufferIndex);
    if ( GD_NNRESULT_IS_FAILURE(res) ) {NN_PANIC("\n");}

	nn::gd::VertexBufferResource* buffers[] = { bufferCoord,bufferTexCoord };
	u32 offsets[] = { 0,0 };

	nn::gd::VertexInputStage::SetVertexBuffers(0, 2, buffers, offsets);

    nn::gd::VertexInputStage::SetInputLayout(inputLayout);

    nn::gd::VertexInputStage::SetIndexBuffer(bufferIndex, nn::gd::VertexInputStage::INDEX_FORMAT_USHORT, 0);

	/* プリミティブの指定 */
	nn::gd::VertexInputStage::SetPrimitiveTopology(nn::gd::VertexInputStage::PRIMITIVE_TOPOLOGY_TRIANGLEFAN);
}

//--------------------------------------------------------------
// 透視投影変換 ビューポート変換設定　 バックバッファ作成
//--------------------------------------------------------------
void BeforeRender(void)
{
    static nn::gd::TargetProperties mainRenderTargetProperty;
    if (disp0_Width == 0)
    {   //Done only once
        nngxActiveDisplay(NN_GX_DISPLAY0);
        nngxGetDisplaybufferParameteri(NN_GX_DISPLAYBUFFER_FORMAT, &disp0_Format);
        nngxGetDisplaybufferParameteri(NN_GX_DISPLAYBUFFER_WIDTH, &disp0_Width);
        nngxGetDisplaybufferParameteri(NN_GX_DISPLAYBUFFER_HEIGHT, &disp0_Height);

    	nngxActiveDisplay(NN_GX_DISPLAY1);
        nngxGetDisplaybufferParameteri(NN_GX_DISPLAYBUFFER_FORMAT, &disp1_Format);
        nngxGetDisplaybufferParameteri(NN_GX_DISPLAYBUFFER_WIDTH, &disp1_Width);
        nngxGetDisplaybufferParameteri(NN_GX_DISPLAYBUFFER_HEIGHT, &disp1_Height);
        nn::gd::OutputStage::GetRenderTargetProperties(s_MainRenderTarget, &mainRenderTargetProperty);

        //Set the color/depthstencil targets
        nn::gd::OutputStage::SetRenderTarget(s_MainRenderTarget);
        nn::gd::OutputStage::SetDepthStencilTarget(s_MainDepthStencilTarget);

		//Set the viewport
        nn::gd::Viewport viewPort(0, 0, mainRenderTargetProperty.m_Width, mainRenderTargetProperty.m_Height);
        nn::gd::RasterizerStage::SetViewport(viewPort);

        //Projection matrix
        nn::math::Matrix44 proj;
        //nn::math::MTX44Frustum(&proj, -0.02f, 0.02f, -0.02f*nn::gx::DISPLAY0_HEIGHT/nn::gx::DISPLAY0_WIDTH, 0.02f*nn::gx::DISPLAY0_HEIGHT/nn::gx::DISPLAY0_WIDTH, 0.2f, 100.f);
		nn::math::MTX44Ortho(&proj, -0.5f * nn::gx::DISPLAY0_WIDTH, 0.5f * nn::gx::DISPLAY0_WIDTH, -0.5f * nn::gx::DISPLAY0_HEIGHT, 0.5f * nn::gx::DISPLAY0_HEIGHT, 0.2f, 100.0f);
        nn::gd::ShaderStage::SetShaderPipelineConstantFloat(shaderPipeline, s_shaderVariable_proj, static_cast<f32*>(proj));
		//↑透視投影変換のための行列をシェーダパイプラインにセットしている

		BlendStateDesc();
    }
}

//--------------------------------------------------------------
// ポリゴン描画
//--------------------------------------------------------------
void DrawPolygon(u32 p_prim, u32 p_index, const u8* pColor)
{
	Combiner(pColor);
	nn::gd::System::DrawIndexed(p_prim, p_index);
}

//--------------------------------------------------------------
// バックバッファの転送 画面出力
//--------------------------------------------------------------
void AfterRender(void)
{
	//Transfer framebuffer to display buffer
	{
        int dstAddr;
        nngxActiveDisplay(NN_GX_DISPLAY0);
        nngxBindDisplaybuffer(s_displaybuffer0[s_CurrentDisplaybuffer0]);
        nngxGetDisplaybufferParameteri(NN_GX_DISPLAYBUFFER_ADDRESS, &dstAddr);
        nn::gd::Memory::CopyTexture2DResourceBlockToLinear(
            s_texture2DResource_MainColorBuffer,                    //source
            0,                                                      //sourceMipLevelIndex
            0,                                                      //srcOffestY
            (u8*)dstAddr,                                           //dstAddr
            disp0_Width,                                            //dstWidth
            disp0_Height,                                           //dstHeight
            nn::gd::Resource::NATIVE_FORMAT_RGB_888,                //dstFormat
            nn::gd::Memory::DOWNSCALING_NONE,                       //DownScalingMode
            GD_FALSE                                                //yFlip
        );
    }

    // Wait till all command requests are processed
	//描画処理（表示できるようになるまで）を待つ
	nngxWaitCmdlistDone();

    // Swap the display buffers
    nngxActiveDisplay(NN_GX_DISPLAY1);
    nngxBindDisplaybuffer(s_displaybuffer1[s_CurrentDisplaybuffer1]);
    nngxSwapBuffers(NN_GX_DISPLAY_BOTH);
    s_CurrentDisplaybuffer0 = (s_CurrentDisplaybuffer0 == 0 ? 1 : 0);
    s_CurrentDisplaybuffer1 = (s_CurrentDisplaybuffer1 == 0 ? 1 : 0);

    // Clear the command list
    nngxClearCmdlist();
	
    nngxRunCmdlist();
}

//--------------------------------------------------------------
// コンバイナ設定
//--------------------------------------------------------------
void Combiner(const u8* pColor)
{
	nn::gd::CombinerDescription combiner;
	combiner.ToDefault();

	// 下の情報　TextureColor + 定数 + 定数
	combiner.SetSourceRGB(	nn::gd::CombinerStage::UNIT0,				//input combiner ID
							nn::gd::CombinerStage::SOURCE_TEXTURE0,		//input soruce RGB 0
							nn::gd::CombinerStage::SOURCE_CONSTANT,		//input soruce RGB 1
							nn::gd::CombinerStage::SOURCE_CONSTANT);	//input soruce RGB 2
	// 下の情報　TextureAlpha + 定数 + 定数
	combiner.SetSourceAlpha(nn::gd::CombinerStage::UNIT0,				//input combiner ID
							nn::gd::CombinerStage::SOURCE_TEXTURE0,		//input source Alpha 0
							nn::gd::CombinerStage::SOURCE_CONSTANT,		//input source Alpha 1
							nn::gd::CombinerStage::SOURCE_CONSTANT);	//input source Alpha 2

	combiner.SetOperandRGB(	nn::gd::CombinerStage::UNIT0,					//combiner ID
							nn::gd::CombinerStage::OPERAND_RGB_SRC_COLOR ,	//input0 RGBではRGBすべて使用
							nn::gd::CombinerStage::OPERAND_RGB_SRC_COLOR,	//input1 RGBではRGBすべて使用
							nn::gd::CombinerStage::OPERAND_RGB_SRC_COLOR);	//input2 RGBではRGBすべて使用

	combiner.SetOperandAlpha(nn::gd::CombinerStage::UNIT0,					//combiner ID
							 nn::gd::CombinerStage::OPERAND_ALPHA_SRC_ALPHA,	//input0 Alphaではアルファの値を反転して使う
							 nn::gd::CombinerStage::OPERAND_ALPHA_SRC_ALPHA,	//input1 Alphaではアルファの値をそのまま使う
							 nn::gd::CombinerStage::OPERAND_ALPHA_SRC_ALPHA);//input2 Alphaではアルファの値をそのまま使う

	combiner.SetCombineRGB(	nn::gd::CombinerStage::UNIT0,
							pColor ?
							nn::gd::CombinerStage::COMBINE_RGB_MODULATE :
							nn::gd::CombinerStage::COMBINE_RGB_REPLACE);

	combiner.SetCombineAlpha(nn::gd::CombinerStage::UNIT0,
							 nn::gd::CombinerStage::COMBINE_ALPHA_REPLACE);

	combiner.SetCombinerInUse(nn::gd::CombinerStage::UNIT0, GD_TRUE);

	nn::gd::CombinerState* comState = s_texComState;
	nn::gd::CombinerStage::CreateTextureCombinerState(&combiner, &s_texComState);
	nn::gd::CombinerStage::SetTextureCombinerState(s_texComState);
	if (pColor) {
		nn::gd::CombinerStage::SetTextureCombinerUnitConstantColor(nn::gd::CombinerStage::UNIT0,
			pColor[0], pColor[1], pColor[2], pColor[3]);
	}
	if (comState) nn::gd::CombinerStage::ReleaseTextureCombinerState(comState);
}

//--------------------------------------------------------------
// ブレンドステイト設定
//--------------------------------------------------------------
void BlendStateDesc(void)
{
	nn::gd::BlendState* bd;

    nn::gd::BlendStateDescription desc;
    desc.ToDefault();
    desc.m_BlendType = nn::gd::OutputStage::BLEND_TYPE_BLENDING;
    desc.SetBlendFunc(	nn::gd::OutputStage::BLEND_FUNCTION_SRC_ALPHA,
	     				nn::gd::OutputStage::BLEND_FUNCTION_ONE_MINUS_SRC_ALPHA,
						nn::gd::OutputStage::BLEND_EQUATION_ADD);
	bd = s_bd;
    nn::gd::OutputStage::CreateBlendState(&desc, &s_bd);
    nn::gd::OutputStage::SetBlendState(s_bd);
	if (bd) nn::gd::OutputStage::ReleaseBlendState(bd);
}

//--------------------------------------------------------------
// アニメーション フレーム設定
//--------------------------------------------------------------
void SetFrame(s32 frame)
{
	nn::gd::VertexBufferResource* buffers[2];
	buffers[0] = bufferCoord;
	if (frame >= 0 && frame < 16) {
		buffers[1] = bufferTex4x4[frame];
	} else if (frame >= 16 && frame < 16 + 256) {
		buffers[1] = bufferTex256[frame - 16];
	} else {
		buffers[1] = bufferTexCoord;
	}
	u32 offsets[] = {0, 0};
	nn::gd::VertexInputStage::SetVertexBuffers(0, 2, buffers, offsets);
}
