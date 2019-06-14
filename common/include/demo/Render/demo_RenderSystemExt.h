/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_RenderSystemExt.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_RENDER_SYSTEM_EXT_H_
#define DEMO_RENDER_SYSTEM_EXT_H_

#include "demo/Render/demo_RenderSystemDrawing.h"

namespace demo
{              
    class RenderSystemExt : public RenderSystemDrawing
    {
    public:
        RenderSystemExt(void);
        virtual ~RenderSystemExt(void);
        
    public:
        virtual void Initialize(const uptr fcramAddress, const size_t memorySize,
            const u32 commandBufferSize = 0x40000, const u32 requestNum = 128,
            const bool serialRunMode = true,
            const DisplayBuffersDescription& displayBuffers0Desc = DisplayBuffersDescription::GetDefaultDisplay0Description(),
            const DisplayBuffersDescription& displayBuffers1Desc = DisplayBuffersDescription::GetDefaultDisplay1Description(),            
            const FrameBufferDescription& frameBuffer0Desc = FrameBufferDescription::GetDefaultDisplay0FrameBufferDescription(),
            const DisplayBuffersDescription& displayBuffers0ExtDesc = DisplayBuffersDescription::GetDefaultDisplay0ExtDescription(),
            const bool isFillBlackLCD = true );
        virtual void Finalize(void);
    protected:
        virtual void InitializeLcdDisplay(void);

    public:
        void SetLcdMode(const GLenum& lcdMode);
        GLenum GetLcdMode(void) const;
        void PrintLcdMode(void) const;

    public:
        virtual void SetClearColor(const s32 display, const f32 red, const f32 green, const f32 blue, const f32 alpha);
        virtual void Clear(const GLbitfield mask = (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) );

    public:
        virtual void SetRenderTarget(const GLenum targetDisplay);        

    public:
        virtual GLuint GetFramebufferObjectId(const GLenum targetDisplay);        
        virtual GLuint GetRenderbufferObjectId(const GLenum targetDisplay, const GLenum renderBufferType);

    protected:
        DisplayBuffersDescription GetDisplay0ExtDescription(void);
        
    public:
        virtual void Transfer(void);
        virtual void SwapBuffers(void);
        virtual void WaitVsync(const GLenum targetDisplay);

    protected:
        virtual void CheckRenderTarget(void);
        
    protected:    
        GLenum m_LcdMode;
        DisplayBuffers m_DisplayBuffers0Ext;
        
        bool m_ChangeLcdMode;
        bool m_Padding[3];
    };      

}

#endif
