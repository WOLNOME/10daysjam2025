#pragma once
#include "application/character/CharacterBase.h"
#include "application/character/Dog.h"
#include "application/character/Monkey.h"
#include "application/map/MapTypes.h"

// 前方宣言
class Map;

class Player
{
public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(const Map& map);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(const Map& map);

	/// <summary>
	/// 猿、犬の移動挙動
	/// </summary>
	void Move(const Map& map);

private:



private:

	// 現在プレイヤーが「操作している」キャラ種別
	enum class Active { Dog, Monkey };
	// 初期操作対象は犬。キー入力などで Monkey に切り替える
	Active active_ = Active::Dog;

	// キャラの生成
	std::unique_ptr<Dog>    dog_;
	std::unique_ptr<Monkey> monkey_;


	GridPos dogGrid_{ 0,0 };
	GridPos monkeyGrid_{ 0,0 };

	// 補助
	void TryStep(Active who, int dx, int dy, const Map& map);
	void SnapToWorld(Active who, const Map& map);

};

