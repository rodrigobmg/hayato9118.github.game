/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_FontProgram.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_FONT_PROGRAM_OBJECT_H_
#define DEMO_FONT_PROGRAM_OBJECT_H_

#include "demo/ShaderProgram/demo_ProgramObject.h"

namespace demo
{ 
  
    /*!
        :private

        @brief フォントを描画するためのシェーダプログラムのクラスです。    
    */

    class FontProgram : public ProgramObject
    {
    public:
        /*!
        :private

        @brief    コンストラクタです。        
        */
        FontProgram(void);

        /*!
        :private

        @brief    デストラクタです。       
        */
        virtual ~FontProgram(void);

    public:
        /*!
        :private

        @brief    シェーダプログラムオブジェクトの初期処理を行います。        
        */
        virtual bool Initialize(const GLuint shaderId);

        /*!
        :private

        @brief    シェーダプログラムオブジェクトを使用します。       
        */
        virtual bool Finalize(void);

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
        virtual bool Use(void);

        /*!
        :private

        @brief    シェーダプログラムオブジェクト使用後のレンダリングステートの設定を行います。        
        */
        virtual bool End(void);
    
    protected:
        virtual void InitializeVertexAttributes(void);
        void InitializeFontTexture(void);

    protected:
        GLuint m_FontTextureId;
    };

}

#endif
