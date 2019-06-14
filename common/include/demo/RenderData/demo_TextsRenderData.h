/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_TextsRenderData.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_TEXTS_RENDER_DATA_H_
#define DEMO_TEXTS_RENDER_DATA_H_

#include "demo/RenderData/demo_TrianglesRenderData.h"

namespace demo
{ 
    
    /*!
    :private
    
    フォントテクスチャのS方向のオフセット値    
    */
    const f32 FONT_OFFSET_S = 0.125f;    

    /*!
    :private
    
    フォントテクスチャのT方向のオフセット値
    */
    const f32 FONT_OFFSET_T = 0.0625f;
    
    /*! 
    :private

    フォントテクスチャの基本サイズ
    */
    const u32 FONT_BASE_SIZE = 8;

    /*! 
    :private

    1行に出力可能な最大文字数
    */
    const u32 MAX_CHAR_LENGTH_PER_LINE = 256;

    /*! 
    :private

    出力可能な最大文字数
    */
    const u32 MAX_TEXT_LENGTH = MAX_TRIANGLES_NUM / 2;

    /*!
        :private

        @brief テキストを描画するための描画用データのクラスです。        
    */

    class TextsRenderData : public demo::TrianglesRenderData
    {
    public:
        /*!
        :private

        @brief    コンストラクタです。        
        */
        TextsRenderData(void);

        /*!
        :private

        @brief    デストラクタです。        
        */
        virtual ~TextsRenderData(void);
    
   public:
        /*!
        :private

        @brief    描画用データの初期処理を行います。        
        */
        virtual void Initialize(void);

        /*!
        :private

        @brief    描画用データの終了処理を行います。        
        */
        virtual void Finalize(void);

    public:
        /*!
        :private

        @brief    頂点バッファの初期化処理を行います。
                
        @param[in]    vertexAttributes    頂点属性の論理和
        @param[in]    triangleType        描画する三角形プリミティブの種類
        */
        void InitializeVertexBuffers(const u32 vertexAttributes, const GLenum triangleType, const u32 maxTextLength);
    protected:
        void SetMaxTextLength(const u32 maxTextLength);
    
    public:
        /*!
        :private

        @brief    描画するテキストを描画用データに追加します。
        
        @param[in]    windowCoordinateX   テキストを描画するウィンドウ座標系の左上の長方形のX座標 (f32の範囲)
        @param[in]    windowCoordinateY   テキストを描画するウィンドウ座標系の左上の長方形のY座標 (f32の範囲)
        @param[in]    text                描画するテキストの文字列
        @param[in]    textLength          描画するテキストの文字列の長さ
        @param[in]    normalizedDeviceCoordinateZ     テキストの正規化デバイス座標のZ座標
        */
        void AddText(const f32 windowCoordinateX, const f32 windowCoordinateY,
            const char* text, const u32 textLength, const f32 normalizedDeviceCoordinateZ);

    public:
        /*!
        :private

        @brief    パックした描画用のテキストデータをクリアします。        
        */
        void ClearPackedTextLength(void);
    protected:
        void SetPackedTextLength(const u32 packedTextLength);
    public:
        /*!
        :private

        @brief    パックした描画用のテキストデータの長さを取得します。        
        */
        u32 GetPackedTextLength(void) const;
    
    public:
        /*!
        :private

        @brief    描画するフォントのサイズを設定します。
       
        @param[in]    fontSize     フォントサイズ
        */
        void SetFontSize(const f32 fontSize);

        /*!
        :private

        @brief    描画するフォントのカラーを設定します。
                
        @param[in]    red          フォントのカラーの赤成分
        @param[in]    green        フォントのカラーの緑成分
        @param[in]    blue         フォントのカラーの青成分
        @param[in]    alpha        フォントのカラーのアルファ成分
        */
        void SetFontColor(const f32 red, const f32 green,
            const f32 blue, const f32 alpha);

    protected:
        u32 m_PackedTextLength;
        u32 m_MaxTextLength;

        f32 m_FontSize;
        f32 m_Color[4];

    protected:    
        u32 m_FontBaseSize;
        f32 m_FontOffsetS;
        f32 m_FontOffsetT;        
    };

}

#endif
