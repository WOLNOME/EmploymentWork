#include "EnemyReactionUI.h"
#include <TextureManager.h>
#include <Object3dManager.h>
#include <GameCamera.h>
#include <cassert>

using namespace Norm;

void EnemyReactionUI::Initialize() {
	//パラメータ読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/enemyUI");
	float hpBarSizeRatio = param_["hpBarSizeRatio"];
	//感知オブジェクトの生成・初期化
	textureHandleSensing_ = TextureManager::GetInstance()->LoadTexture("sensing.png");
	for (int i = 0; i < kNumSensing; i++) {
		sensing_[i].object = std::make_unique<Object3d>();
		sensing_[i].object->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("SensingUI"), Shape::ShapeKind::kPlane);
		sensing_[i].object->SetTexture(textureHandleSensing_);
		sensing_[i].object->SetIsDisplay(false);
		sensing_[i].object->SetIsLightProcess(false);
		sensing_[i].basePosition = { 0,0,0 };
		sensing_[i].isActive = false;
	}
	//見失うオブジェクトの生成・初期化
	textureHandleMissing_ = TextureManager::GetInstance()->LoadTexture("missing.png");
	for (int i = 0; i < kNumSensing; i++) {
		missing_[i].object = std::make_unique<Object3d>();
		missing_[i].object->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("MissingUI"), Shape::ShapeKind::kPlane);
		missing_[i].object->SetTexture(textureHandleMissing_);
		missing_[i].object->SetIsDisplay(false);
		missing_[i].object->SetIsLightProcess(false);
		missing_[i].basePosition = { 0,0,0 };
		missing_[i].isActive = false;
	}
}

void EnemyReactionUI::Update() {
	//カメラチェック
	assert(camera_ != nullptr && "カメラをセットしてください");

	//感知オブジェクトの走査
	for (int i = 0; i < kNumSensing; i++) {
		//アクティブ出ないなら次へ
		if (!sensing_[i].isActive) {
			continue;
		}

		//タイマーを進める
		sensing_[i].dirTimer -= kDeltaTime;

		//サイズを変える
		{
			//サイズを定義
			float size = 0.0f;
			float sensingTargetSize = param_["sensingTargetSize"];
			//線形補完で求める
			size = MyMath::Lerp(0.0f, sensingTargetSize, MyMath::EaseOutBack(1.0f - (sensing_[i].dirTimer / kSensingDirTime)));
			//サイズをセット
			sensing_[i].object->worldTransform.SetScale({ size,size,size });
		}
		//回転を変える（ビルボード）
		{
			//カメラからビュー行列を取得
			Matrix4x4 view = camera_->GetViewMatrix();
			//平行移動を消す
			view.m[3][0] = 0.0f;
			view.m[3][1] = 0.0f;
			view.m[3][2] = 0.0f;
			//ビルボード行列を定義
			Matrix4x4 billboardMatrix = MyMath::Inverse(view);
			//回転を定義
			Vector3 rotate{};
			rotate.y = std::asinf(-billboardMatrix.m[0][2]);
			if (std::cos(rotate.y) != 0) {
				//X軸回転の抽出
				rotate.x = std::atan2f(billboardMatrix.m[1][2], billboardMatrix.m[2][2]);
				//Z軸回転の抽出
				rotate.z = std::atan2f(billboardMatrix.m[0][1], billboardMatrix.m[0][0]);
			}
			else {
				//ジンバルロックが発生している場合（Y軸が±90度付近）
				rotate.x = std::atan2f(-billboardMatrix.m[2][1], billboardMatrix.m[1][1]);
				rotate.z = 0;
			}
			//回転をセット
			sensing_[i].object->worldTransform.SetRotate(rotate);
		}
		//座標を変える
		{
			//基準座標をセット
			sensing_[i].object->worldTransform.SetTranslate(sensing_[i].basePosition);
		}

		//透明度を変える
		{
			//透明度を定義
			float alpha = 0.0f;
			//線形補完で求める
			alpha = MyMath::Lerp(1.0f, 0.0f, MyMath::EaseOutBack(1.0f - (sensing_[i].dirTimer / kSensingDirTime)));
			//透明度をセット
			sensing_[i].object->SetColor({ 1,1,1,alpha });
		}

		//タイマーが時間に達したら
		if (sensing_[i].dirTimer <= 0.0f) {
			//非アクティブ状態へ切り替える
			sensing_[i].isActive = false;
			//タイマーを元に戻す
			sensing_[i].dirTimer = kSensingDirTime;
			//オブジェクトを非表示にする
			sensing_[i].object->SetIsDisplay(false);
		}
	}
	//見失うオブジェクトの走査
	for (int i = 0; i < kNumMissing; i++) {
		//アクティブ出ないなら次へ
		if (!missing_[i].isActive) {
			continue;
		}

		//タイマーを進める
		missing_[i].dirTimer -= kDeltaTime;

		//サイズを変える
		{
			//サイズを定義
			float size = 0.0f;
			float missingTargetSize = param_["missingTargetSize"];
			//線形補完で求める
			size = MyMath::Lerp(0.0f, missingTargetSize, MyMath::EaseOutBack(1.0f - (missing_[i].dirTimer / kMissingDirTime)));
			//サイズをセット
			missing_[i].object->worldTransform.SetScale({ size,size,size });
		}
		//回転を変える（ビルボード）
		{
			//カメラからビュー行列を取得
			Matrix4x4 view = camera_->GetViewMatrix();
			//平行移動を消す
			view.m[3][0] = 0.0f;
			view.m[3][1] = 0.0f;
			view.m[3][2] = 0.0f;
			//ビルボード行列を定義
			Matrix4x4 billboardMatrix = MyMath::Inverse(view);
			//回転を定義
			Vector3 rotate{};
			rotate.y = std::asinf(-billboardMatrix.m[0][2]);
			if (std::cos(rotate.y) != 0) {
				//X軸回転の抽出
				rotate.x = std::atan2f(billboardMatrix.m[1][2], billboardMatrix.m[2][2]);
				//Z軸回転の抽出
				rotate.z = std::atan2f(billboardMatrix.m[0][1], billboardMatrix.m[0][0]);
			}
			else {
				//ジンバルロックが発生している場合（Y軸が±90度付近）
				rotate.x = std::atan2f(-billboardMatrix.m[2][1], billboardMatrix.m[1][1]);
				rotate.z = 0;
			}
			//回転をセット
			missing_[i].object->worldTransform.SetRotate(rotate);
		}
		//座標を変える
		{
			//基準座標をセット
			missing_[i].object->worldTransform.SetTranslate(missing_[i].basePosition);
		}

		//透明度を変える
		{
			//透明度を定義
			float alpha = 0.0f;
			//線形補完で求める
			alpha = MyMath::Lerp(1.0f, 0.0f, MyMath::EaseOutBack(1.0f - (missing_[i].dirTimer / kMissingDirTime)));
			//透明度をセット
			missing_[i].object->SetColor({ 1,1,1,alpha });
		}

		//タイマーが時間に達したら
		if (missing_[i].dirTimer <= 0.0f) {
			//非アクティブ状態へ切り替える
			missing_[i].isActive = false;
			//タイマーを元に戻す
			missing_[i].dirTimer = kMissingDirTime;
			//オブジェクトを非表示にする
			missing_[i].object->SetIsDisplay(false);
		}
	}

}

void EnemyReactionUI::SensingSpawn(const Norm::Vector3& _subjectPos, float _height, float _front) {
	//発見コンテナを走査
	for (int i = 0; i < kNumSensing; i++) {
		//アクティブであれば次へ
		if (sensing_[i].isActive) {
			continue;
		}

		//アクティブ状態に切り替える
		sensing_[i].isActive = true;
		//タイマーをセット
		sensing_[i].dirTimer = kSensingDirTime;
		//オブジェクトを表示する
		sensing_[i].object->SetIsDisplay(true);
		//基準座標をセットする
		{
			//基準座標の定義
			Vector3 basePosition{};
			//対象座標→カメラ座標の方向を求める
			Vector3 direction = camera_->worldTransform.GetWorldTranslate() - _subjectPos;
			direction.Normalize();
			//高さを設定
			basePosition.y = _subjectPos.y + _height;
			//奥行を設定
			basePosition.x = _subjectPos.x + direction.x * _front;
			basePosition.z = _subjectPos.z + direction.z * _front;
			//基準座標をセット
			sensing_[i].basePosition = basePosition;
		}

		break;
	}
}

void EnemyReactionUI::MissingSpawn(const Norm::Vector3& _subjectPos, float _height, float _front) {
	//発見コンテナを走査
	for (int i = 0; i < kNumMissing; i++) {
		//アクティブであれば次へ
		if (missing_[i].isActive) {
			continue;
		}

		//アクティブ状態に切り替える
		missing_[i].isActive = true;
		//タイマーをセット
		missing_[i].dirTimer = kMissingDirTime;
		//オブジェクトを表示する
		missing_[i].object->SetIsDisplay(true);
		//基準座標をセットする
		{
			//基準座標の定義
			Vector3 basePosition{};
			//対象座標→カメラ座標の方向を求める
			Vector3 direction = camera_->worldTransform.GetWorldTranslate() - _subjectPos;
			direction.Normalize();
			//高さを設定
			basePosition.y = _subjectPos.y + _height;
			//奥行を設定
			basePosition.x = _subjectPos.x + direction.x * _front;
			basePosition.z = _subjectPos.z + direction.z * _front;
			//基準座標をセット
			missing_[i].basePosition = basePosition;
		}

		break;
	}
}
