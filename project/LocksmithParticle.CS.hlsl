#include "ParticleCommon.hlsli"

//粒のIndex配列情報
RWStructuredBuffer<int> gGrainIndices : register(u0);
//エミッターの範囲情報
RWStructuredBuffer<EmitterRange> gEmitterRange : register(u1);

// 粒の配列
StructuredBuffer<Grain> gGrains : register(t0);
// エミッター情報
StructuredBuffer<EmitterInfo> gEmitterInfo : register(t1);

[numthreads(1024, 1, 1)]
//粒配列からエミッターIDで対象の要素番号を取得できるようにするためにするための処理
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint grainIndex = DTid.x;
    
    //稼働する必要のないスレッドでは計算処理を省く
    if (grainIndex >= gGeneralInfo.maxGrains)
        return;
    
    //粒インデックスの「生存時間」== 0.0fならこの粒は空なのでreturn
    if (gGrains[grainIndex].lifeTime == 0.0f)
        return;
    
    //粒のエミッターIDを使ってエミッターの範囲情報に書き込む
    
    
    
}