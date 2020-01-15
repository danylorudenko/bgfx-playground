#include <scene/scene_importer.h>

#include <cstdint>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <gfx/gfx_vertex.h>

#include <scene/scene.h>

namespace pg
{

SceneImporter::SceneImporter()
    : m_ImportedScene{ nullptr }
{}

SceneImporter::SceneImporter(SceneImporter&& rhs) = default;

SceneImporter& SceneImporter::operator=(SceneImporter&& rhs) = default;

SceneImporter::~SceneImporter() = default;

void SceneImporter::ImportScene(std::string file, Scene* scene)
{
    assert(scene->GetRootEntityRef().GetChildCount() == 0 && "Scene can only parse files when being empty.");

    m_ParsedMeshes.clear();

    Assimp::Importer importer;

    aiScene const* aiscene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
    assert(scene != nullptr && "Failed to load scene");
    m_ImportedScene = aiscene;

    std::uint32_t const meshCount = static_cast<std::uint32_t>(aiscene->mNumMeshes);
    for (std::uint32_t i = 0; i < meshCount; i++)
    {
        ParseMesh(aiscene->mMeshes[i]);
    }
}

void SceneImporter::ParseNode(aiNode* node)
{
    std::uint32_t const meshCount = node->mNumMeshes;
    for (std::uint32_t i = 0; i < meshCount; i++)
    {
        aiMesh* mesh = m_ImportedScene->mMeshes[node->mMeshes[i]];
        auto parsedMesh = m_ParsedMeshes.find(mesh);
        if (parsedMesh == m_ParsedMeshes.end())
        {
            ParseMesh(mesh);
        }
    }
    //for(std::uint32_t)
}

SceneImporter::MeshInternal SceneImporter::ParseMesh(aiMesh* mesh)
{
    assert(mesh->HasFaces()             && "SceneImporter: no faces found.");
    assert(mesh->mFaces->mNumIndices == 3 && "SceneImporter: faces contain not only triangles.");
    assert(mesh->HasPositions()         && "SceneImporter: no positions found.");
    assert(mesh->HasNormals()           && "SceneImporter: no normals found.");
    assert(mesh->HasTangentsAndBitangents() && "SceneImporter: no tangets or bitangents found.");
    assert(mesh->HasTextureCoords(0)    && "SceneImporter: no texture coordinates found.");
    
    std::vector<gfx::Vertex> importedVertices;
    
    std::uint32_t const vertexCount = mesh->mNumVertices;
    importedVertices.resize(vertexCount);

    for (std::uint32_t i = 0; i < vertexCount; i++)
    {
        auto& pos = mesh->mVertices[i];
        importedVertices[i].m_Pos[0] = pos.x;
        importedVertices[i].m_Pos[1] = pos.y;
        importedVertices[i].m_Pos[2] = pos.z;
    }

    for (std::uint32_t i = 0; i < vertexCount; i++)
    {
        auto& norm = mesh->mNormals[i];
        importedVertices[i].m_Normal[0] = norm.x;
        importedVertices[i].m_Normal[1] = norm.y;
        importedVertices[i].m_Normal[2] = norm.z;
    }

    for (std::uint32_t i = 0; i < vertexCount; i++)
    {
        auto& t = mesh->mTangents[i];
        importedVertices[i].m_Tangent[0] = t.x;
        importedVertices[i].m_Tangent[1] = t.y;
        importedVertices[i].m_Tangent[2] = t.z;
    }

    for (std::uint32_t i = 0; i < vertexCount; i++)
    {
        auto& bt = mesh->mBitangents[i];
        importedVertices[i].m_Bitangent[0] = bt.x;
        importedVertices[i].m_Bitangent[1] = bt.y;
        importedVertices[i].m_Bitangent[2] = bt.z;
    }

    for (std::uint32_t i = 0; i < vertexCount; i++)
    {
        auto& uv = mesh->mTextureCoords[i][0];
        importedVertices[i].m_UV[0] = uv.x;
        importedVertices[i].m_UV[1] = uv.y;
    }


    std::vector<std::uint32_t> indicies;

    std::uint32_t const indiciesCount = mesh->mNumFaces * 3;
    indicies.resize(indiciesCount);
    for (std::uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
        assert(mesh->mFaces[i].mIndices[0] < UINT16_MAX && "Indicies are greater than UINT16_MAX");
        assert(mesh->mFaces[i].mIndices[1] < UINT16_MAX && "Indicies are greater than UINT16_MAX");
        assert(mesh->mFaces[i].mIndices[2] < UINT16_MAX && "Indicies are greater than UINT16_MAX");

        indicies[i * 3 + 0] = static_cast<std::uint32_t>(mesh->mFaces[i].mIndices[0]);
        indicies[i * 3 + 1] = static_cast<std::uint32_t>(mesh->mFaces[i].mIndices[1]);
        indicies[i * 3 + 2] = static_cast<std::uint32_t>(mesh->mFaces[i].mIndices[2]);
    }

}

} // namespace pg
