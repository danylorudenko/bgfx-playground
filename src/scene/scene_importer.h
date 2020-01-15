#pragma once

#include <string>
#include <map>

#include <assimp/mesh.h>
#include <assimp/scene.h>
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
    struct MeshInternal
    {
        int kek;
    };

    void ParseNode(aiNode* node);
    MeshInternal ParseMesh(aiMesh* mesh);

private:

    aiScene const*                  m_ImportedScene;
    std::map<aiMesh*, MeshInternal> m_ParsedMeshes;
};


} // namespace pg

