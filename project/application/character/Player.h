#pragma once
#include "application/character/CharacterBase.h"
#include "application/character/Dog.h"
#include "application/character/Monkey.h"

class Player
{
public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

private:

	/// <summary>
	/// 猿、犬の移動挙動
	/// </summary>
	void Move();

private:

	// 現在プレイヤーが「操作している」キャラ種別
	enum class Active { Dog, Monkey };
	// 初期操作対象は犬。キー入力などで Monkey に切り替える
	Active active_ = Active::Dog;

	// キャラの生成
	std::unique_ptr<Dog>    dog_;
	std::unique_ptr<Monkey> monkey_;


};

