/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_DecalTextureProgram.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_DECAL_TEXTURE_PROGRAM_OBJECT_H_
#define DEMO_DECAL_TEXTURE_PROGRAM_OBJECT_H_

#include "demo/ShaderProgram/demo_ProgramObject.h"

namespace demo
{ 

    /*!
    :private
    
    @brief ポリゴンに対してテクスチャデカールを行うためのシェーダプログラムのクラスです。       
    */

    class DecalTextureProgram : public ProgramObject
    {
    public:
        /*!
        :private

        @brief    コンストラクタです。        
        */
        DecalTextureProgram(void);
        
        /*!
        :private

        @brief    デストラクタです。        
        */
        virtual ~DecalTextureProgram(void);

    public:
        /*!
        :private

        @brief    シェーダプログラムオブジェクトの初期処理を行います。        
        */
        virtual bool Initialize(const GLuint shaderId);

        /*!
        :private

        @brief    シェーダプログラムオブジェクトの終了処理を行います。        
        */
        virtual bool Finalize(void);
    public:
        /*!
        :private

        @brief    シェーダプログラムオブジェクトを使用します。        
        */
        virtual bool Use(void);
    
    public:
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

    protected:
        GLuint m_TextureId;
    };

}

#endif
