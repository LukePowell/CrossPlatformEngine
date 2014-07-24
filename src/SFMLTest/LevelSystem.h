#pragma once
#include <string>
#include <vector>
#include "ISystem.h"
#include "XMLReader.h"
#include "Maths/Mathlib.h"
#include <thread>

namespace ElysiumEngine
{
    struct Instance
    {
        Instance();
        
        std::string basefile;
        Vec4 position;
        Vec4 rotation;
        Vec4 scale;
        
        bool positionOverloaded;
        bool rotationOverloaded;
        bool scaleOverloaded;
    };
    
	class Level
	{
	public:
		Level();
        virtual ~Level(){}
        
        void update(float dt);
        bool load();
        bool reload();
        bool deserialize(FileIO::XMLReader &reader);
        
        std::string getType();
    private:
        std::vector<Instance> instances;
	};

	class LevelSystem : public ISystem
	{
	public:
		LevelSystem();

        int getCurrentLevelIndex(){return currentIndex;}
        
		int getLevelIndex(std::string levelName);
        int getLevelCount(){return levelFiles.size();}
		bool loadLevels(std::string filename);
		bool loadLevel(int levelIndex);
		bool reloadCurrentLevel();
        
        static LevelSystem *g_LevelSystem;
        
        virtual void initialize(){};
		virtual void update(float dt){}
        
	private:
        int currentIndex;
		std::vector<Level *> levelFiles;
	};

}