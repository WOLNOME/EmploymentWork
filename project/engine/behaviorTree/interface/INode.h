#pragma once

namespace Norm {

	enum class NodeResult {
		Idle,		//待機中
		Running,	//実行中
		Success,	//成功
		Fail		//失敗
	};

	/// <summary>
	/// インターフェース
	/// </summary>
	class INode {
	public:
		//仮想デストラクタ
		virtual ~INode() = default;
		//初期化
		virtual void Initialize() = 0;
		//更新
		virtual void Update() = 0;
		//終了時
		virtual void Finalize() = 0;
		//デバッグ
		virtual void Debug() = 0;
		//ノードの状態を取得
		virtual NodeResult GetNodeResult() const = 0;
		//ノードIDのセット
		virtual void SetNodeID(const int id) = 0;
		//実行中のノードIDの取得
		virtual int GetRunningNodeID() const = 0;
	};

}