#include "CollisionManager.h"
#include "CollisionConfig.h"
#include "SphereColliderBase.h"
#include "AABBColliderBase.h"
#include "OBBColliderBase.h"
#include "MyMath.h"

namespace Norm {

	using CollisionFunc = bool(*)(ICollider*, ICollider*);

	//衝突関数群
	bool SphereXSphere(ICollider* a, ICollider* b);
	bool SphereXAABB(ICollider* a, ICollider* b);
	bool SphereXOBB(ICollider* a, ICollider* b);
	bool AABBXSphere(ICollider* a, ICollider* b);
	bool AABBXAABB(ICollider* a, ICollider* b);
	bool AABBXOBB(ICollider* a, ICollider* b);
	bool OBBXSphere(ICollider* a, ICollider* b);
	bool OBBXAABB(ICollider* a, ICollider* b);
	bool OBBXOBB(ICollider* a, ICollider* b);

	constexpr CollisionFunc kCollisionTable[3][3] = {
		{ SphereXSphere ,SphereXAABB , SphereXOBB },
		{ AABBXSphere , AABBXAABB ,  AABBXOBB },
		{ OBBXSphere , OBBXAABB , OBBXOBB }
	};


	std::unique_ptr<CollisionManager> CollisionManager::instance_ = nullptr;

	CollisionManager* CollisionManager::GetInstance() {
		if (!instance_) {
			instance_ = std::unique_ptr<CollisionManager>(new CollisionManager());
		}
		return instance_.get();
	}

	void CollisionManager::Initialize() {
	}

	void CollisionManager::Finalize() {
		//インスタンスを削除
		instance_.reset();
	}

	void CollisionManager::CheckCollision() {
		//衝突フィルタリングラムダ式
		auto ShouldIgnore = [](CollisionAttribute a, CollisionAttribute b) {
			// 虚無オブジェクトは何とも当たらない
			if (a == CollisionAttribute::Nothingness ||
				b == CollisionAttribute::Nothingness) {
				return true;
			}

			// 自機と自機系
			if (a == CollisionAttribute::Player &&
				(b == CollisionAttribute::PlayerCannon ||
					b == CollisionAttribute::PlayerBullet)) {
				return true;
			}

			// 敵と敵系
			if (a == CollisionAttribute::Enemy &&
				(b == CollisionAttribute::EnemyCannon ||
					b == CollisionAttribute::EnemyBullet ||
					b == CollisionAttribute::EnemyBlast)) {
				return true;
			}

			return false;
			};

		// リスト内のペアを総当たり（重複しない）
		std::list<ICollider*>::iterator itrA = colliders_.begin();
		for (; itrA != colliders_.end(); ++itrA) {
			// イテレータAからコライダーAを取得する
			ICollider* colliderA = *itrA;
			// イテレータBはイテレーターAの次の要素から回す
			std::list<ICollider*>::iterator itrB = itrA;
			itrB++;
			for (; itrB != colliders_.end(); ++itrB) {
				// イテレーターBからコライダーBを取得する
				ICollider* colliderB = *itrB;
				// 衝突フィルタリング
				CollisionAttribute attrA = colliderA->GetCollisionAttribute();
				CollisionAttribute attrB = colliderB->GetCollisionAttribute();

				if (ShouldIgnore(attrA, attrB) ||
					ShouldIgnore(attrB, attrA)) {
					continue;
				}

				// ペアの当たり判定
				CheckCollisionPair(colliderA, colliderB);
			}
		}
	}

	void CollisionManager::RegistCollider(ICollider* _collider) {
		//登録
		colliders_.push_back(_collider);
	}

	void CollisionManager::DeleteCollider(ICollider* _collider) {
		//削除
		colliders_.remove(_collider);
	}

	void CollisionManager::CheckCollisionPair(ICollider* colliderA, ICollider* colliderB) {
		//衝突コールバック
		auto HandleCollisionIf = [&](bool isHit) {
			if (isHit) {
				// あらかじめ衝突属性を取得しておく(OnCollision内で変化したときのため)
				CollisionAttribute attrA = colliderA->GetCollisionAttribute();
				CollisionAttribute attrB = colliderB->GetCollisionAttribute();

				// 衝突時コールバックを呼び出す
				colliderA->OnCollision(colliderB);
				colliderB->OnCollision(colliderA);
			}
			};

		//形状種別取得
		const auto shapeA = colliderA->GetCollderShape();
		const auto shapeB = colliderB->GetCollderShape();

		//関数テーブルから衝突関数取得
		CollisionFunc func = kCollisionTable
			[static_cast<int>(shapeA)]
			[static_cast<int>(shapeB)];

		//衝突していないのでreturn
		if (!func) {
			return;
		}

		//衝突判定
		HandleCollisionIf(func(colliderA, colliderB));
	}

	bool SphereXSphere(ICollider* a, ICollider* b) {
		auto sa = static_cast<SphereColliderBase*>(a);
		auto sb = static_cast<SphereColliderBase*>(b);

		//判定
		return MyMath::IsCollision(sa->GetCapsule(), sb->GetCapsule());
	}

	bool SphereXAABB(ICollider* a, ICollider* b) {
		auto sa = static_cast<SphereColliderBase*>(a);
		auto sb = static_cast<AABBColliderBase*>(b);

		//判定
		return MyMath::IsCollision(sa->GetCapsule(), sb->GetAABB());
	}

	bool SphereXOBB(ICollider* a, ICollider* b) {
		auto sa = static_cast<SphereColliderBase*>(a);
		auto ob = static_cast<OBBColliderBase*>(b);

		//判定
		return MyMath::IsCollision(sa->GetCapsule(), ob->GetOBB());
	}

	bool AABBXSphere(ICollider* a, ICollider* b) {
		//順序入れ替え
		return SphereXAABB(b, a);
	}

	bool AABBXAABB(ICollider* a, ICollider* b) {
		auto aa = static_cast<AABBColliderBase*>(a);
		auto ab = static_cast<AABBColliderBase*>(b);

		//判定
		return MyMath::IsCollision(aa->GetAABB(), ab->GetAABB());
	}

	bool AABBXOBB(ICollider* a, ICollider* b) {
		auto aa = static_cast<AABBColliderBase*>(a);
		auto ob = static_cast<OBBColliderBase*>(b);

		//判定
		return MyMath::IsCollision(aa->GetAABB(), ob->GetOBB());
	}

	bool OBBXSphere(ICollider* a, ICollider* b) {
		//順序入れ替え
		return SphereXOBB(b, a);
	}

	bool OBBXAABB(ICollider* a, ICollider* b) {
		//順序入れ替え
		return AABBXOBB(b, a);
	}

	bool OBBXOBB(ICollider* a, ICollider* b) {
		auto oa = static_cast<OBBColliderBase*>(a);
		auto ob = static_cast<OBBColliderBase*>(b);

		//判定
		return MyMath::IsCollision(oa->GetOBB(), ob->GetOBB());
	}

}