#include "Player.h"

void Player::Initialize()
{
    dog_ = std::make_unique<Dog>();
    dog_->Initialize();

    monkey_ = std::make_unique<Monkey>();
    monkey_->Initialize();
}

void Player::Update()
{
    dog_->Update();
    monkey_->Update();
}

void Player::Move()
{
}
