#include "Player.h"
#include "application/map/Map.h"

void Player::Initialize(const Map& map)
{
    dog_ = std::make_unique<Dog>();
    dog_->Initialize();

    monkey_ = std::make_unique<Monkey>();
    monkey_->Initialize();

    if (auto p = map.GetDogSpawnWorld()) { dog_->SetWorldPosition(*p); }
    if (auto p = map.GetMonkeySpawnWorld()) { monkey_->SetWorldPosition(*p); }
}

void Player::Update()
{
    dog_->Update();
    monkey_->Update();
}

void Player::Move()
{
}
