/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_ProgramObject.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_PROGRAM_OBJECT_H_
#define DEMO_PROGRAM_OBJECT_H_

#include <nn/gx.h>
#include <nn/math.h>

#include "demo/Utility/demo_Utility.h"
#include "demo/RenderData/demo_TrianglesRenderData.h"

namespace demo
{ 
    
    /*!
    :private

    @brief    ProgramObjectクラスが保持できる最大シェーダ数    
    */
    const u32 PROGRAM_OBJECT_MAX_SHADERS = 1;
     
    /*!
    :private

    @brief シェーダプログラムの基底クラスです。    
    */

    class ProgramObject : private nn::util::NonCopyable<ProgramObject>
    {
    public:
        /*!
        :private

        @brief    コンストラクタです。        
        */
        ProgramObject(void);

        /*!
        :private

        @brief    デストラクタです。        
        */
        virtual ~ProgramObject(void);

    public:        
        /*!
        :private

        @brief    シェーダプログラムオブジェクトの初期処理を行います。        
        */        
        virtual bool Initialize(const GLuint shaderId) = 0;

        /*!
        :private

        @brief    シェーダプログラムオブジェクトの終了処理を行います。        
        */
        virtual bool Finalize(void) = 0;

    public:
        /*!
        :private

        @brief    シェーダプログラムオブジェクト使用前のレンダリングステートの設定を行います。        
        */
        virtual bool Begin(void);

        /*!
        :private

        @brief    シェーダプログラムオブジェクトを使用します。        
        */
        virtual bool Use(void) = 0;

        /*!
        :private

        @brief    シェーダプログラムオブジェクト使用後のレンダリングステートの設定を行います。        
        */
        virtual bool End(void);

    public:
        /*!
        :private

        @brief    シェーダプログラムオブジェクトに設定された頂点属性を取得します。
        
        @return   頂点属性
        */
        virtual u32 GetVertexAttributes(void);
        
        /*!
        :private

        @brief    3次元の頂点の位置座標を、射影行列とモデルビュー行列によってクリップ座標に変換するかどうかを設定します。
        
        @param[in]    use3d    クリップ座標系に変換するかどうかについて(trueかfalse)
        */        
        virtual void SetUse3d(const bool use3d);

        /*!
        :private

        @brief    シェーダプログラムオブジェクトの射影行列を設定します。
                
        @param[in]    projectionMatrix    射影行列
        */
        virtual void SetProjectionMatrix(const nn::math::MTX44& projectionMatrix);
        
        /*!
        :private

        @brief    シェーダプログラムオブジェクトのモデルビュー行列を設定します。
        
        @param[in]    modelViewMatrix    モデルビュー行列
        */        
        virtual void SetModelViewMatrix(const nn::math::MTX44& modelViewMatrix);

        /*!
        :private

        @brief    テクスチャオブジェクトのハンドルを設定します。
        
        @param[in]    textureId    テクスチャオブジェクトのハンドル
        */
        virtual void SetTextureId(const GLuint textureId);

        /*!
        :private

        @brief    シェーダプログラムオブジェクトの内部状態を更新します。        
        */
        virtual void Update(void);

    protected:
        virtual void InitializeVertexAttributes(void);
        virtual void InitializeUniforms(void);
        virtual void UpdateModelViewProjectionMatrix(void);
    
    protected:
        bool m_Initialized;
        bool m_Padding0[3];

        u32 m_VertexAttributes;
        bool m_Use3d;
        u8 m_Padding[3];
        
        GLuint m_ProgramId;
        GLuint m_ShaderIds[PROGRAM_OBJECT_MAX_SHADERS];
        GLint m_UniformLocation[MAX_UNIFORM_LOCATIONS_NUM];

        nn::math::MTX44 m_ProjectionMatrix;
        nn::math::MTX44 m_ModelViewMatrix;
    };

}

#endif
