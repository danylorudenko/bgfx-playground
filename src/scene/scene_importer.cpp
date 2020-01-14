#include <scene/scene_importer.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <gfx/gfx_vertex.h>

#include <scene/scene.h>

namespace pg
{

SceneImporter::SceneImporter() = default;

SceneImporter::SceneImporter(SceneImporter&& rhs) = default;

SceneImporter& SceneImporter::operator=(SceneImporter&& rhs) = default;

SceneImporter::~SceneImporter() = default;

void SceneImporter::ImportScene(std::string file, Scene* scene)
{
    assert(scene->GetRootEntityRef().GetChildCount() == 0 && "Scene can only parse files when being empty.");

    Assimp::Importer importer;

    aiScene const* aiscene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
    assert(scene != nullptr && "Failed to load scene");

    std::uint32_t const meshCount = static_cast<std::uint32_t>(aiscene->mNumMeshes);
    for (std::uint32_t i = 0; i < meshCount; i++)
    {
        ParseMesh(aiscene->mMeshes[i]);
    }
}

void SceneImporter::ParseMesh(aiMesh* mesh)
{
    assert(mesh->HasFaces()             && "SceneImporter: no faces found.");
    assert(mesh->HasPositions()         && "SceneImporter: no positions found.");
    assert(mesh->HasNormals()           && "SceneImporter: no normals found.");
    assert(mesh->HasTangentsAndBitangents() && "SceneImporter: no tangets or bitangents found.");
    assert(mesh->HasTextureCoords(0)    && "SceneImporter: no texture coordinates found.");
    
    std::vector<gfx::Vertex> importedVertices;
    
    std::uint32_t const vertexCount = mesh->mNumVertices;
    importedVertices.reserve(vertexCount);

    for (std::uint32_t i = 0; i < vertexCount; i++)
    {
        
    }


}

} // namespace pg
