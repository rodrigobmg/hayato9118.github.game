/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_RenderSystemDrawing.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_RENDER_SYSTEM_DRAWING_H_
#define DEMO_RENDER_SYSTEM_DRAWING_H_

#include "demo/Utility/demo_Utility.h"
#include "demo/Render/demo_RenderSystem.h"
#include "demo/Render/demo_GraphicsDrawing.h"

namespace demo
{
    /*!
        @brief CTRで、簡易2D描画を行うためのクラス。        
    */
  
    /*!
    :private    

    @brief GraphicsDrawingクラスが同時に保持できる最大テクスチャオブジェクト数
    */
    const u32 MAX_TEXTURES_NUM = 16;
    
    /*!
    :private

    @brief GraphicsDrawingクラスがテクスチャマッピングの際に描画データとして持つことが可能な三角形の最大数
    */
    const u32 MAX_TEXTURED_TRIANGLES_NUM = 64;
    
    /*! 
    :private
    
    @brief GraphicsDrawingクラスがテクスチャマッピングの際に描画データとして持つことが可能な三角形の最大インデックス数
    */    
    const u32 MAX_TEXTURED_INDICES_NUM = MAX_TEXTURED_TRIANGLES_NUM * 3;

    class RenderSystemDrawing : public demo::RenderSystem
    {
    public:
        RenderSystemDrawing(void);
        virtual ~RenderSystemDrawing(void);

    public:
        virtual void Initialize(const uptr fcramAddress, const size_t memorySize,
            const u32 commandBufferSize = 0x40000, const u32 requestNum = 128,
            const bool serialRunMode = true,
            const DisplayBuffersDescription& displayBuffers0Desc = DisplayBuffersDescription::GetDefaultDisplay0Description(),
            const DisplayBuffersDescription& displayBuffers1Desc = DisplayBuffersDescription::GetDefaultDisplay1Description(),
            const FrameBufferDescription& frameBuffer0Desc = FrameBufferDescription::GetDefaultDisplay0FrameBufferDescription(),
            const DisplayBuffersDescription& displayBuffers0ExtDesc = DisplayBuffersDescription::GetDefaultDisplay0ExtDescription(),
            const bool isFillBlackLCD = true );

   public:
        /*!
        @brief    描画する2Dの三角形の初期処理をします。

                  この関数はDrawTriangle2d()内で暗黙的に呼ばれるため、明示的に呼ぶ必要はないです。                  
                  上下のLCDに描画する2Dの三角形の最大数を変更したいときだけ,                  
                  Initialize()の直後に呼んで下さい。
                          
        @param[in]   maxTrianglesNum      上下のLCDそれぞれに描画する三角形の最大数(デフォルトではMAX_TRIANGLES_NUM)
        */
        void InitializeTriangles(const u32 maxTrianglesNum = demo::MAX_TRIANGLES_NUM);          

        /*!
        @brief    描画する2Dの四角形の初期処理をします。

                  この関数はDrawSquare2d()内で暗黙的に呼ばれるため、明示的に呼ぶ必要はないです。                  
                  上下のLCDに描画する2Dの四角形(点、直線を含む)の最大数を変更したいときだけ,                  
                  Initialize()の直後に呼んで下さい。
                          
        @param[in]   maxSquaresNum      上下のLCDそれぞれに描画する四角形の最大数(デフォルトではMAX_SQUARES_NUM)
        */
        void InitializeSquares(const u32 maxSquaresNum = demo::MAX_SQUARES_NUM);

        /*!
        @brief    描画する2Dのテキストの初期処理をします。

                  この関数はDrawText2d()内で暗黙的に呼ばれるため、明示的に呼ぶ必要はないです。                  
                  上下のLCDに描画する2Dのテキストの最大文字数を変更したいときだけ,                  
                  Initialize()の直後に呼んで下さい。
                          
        @param[in]   maxLength      上下のLCDそれぞれに描画する最大文字数(デフォルトではMAX_TEXT_LENGTH)
        */
        void InitializeTexts(const u32 maxLength = demo::MAX_TEXT_LENGTH);

        /*!
        @brief    描画する2Dのテクスチャマッピングした三角形の初期処理をします。

                  この関数はDrawTexturedTriangle2d()内で暗黙的に呼ばれるため、明示的に呼ぶ必要はないです。                  
                  上下のLCDに描画する2Dのテクスチャマッピングした三角形の最大数を変更したいときだけ,                  
                  Initialize()の直後に呼んで下さい。
                          
        @param[in]   maxTrianglesNum      上下のLCDそれぞれに描画するテクスチャマッピングした三角形の最大数(デフォルトではMAX_TEXTURED_TRIANGLES_NUM)
        */
        void InitializeTexturedTriangles(const u32 maxTrianglesNum = MAX_TEXTURED_TRIANGLES_NUM);

        virtual void Finalize(void);

        virtual void SetViewport(const GLint x, const GLint y, const GLsizei width, const GLsizei height);
        
        virtual void SetClearColor(const s32 display, const f32 red, const f32 green, const f32 blue, const f32 alpha);

        virtual void Clear(const GLbitfield mask = (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) );

    public:
        /*!
        @brief    2D描画するプリミティブの頂点カラーを設定します。
                        
        @param[in]    red        頂点カラーの赤成分 ([0.0f,1.0f])
        @param[in]    green      頂点カラーの緑成分 ([0.0f,1.0f])
        @param[in]    blue       頂点カラーの青成分 ([0.0f,1.0f])
        @param[in]    alpha      頂点カラーのアルファ成分 ([0.0f,1.0f])
        */
        virtual void SetColor(const f32 red, const f32 green, const f32 blue, const f32 alpha);

        /*!
        @brief    2D描画するプリミティブの頂点カラーを設定します。
                        
        @param[in]    red        頂点カラーの赤成分 ([0.0f,1.0f])
        @param[in]    green      頂点カラーの緑成分 ([0.0f,1.0f])
        @param[in]    blue       頂点カラーの青成分 ([0.0f,1.0f])
        */
        virtual void SetColor(const f32 red, const f32 green, const f32 blue);

        /*!
        @brief    2D描画するプリミティブの深度値を設定します。
                        
        @param[in]    depth2d    正規化デバイス座標の深度値 ([0.0f,1.0f])
        */
        virtual void SetDepth(const f32 depth);

        /*!
        @brief    プリミティブを2D描画するごとに自動的に深度値を減らす設定します。
                        
        @param[in]    enableAutoDepth2d   自動的に深度値を減らす設定をするかどうかについて (true または false)
        */
        virtual void SetAutoDepth(const bool enableAutoDepth);


    public:
        /*!
        @brief    点を2D描画するときのサイズを設定します。
                        
        @param[in]     pointSize   点のピクセル単位の大きさ ([1.0f,])
        */
        virtual void SetPointSize(const f32 pointSize);

        /*!
        @brief    ウィンドウ座標系で指定した点を2D描画します。
        
        @param[in]    windowCoordinateX   ウィンドウ座標系の点の中心のX座標 (f32の範囲)
        @param[in]    windowCoordinateY   ウィンドウ座標系の点の中心のY座標 (f32の範囲)
        */
        virtual void DrawPoint(const f32 windowCoordinateX, const f32 windowCoordinateY);
        
    public:
        /*!
        @brief    線を2D描画するときの太さを設定します。
                        
        @param[in]    lineWidth    線のピクセル単位の太さ ([1.0f,])
        */
        virtual void SetLineWidth(const f32 lineWidth);
        
        /*!
        @brief    ウィンドウ座標系で指定した線分を2D描画します。

        @param[in]    windowCoordinateX0   ウィンドウ座標系の線分の始点のX座標 (f32の範囲)
        @param[in]    windowCoordinateY0   ウィンドウ座標系の線分の始点のY座標 (f32の範囲)
        @param[in]    windowCoordinateX1   ウィンドウ座標系の線分の終点のX座標 (f32の範囲)
        @param[in]    windowCoordinateY1   ウィンドウ座標系の線分の終点のY座標 (f32の範囲)
        */
        virtual void DrawLine(const f32 windowCoordinateX0, const f32 windowCoordinateY0,
            const f32 windowCoordinateX1, const f32 windowCoordinateY1);

    public:
        /*!
        @brief    ウィンドウ座標で指定した四角形を2D描画します。
        
        頂点のデータは反時計周りに与えて下さい。

        @param[in]    windowCoordinateX0   ウィンドウ座標系の四角形の左上の頂点のX座標 (f32の範囲)
        @param[in]    windowCoordinateY0   ウィンドウ座標系の四角形の左上の頂点のY座標 (f32の範囲)
        @param[in]    windowCoordinateX1   ウィンドウ座標系の四角形の左下の頂点のX座標 (f32の範囲)
        @param[in]    windowCoordinateY1   ウィンドウ座標系の四角形の左下の頂点のY座標 (f32の範囲)
        @param[in]    windowCoordinateX2   ウィンドウ座標系の四角形の右下の頂点のX座標 (f32の範囲)
        @param[in]    windowCoordinateY2   ウィンドウ座標系の四角形の右下の頂点のY座標 (f32の範囲)
        @param[in]    windowCoordinateX3   ウィンドウ座標系の四角形の右上の頂点のX座標 (f32の範囲)
        @param[in]    windowCoordinateY3   ウィンドウ座標系の四角形の右上の頂点のY座標 (f32の範囲)            
        */
        virtual void FillSquare(const f32 windowCoordinateX0, const f32 windowCoordinateY0,
            const f32 windowCoordinateX1, const f32 windowCoordinateY1,
            const f32 windowCoordinateX2, const f32 windowCoordinateY2,
            const f32 windowCoordinateX3, const f32 windowCoordinateY3);
    
    public:
        /*!
        @brief    ウィンドウ座標で指定した長方形を2D描画します。

        @param[in]    windowCoordinateX   ウィンドウ座標系の長方形の左上の頂点のX座標 (f32の範囲)
        @param[in]    windowCoordinateY   ウィンドウ座標系の長方形の左上の頂点のY座標 (f32の範囲)
        @param[in]    width             ウィンドウ座標系の長方形のX方向の辺の長さ (f32の範囲)
        @param[in]    height            ウィンドウ座標系の長方形のY方向の辺の長さ (f32の範囲)        
        */     
        virtual void FillRectangle(const f32 windowCoordinateX,
            const f32 windowCoordinateY,
            const f32 width, const f32 height);
    
    public:
        /*!
        @brief    ウィンドウ座標で指定した三角形を2D描画します。
        
        頂点のデータは反時計周りに与えて下さい。

        @param[in]    windowCoordinateX0   ウィンドウ座標系の三角形の1つ目の頂点のX座標 (f32の範囲)
        @param[in]    windowCoordinateY0   ウィンドウ座標系の三角形の1つ目の頂点のY座標 (f32の範囲)
        @param[in]    windowCoordinateX1   ウィンドウ座標系の三角形の2つ目の頂点のX座標 (f32の範囲)
        @param[in]    windowCoordinateY1   ウィンドウ座標系の三角形の2つ目の頂点のY座標 (f32の範囲)
        @param[in]    windowCoordinateX2   ウィンドウ座標系の三角形の3つ目の頂点のX座標 (f32の範囲)
        @param[in]    windowCoordinateY2   ウィンドウ座標系の三角形の3つ目の頂点のY座標 (f32の範囲)
        */     
        virtual void FillTriangle(const f32 windowCoordinateX0, const f32 windowCoordinateY0,
            const f32 windowCoordinateX1, const f32 windowCoordinateY1,
            const f32 windowCoordinateX2, const f32 windowCoordinateY2);

    public:
        /*!
        @brief    フォントを2D描画するときのサイズを設定します。
        
        但し、元のフォントデータのサイズは8x8ピクセルなので、フォントサイズは8を推奨します。

        @param[in]     fontSize   フォントのピクセル単位のサイズ ([1.0f,])
        */
        virtual void SetFontSize(const f32 fontSize);

        /*!
        @brief    ウィンドウ座標で指定した位置にテキストを描画します。
        
        内部では、TRIANGLE_STRIPを使って長方形を作り、
        それをラスタライズしたときに発生するピクセルとフォントテクスチャの乗算を
        ピクセル単位で行って、文字列を描画します。

        @param[in]    windowCoordinateX   テキストを描画するウィンドウ座標系の左上の長方形のX座標 (f32の範囲)
        @param[in]    windowCoordinateY   テキストを描画するウィンドウ座標系の左上の長方形のY座標 (f32の範囲)
        @param[in]    format   フォーマット文字列
        */    
        virtual void DrawText(const f32 windowCoordinateX, const f32 windowCoordinateY, const char* format, ...);


    public:
        /*!
        @brief    テクスチャオブジェクトのハンドルが有効であるかを調べます。
        　　　　　                        
        @param[in]     textureId   テクスチャオブジェクトのハンドル
        @return        テクスチャオブジェクトのハンドルが有効であるときtrue、そうでないときfalseを返します。
        */
        virtual bool HasTexture(const GLuint textureId);
        
        /*!
        @brief    テクスチャオブジェクトを作成します。

                    このメソッドの引数で指定したものは、そのままglTexImage2D()に引き渡されます。
        
        @param[in]     target           テクスチャの種類と確保場所
        @param[in]     internalFormat   テクスチャを保存するときの内部フォーマット
        @param[in]     width            画像のピクセル単位の幅
        @param[in]     height           画像のピクセル単位の高さ
        @param[in]     format           渡すテクスチャデータのフォーマット
        @param[in]     type             ピクセルデータの種類
        @param[in]     pixels           画像データへのポインタ
        @param[out]    textureId        テクスチャオブジェクトのハンドル
        @return        テクスチャオブジェクトの作成に成功したときはtrue、そうでないときfalseを返します。
        */
        virtual bool GenerateTexture(const GLenum target,
            const GLenum internalFormat,
            const GLsizei width, const GLsizei height,
            const GLenum format, const GLenum type, void* pixels,
            GLuint& textureId);

        /*!
        @brief    テクスチャオブジェクトのハンドルによって指定したテクスチャオブジェクトを削除します。
        　　　　　                        
        @param[in]     textureId   テクスチャオブジェクトのハンドル
        @return        テクスチャオブジェクトの削除に成功したときtrue、そうでないときfalseを返します。
        */
        virtual bool DeleteTexture(const GLuint textureId);

    protected:
        virtual bool FindTextureArrayIndex(const GLuint textureId,
            u32& arrayIndex);

    public:
        /*!
        @brief    テクスチャオブジェクトのハンドルで指定したテクスチャオブジェクトを使って、
                  ウィンドウ座標系で指定した長方形に対してテクスチャマッピングを行って2D描画します。
        　　　　　                        
        @param[in]    textureId         テクスチャオブジェクトのハンドル        
        @param[in]    windowCoordinateX   ウィンドウ座標系の長方形の左上の頂点のX座標 (f32の範囲)
        @param[in]    windowCoordinateY   ウィンドウ座標系の長方形の左上の頂点のY座標 (f32の範囲)
        @param[in]    rectangleWidth    ウィンドウ座標系の長方形のX方向の辺の長さ (f32の範囲)
        @param[in]    rectangleHeight   ウィンドウ座標系の長方形のY方向の辺の長さ (f32の範囲)
        @param[in]    imageWidth        画像の幅 (f32の範囲)
        @param[in]    imageHeight       画像の高さ (f32の範囲)
        @param[in]    textureWidth      テクスチャの幅 (f32の範囲)
        @param[in]    textureHeight     テクスチャの高さ (f32の範囲)
        @return       テクスチャオブジェクトの削除に成功したときtrue、そうでないときfalseを返します。
        */
        virtual void FillTexturedRectangle(const GLuint textureId,
            const f32 windowCoordinateX, const f32 windowCoordinateY,
            const f32 rectangleWidth, const f32 rectangleHeight,
            const f32 imageWidth, const f32 imageHeight,
            const f32 textureWidth, const f32 textureHeight);

        /*!
        @brief    テクスチャオブジェクトのハンドルで指定したテクスチャオブジェクトを使って、  
                  ウィンドウ座標で指定した三角形を2D描画します。
                  
                  頂点のデータは反時計周りに与えて下さい。

        @param[in]    textureId          テクスチャオブジェクトのハンドル        
        @param[in]    windowCoordinateX0   ウィンドウ座標系の三角形の1つ目の頂点のX座標 (f32の範囲)
        @param[in]    windowCoordinateY0   ウィンドウ座標系の三角形の1つ目の頂点のY座標 (f32の範囲)
        @param[in]    texcoordS0         三角形の1つ目の頂点のS座標 (f32の範囲)
        @param[in]    texcoordT0         三角形の1つ目の頂点のT座標 (f32の範囲)
        @param[in]    windowCoordinateX1   ウィンドウ座標系の三角形の2つ目の頂点のX座標 (f32の範囲)
        @param[in]    windowCoordinateY1   ウィンドウ座標系の三角形の2つ目の頂点のY座標 (f32の範囲)
        @param[in]    texcoordS1         三角形の2つ目の頂点のS座標 (f32の範囲)
        @param[in]    texcoordT1         三角形の2つ目の頂点のT座標 (f32の範囲)
        @param[in]    windowCoordinateX2   ウィンドウ座標系の三角形の3つ目の頂点のX座標 (f32の範囲)
        @param[in]    windowCoordinateY2   ウィンドウ座標系の三角形の3つ目の頂点のY座標 (f32の範囲)
        @param[in]    texcoordS2         三角形の3つ目の頂点のS座標 (f32の範囲)        
        @param[in]    texcoordT2         三角形の3つ目の頂点のT座標 (f32の範囲)
        */
        virtual void FillTexturedTriangle(const GLuint textureId,
            const f32 windowCoordinateX0, const f32 windowCoordinateY0,
            const f32 texcoordS0, const f32 texcoordT0,
            const f32 windowCoordinateX1, const f32 windowCoordinateY1,
            const f32 texcoordS1, const f32 texcoordT1,
            const f32 windowCoordinateX2, const f32 windowCoordinateY2,
            const f32 texcoordS2, const f32 texcoordT2);

        /*!
        @brief    指定したターゲットへ描画結果を表示します。        
        */        

    public:        
        virtual void SwapBuffers(void);

    protected:
        virtual void Flush(void);    
        virtual void FlushTexturedPrimitives(void);

    protected:
        GraphicsDrawing m_GraphicsDrawing;

    protected:
        bool m_InitializeTexturedTriangles;
        bool m_UseDecalTextureProgram;
        bool m_Padding[2];

        u32 m_MaxTexturedTrianglesNum;
        GLuint m_TextureIdArray[demo::MAX_TEXTURES_NUM];

        u32 m_TotalTexturedTrianglesNumArray[demo::MAX_TEXTURES_NUM];
        demo::TrianglesRenderData m_TexturedTrianglesRenderDataArray[demo::MAX_TEXTURES_NUM];

        f32 m_ClearColorArray[3][4];
    };

}

#endif
