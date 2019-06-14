/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_TrianglesRenderData.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_TRIANGLES_RENDER_DATA_H_
#define DEMO_TRIANGLES_RENDER_DATA_H_

#include <nn/gx.h>
#include <nn/math.h>

#include "demo/Utility/demo_MemoryManager.h"

#include "demo/RenderData/demo_RenderData.h"

namespace demo
{
    /*!
    :private

    @brief    頂点属性を表す列挙型    
    */
    enum VertexAttributes
    {
        VERTEX_NONE_ATTRIBUTE = 0,            //!< 頂点属性が無属性であることを示す
        VERTEX_POSITION_ATTRIBUTE = (1 << 0), //!< 頂点属性の位置座標属性
        VERTEX_COLOR_ATTRIBUTE    = (1 << 1), //!< 頂点属性の頂点カラー属性
        VERTEX_TEXCOORD_ATTRIBUTE = (1 << 2), //!< 頂点属性のテクスチャ座標属性
        VERTEX_NORMAL_ATTRIBUTE   = (1 << 3), //!< 頂点属性の法線属性
        VERTEX_TANGENT_ATTRIBUTE  = (1 << 4)  //!< 頂点属性の接線属性
    };

    /*!
    :private

    @brief 頂点の位置座標の要素数（要素は全てf32 型で、xyzw）       
    */
    const int VERTEX_POSITION_ATTRIBUTE_SIZE = 4;
    
    /*!
    :private

    @brief 頂点カラーの要素数（要素は全てf32 型で、red、green、blue、alpha）        
    */
    const int VERTEX_COLOR_ATTRIBUTE_SIZE  = 4;    
    
    /*!
    :private

    @brief 頂点のテクスチャ座標の要素数（要素は全てf32 型で、s、t、r)       
    */       
    const int VERTEX_TEXCOORD_ATTRIBUTE_SIZE = 3;
    
    /*!
    :private

    @brief 頂点の法線の要素数（要素は全てf32 型で、nx、ny、nz)       
    */       
    const int VERTEX_NORMAL_ATTRIBUTE_SIZE = 3;
    
    /*!
    :private

    @brief 頂点の接線の要素数（要素は全てf32 型で、tx、ty、tz)       
    */  
    const int VERTEX_TANGENT_ATTRIBUTE_SIZE = 3;

    /*!
    :private

    @brief TrianglesRenderDataクラスが描画データとして持つことが可能な三角形の最大数        
    */  
    const u32 MAX_TRIANGLES_NUM = 8192;

    /*!
    :private

    @brief TrianglesRenderDataクラスが描画データとして持つことが可能な四角形の最大数        
    */  
    const u32 MAX_SQUARES_NUM = MAX_TRIANGLES_NUM / 2;

    /*!
    :private

    @brief TrianglesRenderDataクラスが描画データとして持つことが可能な三角形の最大頂点数    
    */
    const u32 MAX_TRIANGLE_VERTICES_NUM = MAX_TRIANGLES_NUM * 3;
    
    /*!
    :private

    @brief TrianglesRenderDataクラスが描画データとして持つことが可能な三角形の最大インデックス数    
    */
    const u32 MAX_INDICES_NUM = MAX_TRIANGLES_NUM * 3;
    
    /*!
    :private

    @brief 頂点バッファのデータタイプ
    */
    const GLenum ARRAY_BUFFER_DATA_TYPE = GL_ARRAY_BUFFER;

    /*!
    :private

    @brief インデックスバッファのデータタイプ
    */
    const GLenum ELEMENT_ARRAY_BUFFER_DATA_TYPE = GL_ELEMENT_ARRAY_BUFFER;

    /*!
    :private

    @brief 描画用データである三角形メッシュのクラスです。
    */
    class TrianglesRenderData : public RenderData
    {
    public:
        /*!
        :private

        @brief    コンストラクタです。        
        */
        TrianglesRenderData(void);

        /*!
        :private

        @brief    デストラクタです。        
        */
        virtual ~TrianglesRenderData(void);

    public:
        /*!
        :private

        @brief    描画用データの初期処理を行います。        
        */
        virtual void Initialize(void);

        /*!
        :private

        @brief    描画用データの終了処理を行います。       
        */
        virtual void Finalize(void);

    public:
        /*!
        :private

        @brief    頂点バッファの初期化処理を行います。
        
        @param[in]    vertexAttributes    頂点属性の論理和
        @param[in]    triangleType        描画する三角形プリミティブの種類
        @param[in]    verticesNum         三角形プリミティブの総頂点数
        @param[in]    trianglesNum        三角形プリミティブの総数
        */
        void InitializeVertexBuffers(const u32 vertexAttributes,
            const GLenum triangleType, const u32 verticesNum, const u32 trianglesNum);

    protected:        
        void SetVertexAttributes(const u32 vertexAttributes);        
        void SetTriangleType(const GLenum triangleType);
        void SetVerticesNum(const u32 verticesNum);
        void SetTrianglesNum(const u32 trianglesNum);
        void CreateArrays(void);
        void DestroyArrays(void);

    public:
        /*!
        :private

        @brief    パックした頂点の数を設定します。
        
        @param[in]    packedVerticesNum    パックした頂点数
        */
        void SetPackedVerticesNum(const u32 packedVerticesNum);
        
        /*!
        :private

        @brief    パックした頂点の数を追加します。
        
        @param[in]    packedVerticesNum    追加するパックした頂点数
        */
        void AddPackedVerticesNum(const u32 packedVerticesNum);

        /*!
        :private

        @brief    パックしている頂点の数を取得します。
        
        @return   パックしている頂点の数
        */
        u32 GetPackedVerticesNum(void) const;
    
    public:
        /*!
        :private

        @brief    パックした三角形の数を設定します。
        
        @param[in]    packedTrianglesNum    パックした三角形の数
        */
        void SetPackedTrianglesNum(const u32 packedTrianglesNum);

        /*!
        :private

        @brief    パックした三角形の数を追加します。
        
        @param[in]    packedTrianglesNum    追加するパックした三角形の数
        */
        void AddPackedTrianglesNum(const u32 packedTrianglesNum);

        /*!
        :private

        @brief    パックしている三角形の数を取得します。
        
        @return   パックしている三角形の数
        */
        u32 GetPackedTrianglesNum(void) const;

    public:
        /*!
        :private

        @brief    パックしている三角形を描画します。
        
        */
        void DrawPackedTriangles(void);

    public:
        /*!
        :private

        @brief    パックしている三角形の数をクリアします。        
        */
        void ClearPackedNum(void);

        /*!
        :private

        @brief    パックしている三角形のデータを元に、更新する頂点バッファのサイズを求めます。        
        */
        void SetPackedArraysSize(void);

    public:
        /*!
        :private

        @brief    頂点バッファの更新を行います。        
        */
        void UpdateBuffers(void);

        /*!
        :private

        @brief    描画前に毎回頂点バッファの更新を行うかどうかを設定します。        
        */
        void SetUpdateBufferBeforeDraw(const bool updateBufferFlag);
    protected:
        void DestroyBuffers(void);    

    public:
        /*!
        :private

        @brief   頂点配列に位置座標を設定します。
                                
        @param[in]    index              三角形のインデックス
        @param[in]    positionX          位置座標のX座標
        @param[in]    positionY          位置座標のY座標
        @param[in]    positionZ          位置座標のZ座標
        */
        void SetPosition(const u32 index,
            const GLfloat positionX, const GLfloat positionY, const GLfloat positionZ);
    
        /*!
        :private

        @brief   頂点配列に位置座標を設定します。
                               
        @param[in]    index              三角形のインデックス
        @param[in]    positionX          位置座標のX座標
        @param[in]    positionY          位置座標のY座標
        @param[in]    positionZ          位置座標のZ座標
        @param[in]    positionW          位置座標のW座標
        */
        void SetPosition(const u32 index,
            const GLfloat positionX, const GLfloat positionY, 
            const GLfloat positionZ, const GLfloat positionW);

        /*!
        :private

        @brief   頂点配列に頂点カラーを設定します。
                                
        @param[in]    index              三角形のインデックス
        @param[in]    red                頂点カラーの赤成分
        @param[in]    green              頂点カラーの緑成分
        @param[in]    blue               頂点カラーの青成分                
        */
        void SetColor(const u32 index, 
            const GLfloat red, const GLfloat green, const GLfloat blue);

        /*!
        :private

        @brief   頂点配列に頂点カラーを設定します。
                                
        @param[in]    index              三角形のインデックス
        @param[in]    red                頂点カラーの赤成分
        @param[in]    green              頂点カラーの緑成分
        @param[in]    blue               頂点カラーの青成分
        @param[in]    alpha              頂点カラーのアルファ成分        
        */
        void SetColor(const u32 index,
            const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha);

        /*!
        :private

        @brief   頂点配列に頂点カラーを設定します。
                                        
        @param[in]    red                頂点カラーの赤成分
        @param[in]    green              頂点カラーの緑成分
        @param[in]    blue               頂点カラーの青成分
        */
        virtual void SetColor(const GLfloat red, const GLfloat green, const GLfloat blue);

        /*!
        :private

        @brief   頂点配列に頂点カラーを設定します。
                                
        @param[in]    index              三角形のインデックス
        @param[in]    red                頂点カラーの赤成分
        @param[in]    green              頂点カラーの緑成分
        @param[in]    blue               頂点カラーの青成分
        @param[in]    alpha              頂点カラーのアルファ成分        
        */
        virtual void SetColor(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha);

        /*!
        :private

        @brief   頂点配列にテクスチャ座標を設定します。
                                
        @param[in]    index              三角形のインデックス
        @param[in]    texcoordS          テクスチャ座標のS座標
        @param[in]    texcoordT          テクスチャ座標のT座標        
        */
        void SetTexcoord(const u32 index,
            const GLfloat texcoordS, const GLfloat texcoordT);

        /*!
        :private

        @brief   頂点配列にテクスチャ座標を設定します。
                                
        @param[in]    index              三角形のインデックス
        @param[in]    texcoordS          テクスチャ座標のS座標
        @param[in]    texcoordT          テクスチャ座標のT座標
        @param[in]    texcoordR          テクスチャ座標のR座標
        */
        void SetTexcoord(const u32 index,
            const GLfloat texcoordS, const GLfloat texcoordT, const GLfloat texcoordR);

        /*!
        :private

        @brief   頂点配列に法線を設定します。
                                
        @param[in]    index              三角形のインデックス
        @param[in]    normalX            法線のX座標
        @param[in]    normalY            法線のY座標
        @param[in]    normalZ            法線のZ座標
        */
        void SetNormal(const u32 index,
            const GLfloat normalX, const GLfloat normalY, const GLfloat normalZ);
        
        /*!
        :private

        @brief   頂点配列に接線を設定します。
                                
        @param[in]    index               三角形のインデックス
        @param[in]    tangentX            接線のX座標
        @param[in]    tangentY            接線のY座標
        @param[in]    tangentZ            接線のZ座標
        */
        void SetTangent(const u32 index,
            const GLfloat tangentX, const GLfloat tangentY, const GLfloat tangentZ);

        /*!
        :private

        @brief   インデックス配列に三角形のインデックスを設定します。
                               
        @param[in]    index              三角形のインデックス
        @param[in]    index0             三角形の1つ目の頂点のインデックス
        @param[in]    index1             三角形の2つ目の頂点のインデックス
        @param[in]    index2             三角形の3つ目の頂点のインデックス        
        */
        void SetIndex(const u32 index,
            const GLuint index0, const GLuint index1, const GLuint index2);

        /*!
        :private

        @brief   インデックス配列にTRIANGLE_STRIPで四角形を作るためのインデックスを設定します。
                        
        @param[in]    index              四角形のインデックス
        @param[in]    index0             四角形の1つ目の頂点のインデックス
        @param[in]    index1             四角形の2つ目の頂点のインデックス
        @param[in]    index2             四角形の3つ目の頂点のインデックス
        @param[in]    index3             四角形の4つ目の頂点のインデックス
        */
        void SetSquareIndex(const u32 square_index,
            const GLuint index0, const GLuint index1,
            const GLuint index2, const GLuint index3);
    
    public:
        /*!
        :private

        @brief    ワールド座標を設定します。
        
        @param[in]    worldPositionX   ワールド座標のX座標
        @param[in]    worldPositionY   ワールド座標のY座標
        @param[in]    worldPositionZ   ワールド座標のZ座標
        */
        void SetWorldPosition(const f32 worldPositionX, const f32 worldPositionY, const f32 worldPositionZ);

        /*!
        :private

        @brief    ワールド座標のベクトル分だけ平行移動させます。
        
        @param[in]    worldPositionX   ワールド座標のベクトルのX座標
        @param[in]    worldPositionY   ワールド座標のベクトルのY座標
        @param[in]    worldPositionZ   ワールド座標のベクトルのZ座標
        */
        void TranslateWorldPosition(const f32 worldPositionX, const f32 worldPositionY, const f32 worldPositionZ);

        /*!
        :private

        @brief    ワールド座標を取得します。
        
        @param[out]    worldPositionX   ワールド座標のX座標
        @param[out]    worldPositionY   ワールド座標のY座標
        @param[out]    worldPositionZ   ワールド座標のZ座標
        */
        void GetWorldPosition(f32& worldPositionX, f32& worldPositionY, f32& worldPositionZ);

        /*!
        :private

        @brief    ワールド座標系での回転角を設定します。
        
        @param[in]    worldAngleX   X軸周りの角度
        @param[in]    worldAngleY   Y軸周りの角度
        @param[in]    worldAngleZ   Z軸周りの角度
        */
        void SetWorldAngle(const f32 worldAngleX, const f32 worldAngleY, const f32 worldAngleZ);
        
        /*!
        :private

        @brief    ワールド座標系での回転角分だけ回転させます。
        
        @param[in]    worldAngleX   ワールド座標のX軸周りの角度
        @param[in]    worldAngleY   ワールド座標のY軸周りの角度
        @param[in]    worldAngleZ   ワールド座標のZ軸周りの角度
        */
        void RotateWorldAngle(const f32 worldAngleX, const f32 worldAngleY, const f32 worldAngleZ);

        /*!
        :private

        @brief    ワールド座標系での回転角を取得します。
        
        @param[in]    worldAngleX   X軸周りの角度
        @param[in]    worldAngleY   Y軸周りの角度
        @param[in]    worldAngleZ   Z軸周りの角度
        */
        void GetWorldAngle(f32& worldAngleX, f32& worldAngleY, f32& worldAngleZ);
        
    public:
        /*!
        :private

        @brief    ワールド行列を取得します。
        
        @return   ワールド行列
        */
        virtual nn::math::MTX44 GetWorldMatrix(void) const;

    public:
        /*!
        :private

        @brief    描画用データを使った描画を行います。        
        */
        virtual void Draw(void);

    protected:
        virtual void DrawElements(void);

    public:
        /*!
        :private

        @brief    頂点属性を有効にします。        
        */
        virtual void EnableVertexAttributes(void);

        /*!
        :private

        @brief    描画前に頂点属性を有効にするかどうかを設定します。        

        @param[in]    updateAttributesFlag  描画前に頂点属性を有効にするときはtrue、そうでないときはfalseにする。
        */
        void SetEnableVertexAttributesBeforeDraw(const bool updateAttributesFlag);

    protected:
        u32 m_ShaderType;
        u32 m_VertexAttributes;
        GLenum m_TriangleType;
        u32 m_VerticesNum;
        u32 m_TrianglesNum;
        u32 m_PackedVerticesNum;
        u32 m_PackedTrianglesNum;

    protected:
        u32 m_PositionArraySize;
        u32 m_ColorArraySize;
        u32 m_TexcoordArraySize;
        u32 m_NormalArraySize;        
        u32 m_TangentArraySize;
        u32 m_TotalArraySize;
        u32 m_OldTotalArraySize;

        u32 m_IndexArraySize;
        u32 m_OldIndexArraySize;
    
    protected:
        GLfloat* m_PositionArray;
        GLfloat* m_ColorArray;
        GLfloat* m_TexcoordArray;
        GLfloat* m_NormalArray;        
        GLfloat* m_TangentArray;        
        GLushort* m_IndexArray;
    
    protected:
        GLuint m_ArrayBufferId;
        GLuint m_ElementArrayBufferId;
   
    protected:
        bool m_InitArrayFlag;
        bool m_InitBufferFlag;
        bool m_UpdateBufferFlag;
        bool m_UpdateAttributesFlag;

    protected:
        f32 m_PositionX;
        f32 m_PositionY;
        f32 m_PositionZ;
        f32 m_AngleX;
        f32 m_AngleY;
        f32 m_AngleZ;
    };

}

#endif
