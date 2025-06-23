#include "CollisionManager.h"
#include "CollisionConfig.h"
#include "MyMath.h"

CollisionManager* CollisionManager::instance = nullptr;

CollisionManager* CollisionManager::GetInstance() {
	if (instance == nullptr) {
		instance = new CollisionManager;
	}
	return instance;
}

void CollisionManager::Initialize() {
}

void CollisionManager::Finalize() {
	delete instance;
	instance = nullptr;
}

void CollisionManager::CheckCollision() {
	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		Collider* colliderA = *itrA;
		// イテレータBはイテレーターAの次の要素から回す
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			// イテレーターBからコライダーBを取得する
			Collider* colliderB = *itrB;
			// 衝突フィルタリング
			// 自分と同じ属性だと当たらない
			if (colliderA->GetCollisionAttribute() == colliderB->GetCollisionAttribute()) {
				continue;
			}
			// 自機と自弾
			if (colliderA->GetCollisionAttribute() == CollisionAttribute::Player) {
				if (colliderB->GetCollisionAttribute() == CollisionAttribute::PlayerCannon) {
					continue;
				}
				else if (colliderB->GetCollisionAttribute() == CollisionAttribute::PlayerBullet) {
					continue;
				}
			}
			if (colliderB->GetCollisionAttribute() == CollisionAttribute::Player) {
				if (colliderA->GetCollisionAttribute() == CollisionAttribute::PlayerCannon) {
					continue;
				}
				else if (colliderA->GetCollisionAttribute() == CollisionAttribute::PlayerBullet) {
					continue;
				}
			}
			// 敵と敵弾
			if (colliderA->GetCollisionAttribute() == CollisionAttribute::Enemy) {
				if (colliderB->GetCollisionAttribute() == CollisionAttribute::EnemyCannon) {
					continue;
				}
				else if (colliderB->GetCollisionAttribute() == CollisionAttribute::EnemyBullet) {
					continue;
				}
			}
			if (colliderB->GetCollisionAttribute() == CollisionAttribute::Enemy) {
				if (colliderA->GetCollisionAttribute() == CollisionAttribute::EnemyCannon) {
					continue;
				}
				else if (colliderA->GetCollisionAttribute() == CollisionAttribute::EnemyBullet) {
					continue;
				}
			}
			// 虚無オブジェクトは当たり判定を取らない
			if (colliderA->GetCollisionAttribute() == CollisionAttribute::Nothingness || colliderB->GetCollisionAttribute() == CollisionAttribute::Nothingness) {
				continue;
			}

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::SetColliders(Collider* colliders) {
	// コライダーリストに追加
	colliders_.push_back(colliders);
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	//衝突コールバック
	auto HandleCollisionIf = [&](bool isHit) {
		if (isHit) {
			// あらかじめ衝突属性を取得しておく(OnCollision内で変化したときのため)
			CollisionAttribute attrA = colliderA->GetCollisionAttribute();
			CollisionAttribute attrB = colliderB->GetCollisionAttribute();

			// 衝突時コールバックを呼び出す
			colliderA->OnCollision(attrB);
			colliderB->OnCollision(attrA);
		}
		};


	switch (colliderA->GetCollisionShapeKind()) {
	case Collider::CollisionShapeKind::Sphere: {
		//カプセルを作る
		Capsule capsuleA;
		capsuleA.radius = colliderA->GetRadius();
		capsuleA.segment.origin = colliderA->GetPrePosition();
		capsuleA.segment.diff = colliderA->GetWorldPosition() - colliderA->GetPrePosition();
		//Bの形状によって分岐
		switch (colliderB->GetCollisionShapeKind()) {
		case Collider::CollisionShapeKind::Sphere: {
			//カプセルを作る
			Capsule capsuleB;
			capsuleB.radius = colliderB->GetRadius();
			capsuleB.segment.origin = colliderB->GetPrePosition();
			capsuleB.segment.diff = colliderB->GetWorldPosition() - colliderB->GetPrePosition();
			//カプセル同士の衝突判定
			HandleCollisionIf(MyMath::IsCollision(capsuleA, capsuleB));
			break;
		}
		case Collider::CollisionShapeKind::AABB: {
			//AABBを作る
			AABB aabbB = {
				.min = colliderB->GetAABB().min + colliderB->GetWorldPosition(),
				.max = colliderB->GetAABB().max + colliderB->GetWorldPosition()
			};
			//カプセルとAABBの衝突判定
			HandleCollisionIf(MyMath::IsCollision(capsuleA, aabbB));
			break;
		}

		default:
			break;
		}
		break;
	}
	case Collider::CollisionShapeKind::AABB: {
		//AABBを作る
		AABB aabbA = {
			.min = colliderA->GetAABB().min + colliderA->GetWorldPosition(),
			.max = colliderA->GetAABB().max + colliderA->GetWorldPosition()
		};
		//Bの形状によって分岐
		switch (colliderB->GetCollisionShapeKind()) {
		case Collider::CollisionShapeKind::Sphere: {
			//カプセルを作る
			Capsule capsuleB;
			capsuleB.radius = colliderB->GetRadius();
			capsuleB.segment.origin = colliderB->GetPrePosition();
			capsuleB.segment.diff = colliderB->GetWorldPosition() - colliderB->GetPrePosition();
			//AABBとカプセルの衝突判定
			HandleCollisionIf(MyMath::IsCollision(aabbA, capsuleB));
			break;
		}
		case Collider::CollisionShapeKind::AABB: {
			//AABBを作る
			AABB aabbB = {
				.min = colliderB->GetAABB().min + colliderB->GetWorldPosition(),
				.max = colliderB->GetAABB().max + colliderB->GetWorldPosition()
			};
			//AABB同士の衝突判定
			HandleCollisionIf(MyMath::IsCollision(aabbA, aabbB));
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}
