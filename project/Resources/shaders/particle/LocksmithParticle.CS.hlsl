#include "ParticleCommon.hlsli"

// 粒の配列
RWStructuredBuffer<Grain> gGrains : register(u0);
// 粒のIndex配列情報
RWStructuredBuffer<int> gGrainIndices : register(u1);
// エミッターの範囲情報
RWStructuredBuffer<EmitterRange> gEmitterRange : register(u2);

// エミッター情報
StructuredBuffer<EmitterInfo> gEmitterInfo : register(t0);
// JSON情報の配列
StructuredBuffer<JsonInfo> gJsonInfo : register(t1);

// 総合情報
ConstantBuffer<GeneralInfo> gGeneralInfo : register(b0);

[numthreads(1024, 1, 1)]
//粒配列からエミッターIDで対象の要素番号を取得できるようにするためにするための処理
//Update.CSで既に初期化済み
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint grainIndex = DTid.x;
    
    //稼働する必要のないスレッドでは計算処理を省く
    if (grainIndex >= gGeneralInfo.maxGrains)
        return;
    //粒インデックスの「生存時間」== 0.0fならこの粒は空なのでreturn
    if (gGrains[grainIndex].lifeTime == 0.0f)
        return;
    
    //エミッターの範囲情報の生存カウントをインクリメント
    uint emitterID = gGrains[grainIndex].emitterID;
    uint countValue;
    InterlockedAdd(gEmitterRange[emitterID].aliveCount, 1, countValue);
    //エミッターの範囲情報の開始要素番号を入れる(最初の生存粒のみ)
    if (countValue == 0)
    {
        int startValue = 0;
        //for分でstartValueを計算
        for (int i = 0; i < emitterID; i++)
        {
            startValue += gJsonInfo[i].maxGrains;
        }
        gEmitterRange[emitterID].start = startValue;
    }
    
    //粒のIndex情報に登録
    int start = gEmitterRange[emitterID].start;
    gGrainIndices[start + countValue] = grainIndex;
}