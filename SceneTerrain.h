
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
    bool mTerrainsImported;
    TerrainMaterial* terrainMaterial;
    Ogre::TerrainMaterialGeneratorPtr* terrainMaterialGenerator;

    void defineTerrain(long x, long y);
    void configureTerrainDefaults(Ogre::Light* light);
    std::string land;
    Ogre::SceneManager* mySceneMgr;
    //Terrain stuff end
	int iii;

public:
	SceneTerrain(char* t,  Ogre::SceneManager* mSceneMgr);
	virtual ~SceneTerrain(void);
	void generateTerrains(void) {  
		// Init custom materialgenerator
		terrainMaterialGenerator = new Ogre::TerrainMaterialGeneratorPtr();
    		// Set Ogre Material  with the name "TerrainMaterial" in constructor
    		switch(iii){
			case 0:
    			terrainMaterial = OGRE_NEW TerrainMaterial("PlutoTerrainTexture");      break;
			case 1:
    			terrainMaterial = OGRE_NEW TerrainMaterial("PlutoTerrainTexture1");      break;
			case 2:
    			terrainMaterial = OGRE_NEW TerrainMaterial("PlutoTerrainTexture2");      break;
			case 3:
    			terrainMaterial = OGRE_NEW TerrainMaterial("PlutoTerrainTexture3");      break;
			case 4:
    			terrainMaterial = OGRE_NEW TerrainMaterial("PlutoTerrainTexture4");      break;
		}
	iii++;
   		terrainMaterialGenerator->bind( terrainMaterial );  
		//terrainMaterial->setMaterialByName("PlutoTerrainTexture");
		//terrainMaterial->generate(mTerrainGroup->getTerrain(0,0));
		mTerrainGlobals->setDefaultMaterialGenerator( *terrainMaterialGenerator );
		configureTerrainDefaults(mySceneMgr->getLight("tstLight"));
 
    for (long x = 0; x <= 0; ++x)
        for (long y = 0; y <= 0; ++y)
            defineTerrain(x, y);
 
    // sync load since we want everything in place when we start
    mTerrainGroup->loadAllTerrains(true);

    mTerrainGroup->freeTemporaryResources();}
	void removeTerrains(void) { mTerrainGroup->removeAllTerrains(); OGRE_DELETE terrainMaterialGenerator;}
	Ogre::Real getHeightAtWorldPosition(const Ogre::Vector3& pos) {
		return mTerrainGroup->getHeightAtWorldPosition(pos);
	}
};

#endif // #ifndef __SceneTerrain_h_
