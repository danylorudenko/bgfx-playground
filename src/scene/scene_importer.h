#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <assimp/mesh.h>
#include <assimp/scene.h>

#include <class_features/class_features.hpp>

#include <gfx/gfx_objects.h>
#include <gfx/gfx_vertex.h>
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

    void ImportScene(std::string file, Scene& importedScene);
    
private:
    struct MaterialInternal
    {
        std::string         m_Name;
        gfx::SharedTexture  m_DiffuseTexture;
    };

    struct MeshInternal
    {
        std::string                 m_Name;
        std::uint32_t               m_MaterialIndex;
        std::vector<gfx::Vertex>    m_Vertices;
        std::vector<std::uint32_t>  m_Indicies;
        std::vector<MeshInternal>   m_SubMeshes;

    };

    // returns number of imported meshes
    std::uint32_t ParseNodeToMeshInternalRecursive(aiNode* node, MeshInternal& nodeMesh);

    void ParseMeshVertexData(aiMesh* mesh, MeshInternal& result);
    void ParseMeshMaterials(aiMesh* mesh, MeshInternal& result);

    void ParseMeshInternalHierarchyToScene(Scene& scene);
    void MeshInternalToEntitiesRecursive(MeshInternal& meshInternal, Entity& entity);

private:

    std::string                                 m_ImportedScenePath;
    aiScene const*                              m_ImportedScene;
    MeshInternal                                m_RootMesh;
    std::map<std::uint32_t, MeshInternal>       m_ParsedMeshes;
    std::map<std::uint32_t, MaterialInternal>   m_ParsedMaterials;
    std::map<std::string, gfx::SharedTexture>   m_ParsedTextures;
};


} // namespace pg

