
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
    Ogre::TerrainGroup* mTerrainGroup;
    TerrainMaterial* terrainMaterial;
    bool mTerrainsImported;

    void defineTerrain(long x, long y);
    void configureTerrainDefaults(Ogre::Light* light);
    std::string land;
    Ogre::SceneManager* mySceneMgr;
    //Terrain stuff end

public:
	SceneTerrain(char* t,  Ogre::SceneManager* mSceneMgr);
	virtual ~SceneTerrain(void);
	Ogre::Real getHeightAtWorldPosition(const Ogre::Vector3& pos) {
		return mTerrainGroup->getHeightAtWorldPosition(pos) - 490;
	}
};

#endif // #ifndef __SceneTerrain_h_
