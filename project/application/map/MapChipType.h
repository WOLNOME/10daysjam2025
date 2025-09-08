#pragma once

enum class MapChipType {
	Empty = 0,
	StartDog = 1,	// 犬のスタート
	StartMonkey = 2, // 猿のスタート
	FloorDog = 3,		// 犬の足場
	FloorMonkey = 4,	// 猿の足場 / 犬が動かせない
	BlockMonkey = 5,	// 猿の足場 / 犬が動かせる
	GoalDog = 6,		// 犬のゴール
	GoalMonkey = 7,		// 猿のゴール
	SwitchOff = 8, 		// スイッチ（Off状態）
	SwitchOn = 9,		// スイッチ（On状態）
	BootBlockOff = 10, // ブーツブロック（Off状態）
	BootBlockOn = 11,  // ブーツブロック（On状態）

	kMaxNumMapChipType,
};


inline std::string ToFile(MapChipType t) {
	switch (t) {
	/*case MapChipType::Empty:        return "Empty";
	case MapChipType::StartDog:     return "startDog";
	case MapChipType::StartMonkey:  return "startMonkey";*/
	case MapChipType::FloorDog:     return "floorDog";
	case MapChipType::FloorMonkey:  return "blockMonkey";
	case MapChipType::BlockMonkey:  return "floorMonkey";
	case MapChipType::GoalDog:      return "goalDog";
	case MapChipType::GoalMonkey:   return "goalMonkey";
	case MapChipType::SwitchOff:    return "switchOff";
	case MapChipType::SwitchOn:     return "switchOn";
	case MapChipType::BootBlockOff: return "bootBlockOff";
	case MapChipType::BootBlockOn:  return "bootBlockOn";
	default:                        return "Empty"; // フォールバック
	}
}