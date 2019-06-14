/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_FrameBuffer.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_FRAME_BUFFER_H_
#define DEMO_FRAME_BUFFER_H_

#include "demo/Utility/demo_Utility.h"

namespace demo
{
        
    const u32 DEMO_RENDER_BUFFER_NUM = 2;
    static const u32 DEMO_COLOR_BUFFER_INDEX = 0;
    static const u32 DEMO_DEPTH_STENCIL_BUFFER_INDEX = 1;

    /*!
        @brief 作成するレンダーバッファのデータを保持するクラス
    */

    class RenderBufferDescription
    {
    public:
        RenderBufferDescription(void);
        virtual ~RenderBufferDescription(void);

    public:
        GLenum m_Format;
        u32 m_Width;
        u32 m_Height;
        GLenum m_Area;
        GLenum m_Attachment;

    public:
        static RenderBufferDescription GetDefaultDisplay0ColorDescription(void);
        static RenderBufferDescription GetDefaultDisplay0DepthStencilDescription(void);
        static RenderBufferDescription GetDefaultDisplay1ColorDescription(void);
        static RenderBufferDescription GetDefaultDisplay1DepthStencilDescription(void);
    };



    /*!
        @brief 作成するフレームバッファのデータを保持するクラス
    */

    class FrameBufferDescription
    {
    public:        
        FrameBufferDescription(void);
        virtual ~FrameBufferDescription(void);
        FrameBufferDescription(const FrameBufferDescription& rhs);        
        FrameBufferDescription& operator=(const FrameBufferDescription& rhs);
    
    public:
        void Initialize(void);
        void Finalize(void);

    public:
        RenderBufferDescription m_RenderBufferDescriptionArray[DEMO_RENDER_BUFFER_NUM];

    public:
        static FrameBufferDescription GetDefaultDisplay0FrameBufferDescription(void);
        static FrameBufferDescription GetDefaultDisplay1FrameBufferDescription(void);
    };    
   
    /*!
        @brief フレームバッファのクラス
    */

    class FrameBuffer : private nn::util::NonCopyable<FrameBuffer>
    {
    public:
        FrameBuffer(void);
        virtual ~FrameBuffer(void);

    protected:        
        void Initialize(const FrameBufferDescription& frameBufferDesc);
        void Finalize(void);
   
    public:        
        static void Create(const FrameBufferDescription& frameBufferDesc,
            FrameBuffer& frameBuffer);
        static void Destroy(FrameBuffer& frameBuffer);

    public:
        void Bind(void);
        void SetViewport(void);
        void SetViewport(const GLint x, const GLint y, const GLsizei width, const GLsizei height);
        GLsizei GetWidth(void);
        GLsizei GetHeight(void);    
    public:
        void ClearBuffer(const GLbitfield mask = (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) );
        void ClearColorDepthStencilBuffer(const GLclampf red, const GLclampf green,
            const GLclampf blue, const GLclampf alpha,
            const GLclampf depth = 1.0f, const GLclampf stencil = 0.0f);
        void ClearDepthStencilBuffer(const GLclampf depth = 1.0f, const GLclampf stencil = 0.0f);
        void ClearColorDepthBuffer(const GLclampf red, const GLclampf green,
            const GLclampf blue, const GLclampf alpha,
            const GLclampf depth = 1.0f);
        void ClearColorBuffer(const GLclampf red, const GLclampf green,
            const GLclampf blue, const GLclampf alpha);        
        void ClearDepthBuffer(const GLclampf depth = 1.0f);
        void ClearStencilBuffer(const GLclampf stencil = 0.0f);

    public:
        void Transfer(const GLuint displayBufferId, const GLenum mode,
            const GLboolean yflip, const GLint colorX, const GLint colorY);

    public:
        GLuint GetFrameBufferId(void);
        GLuint GetRenderBufferId(const u32 bufferIndex);

    protected:
        bool m_InitializeFlag;
        bool m_Padding[3];

        GLuint m_FrameBufferId;
        u32 m_RenderBufferNum;
        GLuint m_RenderBufferIdArray[DEMO_RENDER_BUFFER_NUM];
        u32 m_Width;
        u32 m_Height;
    };
   
}

#endif
