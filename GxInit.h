// (GxInit.h)
#ifndef ___GXINIT_H
#define ___GXINIT_H

#include <nn/gd.h>
#include <nn/fnd.h>

// ÉOÉçÅ[ÉoÉãïœêî

//Display buffers
extern u32 s_displaybuffer0[];
extern u32 s_displaybuffer1[];
extern u32 s_CurrentDisplaybuffer0;
extern u32 s_CurrentDisplaybuffer1;

//Texture combiner
extern nn::gd::CombinerState* s_texComState;
extern nn::gd::BlendState* s_bd;

//FrameBuffers
extern nn::gd::Texture2DResource* s_texture2DResource_MainColorBuffer;
extern nn::gd::Texture2DResource* s_texture2DResource_MainDepthStencilBuffer;
extern nn::gd::RenderTarget* s_MainRenderTarget;
extern nn::gd::DepthStencilTarget* s_MainDepthStencilTarget;

/* ExpHeap for app. */
extern nn::fnd::ExpHeap s_AppHeap;
extern uptr s_HeapForGx;
extern const u32 s_GxHeapSize;

//Shader
extern nn::gd::ShaderPipeline* shaderPipeline;
extern nn::gd::Shader* vertexShader;
extern nn::gd::UniformLocation s_shaderVariable_proj;
extern nn::gd::UniformLocation s_shaderVariable_view;

//Vertex input
extern nn::gd::InputLayout* inputLayout;
extern nn::gd::VertexBufferResource* bufferCoord;
extern nn::gd::VertexBufferResource* bufferColor;
extern nn::gd::VertexBufferResource* bufferTexCoord;
extern nn::gd::VertexBufferResource* bufferTex4x4[];
extern nn::gd::VertexBufferResource* bufferIndex;
extern nn::gd::VertexBufferResource* bufferTex256[];

#endif // ___GXINIT_H
