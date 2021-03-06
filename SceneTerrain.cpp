#include "SceneTerrain.h"


//-------------------------------------------------------------------------------------
SceneTerrain::SceneTerrain(char* t, Ogre::String meterialName, Ogre::SceneManager* mSceneMgr)
{
	land = std::string(t);
	mySceneMgr = mSceneMgr;
	//Terrain stuff
	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);
	Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
	lightdir.normalise();
	Ogre::Light* light = mSceneMgr->createLight();
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
    
    mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();

    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, 513, 12000.0f);
    mTerrainGroup->setFilenameConvention(Ogre::String("PlutoPissedTerrain"), Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

    // Init custom materialgenerator
    Ogre::TerrainMaterialGeneratorPtr terrainMaterialGenerator;

    // Set Ogre Material  with the name "TerrainMaterial" in constructor
    terrainMaterial = OGRE_NEW TerrainMaterial(meterialName);         
    terrainMaterialGenerator.bind( terrainMaterial );  
               
    mTerrainGlobals->setDefaultMaterialGenerator( terrainMaterialGenerator );

    configureTerrainDefaults(light);


    Ogre::Image img;
    img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    mTerrainGroup->defineTerrain(0, 0, &img);

 /*
    for (long x = 0; x <= 0; ++x)
        for (long y = 0; y <= 0; ++y)
            defineTerrain(x, y);
*/ 
    // sync load since we want everything in place when we start
    mTerrainGroup->loadAllTerrains(true);

    mTerrainGroup->freeTemporaryResources();

	// may need to put these...somewhere after Terrain is generated
	//terrainMaterial->setMaterialByName("Examples/TerrainTest");
	//terrainMaterial->generate(mTerrainGroup->getTerrain(0,0));

 	//Terrain stuff end
}
//-------------------------------------------------------------------------------------
SceneTerrain::~SceneTerrain(void)
{
	OGRE_DELETE mTerrainGroup;
	OGRE_DELETE mTerrainGlobals;
}

//-------------------------------------------------------------------------------------

void SceneTerrain::setMaterial(Ogre::String meterialName, int scale) {
	terrainMaterial->setMaterialByName(meterialName);
	
    	Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
	defaultimp.inputScale = scale; // due terrain.png is 8 bpp
	Ogre::Image img;
	img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	mTerrainGroup->defineTerrain(0, 0, &img);
	mTerrainGroup->loadAllTerrains(true);
}
//-------------------------------------------------------------------------------------

void SceneTerrain::configureTerrainDefaults(Ogre::Light* light)
{
    mTerrainGlobals->setMaxPixelError(8);
    mTerrainGlobals->setCompositeMapDistance(3000);
    mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
    mTerrainGlobals->setCompositeMapAmbient(mySceneMgr->getAmbientLight());
    mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());

    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = 513;
    defaultimp.worldSize = 12000.0f;
    defaultimp.inputScale = 600; // due terrain.png is 8 bpp
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;

/*
    // textures
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 100;
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 30;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
*/
}

//-------------------------------------------------------------------------------------

