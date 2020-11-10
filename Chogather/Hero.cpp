#include "Hero.h"

Hero::Hero(Object* object) {
    hero = object;
    speed = SPEED;
    jump_height = JUMP_HEIGHT;
    state = STAYING;
}

void Hero::Move(Movement playerChoice) {
    float currentVelocityX = hero->physicsObject->pRigidBody->getLinearVelocity().getX();
    float currentVelocityY = hero->physicsObject->pRigidBody->getLinearVelocity().getY();
    switch (playerChoice) {
    case LEFT_MOVE:
        isFacingRight = false;
        if(currentVelocityX - speed < -speed)
            hero->physicsObject->pRigidBody->setLinearVelocity(btVector3(-speed, currentVelocityY, 0.0f));
        else
            hero->physicsObject->pRigidBody->setLinearVelocity(btVector3(currentVelocityX - speed, currentVelocityY, 0.0f));
        state = WALKING;
        break;
    case RIGHT_MOVE:
        isFacingRight = true;
        if (currentVelocityX + speed > speed)
            hero->physicsObject->pRigidBody->setLinearVelocity(btVector3(speed, currentVelocityY, 0.0f));
        else
            hero->physicsObject->pRigidBody->setLinearVelocity(btVector3(currentVelocityX + speed, currentVelocityY, 0.0f));
        state = WALKING;
        break;
    case JUMP:
        hero->physicsObject->pRigidBody->setLinearVelocity(btVector3(currentVelocityX, jump_height, 0.0f));
        state = JUMPING;
        break;
    case CROUCH:
        break;
    case INTERACTION:
        break;
    }
}