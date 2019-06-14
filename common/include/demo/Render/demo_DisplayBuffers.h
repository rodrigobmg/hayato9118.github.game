/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_DisplayBuffers.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_DISPLAY_BUFFERS_H_
#define DEMO_DISPLAY_BUFFERS_H_

#include "demo/Utility/demo_Utility.h"

namespace demo
{
    const u32 DEMO_DISPLAY_BUFFER_NUM = 2;
    
    /*!
        @brief 作成する複数のディスプレイバッファのデータを保持するクラス
    */
    
    class DisplayBuffersDescription
    {
    public:
        DisplayBuffersDescription(void);
        virtual ~DisplayBuffersDescription(void);

    public:
        GLenum m_TargetDisplay;
        u32 m_DisplayBufferNum;
        GLenum m_Format;
        u32 m_Width;
        u32 m_Height;
        GLenum m_Area;

    public:
        static DisplayBuffersDescription GetDefaultDisplay0Description(void);
        static DisplayBuffersDescription GetDefaultDisplay1Description(void);
        static DisplayBuffersDescription GetDefaultDisplay0ExtDescription(void);
    };
            
    
    /*!
        @brief 複数のディスプレイバッファのクラス
    */
    
    class DisplayBuffers :  private nn::util::NonCopyable<DisplayBuffers>
    {
    public:
        DisplayBuffers(void);
        virtual ~DisplayBuffers(void); 

    protected:        
        void Initialize(const DisplayBuffersDescription& displayBufferDescription);
        void Finalize(void);
   
    public:
        static void Create(const DisplayBuffersDescription& displayBuffersDesc,
            DisplayBuffers& displayBuffers);
        static void Destroy(DisplayBuffers& displayBuffers);

    public:
        GLuint GetTargetDisplayBufferId(void) const;
        GLuint GetPreviousTargetDisplayBufferId(void) const;
        GLuint GetDisplayBufferId(const u32 index) const;
        void Bind(void);
        void IncreaseBufferIndex(void);

    public:
        u32 GetWidth(void) const;
        u32 GetHeight(void) const;
    protected:
        bool m_InitializeFlag;
        bool m_Padding[3];

        GLenum m_TargetDisplay;
        u32 m_DisplayBufferNum;
        GLuint m_DisplayBufferIdArray[DEMO_DISPLAY_BUFFER_NUM];
        u32 m_CurrentBufferIndex;

        u32 m_Width;
        u32 m_Height;
    };
     
}

#endif
