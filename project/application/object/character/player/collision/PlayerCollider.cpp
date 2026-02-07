#include "PlayerCollider.h"
#include "GameCamera.h"

//アプリケーション
#include <application/object/character/player/Player.h>
#include <application/ui/message/MessageUI.h>

using namespace Norm;

PlayerCollider::PlayerCollider(Player* _holder) : OBBColliderBase(), holder_(_holder) {
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
	uint32_t item_reloadSpeedUp = holder_->GetItemReloadSpeedUp();
	uint32_t item_moveSpeedUp = holder_->GetItemMoveSpeedUp();
	uint32_t item_turnSpeedUp = holder_->GetItemTurnSpeedUp();
	bool isDamage = holder_->GetIsDamage();
	Vector3 reflectVelocity = holder_->GetReflectVelocity();

	//ローカル変数
	int maxHP = holder_->GetParam()["maxHP"];
	int item_maxNum = holder_->GetParam()["item_maxNum"];

	//当たり判定時の処理
	switch (_attribute) {
	case CollisionAttribute::Enemy:
	{
		//HPを減らす
		hp -= holder_->GetParam()["tackleDamage"];
		//0~MaxHPの範囲に収める
		hp = std::clamp(hp, 0, maxHP);
		//カメラシェイクを入れるmaxHP
		holder_->GetCamera()->RegistShake(0.4f, 0.8f);

		//ダメージヒット
		isDamage = true;

		//相手の座標の方向と反対方向のベクトルを速度に加算
		Vector3 reflectVec = -(_other->GetWorldTransform().GetWorldTranslate() - GetWorldTransform().GetWorldTranslate()).Normalized() * 10.0f;
		reflectVelocity.x = reflectVec.x;
		reflectVelocity.z = reflectVec.z;

		break;
	}
	case CollisionAttribute::EnemyCannon:
		//HPを減らす
		hp -= holder_->GetParam()["cannonDamage"];
		//0~MaxHPの範囲に収める
		hp = std::clamp(hp, 0, maxHP);
		//カメラシェイクを入れる
		holder_->GetCamera()->RegistShake(0.4f, 0.8f);

		//ダメージヒット
		isDamage = true;

		break;
	case CollisionAttribute::EnemyBullet:
		//HPを減らす
		hp -= holder_->GetParam()["bulletDamage"];
		//0~MaxHPの範囲に収める
		hp = std::clamp(hp, 0, maxHP);
		//カメラシェイクを入れる
		holder_->GetCamera()->RegistShake(0.2f, 0.3f);

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
		holder_->GetCamera()->RegistShake(0.4f, 0.8f);

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
	case CollisionAttribute::Item_ReloadSpeedUp:
		//アイテムが制限を超えていなければ取得
		if (item_reloadSpeedUp < (uint32_t)item_maxNum) {
			//アイテム取得
			item_reloadSpeedUp++;
			//メッセージUIにアイテム取得を通知
			std::wstring message = L"リロード速度UP！";
			holder_->GetMessageUI()->AddMessage(message);
		}

		break;
	case CollisionAttribute::Item_MoveSpeedUp:
		//アイテムが制限を超えていなければ取得
		if (item_moveSpeedUp < (uint32_t)item_maxNum) {
			//アイテム取得
			item_moveSpeedUp++;
			//メッセージUIにアイテム取得を通知
			std::wstring message = L"移動速度UP！";
			holder_->GetMessageUI()->AddMessage(message);
		}

		break;
	case CollisionAttribute::Item_TurnSpeedUp:
		//アイテムが制限を超えていなければ取得
		if (item_turnSpeedUp < (uint32_t)item_maxNum) {
			//アイテム取得
			item_turnSpeedUp++;
			//メッセージUIにアイテム取得を通知
			std::wstring message = L"回転速度UP！";
			holder_->GetMessageUI()->AddMessage(message);
		}

		break;
	default:
		break;
	}

	//変更した変数のセット
	holder_->SetHP(hp);
	holder_->SetItemReloadSpeedUp(item_reloadSpeedUp);
	holder_->SetItemMoveSpeedUp(item_moveSpeedUp);
	holder_->SetItemTurnSpeedUp(item_turnSpeedUp);
	holder_->SetIsDamage(isDamage);
	holder_->SetReflectVelocity(reflectVelocity);

}
