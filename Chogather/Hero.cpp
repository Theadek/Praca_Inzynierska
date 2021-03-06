#include "Hero.h"

bool Hero::isLoaded = false;
std::vector<Model*> Hero::modelsForJumpAnimation;
std::vector<Model*> Hero::modelsForWalkAnimation;
Hero::Hero(glm::vec2 position) {
    if (!isLoaded) {
        for (int i = 1; i <= JUMP_MODELS; i++) {
            string jumpModelName = (i < 10) ? "Models/Jump/jump_00000" + to_string(i) + ".obj" : "Models/Jump/jump_0000" + to_string(i) + ".obj";
            Model* jump = new Model(jumpModelName);
            modelsForJumpAnimation.push_back(jump);
        }

        for (int i = 1; i <= WALK_MODELS; i++) {
            string walkModelName = (i < 10) ? "Models/Walk/walk_00000" + to_string(i) + ".obj" : "Models/Walk/walk_0000" + to_string(i) + ".obj";
            Model* walk = new Model(walkModelName);
            modelsForWalkAnimation.push_back(walk);
        }
        isLoaded = true;
    }
    AnimatedGraphicsObject* graphicsObject = new AnimatedGraphicsObject(glm::vec3(position, positionOnZ), glm::vec3(0.15f, 0.15f, 0.15f), 90.0f, modelsForWalkAnimation, modelsForJumpAnimation);
    PhysicsObject* physicsObject = new PhysicsObject(graphicsObject, 1.0f);
    this->object = new Object(graphicsObject, physicsObject, HERO);
    this->object->physicsObject->pRigidBody->setLinearFactor(btVector3(1.0f, 1.0f, 1.0f));
    this->object->physicsObject->pRigidBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
    this->object->physicsObject->pRigidBody->setFriction(1.0f);
    this->object->physicsObject->pRigidBody->setRestitution(0.0f);
    this->lastPos = glm::vec3(position, positionOnZ);
    this->isLeftWallJumpAvailable = true;
    this->isRightWallJumpAvailable = true;
    this->speed = SPEED;
    this->jump_height = JUMP_HEIGHT;
    this->startPosition = position;
    state = STAYING;
}

void Hero::move(Movement playerChoice) {
    float currentVelocityX = object->physicsObject->pRigidBody->getLinearVelocity().getX();
    float currentVelocityY = object->physicsObject->pRigidBody->getLinearVelocity().getY();
    switch (playerChoice) {
    case LEFT_MOVE:
        isFacingRight = false;
        if(currentVelocityX - speed < -speed)
            object->physicsObject->pRigidBody->setLinearVelocity(btVector3(-speed, currentVelocityY, 0.0f));
        else
            object->physicsObject->pRigidBody->setLinearVelocity(btVector3(currentVelocityX - speed, currentVelocityY, 0.0f));
        ((AnimatedGraphicsObject*)object->graphicsObject)->isWalking = true;
        ((AnimatedGraphicsObject*)object->graphicsObject)->isJumping = false;
        state = WALKING;
        break;
    case RIGHT_MOVE:
        isFacingRight = true;
        if (currentVelocityX + speed > speed)
            object->physicsObject->pRigidBody->setLinearVelocity(btVector3(speed, currentVelocityY, 0.0f));
        else
            object->physicsObject->pRigidBody->setLinearVelocity(btVector3(currentVelocityX + speed, currentVelocityY, 0.0f));
        ((AnimatedGraphicsObject*)object->graphicsObject)->isWalking = true;
        ((AnimatedGraphicsObject*)object->graphicsObject)->isJumping = false;
        state = WALKING;
        break;
    case JUMP:
        object->physicsObject->pRigidBody->setLinearVelocity(btVector3(currentVelocityX, jump_height, 0.0f));
        ((AnimatedGraphicsObject*)object->graphicsObject)->isJumping = true;
        ((AnimatedGraphicsObject*)object->graphicsObject)->isWalking = false;
        state = JUMPING;
        break;
    case INTERACTION:
        ((AnimatedGraphicsObject*)object->graphicsObject)->isJumping = false;
        ((AnimatedGraphicsObject*)object->graphicsObject)->isWalking = false;
        state = ACTION;
        break;
    }
}

void Hero::update(bool isOnTheGround) {
    //update graphicsObject position based on physicsObject
    btTransform trans = object->physicsObject->pRigidBody->getWorldTransform();
    object->graphicsObject->position.x = trans.getOrigin().x();
    object->graphicsObject->position.y = trans.getOrigin().y();
    object->physicsObject->pRigidBody->setActivationState(ACTIVE_TAG);

    ((AnimatedGraphicsObject*)object->graphicsObject)->rotate = isFacingRight ? 90.0f : -90.0f;
    ((AnimatedGraphicsObject*)object->graphicsObject)->isJumping = !isOnTheGround;
    ((AnimatedGraphicsObject*)object->graphicsObject)->isWalking = (lastPos == object->graphicsObject->position) ? false : true;
    lastPos = object->graphicsObject->position;
}

void Hero::resetPosition() {
    btTransform initialTransform;
    btVector3 positionbt = btVector3(this->startPosition.x, this->startPosition.y, positionOnZ);
    initialTransform.setOrigin(positionbt);
    initialTransform.setRotation(this->object->physicsObject->btRotation);
    this->object->physicsObject->pRigidBody->clearForces();
    this->object->physicsObject->pRigidBody->setWorldTransform(initialTransform);
    this->object->physicsObject->pMotionState->setWorldTransform(initialTransform);
    this->diamondPos = glm::vec2(-100.0f, -100.0f);
}
