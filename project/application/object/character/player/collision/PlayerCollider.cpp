#include "PlayerCollider.h"
#include <GameCamera.h>

//アプリケーション
#include <application/system/CameraManager.h>
#include <application/object/character/player/Player.h>
#include <application/ui/message/MessageUI.h>

using namespace Norm;

PlayerCollider::PlayerCollider(Player* _holder) : OBBColliderBase(), holder_(_holder) {
	//SEの初期化
	collideSE_ = std::make_unique<Audio>();
	collideSE_->Initialize("se/collide.mp3");
}

void PlayerCollider::Debug() {
#ifdef _DEBUG
	//基底クラスのデバッグ処理
	OBBColliderBase::Debug();

	//debugLineColorを元に戻す
	debugLineColor_ = { 1,1,1,1 };

#endif // _DEBUG

}

void PlayerCollider::OnCollision(ICollider* _other, CollisionAttribute _attribute) {
	//変更する変数の定義
	int hp = holder_->GetHP();
	int specialNum = holder_->GetSpecialNum();
	int keyNum = holder_->GetKeyNum();
	int tutorialItemNum = holder_->GetTutorialItemNum();
	bool isDamage = holder_->GetIsDamage();
	Vector3 reflectVelocity = holder_->GetReflectVelocity();

	//ローカル変数
	int maxHP = holder_->GetParam()["maxHP"];
	GameCamera* gameCamera = dynamic_cast<GameCamera*>(holder_->GetCameraManager()->GetActiveCamera());

	//当たり判定時の処理
	switch (_attribute) {
	case CollisionAttribute::Enemy:
	{
		//HPを減らす
		hp -= holder_->GetParam()["tackleDamage"];
		//0~MaxHPの範囲に収める
		hp = std::clamp(hp, 0, maxHP);
		//カメラシェイクを入れるmaxHP
		gameCamera->RegistShake(0.4f, 0.8f);

		//ダメージヒット
		isDamage = true;

		//相手の座標の方向と反対方向のベクトルを速度に加算
		Vector3 reflectVec = -(_other->GetWorldTransform().GetWorldTranslate() - GetWorldTransform().GetWorldTranslate()).Normalized() * 10.0f;
		reflectVelocity.x = reflectVec.x;
		reflectVelocity.z = reflectVec.z;

		//衝突SE
		collideSE_->Play(false, 1.0f);

		break;
	}
	case CollisionAttribute::EnemyCannon:
		//HPを減らす
		hp -= holder_->GetParam()["cannonDamage"];
		//0~MaxHPの範囲に収める
		hp = std::clamp(hp, 0, maxHP);
		//カメラシェイクを入れる
		gameCamera->RegistShake(0.4f, 0.8f);

		//ダメージヒット
		isDamage = true;

		break;
	case CollisionAttribute::EnemyBullet:
		//HPを減らす
		hp -= holder_->GetParam()["bulletDamage"];
		//0~MaxHPの範囲に収める
		hp = std::clamp(hp, 0, maxHP);
		//カメラシェイクを入れる
		gameCamera->RegistShake(0.2f, 0.3f);

		//ダメージヒット
		isDamage = true;

		break;
	case CollisionAttribute::EnemyBlast:
	{
		//HPを減らす
		hp -= holder_->GetParam()["bombDamage"];
		//0~MaxHPの範囲に収める
		hp = std::clamp(hp, 0, maxHP);
		//カメラシェイクを入れるmaxHP
		gameCamera->RegistShake(0.4f, 0.8f);

		//ダメージヒット
		isDamage = true;

		//相手の座標の方向と反対方向のベクトルを速度に加算(大きく)
		Vector3 reflectVec = -(_other->GetWorldTransform().GetWorldTranslate() - GetWorldTransform().GetWorldTranslate()).Normalized() * 40.0f;
		reflectVelocity.x = reflectVec.x;
		reflectVelocity.z = reflectVec.z;

		break;
	}
	case CollisionAttribute::Item_Heal:
	{
		//HPを回復
		int healValue = holder_->GetParam()["item_healValue"];
		hp += healValue;

		//0~MaxHPの範囲に収める
		hp = std::clamp(hp, 0, maxHP);

		//メッセージUIにアイテム取得を通知
		std::wstring message = L"HPを" + std::to_wstring(healValue) + L"回復！";
		holder_->GetMessageUI()->AddMessage(message);

		break;
	}
	case CollisionAttribute::Item_Charge:
	{
		//必殺弾の数をインクリメント
		specialNum++;

		//必殺弾の数をクランプ
		int maxSpecial = holder_->GetParam()["specialMagazine"];
		specialNum = std::clamp(specialNum, 0, maxSpecial);

		//メッセージUIにアイテム取得を通知
		holder_->GetMessageUI()->AddMessage(L"必殺弾を獲得！");

		break;
	}
	case CollisionAttribute::Item_Key:
	{
		//鍵の数をインクリメント
		keyNum++;

		//メッセージUIにアイテム取得を通知
		std::wstring message = std::to_wstring(keyNum) + L"つめのキーを入手！";
		holder_->GetMessageUI()->AddMessage(message);

		break;
	}
	case CollisionAttribute::Item_TutorialCollectible:
	{
		//アイテムの数をインクリメント
		tutorialItemNum++;

		//メッセージUIにアイテム取得を通知
		std::wstring message = std::to_wstring(tutorialItemNum) + L"つめのアイテムを入手！";
		holder_->GetMessageUI()->AddMessage(message);

		break;
	}
	case CollisionAttribute::Wall:
	{
		//相手の座標の方向と反対方向のベクトルを速度に加算
		Vector3 reflectVec = -(_other->GetWorldTransform().GetWorldTranslate() - GetWorldTransform().GetWorldTranslate()).Normalized() * 10.0f;
		reflectVelocity.x = reflectVec.x;
		reflectVelocity.z = reflectVec.z;

		//衝突SE
		collideSE_->Play(false, 1.0f);

		break;
	}
	case CollisionAttribute::Barrier:
	{
		//相手の座標の方向と反対方向のベクトルを速度に加算
		Vector3 reflectVec = -(_other->GetWorldTransform().GetWorldTranslate() - GetWorldTransform().GetWorldTranslate()).Normalized() * 10.0f;
		reflectVelocity.x = reflectVec.x;
		reflectVelocity.z = reflectVec.z;

		break;
	}
	default:
		break;
	}

	//変更した変数のセット
	holder_->SetHP(hp);
	holder_->SetTutorialItemNum(tutorialItemNum);
	holder_->SetSpecialNum(specialNum);
	holder_->SetKeyNum(keyNum);
	holder_->SetIsDamage(isDamage);
	holder_->SetReflectVelocity(reflectVelocity);

}
