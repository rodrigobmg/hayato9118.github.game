/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_RenderData.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_RENDER_DATA_H_
#define DEMO_RENDER_DATA_H_

#include <nn/gx.h>
#include <nn/util.h>

namespace demo
{
      
    /*!
    :private

    @brief    LCDの幅を取得します。    

    @param[in]    display    対象とするLCD (NN_GX_DISPLAY0 または NN_GX_DISPLAY1)
    @return       LCDの幅
    */
    f32 GetLcdWidth(const s32 display);

    /*!
    :private

    @brief    LCDの高さを取得します。
    
    @param[in]    display    対象とするLCD (NN_GX_DISPLAY0 または NN_GX_DISPLAY1)
    @return       LCDの高さ
    */
    f32 GetLcdHeight(const s32 display);
     
    /*!
    :private
    
    @brief 描画用データの基底クラスです。       
    */
    class RenderData : private nn::util::NonCopyable<RenderData>
    {
    public:
        /*!
        :private

        @brief    コンストラクタです。        
        */
        RenderData(void);
        /*!
        :private

        @brief    デストラクタです。        
        */
        virtual ~RenderData(void);

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

        @brief    描画用データを使った描画を行います。        
        */
        virtual void Draw(void) = 0;

    public:
        /*!
        :private

        @brief    正規化デバイス座標系からウィンドウ座標系に変換するためのウィンドウサイズを設定します。
                
        @param[in]    windowWidth    ウィンドウの幅
        @param[in]    windowHeight   ウィンドウの高さ
        */
        virtual void SetWindowSize(const f32 windowWidth, const f32 windowHeight);

    public:
        /*!
        :private

        @brief   与えられたウィンドウ座標を正規化デバイス座標へ変換します。
                
        @param[in]    windowCoordinateX    ウィンドウ座標のX座標
        @param[in]    windowCoordinateY    ウィンドウ座標のY座標
        @param[out]   normalizedDeviceCoordinateX      正規化デバイス座標のX座標
        @param[out]   normalizedDeviceCoordinateY      正規化デバイス座標のY座標
        */
        virtual void GetNormalizedDeviceCoordinateXY(const f32 windowCoordinateX, const f32 windowCoordinateY,
            f32& normalizedDeviceCoordinateX, f32& normalizedDeviceCoordinateY);

        /*!
        :private

        @brief   与えられたウィンドウ座標のY座標を正規化デバイス座標のX座標へ変換します。
                
        @param[in]    windowCoordinateY    ウィンドウ座標のY座標 
        @return       正規化デバイス座標のX座標
        */
        virtual f32 GetNormalizedDeviceCoordinateX(const f32 windowCoordinateY);

        /*!
        :private

        @brief   与えられたウィンドウ座標のX座標を正規化デバイス座標のY座標へ変換します。
                
        @param[in]    windowCoordinateX    ウィンドウ座標のX座標 
        @return       正規化デバイス座標のY座標
        */
        virtual f32 GetNormalizedDeviceCoordinateY(const f32 windowCoordinateX);
    
    protected:
        virtual void CalculateInverseWindowSize(void);

    protected:
        s32 m_RenderTarget;
        f32 m_WindowWidth;
        f32 m_WindowHeight;

        f32 m_InverseWindowWidth;
        f32 m_InverseWindowHeight;
    };

}

#endif
