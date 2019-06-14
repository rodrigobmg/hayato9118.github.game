/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     TenKey.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_COMMON_SCENE_TENKEY_H_
#define NN_COMMON_SCENE_TENKEY_H_

#include <nn.h>
#include <nn/util/util_SizedEnum.h>
#include "GuiControlManager.h"

namespace scene
{

class TenKey
{
public:
    enum State
    {
        STATE_NORMAL,    // 通常
        STATE_OK,        // OK
        STATE_CANCELLED, // キャンセル

        STATE_MAX
    };

public:
    /*!
        @brief コンストラクタです。
    */
    TenKey();

    /*!
        @brief デストラクタです。
    */
    virtual ~TenKey();

    /*!
        @brief 初期化処理を行います。

        @param[in]  length          文字数。
        @param[in]  split           区切り位置。@n
                                    指定した値の間隔で、ハイフンを間にはさみます。
        @param[in]  pInitial        初期値。@n
                                    長さが length 以上の場合、はみ出した部分は捨てられます。
        @param[in]  numLowestChars  最低文字数。@n
                                    入力された文字数が numLowestChars 以上でないと OK ボタンが押せなくなります。
        @param[in]  isPassword      パスワードかどうか。@n
                                    true の場合、入力文字が * で表示されます。
    */
    void Initialize(u32 length, u32 split = 0, const wchar_t* pInitial = NULL, u32 numLowestChars = 1, bool isPassword = false);

    /*!
        @brief 終了処理を行います。
    */
    void Finalize();

    /*!
        @brief 更新処理を行います。
    */
    void Update();

    /*!
        @brief 描画処理を行います。
    */
    void Draw();

    /*!
        @brief 状態をリセットします。

        @param[in]  pInitial  初期値。@n
                              長さが入力文字のサイズ以上の場合、はみ出した部分は捨てられます。
    */
    void Reset(const wchar_t* pInitial = NULL);

    /*!
        @brief 状態を設定します。

        @param[in]  state  状態。
    */
    void SetState(State state) {m_state = state;}

    /*!
        @brief OKボタンが押されたかどうかを取得します。

        @return OKボタンが押されていたら true。
    */
    bool IsOK() const {return (m_state == STATE_OK);}

    /*!
        @brief キャンセルボタンが押されたかどうかを取得します。

        @return キャンセルボタンが押されていたら true。
    */
    bool IsCancelled() const {return (m_state == STATE_CANCELLED);}

    /*!
        @brief 入力文字列を取得します。

        @return 入力文字列。
    */
    const wchar_t* GetInput() const {return m_pInput;}

    /*!
        @brief 入力した数値を取得します。

        @return 入力した数値。
    */
    u32 GetInputInteger() const;

    /*!
        @brief カーソル位置を取得します。

        @return カーソル位置。
    */
    u32 GetCursor() const {return m_cursor;}

protected:
    // イベントコールバック
    static void MyControlEventCallback(scene::ControlEvent event, scene::ControlBase* pControl, s16 x, s16 y, void* pParam);

protected:
    // この位置に、vtable 4byte が存在する (パディング調整の際に留意)

    // 入力文字
    wchar_t* m_pInput;
    // 文字数
    u32 m_length;
    // 区切り位置
    u32 m_split;
    // 最低文字数
    u32 m_numLowestChars;
    // カーソル位置
    u32 m_cursor;
    // 状態
    nn::util::SizedEnum4<State> m_state;
    // パスワードかどうか
    bool m_isPassword;
    NN_PADDING3;
    // GUIマネージャ
    scene::ControlManager m_controlManager;
};

} // namespace scene

#endif // NN_COMMON_SCENE_TENKEY_H_
