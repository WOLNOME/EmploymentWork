#include "BossCollider.h"

using namespace Norm;

BossCollider::BossCollider(Boss* _holder) : OBBColliderBase(), holder_(_holder) {
}

void BossCollider::OnCollision(ICollider* _other, CollisionAttribute _attribute) {

}
