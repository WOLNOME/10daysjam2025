#pragma once

//列挙体で管理
enum class CollisionAttribute {
	Player,				//プレイヤー
	Enemy,				//敵	
	PlayerCannon,		//プレイヤー砲弾
	PlayerBullet,		//プレイヤー弾
	EnemyCannon,		//敵砲弾
	EnemyBullet,		//敵弾
	Item_Heal,			//回復アイテム
	Item_ReloadSpeedUp,	//リロード速度アップアイテム
	Item_MoveSpeedUp,	//移動速度アップアイテム
	Item_TurnSpeedUp,	//回転速度アップアイテム
	Wall,				//壁

	Nothingness,		//虚無オブジェクト
};


