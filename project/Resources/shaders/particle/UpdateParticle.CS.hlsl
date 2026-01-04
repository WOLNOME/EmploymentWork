#include "ParticleCommon.hlsli"

//粒の配列
RWStructuredBuffer<Grain> gGrains : register(u0);
//フリーリストのインデックス
RWStructuredBuffer<int> gFreeListIndex : register(u1);
//フリーリスト
RWStructuredBuffer<uint> gFreeList : register(u2);

//エミッターの配列
ConstantBuffer<EmitterInfo> gEmitterInfo : register(b0);
//JSON情報の配列
ConstantBuffer<JsonInfo> gJsonInfo : register(b1);
//フレーム情報
ConstantBuffer<PerFrame> gPerFrame : register(b2);

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint grainIndex = DTid.x;
    //稼働する必要のないスレッドでは計算処理を省く
    if (grainIndex >= gJsonInfo.maxGrains)
        return;
    // 寿命が0の粒子は死亡済みと見なして即スキップ
    if (gGrains[grainIndex].lifeTime == 0)
        return;
    
    //Outputから粒の情報を受け取る
    Grain grain = gGrains[grainIndex];
    
    //現在時間の更新
    grain.currentTime += gPerFrame.deltaTime;
    ///==================///
    /// 粒の削除処理
    ///==================///
    //寿命を迎えたら
    if (grain.currentTime > grain.lifeTime)
    {
        //全データに0を入れる
        gGrains[grainIndex] = (Grain) 0;
        int freeListIndex;
        InterlockedAdd(gFreeListIndex[0], 1, freeListIndex);
        //最新のFreeListIndexの場所に死亡済みGrainのIndexを設定する。
        if ((freeListIndex + 1) < gJsonInfo.maxGrains)
        {
            gFreeList[freeListIndex + 1] = grainIndex;
            return;
        }
    }
    ///==================///
    /// 粒情報の処理
    ///==================///
    
    //重力処理
    if (gJsonInfo.isGravity == 1)
        grain.velocity.y += gJsonInfo.gravity * gPerFrame.deltaTime;
    //バウンド処理
    if (gJsonInfo.isBound == 1)
    {
        //粒の最底辺位置の計算
        float leg = grain.transform.translate.y - (grain.transform.scale.y + gPerFrame.deltaTime * grain.sizeVelocity);
        //床の反発処理
        if (leg > gJsonInfo.floorHeight && leg + (gPerFrame.deltaTime * grain.velocity.y) < gJsonInfo.floorHeight)
            grain.velocity.y *= (-1.0f) * gJsonInfo.repulsion;
    }
    //速度加算
    float4 currentVelocity = gPerFrame.deltaTime * grain.velocity;
    grain.transform.translate += currentVelocity;
    //回転更新
    float4 currentRotate = gPerFrame.deltaTime * grain.angularVelocity;
    grain.transform.rotate += currentRotate;
    //サイズ更新(初期値からrateを割り出す)
    float t = saturate(grain.currentTime / grain.lifeTime); // 0〜1
    float rate = 1.0f + grain.sizeVelocity * t;
    rate = max(rate, 0.0f);
    grain.transform.scale = grain.initialTransform.scale * rate;
    
    //更新後の粒データを書き込む
    gGrains[grainIndex] = grain;
}