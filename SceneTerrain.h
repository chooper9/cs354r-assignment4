
#ifndef __SceneTerrain_h_
#define __SceneTerrain_h_

#include "GameResource.h"
#include "Scene.h"
#include <string>
// Terrain stuff
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include "TerrainMaterial.h"
// Terrain stuff end

class SceneTerrain
{
private:
	//Terrain stuff
    Ogre::TerrainGlobalOptions* mTerrainGlobals;
    TerrainMaterial* terrainMaterial;
    bool mTerrainsImported;

    void configureTerrainDefaults(Ogre::Light* light);
    std::string land;
    Ogre::SceneManager* mySceneMgr;
    //Terrain stuff end

public:
	Ogre::TerrainGroup* mTerrainGroup;
	SceneTerrain(char* t, Ogre::String meterialName, Ogre::SceneManager* mSceneMgr);
	virtual ~SceneTerrain(void);
	void setMaterial(Ogre::String meterialName, int scale=600);
	void hideTerrain(void) {	
		mTerrainGroup->setOrigin(Ogre::Vector3(0, 100000, 0));
	}
	void showTerrain(void) {	
		mTerrainGroup->setOrigin(Ogre::Vector3(0, 0, 0));
	}
	Ogre::Real getHeightAtWorldPosition(const Ogre::Vector3& pos) {
		return mTerrainGroup->getHeightAtWorldPosition(pos);
	}
};

#endif // #ifndef __SceneTerrain_h_

