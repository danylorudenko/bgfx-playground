#pragma once

#include <string>

#include <assimp/mesh.h>
#include <class_features/class_features.hpp>

namespace pg
{

class Scene;

class SceneImporter 
    : public pg::class_features::NonCopyable
{
public:
    SceneImporter();

    SceneImporter(SceneImporter&& rhs);
    SceneImporter& operator=(SceneImporter&& rhs);

    ~SceneImporter();

    void ImportScene(std::string file, Scene* importedScene);
    
private:
    void ParseMesh(aiMesh* mesh);
};


} // namespace pg

