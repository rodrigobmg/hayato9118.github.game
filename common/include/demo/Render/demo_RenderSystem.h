/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_RenderSystem.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_RENDER_SYSTEM_H_
#define DEMO_RENDER_SYSTEM_H_

#include <nn/gx.h>

#include "demo/Utility/demo_Utility.h"
#include "demo/Utility/demo_MemoryManager.h"

#include "demo/Render/demo_DisplayBuffers.h"
#include "demo/Render/demo_FrameBuffer.h"
#include "demo/Render/demo_CommandList.h"

#include "demo/ShaderProgram/demo_ShaderManager.h"

namespace demo
{
    /*!
        @brief CTRで、ディスプレイバッファやフレームバッファオブジェクトを
               利用するためのクラス。               
    */

    class RenderSystem : private nn::util::NonCopyable<RenderSystem>
    {
    public:
        RenderSystem(void);
        virtual ~RenderSystem(void);

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
        virtual void SetRenderTarget(const GLenum targetDisplay);
        virtual void SetViewport(const GLint x, const GLint y, const GLsizei width, const GLsizei height);

        virtual void Clear(const GLbitfield mask = (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) );
        virtual void ClearColorDepthStencilBuffer(const GLclampf red, const GLclampf green,
            const GLclampf blue, const GLclampf alpha,
            const GLclampf depth = 1.0f, const GLclampf stencil = 0.0f);
        virtual void ClearColorDepthBuffer(const GLclampf red, const GLclampf green,
            const GLclampf blue, const GLclampf alpha,
            const GLclampf depth = 1.0f);
        virtual void ClearColorBuffer(const GLclampf red, const GLclampf green,
            const GLclampf blue, const GLclampf alpha);        
        virtual void ClearDepthStencilBuffer(const GLclampf depth = 1.0f, const GLclampf stencil = 0.0f);

    public:
        virtual void GetDisplayBufferObjectId(const GLenum targetDisplay,
            GLuint& DisplayBufferId0, GLuint& DisplayBufferId1);
        virtual GLuint GetFrameBufferObjectId(const GLenum targetDisplay);        
        virtual GLuint GetRenderBufferObjectId(const GLenum targetDisplay, const GLenum renderBufferType);

    public:
        virtual void SwapBuffers(void);
        virtual void WaitVsync(const GLenum targetDisplay);
        virtual void SetSerialRun(const bool flag);
        virtual void CheckRenderTarget(void);

    protected:
        bool m_InitializeFlag;
        bool m_InitializeLcdDisplayFlag;
        bool m_Padding[2];
                
        GLenum m_TargetDisplay;

        DisplayBuffers* m_CurrentDisplayBuffersPtr;
        FrameBuffer* m_CurrentFrameBufferPtr;

        DisplayBuffers m_DisplayBuffers0;
        FrameBuffer m_FrameBuffer0;

        DisplayBuffers m_DisplayBuffers1;
        
        CommandList m_CommandList;
    };      

}

#endif
