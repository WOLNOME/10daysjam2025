#pragma once
#include "Collider.h"
#include <Object3d.h>
#include <MyMath.h>
#include <string>
#include <memory>

class LevelObject : public Collider {
public:
	//コンストラクタ
	LevelObject() = default;
	//デストラクタ
	~LevelObject() override = default;
	//初期化
	void Initialize(const std::string& _name, const std::string& _filePath, const TransformEuler& _transform);
	//更新
	void Update();
	//デバッグ用パラメーター調整
	void DebugWithImGui();

	//当たり判定時の処理
	void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) override;

public://getter
	//名前の取得
	const std::string& GetName() const { return name_; }
	//ワールドトランスフォームの取得
	const WorldTransform& GetWorldTransform() const { return object3d_->worldTransform; }
	//コライダー用のワールド座標を取得
	Vector3 GetWorldPosition() override { return object3d_->worldTransform.worldTranslate; }
	//コライダー用回転を取得
	Vector3 GetRotate() override { return object3d_->worldTransform.rotate; }
	//コライダー用スケールを取得
	Vector3 GetScale() override { return object3d_->worldTransform.scale; }
	//コライダー用前フレーム座標を取得
	Vector3 GetPreWorldPosition() override { return prePosition_; }

public://setter
	//コリジョン情報のセット
	void SetCollisionInfo(const Vector3& _center, const Vector3& _size);

private://非公開メンバ関数
	//ツリーオブジェクトの転倒アクション
	void TreeInvertProcess();

protected://オブジェクト
	//テクスチャハンドル
	int32_t textureHandle_ = EOF;
	//オブジェクト3D
	std::unique_ptr<Object3d> object3d_ = nullptr;
	//前フレームの座標
	Vector3 prePosition_ = {};

private:
	//名前
	std::string name_;

	//コリジョンが有効か
	bool isCollisionEnabled_ = false;


	//ツリーオブジェクト限定変数
	bool isAction_ = false;		// 倒れるアクションをするかどうか
	Vector3 invertDirection_ = { 0.0f,0.0f,0.0f };	// 倒れる方向
	const float time_ = 1.0f;
	float timer_ = 0.0f;	// タイマー

};

