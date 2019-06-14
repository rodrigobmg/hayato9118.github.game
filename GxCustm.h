// (GxCustm.h)
#ifndef ___GXCUSTM_H
#define ___GXCUSTM_H

#include <nn/gx.h>

void BeforeRender(void);
void DrawPolygon(u32 p_prim, u32 p_index, const u8* pColor = NULL);
void AfterRender(void);
void SetFrame(s32 frame = -1);
void Combiner(const u8* pColor = NULL);
void BlendStateDesc(void);

#endif // ___GXCUSTM_H
