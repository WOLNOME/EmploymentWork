#include "ApproachLeaf.h"
#include <MyMath.h>
#include <ImGuiManager.h>

using namespace Norm;

ApproachLeaf::ApproachLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

ApproachLeaf::~ApproachLeaf() {
}

void ApproachLeaf::Update() {
    // === BlackBoard取得 ===
    Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
    Vector3 bossVelocity = mpBlackBoard->GetValue<Vector3>("BossVelocity");
    float   bossMaxSpeed = mpBlackBoard->GetValue<float>("BossMaxSpeed");
    float   bossSpeed = mpBlackBoard->GetValue<float>("BossSpeed");
    Vector3 bossRotate = mpBlackBoard->GetValue<Vector3>("BossRotate");
    float   bossTurnSpeed = mpBlackBoard->GetValue<float>("BossTurnSpeed");
    Vector3 playerPos = mpBlackBoard->GetValue<Vector3>("PlayerPos");
    float   floorFriction = mpBlackBoard->GetValue<float>("FloorFriction");

    // === 向き・角度計算===
    Vector3 currentDir = {
        std::sinf(bossRotate.y),
        0.0f,
        std::cosf(bossRotate.y)
    };
    currentDir.Normalize();

    Vector3 targetDir = playerPos - bossPos;
    targetDir.Normalize();

    float angle = std::atan2f(targetDir.x, targetDir.z)
        - std::atan2f(currentDir.x, currentDir.z);

    if (angle > pi) angle -= 2 * pi;
    else if (angle < -pi) angle += 2 * pi;

    bool playerBehind = std::abs(angle) > (0.5f * pi);

    // === 回転処理（共通）===
    float maxRotate = bossTurnSpeed * kDeltaTime;
    float rotateAmount =
        (std::abs(angle) < maxRotate)
        ? angle
        : (angle > 0 ? maxRotate : -maxRotate);

    bossRotate.y += rotateAmount;

    if (bossRotate.y > pi) bossRotate.y -= 2.0f * pi;
    else if (bossRotate.y < -pi) bossRotate.y += 2.0f * pi;

    // === 移動処理（前にいる時だけ）===
    if (!playerBehind) {
        Vector3 forward = {
            std::sinf(bossRotate.y),
            0.0f,
            std::cosf(bossRotate.y)
        };
        forward.Normalize();

        //速度加算
        bossVelocity += forward * bossSpeed;

        // 摩擦
        if (bossVelocity.Length() > 0.0f) {
            Vector3 friction = -bossVelocity.Normalized() * floorFriction;
            bossVelocity += friction * kDeltaTime;
        }

        // 速度制限
        if (bossVelocity.Length() > bossMaxSpeed) {
            bossVelocity.Normalize();
            bossVelocity *= bossMaxSpeed;
        }

        if ((bossVelocity * kDeltaTime).Length() < 0.01f) {
            bossVelocity = { 0.0f, 0.0f, 0.0f };
        }

        bossPos += bossVelocity * kDeltaTime;
    }

    // === BlackBoard反映 ===
    mpBlackBoard->SetValue("BossRotate", bossRotate);
    mpBlackBoard->SetValue("BossPos", bossPos);
    mpBlackBoard->SetValue("BossVelocity", bossVelocity);
}

void ApproachLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("接近");
	ImGui::End();
#endif // _DEBUG
}

NodeResult ApproachLeaf::GetNodeResult() const {
    // === BlackBoard取得 ===
    Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
    Vector3 playerPos = mpBlackBoard->GetValue<Vector3>("PlayerPos");

    //距離を求めて350~450ならrunning、それ以外ならsuccess
    float distance = Vector3(bossPos - playerPos).Length();
    if (distance > 350.0f && distance <= 450.0f) {
        return NodeResult::Running;
    }

	//成功を返す
	return NodeResult::Success;
}
