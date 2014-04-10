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

// ======================== Player Constants ======================== 
enum PlayerAction { IDLE, KICK, ATTACK, BLOCK, STOP_BLOCK, JUMP, DIE };
enum PlayerHp { HP_PLUTO = 10000, HP_NINJA = 20 };
enum PlayerHeight { HEIGHT_NINJA = 90 };
enum PlayerStep { STEP_NINJA = 150 };
enum PlayerAttack { ATTACK_BLADE = 50, ATTACK_KICK = 10 }; 

typedef struct PlayerState {
	enum PlayerAction action;
	enum PlayerStep step;
	enum PlayerHp defaultHP;
	int hp;	
	Ogre::Real degreeYaw;
	bool movingLeft;
	bool movingRight;
	bool movingForward;
	bool movingBackward;
} PlayerState;

// ======================== ScenePlanet Constants ======================== 
enum ScenePlanetSize { LENGTH_ScenePlanet = 1000, WIDTH_ScenePlanet = 1000 };

// ======================== SceneSpace Constants ======================== 
enum SceneSpaceSize { LENGTH_SceneSpace = 1000, WIDTH_SceneSpace = 1000, HEIGHT_SceneSpace = 1000 };

// ============================ Game Constants ==========================
enum CameraMode { CAM_THIRD_PERSON, CAM_FIRST_PERSON };
enum GameScene { SCENE_NONE, SCENE_SPACE, SCENE_PLANET };

enum KeyboardEvent {
	GO_LEFT, GO_RIGHT, GO_FORWARD, GO_BACKWARD,		// player movement
	STOP_LEFT, STOP_RIGHT, STOP_FORWARD, STOP_BACKWARD,	// player movement
	RUN, STOP_RUN, USE_WEAK_HIT, USE_NORMAL_HIT, USE_STRONG_HIT,	// change player state
	TOGGLE_CAMERA, PAUSE, RESTART, TOGGLE_GAME_MODE		// overall game params
};
enum MouseEvent { HIT_START, HIT_STOP };

// =================== Global Variables and Functions ===================
static Sound* soundHandler;

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
