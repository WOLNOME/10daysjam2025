#pragma once

// ここは無名名前空間や独自namespaceに入れないでください（まずは大域でOK）
struct GridPos { int x; int y; };

// 列挙子名の衝突を避けるため enum class
enum class ActorKind { Dog, Monkey };
