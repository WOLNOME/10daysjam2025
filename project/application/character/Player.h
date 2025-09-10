#pragma once
#include "application/character/CharacterBase.h"
#include "application/character/Dog.h"
#include "application/character/Monkey.h"
#include "application/map/MapTypes.h"
#include <Particle.h>

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
	void Update(Map& map);

	/// <summary>
	/// 猿、犬の移動挙動
	/// </summary>
	void Move(Map& map);

	/// <summary>
	/// リドゥ・アンドゥ処理
	/// </summary>
	void RedoUndo(Map& map);


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
	void TryStep(Active who, int dx, int dy, Map& map);
	void SnapToWorld(Active who, const Map& map);

	// 1マス移動のイージング用
	struct Tween {
		bool    active = false;
		float   t = 0.0f;         // 0→1
		float   duration = 0.15f;
		Vector3 from{};
		Vector3 to{};
	};
	Tween dogTween_;
	Tween monkeyTween_;
	float moveDuration_ = 0.25f;  // 1マスの所要時間

	// Tween開始・更新
	void BeginTween(Active who, const Vector3& from, const Vector3& to, float durationSec);
	void UpdateTweens(float dtSec);

	struct BobState {
		float time = 0.0f;  // 経過時間
		float lastOffset = 0.0f;  // 直前フレームのyオフセット
	};

	BobState dogBob_;
	BobState monkeyBob_;

	// 振幅と速度（ゆっくりめ）
	float bobAmplitude_ = 0.6f;  // 上下幅（ワールド座標）
	float bobFreqHz_ = 0.25f;   // 周波数（Hz）

	// ボビング適用
	void ApplyGoalBobbing(Map& map, float dtSec);

	void FaceTowards(Active who, int dx, int dy);

	// モデルの“前”が +Z でない場合の補正（例: +X が前なら -90° = -HalfPI）
	float modelYawOffset_ = 0.0f;  // 必要に応じて調整

	float clearWaitSec_ = 2.0f;  // 待機秒数
	float clearTimerSec_ = 0.0f;  // 経過時間
	bool  clearQueued_ = false; // 多重遷移防止フラグ

	//パーティクル
	std::unique_ptr<Particle> particle_ = nullptr;
	bool clearEffectSpawned_ = false;
};

