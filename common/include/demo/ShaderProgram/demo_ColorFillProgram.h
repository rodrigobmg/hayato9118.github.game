/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_ColorFillProgram.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_COLOR_FILL_PROGRAM_OBJECT_H_
#define DEMO_COLOR_FILL_PROGRAM_OBJECT_H_

#include "demo/ShaderProgram/demo_ProgramObject.h"

namespace demo
{ 
    /*!
        :private

        @brief 頂点カラーによってポリゴンを描画するシェーダプログラムのクラスです。        
    */

    class ColorFillProgram : public ProgramObject
    {
    public:
        /*!
        :private

        @brief    コンストラクタです。        
        */
        ColorFillProgram(void);
        
        /*!
        :private

        @brief    デストラクタです。        
        */
        virtual ~ColorFillProgram(void);

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

    protected:
        virtual void InitializeVertexAttributes(void);
    };

}

#endif // NN_DEMO_CTR_DEMO_COLOR_FILL_PROGRAM_OBJECT_H_
