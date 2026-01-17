#include "BossBehaivorTreeBuilder.h"
#include "JsonUtil.h"
#include <unordered_map>
#include <cassert>

//ノードの各種ヘッダーファイル
#include "compositeNode/Selector.h"
#include "compositeNode/Sequence.h"
#include "decoratorNode/Inverter.h"

std::unique_ptr<INode> BossBehaivorTreeBuilder::BuildBehaviorTree(BlackBoard* _blackBoard) {
	//JSONファイルを読み込む
	json jsonData;
	jsonData = JsonUtil::GetJsonData("Resources/behaviorTree/boss_data");
	if (!jsonData) {
		assert(false && "JSONデータの読み込みに失敗しました");
	}

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
			std::unique_ptr<Sequence> seq = std::make_unique<Sequence>(_blackBoard);
			for (auto childId : nodeJson["children"]) {
				seq->AddNode(buildNode(childId.get<int>()));
			}
			node = std::move(seq);
		}
		else if (name == "Selector") {
			std::unique_ptr<Selector> selector = std::make_unique<Selector>(_blackBoard);
			for (auto childId : nodeJson["children"]) {
				selector->AddNode(buildNode(childId.get<int>()));
			}
			node = std::move(selector);
		}

		// --- Decoratorノード ---
		else if (name == "Inverter") {
			int childId = nodeJson["children"][0].get<int>();
			node = std::make_unique<Inverter>(_blackBoard, buildNode(childId));
		}

		// --- Branchノード ---
		//else if (name == "CheckNearPlayer") {
		//	int trueChildId = nodeJson["true_child"].get<int>();
		//	int falseChildId = nodeJson["false_child"].get<int>();
		//	float limitDistance = nodeJson["limit_distance"].get<float>();
		//	node = new CheckNearPlayer(_blackBoard, buildNode(trueChildId), buildNode(falseChildId), limitDistance);
		//}


		// --- Leafノード ---
		//else if (name == "WaitLeaf") {
		//	float waitTime = nodeJson["wait_time"].get<float>();
		//	node = new WaitLeaf(_blackBoard, waitTime);
		//}


		// --- 未知のノード ---
		else {
			assert(false && "未知のノードタイプです");
		}

		// すべてのノードにIDを設定
		if (node) {
			node->SetNodeID(nodeId);
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
