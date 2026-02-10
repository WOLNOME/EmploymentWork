#include "BossBehaivorTreeBuilder.h"
#include "JsonUtil.h"
#include <unordered_map>
#include <cassert>

//汎用ノードの各種ヘッダーファイル
#include "compositeNode/Selector.h"
#include "compositeNode/Sequence.h"
#include "decoratorNode/Inverter.h"
//個別ノードの各種ヘッダーファイル
#include "../node/decorator/Interruption.h"
#include "../node/branch/CheckPlayerFront.h"
#include "../node/branch/CheckPlayerDistance.h"
#include "../node/branch/CheckCannonReload.h"
#include "../node/branch/CheckBulletReload.h"
#include "../node/branch/CheckBombReload.h"
#include "../node/leaf/ApproachLeaf.h"
#include "../node/leaf/BarrierLeaf.h"
#include "../node/leaf/BombAttackLeaf.h"
#include "../node/leaf/BombReloadLeaf.h"
#include "../node/leaf/BulletAttackLeaf.h"
#include "../node/leaf/BulletReloadLeaf.h"
#include "../node/leaf/CannonAttackLeaf.h"
#include "../node/leaf/CannonReloadLeaf.h"
#include "../node/leaf/DirectionConfusionLeaf.h"
#include "../node/leaf/DirectionMissingLeaf.h"
#include "../node/leaf/DirectionSensingLeaf.h"
#include "../node/leaf/JudgeBarrierCoolTimeLeaf.h"
#include "../node/leaf/JudgeBarrierStateLeaf.h"
#include "../node/leaf/JudgeHPHalfLeaf.h"
#include "../node/leaf/JudgePlayerDistanceLeaf.h"
#include "../node/leaf/JudgePlayerPreDistanceLeaf.h"
#include "../node/leaf/JudgeSummonCoolTimeLeaf.h"
#include "../node/leaf/KeepDistanceLeaf.h"
#include "../node/leaf/PatrolLeaf.h"
#include "../node/leaf/SummonLeaf.h"

using namespace Norm;

std::unique_ptr<INode> BossBehaivorTreeBuilder::BuildBehaviorTree(BlackBoard* _blackBoard) {
	//JSONファイルを読み込む
	json jsonData;
	jsonData = JsonUtil::GetJsonData("Resources/behaviorTree/boss_data");

	//IDからJSON要素へのマッピングを構築する
	std::unordered_map<int, json> nodeMap;
	for (auto& node : jsonData) {
		int id = node["id"].get<int>();
		nodeMap[id] = node;
	}

	// 再帰的にノードを構築するラムダ関数
	std::function<std::unique_ptr<INode>(int)> buildNode = [&](int nodeId) -> std::unique_ptr<INode> {

		if (nodeMap.find(nodeId) == nodeMap.end()) {
			assert(false && "JSONデータからノードIDが見つかりませんでした");
		}
		auto nodeJson = nodeMap[nodeId];
		std::string name = nodeJson["name"].get<std::string>();

		std::unique_ptr<INode> node = nullptr;

		// --- Compositeノード ---
		if (name == "Sequence") {
			std::unique_ptr<Sequence> seq = std::make_unique<Sequence>(nodeId, _blackBoard);
			for (auto childId : nodeJson["children"]) {
				seq->AddNode(buildNode(childId.get<int>()));
			}
			node = std::move(seq);
		}
		else if (name == "Selector") {
			std::unique_ptr<Selector> selector = std::make_unique<Selector>(nodeId, _blackBoard);
			for (auto childId : nodeJson["children"]) {
				selector->AddNode(buildNode(childId.get<int>()));
			}
			node = std::move(selector);
		}

		// --- Decoratorノード ---
		else if (name == "Inverter") {
			int childId = nodeJson["children"].get<int>();
			node = std::make_unique<Inverter>(nodeId, _blackBoard, buildNode(childId));
		}
		else if (name == "Interruption") {
			int childId = nodeJson["children"].get<int>();
			node = std::make_unique<Interruption>(nodeId, _blackBoard, buildNode(childId));
		}

		// --- Branchノード ---

		else if (name == "CheckBombReload") {
			int trueChildId = nodeJson["true_child"].get<int>();
			int falseChildId = nodeJson["false_child"].get<int>();
			node = std::make_unique<CheckBombReload>(nodeId, _blackBoard, buildNode(trueChildId), buildNode(falseChildId));
		}
		else if (name == "CheckBulletReload") {
			int trueChildId = nodeJson["true_child"].get<int>();
			int falseChildId = nodeJson["false_child"].get<int>();
			node = std::make_unique<CheckBulletReload>(nodeId, _blackBoard, buildNode(trueChildId), buildNode(falseChildId));
		}
		else if (name == "CheckCannonReload") {
			int trueChildId = nodeJson["true_child"].get<int>();
			int falseChildId = nodeJson["false_child"].get<int>();
			node = std::make_unique<CheckCannonReload>(nodeId, _blackBoard, buildNode(trueChildId), buildNode(falseChildId));
		}
		else if (name == "CheckPlayerFront") {
			int trueChildId = nodeJson["true_child"].get<int>();
			int falseChildId = nodeJson["false_child"].get<int>();
			node = std::make_unique<CheckPlayerFront>(nodeId, _blackBoard, buildNode(trueChildId), buildNode(falseChildId));
		}
		else if (name == "CheckPlayerDistance") {
			int trueChildId = nodeJson["true_child"].get<int>();
			int falseChildId = nodeJson["false_child"].get<int>();
			float distance = nodeJson["distance"].get<float>();
			node = std::make_unique<CheckPlayerDistance>(nodeId, _blackBoard, buildNode(trueChildId), buildNode(falseChildId), distance);
		}

		// --- Leafノード ---

		else if (name == "ApproachLeaf") {
			node = std::make_unique<ApproachLeaf>(nodeId, _blackBoard);
		}
		else if (name == "BarrierLeaf") {
			node = std::make_unique<BarrierLeaf>(nodeId, _blackBoard);
		}
		else if (name == "BombAttackLeaf") {
			node = std::make_unique<BombAttackLeaf>(nodeId, _blackBoard);
		}
		else if (name == "BombReloadLeaf") {
			node = std::make_unique<BombReloadLeaf>(nodeId, _blackBoard);
		}
		else if (name == "BulletAttackLeaf") {
			node = std::make_unique<BulletAttackLeaf>(nodeId, _blackBoard);
		}
		else if (name == "BulletReloadLeaf") {
			node = std::make_unique<BulletReloadLeaf>(nodeId, _blackBoard);
		}
		else if (name == "CannonAttackLeaf") {
			node = std::make_unique<CannonAttackLeaf>(nodeId, _blackBoard);
		}
		else if (name == "CannonReloadLeaf") {
			node = std::make_unique<CannonReloadLeaf>(nodeId, _blackBoard);
		}
		else if (name == "DirectionConfusionLeaf") {
			node = std::make_unique<DirectionConfusionLeaf>(nodeId, _blackBoard);
		}
		else if (name == "DirectionMissingLeaf") {
			node = std::make_unique<DirectionMissingLeaf>(nodeId, _blackBoard);
		}
		else if (name == "DirectionSensingLeaf") {
			node = std::make_unique<DirectionSensingLeaf>(nodeId, _blackBoard);
		}
		else if (name == "JudgeBarrierCoolTimeLeaf") {
			node = std::make_unique<JudgeBarrierCoolTimeLeaf>(nodeId, _blackBoard);
		}
		else if (name == "JudgeBarrierStateLeaf") {
			node = std::make_unique<JudgeBarrierStateLeaf>(nodeId, _blackBoard);
		}
		else if (name == "JudgeHPHalfLeaf") {
			node = std::make_unique<JudgeHPHalfLeaf>(nodeId, _blackBoard);
		}
		else if (name == "JudgePlayerDistanceLeaf") {
			float distance = nodeJson["distance"].get<float>();
			node = std::make_unique<JudgePlayerDistanceLeaf>(nodeId, _blackBoard, distance);
		}
		else if (name == "JudgePlayerPreDistanceLeaf") {
			float preDistance = nodeJson["distance"].get<float>();
			node = std::make_unique<JudgePlayerPreDistanceLeaf>(nodeId, _blackBoard, preDistance);
		}
		else if (name == "JudgeSummonCoolTimeLeaf") {
			node = std::make_unique<JudgeSummonCoolTimeLeaf>(nodeId, _blackBoard);
		}
		else if (name == "KeepDistanceLeaf") {
			node = std::make_unique<KeepDistanceLeaf>(nodeId, _blackBoard);
		}
		else if (name == "PatrolLeaf") {
			node = std::make_unique<PatrolLeaf>(nodeId, _blackBoard);
		}
		else if (name == "SummonLeaf") {
			node = std::make_unique<SummonLeaf>(nodeId, _blackBoard);
		}

		// --- 未知のノード ---
		else {
			assert(false && "未知のノードタイプです");
		}

		return node;
		};

	// ルートノードを探す (parentが-1のノード)
	int rootId = -1;
	for (auto& [id, nodeJson] : nodeMap) {
		if (nodeJson["parent"].get<int>() == -1) {
			rootId = id;
			break;
		}
	}
	if (rootId == -1) {
		assert(false && "JSONデータからルートノードが見つかりませんでした");
	}

	return buildNode(rootId);
}
