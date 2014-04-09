#include "PhysicsEngine.h"
#include <iostream>

PhysicsEngine::PhysicsEngine(void) {
	broadphase = new btDbvtBroadphase();
        collisionConfig = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfig);
        solver = new btSequentialImpulseConstraintSolver;
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfig);
        dynamicsWorld->setGravity(btVector3(0,-9.8,0));
}

PhysicsEngine::~PhysicsEngine(void)
{
	delete dynamicsWorld;
        delete solver;
        delete collisionConfig;
        delete dispatcher;
        delete broadphase;
	std::cout << "========= Debug: Physics Engine Deleted =========" << std::endl;
}

void PhysicsEngine::addObject(PhysicsObject* obj) {
	dynamicsWorld->addRigidBody(obj->getRigidBody());
}

void PhysicsEngine::removeObject(PhysicsObject* obj) {
	dynamicsWorld->removeRigidBody(obj->getRigidBody());
}

void PhysicsEngine::setGravity(btScalar x, btScalar y, btScalar z) {
	dynamicsWorld->setGravity(btVector3(x,y,z));
}

void PhysicsEngine::stepSimulation(btScalar step, int maxSubSteps, btScalar fixStep) {
	dynamicsWorld->stepSimulation(step, maxSubSteps, fixStep);
}
