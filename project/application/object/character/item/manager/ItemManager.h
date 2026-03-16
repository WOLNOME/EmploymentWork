#pragma once
#include <vector>
#include <string>
#include <Vector3.h>
#include <memory>
#include <JsonUtil.h>

//アプリケーション
#include <application/object/character/item/heal/ItemHeal.h>
#include <application/object/character/item/charge/ItemCharge.h>
#include <application/object/character/item/key/ItemKey.h>
#include <application/object/character/item/tutorialCollectible/ItemTutorialCollectible.h>

/// <summary>
/// シーン上すべてのアイテム管理を行うクラス
/// </summary>
class ItemManager {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ItemManager() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ItemManager() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui();

	/// <summary>
	/// 回復アイテムのスポーン
	/// </summary>
	/// <param name="_initPos">初期化用座標</param>
	void SpawnHealItem(const Norm::Vector3& _initPos);
	/// <summary>
	/// 必殺弾チャージアイテムのスポーン
	/// </summary>
	/// <param name="_initPos">初期化用座標</param>
	void SpawnChargeItem(const Norm::Vector3& _initPos);
	/// <summary>
	/// キーアイテムのスポーン
	/// </summary>
	/// <param name="_initPos">初期化用座標</param>
	void SpawnKeyItem(const Norm::Vector3& _initPos);
	/// <summary>
	/// チュートリアル収集アイテムのスポーン
	/// </summary>
	/// <param name="_initPos">初期化用座標</param>
	void SpawnTutorialCollectibleItme(const Norm::Vector3& _initPos);

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// 回復アイテムのコンテナを取得
	/// </summary>
	/// <returns>回復アイテムのコンテナ</returns>
	const std::vector<std::unique_ptr<ItemHeal>>& GetHealItems() const { return healItems_; }
	/// <summary>
	/// 必殺弾チャージアイテムのコンテナを取得
	/// </summary>
	/// <returns>必殺弾チャージアイテムのコンテナ</returns>
	const std::vector<std::unique_ptr<ItemCharge>>& GetChargeItems() const { return chargeItems_; }
	/// <summary>
	/// キーアイテムのコンテナを取得
	/// </summary>
	/// <returns>キーアイテムのコンテナ</returns>
	const std::vector<std::unique_ptr<ItemKey>>& GetKeyItems() const { return keyItems_; }
	/// <summary>
	/// チュートリアル収集アイテムのコンテナを取得
	/// </summary>
	/// <returns>チュートリアル収集アイテムのコンテナ</returns>
	const std::vector<std::unique_ptr<ItemTutorialCollectible>>& GetTutorialCollectibleItmes() const { return tutorialCollectibleItems_; }

private:
	/// ============================== ///
	///		メンバ関数（private）
	/// ============================== ///

	/// <summary>
	/// スポーンの共通処理(Spawn処理は爆弾のみ共通ではないので、関数オブジェクトを別で渡している)
	/// </summary>
	/// <typeparam name="T">プール内に格納されているオブジェクトの型</typeparam>
	/// <typeparam name="SpawnFunc">Spawn処理を行う関数オブジェクト</typeparam>
	/// <param name="container">コンテナ</param>
	/// <param name="spawnFunc">実際のSpawn処理を定義する関数オブジェクト</param>
	template<class T, class SpawnFunc>
	void SpawnFromPool(
		std::vector<std::unique_ptr<T>>& container,
		SpawnFunc spawnFunc) {
		for (auto& obj : container) {
			if (obj->GetState() != BaseCharacter::State::kIdle)
				continue;

			spawnFunc(obj.get());
			break;
		}
	}

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//アイテムのパラメーター
	json param_;

	//回復アイテムのコンテナ
	std::vector<std::unique_ptr<ItemHeal>> healItems_;
	//必殺弾チャージアイテムのコンテナ
	std::vector<std::unique_ptr<ItemCharge>> chargeItems_;
	//キーアイテムのコンテナ
	std::vector<std::unique_ptr<ItemKey>> keyItems_;
	//チュートリアルアイテムのコンテナ
	std::vector<std::unique_ptr<ItemTutorialCollectible>> tutorialCollectibleItems_;

};

