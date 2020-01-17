#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <assimp/mesh.h>
#include <assimp/scene.h>

#include <gfx/gfx_vertex.h>
#include <class_features/class_features.hpp>
#include <scene/entity.h>

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
        std::string                 m_Name;
        std::vector<gfx::Vertex>    m_Vertices;
        std::vector<std::uint32_t>  m_Indicies;
        std::vector<MeshInternal>   m_SubMeshes;
    };

    // returns number of imported meshes
    std::uint32_t ParseNodeToMeshInternalHierarchy(aiNode* node, MeshInternal& nodeMesh);
    void ParseMeshInternalHierarchyToScene(Scene* scene);
    MeshInternal ParseMeshVertexData(aiMesh* mesh);

private:

    aiScene const*                          m_ImportedScene;
    std::map<std::uint32_t, MeshInternal>   m_ParsedMeshes;
    MeshInternal                            m_RootMesh;
};


} // namespace pg

