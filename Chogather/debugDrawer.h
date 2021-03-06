#pragma once
#include "Camera.h"
#include "Shader.h"
#include <LinearMath/btIDebugDraw.h>
class debugDrawer : public btIDebugDraw {

public:
    int m_debugMode;
    float SCR_WIDTH, SCR_HEIGHT;
    GLuint lineVAO;
    Shader* shader;
    Camera* camera;
    debugDrawer(Camera* camera, float SCR_WIDTH, float SCR_HEIGHT);
    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
    virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
    virtual void reportErrorWarning(const char* warningString);
    virtual void draw3dText(const btVector3& location, const char* textString);
    virtual void setDebugMode(int debugMode);
    virtual int getDebugMode() const;

};