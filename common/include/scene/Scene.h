/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     Scene.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef NN_COMMON_SCENE_SCENE_H_
#define NN_COMMON_SCENE_SCENE_H_

#include <nn.h>

// シーンスタックからポップする
#define SCENE_ID_POP (-3)
// 定義されていないシーン
#define SCENE_ID_UNDEFINED (-2)
// 空シーン
#define SCENE_ID_NULL (-1)

namespace scene
{

/*!
    @brief シーンのインターフェイスクラス。

           本クラスを継承し、各仮想関数を実装して下さい。@n
           @ref Update() 内で @ref SetNextSceneId() を呼ぶことで、シーンの遷移が行われます。
*/
class Scene
{
public:
    /*!
        @brief コンストラクタです。
    */
    Scene() :
        m_nextSceneId(SCENE_ID_UNDEFINED),
        m_rejectHome(false)
    {}

    /*!
        @brief デストラクタです。
    */
    virtual ~Scene() {};

    /*!
        @brief シーンの初期化処理を行います。

        @return 初期化処理に成功したら true。
    */
    virtual bool Initialize() = 0;

    /*!
        @brief シーンの終了処理を行います。
    */
    virtual void Finalize() = 0;

    /*!
        @brief シーンの更新処理を行います。
    */
    virtual void Update() = 0;

    /*!
        @brief シーンの描画処理を行います。
    */
    virtual void Draw() = 0;

    /*!
        @brief 次のシーンのIDを設定します。

               アプリケーションを終了する場合は、 SCENE_ID_NULL を指定して下さい。

        @param[in]  nextSceneId  次のシーンのID。
        @param[in]  willSave     現在のシーンIDをシーンスタックに保存するか
    */
    void SetNextSceneId(s32 nextSceneId, bool willSave=false) {m_nextSceneId = nextSceneId; m_willSave=willSave;}

    /*!
        @brief 次のシーンのIDを取得います。

        @return 次のシーンのID。
    */
    s32 GetNextSceneId() const {return m_nextSceneId;}

    bool WillSaveSceneIdToStack() const { return m_willSave; }
    bool IsRejectHome() const {return m_rejectHome; }

protected:
    // 次のシーンのID
    s32 m_nextSceneId;

    bool m_willSave;
    bool m_rejectHome;
    NN_PADDING2;
};

} // namespace scene

#endif // NN_COMMON_SCENE_SCENE_H_
