#ifndef __GameResource_h_
#define __GameResource_h_

#include <iostream>
#include <Ogre.h>
#include "Sound.h"
#include "PhysicsEngine.h"

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <CEGUI.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>

// ======================== Player Constants ======================== 
enum PlayerAction { IDLE, KICK, KICKED, ATTACK, BLOCK, STOP_BLOCK, THROW_SHURIKEN, JUMP, DIE };
enum PlayerHp { HP_PLUTO = 100000, HP_NINJA = 100 };
enum PlayerHeight { HEIGHT_PLUTO = 90, HEIGHT_NINJA = 80 };
enum PlayerStep { STEP_NINJA = 150, STEP_NINJA_RUN = 250 };
enum PlayerWeapon { WEAPON_BLADE, WEAPON_SHURIKEN }; 
enum PlayerAttack { ATTACK_BLADE = 50, ATTACK_KICK = 10, ATTACK_SHURIKEN = 7 }; 

typedef struct PlayerState {
	enum PlayerAction action;
	enum PlayerStep step;
	enum PlayerHp defaultHP;
	enum PlayerWeapon weapon;
	int hp;	
	int numShuriken;
	Ogre::Real degreeYaw;
	bool movingLeft;
	bool movingRight;
	bool movingForward;
	bool movingBackward;
} PlayerState;

// ======================== Shuriken Constants ======================== 
enum ShurikenSize { SIZE_REGULAR_SHURIKEN = 5 };
enum ShurikenCollisionEvent { HIT_NOTHING, HIT_NINJA, HIT_WORLD };
struct ShurikenContactResultCallback : public btCollisionWorld::ContactResultCallback {
    ShurikenContactResultCallback(void) : hit(false){}
    virtual btScalar addSingleResult(btManifoldPoint& cp,
	const btCollisionObject* colObj0, int partId0, int index0,
	const btCollisionObject* colObj1, int partId1, int index)
    {
	hit = true;
    }
    bool hit;
};

// ======================== ScenePlanet Constants ======================== 
enum ScenePlanetSize { LENGTH_ScenePlanet = 1000, WIDTH_ScenePlanet = 1000 };
enum ScenePlanetResult { PLUTO_WIN, PLUTO_LOSE, PLUTO_FIGHTING };

// ======================== SceneSpace Constants ======================== 
enum SceneSpaceSize { LENGTH_SceneSpace = 1000, WIDTH_SceneSpace = 1000, HEIGHT_SceneSpace = 1000 };

// ============================ Game Constants ==========================
enum CameraMode { CAM_THIRD_PERSON, CAM_FIRST_PERSON };
enum GameScene { SCENE_NONE, SCENE_SPACE, SCENE_PLANET };


// =================== Global Variables and Functions ===================
static Sound* soundHandler;
static CEGUI::OgreRenderer* mRenderer;

static CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;

    case OIS::MB_Right:
        return CEGUI::RightButton;

    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }
}

static void destroySceneNodeHelper(Ogre::SceneNode* node) {
	if(!node) return;
	Ogre::SceneNode::ObjectIterator itObject = node->getAttachedObjectIterator();
	while ( itObject.hasMoreElements() )
		node->getCreator()->destroyMovableObject(itObject.getNext());
	Ogre::SceneNode::ChildNodeIterator itChild = node->getChildIterator();
	while ( itChild.hasMoreElements() ) {
		Ogre::SceneNode* childNode = static_cast<Ogre::SceneNode*>(itChild.getNext());
		destroySceneNodeHelper( childNode );
	}
}
#endif // #ifndef __GameResource_h_
