/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_GraphicsDrawing.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_GRAPHICS_DRAWING_H_
#define DEMO_GRAPHICS_DRAWING_H_

#include <nn/gx.h>
#include <nn/util/util_NonCopyable.h>

#include "demo/RenderData/demo_TextsRenderData.h"
#include "demo/RenderData/demo_TrianglesRenderData.h"

#include "demo/ShaderProgram/demo_ShaderManager.h"

namespace demo
{
    /*!
        @brief CTR用の2D描画を行うクラス
    */
  
    class GraphicsDrawing : private nn::util::NonCopyable<GraphicsDrawing>
    {
    public:
        GraphicsDrawing(void);
        virtual ~GraphicsDrawing(void);

    public:
        virtual void Initialize(void);            
        virtual void Finalize(void);  

    public:
        /*!
        @brief    描画する2Dの三角形の初期処理をします。

                  この関数はDrawTriangle2d()内で暗黙的に呼ばれるため、明示的に呼ぶ必要はないです。                  
                  上下のLCDに描画する2Dの三角形の最大数を変更したいときだけ,                  
                  Initialize()の直後に呼んで下さい。
                          
        @param[in]   maxTrianglesNum      上下のLCDそれぞれに描画する三角形の最大数(デフォルトではMAX_TRIANGLES_NUM)
        */
        virtual void InitializeTriangles(const u32 maxTrianglesNum = demo::MAX_TRIANGLES_NUM);

        /*!
        @brief    描画する2Dの四角形の初期処理をします。

                  この関数はDrawSquare2d()内で暗黙的に呼ばれるため、明示的に呼ぶ必要はないです。                  
                  上下のLCDに描画する2Dの四角形(点、直線を含む)の最大数を変更したいときだけ,                  
                  Initialize()の直後に呼んで下さい。
                          
        @param[in]   maxSquaresNum      上下のLCDそれぞれに描画する四角形の最大数(デフォルトではMAX_SQUARES_NUM)
        */
        virtual void InitializeSquares(const u32 maxSquaresNum = demo::MAX_SQUARES_NUM);

        /*!
        @brief    描画する2Dのテキストの初期処理をします。

                  この関数はDrawText2d()内で暗黙的に呼ばれるため、明示的に呼ぶ必要はないです。                  
                  上下のLCDに描画する2Dのテキストの最大文字数を変更したいときだけ,                  
                  Initialize()の直後に呼んで下さい。
                          
        @param[in]   maxLength      上下のLCDそれぞれに描画する最大文字数(デフォルトではMAX_TEXT_LENGTH)
        */
        virtual void InitializeTexts(const u32 maxLength = demo::MAX_TEXT_LENGTH);
              
    public:
        /*!
        @brief    2D描画するウィンドウのサイズを設定します。
                        
        @param[in]    windowWidth    ウィンドウの幅
        @param[in]    windowWidth    ウィンドウの高さ
        */
        virtual void SetWindowSize(const f32 windowWidth, const f32 windowHeight);

        /*!
        @brief    2D描画するウィンドウのサイズを取得します。
                        
        @param[out]    windowWidth    ウィンドウの幅
        @param[out]    windowWidth    ウィンドウの高さ
        */
        virtual void GetWindowSize(f32& windowWidth, f32& windowHeight);

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

        /*!
        @brief    プリミティブを2D描画するときの深度値を減らします。                        
        */
        virtual void DecrementDepth(void);

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

        /*!
        @brief    ウィンドウ座標で指定した位置にテキストを描画します。
        
        内部では、TRIANGLE_STRIPを使って長方形を作り、
        それをラスタライズしたときに発生するピクセルとフォントテクスチャの乗算を
        ピクセル単位で行って、文字列を描画します。

        @param[in]    windowCoordinateX   テキストを描画するウィンドウ座標系の左上の長方形のX座標 (f32の範囲)
        @param[in]    windowCoordinateY   テキストを描画するウィンドウ座標系の左上の長方形のY座標 (f32の範囲)
        @param[in]    text   文字列
        */    
        virtual void DrawTextBuffer(const f32 windowCoordinateX, const f32 windowCoordinateY, const char* textBuffer);

    public:
        /*!
        @brief    指定したターゲットへ描画結果を表示します。        
        */        
        virtual void Flush(void);

    protected:    
        virtual void FlushColorFillPrimitives(void);
        virtual void FlushTexts(void);
                
    public:
        f32 m_Color[4];    
        f32 m_Depth;
        bool m_AutoDepth;
        bool m_Padding0[3];
        f32 m_DepthValue;
            
        f32 m_WindowWidth;
        f32 m_WindowHeight;

    public:
        demo::ShaderManager m_ShaderManager;

    protected:
        bool m_InitializeTriangles;
        bool m_InitializeSquares;
        bool m_InitializeTexts;
        bool m_Padding1;
        
    protected:       
        u32 m_MaxTrianglesNum;
        u32 m_TotalTrianglesNum;
    protected:
        u32 m_MaxSquaresNum;        
        u32 m_TotalSquaresNum;
    protected:
        u32 m_MaxTextLength;
        u32 m_TotalTextLength;
  
    protected:
        bool m_UseColorFillProgram;
        bool m_UseFontProgram;
        bool m_Padding2[2];

    protected:
        f32 m_PointSize;
        f32 m_LineWidth;
        
        demo::TrianglesRenderData m_TrianglesRenderData;
        demo::TrianglesRenderData m_SquaresRenderData;
        
        f32 m_FontSize;
        demo::TextsRenderData m_TextsRenderData;
    };      

}

#endif
