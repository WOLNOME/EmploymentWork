#include "BarrierLeaf.h"
#include "ImGuiManager.h"
#include <cstdint>
#include <CombinedParticleManager.h>
#include <TextureManager.h>
#include <Object3dManager.h>
#include <MyMath.h>

//アプリケーション
#include <application/object/character/enemy/boss/Barrier.h>

using namespace Norm;

BarrierLeaf::BarrierLeaf(int _nodeID, BlackBoard* _blackBoard) : LeafNodeBase(_nodeID, _blackBoard) {
	//展開中パーティクルの生成・初期化
	{
		barrierMidst_ = std::make_unique<CombinedParticle>();
		barrierMidst_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("BarrierMidst"), "Barrier_Midst");
		barrierMidst_->SetIsRepeat(true);
	}
	//展開完了パーティクルの生成・初期化
	{
		barrierComplete_ = std::make_unique<CombinedParticle>();
		barrierComplete_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("BarrierComplete"), "Barrier_Complete");
	}

	//バリア演出オブジェクトの生成・初期化
	uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("sky.png");
	dirObject_ = std::make_unique<Object3d>();
	dirObject_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Barrier"), Shape::ShapeKind::kSphere);
	dirObject_->SetTexture(textureHandle);
	dirObject_->SetIsDisplay(false);
	dirObject_->SetIsLightProcess(false);
	dirObject_->SetColor({ 1,1,1,0.6f });
	//ワールドトランスフォームの初期化
	dirWorldTransform_.Initialize();
	//オブジェクトにセット
	dirObject_->RegistWorldTransform(&dirWorldTransform_);

}

BarrierLeaf::~BarrierLeaf() {
}

void BarrierLeaf::Initialize() {
	//基底クラスの初期化
	LeafNodeBase::Initialize();

	//ブラックボードから必要な情報を取得
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");

	//ブラックボードの情報を初期化
	mpBlackBoard->SetValue<bool>("IsBarrier", false);
	float barrierDirTime = mpBlackBoard->GetValue<float>("BarrierDirTime");
	mpBlackBoard->SetValue<float>("BarrierDirTimer", barrierDirTime);

	//バリア展開中エフェクトを発生
	TransformEuler transform = barrierMidst_->GetBaseTransform();
	transform.translate = bossPos;
	transform.translate.y = 0.0f;
	barrierMidst_->SetBaseTransform(transform);
	barrierMidst_->SetIsPlay(true);

}

void BarrierLeaf::Update() {
	//ブラックボードから必要な情報を取得
	Barrier* barrier = mpBlackBoard->GetValue<Barrier*>("Barrier");
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	bool isBarrier = mpBlackBoard->GetValue<bool>("IsBarrier");
	float barrierDirTime = mpBlackBoard->GetValue<float>("BarrierDirTime");
	float barrierDirTimer = mpBlackBoard->GetValue<float>("BarrierDirTimer");
	float barrierCoolTime = mpBlackBoard->GetValue<float>("BarrierCoolTime");
	float barrierCoolTimer = mpBlackBoard->GetValue<float>("BarrierCoolTimer");
	int barrierHP = mpBlackBoard->GetValue<int>("BarrierHP");
	int barrierMaxHP = mpBlackBoard->GetValue<int>("BarrierMaxHP");

	//演出タイマーを更新
	barrierDirTimer -= kDeltaTime;

	//演出中の処理
	{
		//サイズを定義
		float size = 0.0f;

		//演出オブジェクトを表示
		dirObject_->SetIsDisplay(true);

		//サイズをタイマーに合わせて線形補完
		float targetSize = barrier->GetParam()["collisionRadiusSphere"];
		size = MyMath::Lerp(0.0f, targetSize, MyMath::EaseOutCubic(1.0f - (barrierDirTimer / barrierDirTime)));

		//オブジェクトに座標を指定
		dirWorldTransform_.SetTranslate(bossPos);

		//サイズをオブジェクトに適用
		dirWorldTransform_.SetScale({ size,size ,size });

	}

	//回転処理
	Rotate();

	//演出タイマーが0以下になったらバリアを貼る
	if (barrierDirTimer <= 0.0f) {
		//バリアを貼る
		isBarrier = true;
		//バリア演出タイマーをリセット
		barrierDirTimer = 0.0f;
		//バリアのクールタイムをセット
		barrierCoolTimer = barrierCoolTime;
		//バリアのHPを最大にセット
		barrierHP = barrierMaxHP;
		//演出オブジェクトを非表示
		dirObject_->SetIsDisplay(false);
		//バリアをスポーン
		barrier->Spawn(bossPos);

		//バリア展開中エフェクトの停止
		barrierMidst_->SetIsPlay(false);
		//バリア完成エフェクトの発生
		barrierComplete_->SetBaseTransform(barrierMidst_->GetBaseTransform());
		barrierComplete_->SetIsPlay(true);
	}

	//ブラックボードに更新した情報を保存
	mpBlackBoard->SetValue<bool>("IsBarrier", isBarrier);
	mpBlackBoard->SetValue<float>("BarrierDirTimer", barrierDirTimer);
	mpBlackBoard->SetValue<float>("BarrierCoolTimer", barrierCoolTimer);
	mpBlackBoard->SetValue<int>("BarrierHP", barrierHP);
}

void BarrierLeaf::Finalize() {
	//基底クラスの終了処理
	LeafNodeBase::Finalize();

	//バリア展開中エフェクトの停止
	barrierMidst_->SetIsPlay(false);
}

void BarrierLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("バリア");
	ImGui::End();
#endif // _DEBUG
}

NodeResult BarrierLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	bool isBarrier = mpBlackBoard->GetValue<bool>("IsBarrier");

	//バリアがアクティブでないならrunningを返す
	if (!isBarrier) {
		return NodeResult::Running;
	}
	//バリアがアクティブならsuccessを返す
	return NodeResult::Success;
}


void BarrierLeaf::Rotate() {
	//ブラックボードから必要な情報を取得
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	Vector3 bossRotate = mpBlackBoard->GetValue<Vector3>("BossRotate");
	float bossTurnSpeed = mpBlackBoard->GetValue<float>("BossTurnSpeed");
	Vector3 playerPos = mpBlackBoard->GetValue<Vector3>("PlayerPos");

	//ボスの回転速度は半減で使う
	bossTurnSpeed *= 0.5f;

	//現在のボスの向きを求める
	Vector3 currentBossDir = {
		std::sinf(bossRotate.y),
		0.0f,
		std::cosf(bossRotate.y)
	};
	currentBossDir.Normalize();
	//目標ポイント（プレイヤーの位置）への方向を求める
	Vector3 targetDir = playerPos - bossPos;
	targetDir.Normalize();
	//回転の差を求める
	float angle = std::atan2f(targetDir.x, targetDir.z) - std::atan2f(currentBossDir.x, currentBossDir.z);
	//angleを-pi~piでクランプする
	if (angle > pi) {
		angle -= 2 * pi;
	}
	else if (angle < -pi) {
		angle += 2 * pi;
	}
	//angle<回転速度の場合
	float usingRotateSpeed = 0.0f;
	if (std::abs(angle) < bossTurnSpeed * kDeltaTime) {
		//仕上げの角度加算
		usingRotateSpeed = angle;
	}
	else {
		//回転速度を使う場合、符号を揃える
		usingRotateSpeed = (angle > 0) ? bossTurnSpeed * kDeltaTime : -bossTurnSpeed * kDeltaTime;
	}
	//回転加算
	bossRotate.y += usingRotateSpeed;
	//-π~πにクランプ
	if (bossRotate.y > pi) {
		bossRotate.y -= 2.0f * pi;
	}
	else if (bossRotate.y < -pi) {
		bossRotate.y += 2.0f * pi;
	}

	//ブラックボードに更新した情報を保存
	mpBlackBoard->SetValue<Vector3>("BossRotate", bossRotate);
}