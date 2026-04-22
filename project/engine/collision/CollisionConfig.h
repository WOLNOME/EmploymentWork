#pragma once

namespace Norm {

	/// <summary>
	/// 当たり判定の全属性
	/// </summary>
	enum class CollisionAttribute {
		Player,						//プレイヤー
		Enemy,						//敵	
		PlayerSpecial,				//プレイヤー必殺弾
		PlayerCannon,				//プレイヤー砲弾
		PlayerBullet,				//プレイヤー弾
		EnemyCannon,				//敵砲弾
		EnemyBullet,				//敵弾
		EnemyBlast,					//敵爆風
		Item_Heal,					//回復アイテム
		Item_Charge,				//必殺弾チャージアイテム
		Item_Key,					//鍵アイテム
		Item_TutorialCollectible,	//チュートリアル収集アイテム
		Wall,						//壁
		Barrier,					//バリア

		Nothingness,				//虚無オブジェクト
	};


}