/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_Body.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_BODY_H_
#define DEMO_BODY_H_

#include "demo/Utility/demo_Utility.h"

#include "demo/RenderData/demo_TrianglesRenderData.h"

namespace demo
{    
    /*!
        :private

        @brief Bodyクラスです。
    */

    class Body : public demo::TrianglesRenderData
    {        
    public:
        /*!
        @brief    コンストラクタです。
        */
        Body(void);
        
        /*!
        @brief    デストラクタです。
        */
        virtual ~Body(void);

    public:
        virtual void SetColor(const GLfloat red, const GLfloat green, const GLfloat blue);
        virtual void SetColor(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha);
    };

}

#endif
