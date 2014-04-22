#ifndef TERRAINMATERIAL_H
#define TERRAINMATERIAL_H

#include "OgreTerrainMaterialGenerator.h"


    class TerrainMaterial : public Ogre::TerrainMaterialGenerator
    {
    public:
		
		TerrainMaterial(Ogre::String materialName);
		
		void setMaterialByName(const Ogre::String materialname); 
        
        class Profile : public Ogre::TerrainMaterialGenerator::Profile
        {
        public:
            Profile(Ogre::TerrainMaterialGenerator* parent, const Ogre::String& name, const Ogre::String& desc);
            ~Profile();

            bool isVertexCompressionSupported() const { return false; }

            Ogre::MaterialPtr generate(const Ogre::Terrain* terrain);

            Ogre::MaterialPtr generateForCompositeMap(const Ogre::Terrain* terrain);

            Ogre::uint8 getMaxLayers(const Ogre::Terrain* terrain) const;

            void updateParams(const Ogre::MaterialPtr& mat, const Ogre::Terrain* terrain);

            void updateParamsForCompositeMap(const Ogre::MaterialPtr& mat, const Ogre::Terrain* terrain);

            void requestOptions(Ogre::Terrain* terrain);       

        };
    protected:			
		Ogre::String mMaterialName;        
    };
    
#endif
