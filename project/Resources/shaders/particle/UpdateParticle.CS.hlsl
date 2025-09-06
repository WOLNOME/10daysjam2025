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
    //使用するエミッターを選択
    EmitterInfo emitterInfo = gEmitterInfo;
    
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
        else
        {
            //ここに来たら設定がおかしい。安全策をうっておく
            InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
        }
    }
    ///==================///
    /// エミッターとの処理
    ///==================///
    //重力処理
    if (emitterInfo.isGravity == 1)
        grain.velocity.y += emitterInfo.gravity * gPerFrame.deltaTime;
    //バウンド処理
    if (emitterInfo.isBound == 1)
    {
        //粒の最底辺位置の計算
        float leg = grain.transform.translate.y - (grain.transform.scale.y + gPerFrame.deltaTime * grain.sizeValue);
        //床の反発処理
        if (leg > emitterInfo.floorHeight && leg + (gPerFrame.deltaTime * grain.velocity.y) < emitterInfo.floorHeight)
            grain.velocity.y *= (-1.0f) * emitterInfo.repulsion;
    }
    ///==================///
    /// 粒情報の処理
    ///==================///
    //速度加算
    float4 currentVelocity = gPerFrame.deltaTime * grain.velocity;
    //回転更新
    float4 currentRotate = gPerFrame.deltaTime * grain.rotateValue;
    //サイズ更新
    float currentSize = gPerFrame.deltaTime * grain.sizeValue;
    //各粒のトランスフォーム
    grain.transform.translate += currentVelocity;
    grain.transform.rotate += currentRotate;
    grain.transform.scale += currentSize;
    grain.transform.scale = max(grain.transform.scale, 0.0f);
    
    //更新後の粒データを書き込む
    gGrains[grainIndex] = grain;
}