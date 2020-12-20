#include "Object.h"

Object::Object() {

}

Object::Object(GraphicsObject* graphicsObject, PhysicsObject* physicsObject, TAG tag) {
    this->graphicsObject = graphicsObject;
    this->physicsObject = physicsObject;
    this->tag = tag;
}

void Object::move(int direction) {
    if (graphicsObject->debug) {
        btTransform trans = physicsObject->pRigidBody->getWorldTransform();
        switch (direction) {
        case 1:
            trans.setOrigin(btVector3(trans.getOrigin().x(), trans.getOrigin().y() + 0.1f, trans.getOrigin().z()));
            break;
        case 2:
            trans.setOrigin(btVector3(trans.getOrigin().x(), trans.getOrigin().y() - 0.1f, trans.getOrigin().z()));
            break;
        case 3:
            trans.setOrigin(btVector3(trans.getOrigin().x() + 0.1f, trans.getOrigin().y(), trans.getOrigin().z()));
            break;
        case 4:
            trans.setOrigin(btVector3(trans.getOrigin().x() - 0.1f, trans.getOrigin().y(), trans.getOrigin().z()));
            break;
        default:
            break;
        }
        physicsObject->pRigidBody->setWorldTransform(trans);
        graphicsObject->position.x = trans.getOrigin().x();
        graphicsObject->position.y = trans.getOrigin().y();
        physicsObject->pRigidBody->setActivationState(ACTIVE_TAG);
    }
}