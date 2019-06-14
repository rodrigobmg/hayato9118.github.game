/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_Cube.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_CUBE_H_
#define DEMO_CUBE_H_

#include "demo/Body/demo_Body.h"

namespace demo
{
    /*!
    :private
   
    @brief 直方体のクラスです。
    */

    class Cube : public Body
    {        
    public:
        /*!
        @brief    コンストラクタです。
        */
        Cube(void);
        
        /*!
        @brief    デストラクタです。
        */
        virtual ~Cube(void);

    public:
        virtual void Initialize(void);
        void InitializeCube(const u32 vertexAttributes,
            const f32 halfEdgeX, const f32 halfEdgeY, const f32 halfEdgeZ);
       
    protected:
        f32 m_HalfEdges[3];
    };

}

#endif
