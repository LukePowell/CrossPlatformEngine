#include "LevelSystem.h"
#include "GameObjectFactory.h"
#include "Transform.h"
#include <algorithm>

ElysiumEngine::LevelSystem *ElysiumEngine::LevelSystem::g_LevelSystem = nullptr;

ElysiumEngine::Instance::Instance()
{
    positionOverloaded = false;
    rotationOverloaded = false;
    scaleOverloaded = false;
}

ElysiumEngine::Level::Level()
{
    
}

ElysiumEngine::LevelSystem::LevelSystem() : ISystem("LevelSytem")
{
    g_LevelSystem = this;
}

int ElysiumEngine::LevelSystem::getLevelIndex(std::string levelName)
{
    for(int i = 0; i < levelFiles.size(); ++i)
    {
        //if(levelFiles[i] == levelName)
        //{
            return i;
        //}
    }
    return -1;
}


void ElysiumEngine::Level::update(float dt)
{
    
}

bool ElysiumEngine::Level::load()
{
    for(auto &instance : instances)
    {
        GameObject *obj = GameObjectFactory::g_GameObjectFactory->createGameObjectFromFile(instance.basefile.c_str());
        
        if(!obj->hasComponent("Transform"))
            continue;
        else
        {
            Transform *trans = obj->getComponent<Transform>("Transform");
            
            if(instance.positionOverloaded)
            {
                trans->SetPosition(instance.position);
            }
            if(instance.rotationOverloaded)
            {
            }
            else if(instance.scaleOverloaded)
            {
                trans->SetScale(instance.scale);
            }
        }
    }
    
    return true;
}

bool ElysiumEngine::Level::reload()
{
    return false;
}

bool ElysiumEngine::Level::deserialize(FileIO::XMLReader &reader)
{
    reader.GetFirstChild();
    while(reader.ReadNextChild())
    {
        if(reader.GetTag() == "object")
        {
            std::string file;
            reader.GetStringValueAtr("name", &file);
            
            reader.GetFirstChild();
            while(reader.ReadNextChild())
            {
                if(reader.GetTag() == "instance")
                {
                    Instance newInstance;
                    newInstance.basefile = file;
                    reader.GetFirstChild();
                    while(reader.ReadNextChild())
                    {
                        if(reader.GetTag() == "position")
                        {
                            newInstance.positionOverloaded = true;
                            reader.GetFloatValueAtr("x", &newInstance.position.x);
                            reader.GetFloatValueAtr("y", &newInstance.position.y);
                            reader.GetFloatValueAtr("z", &newInstance.position.z);
                        }
                        else if(reader.GetTag() == "rotation")
                        {
                            newInstance.rotationOverloaded = true;
                            reader.GetFloatValueAtr("x", &newInstance.rotation.x);
                            reader.GetFloatValueAtr("y", &newInstance.rotation.y);
                            reader.GetFloatValueAtr("z", &newInstance.rotation.z);
                        }
                        else if(reader.GetTag() == "scale")
                        {
                            newInstance.scaleOverloaded = true;
                            reader.GetFloatValueAtr("x", &newInstance.scale.x);
                            reader.GetFloatValueAtr("y", &newInstance.scale.y);
                            reader.GetFloatValueAtr("z", &newInstance.scale.z);
                        }
                    }
                    instances.push_back(newInstance);
                    reader.GetParent();
                }
                
            }
            reader.GetParent();
        }
    }
    reader.GetParent();
    return true;
}

void loadLevelsThread()
{

}

bool ElysiumEngine::LevelSystem::loadLevels(std::string filename)
{
    FileIO::XMLReader reader;

    if(!reader.Open(filename.c_str()))
    {
        return false;
    }
    
    reader.GetFirstChild();
    
    bool fail = false;
    
    auto thread = [&reader,&fail] () {
		while(reader.ReadNextChild())
		{
		   if(reader.GetTag() == "level")
		   {
			   Level *temp = new Level();
			   FileIO::XMLReader levelReader;
			   std::string file = reader.GetText();
			   std::cout << "Loading level details for: " << file << std::endl;
			   reader.GetIntValueAtr("index", &temp->index);
			   if(!levelReader.Open(reader.GetText().c_str()))
			   {
				   fail = true;
				   continue;
			   }
           
			   temp->deserialize(levelReader);
           
			   g_LevelSystem->levelFiles.push_back(temp);
			}
		}
		reader.GetParent();
    };
    
    thread();

	auto sortFunc = [](ElysiumEngine::Level *lhs, ElysiumEngine::Level *rhs)->bool{return lhs->index < rhs->index; };
    
	std::sort(levelFiles.begin(), levelFiles.end(), sortFunc);
    
    return fail;
}

bool ElysiumEngine::LevelSystem::loadLevel(int levelIndex)
{
    GameObjectFactory::g_GameObjectFactory->destroyAll();//Clear out the objects left from the previous frame
    currentIndex = levelIndex;
    return levelFiles[levelIndex]->load();
}

bool ElysiumEngine::LevelSystem::reloadCurrentLevel()
{
    return false;
}