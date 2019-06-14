/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     TextWriter.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_COMMON_SCENE_TEXTWRITER_H_
#define NN_COMMON_SCENE_TEXTWRITER_H_

#include <nn.h>
#include <nn/pl.h>
#include <nn/font.h>

namespace scene
{

/*!
    @brief テキストライター。
*/
class TextWriter
{
public:
    /*!
        @brief コンストラクタです。
    */
    TextWriter();

    /*!
        @brief デストラクタです。
    */
    virtual ~TextWriter();

    /*!
        @brief 初期化処理を行います。

        @param[in]  pExpHeap     ヒープ。@n
                                 内部で使用するバッファを確保するために使用します。
        @param[in]  pShaderPath  シェーダのパス。
    */
    void Initialize(nn::fnd::ExpHeap* pExpHeap);

    /*!
        @brief 終了処理を行います。
    */
    void Finalize();

    /*!
        @brief 描画を開始します。

        @param[in]  targetDisplay  描画先。@n
                                   NN_GX_DISPLAY0 または NN_GX_DISPLAY1 を指定して下さい。
    */
    void BeginRender(GLenum targetDisplay);

    /*!
        @brief 描画を終了します。
    */
    void EndRender();

    /*!
        @brief 文字列を描画します。

        @param[in]  x        X座標の描画位置。
        @param[in]  y        Y座標の描画位置。
        @param[in]  pFormat  フォーマット文字列。
        @param[in]  ...      可変長引数。
    */
    void Printf(f32 x, f32 y, const wchar_t* pFormat, ...);

    /*!
        @brief 文字列を描画します。

        @param[in]  x        X座標の描画位置。
        @param[in]  y        Y座標の描画位置。
        @param[in]  pString  文字列。
    */
    void PutString(f32 x, f32 y, const wchar_t* pString);

    /*!
        @brief 文字色を設定します。

        @param[in]  red    赤成分。
        @param[in]  green  緑成分。
        @param[in]  blue   青成分。
        @param[in]  alpha  α成分。
    */
    void SetTextColor(u8 red, u8 green, u8 blue, u8 alpha = 255);

    /*!
        @brief フォントサイズを取得します。

        @return フォントサイズ。
    */
    f32 GetFontSize();

    /*!
        @brief フォントサイズを設定します。

        @param[in]  size  フォントサイズ。
    */
    void SetFontSize(f32 size);

    /*!
        @brief フォントの拡大率を取得します。

        @return フォントの拡大率。
    */
    f32 GetFontScale();

    /*!
        @brief フォントの拡大率を設定します。

        @param[in]  scale  フォントの拡大率。
    */
    void SetFontScale(f32 scale);

    /*!
        @brief 文字間を取得します。

        @return 文字間。
    */
    f32 GetCharSpace();

    /*!
        @brief 文字間を設定します。

        @param[in]  charSpace  文字間。
    */
    void SetCharSpace(f32 charSpace);

    /*!
        @brief 文字列の幅を計算します。

        @param[in]  pString  文字列。
    */
    f32 CalculateStringWidth(const wchar_t* pString);

    /*!
        @brief 文字列の高さを計算します。

        @param[in]  pString  文字列。
    */
    f32 CalculateStringHeight(const wchar_t* pString);

protected:
    // フォントの初期化処理を行う。
    bool InitializeFont();
    // シェーダの初期化処理を行う。
    bool InitializeShader();
    // 文字列バッファを確保する。
    nn::font::DispStringBuffer* AllocateStringBuffer(s32 charMax);

protected:
    // ヒープ
    nn::fnd::ExpHeap* m_pExpHeap;
    // フォント
    nn::font::ResFont m_font;
    // 描画オブジェクト
    nn::font::RectDrawer     m_drawer;
    nn::font::WideTextWriter m_writer;
    // 文字列バッファ
    nn::font::DispStringBuffer*m_pStringBuffer;
    // 文字色
    nn::util::Color8 m_textColor;
    // シェーダ用バッファ
    void* m_pShaderBuffer;
    // 描画ターゲット
    u32 m_targetDisplay;
};

} // namespace scene

#endif // NN_COMMON_SCENE_TEXTWRITER_H_
