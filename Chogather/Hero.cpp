#include "Hero.h"

Hero::Hero(Object* object) {
    this->hero = object;
    this->speed = SPEED;
    this->jump_height = JUMP_HEIGHT;
}

void Hero::Move(Movement playerChoice) {
    float currentVelocityX = hero->physicsObject->pRigidBody->getLinearVelocity().getX();
    float currentVelocityY = hero->physicsObject->pRigidBody->getLinearVelocity().getY();
    switch (playerChoice) {
    case LEFT_MOVE:
        if(currentVelocityX - speed < -speed)
            hero->physicsObject->pRigidBody->setLinearVelocity(btVector3(-speed, currentVelocityY, 0.0f));
        else
            hero->physicsObject->pRigidBody->setLinearVelocity(btVector3(currentVelocityX - speed, currentVelocityY, 0.0f));
        break;
    case RIGHT_MOVE:
        if (currentVelocityX + speed > speed)
            hero->physicsObject->pRigidBody->setLinearVelocity(btVector3(speed, currentVelocityY, 0.0f));
        else
            hero->physicsObject->pRigidBody->setLinearVelocity(btVector3(currentVelocityX + speed, currentVelocityY, 0.0f));
        break;
    case JUMP:
        hero->physicsObject->pRigidBody->setLinearVelocity(btVector3(currentVelocityX, jump_height, 0.0f));
        break;
    case CROUCH:
        break;
    case INTERACTION:
        break;
    }
}