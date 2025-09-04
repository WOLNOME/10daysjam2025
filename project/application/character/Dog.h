#pragma once
#include "application/character/CharacterBase.h"

class Dog : public CharacterBase
{
public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
};

