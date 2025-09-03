#pragma once
#include <Object3d.h>
#include <Object3dManager.h>
#include <TextureManager.h>
#include <Collider.h>

class CharacterBase
{
public:
	virtual ~CharacterBase() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

private:

	/// <summary>
	/// 猿、犬の移動挙動
	/// </summary>
	void Move();

protected:

	//3Dオブジェクト
	std::unique_ptr<Object3d> character_ = nullptr;


private:


};

