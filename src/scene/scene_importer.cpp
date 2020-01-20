#include <scene/scene_importer.h>

#include <cstring>
#include <utility>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <gfx/gfx_objects.h>
#include <gfx/gfx_vertex.h>

#include <scene/scene.h>

namespace pg
{

SceneImporter::SceneImporter()
    : m_ImportedScene{ nullptr }
    , m_NonameTemplateCounter{ 0 }
{
}

SceneImporter::SceneImporter(SceneImporter&& rhs) = default;

SceneImporter& SceneImporter::operator=(SceneImporter&& rhs) = default;

SceneImporter::~SceneImporter() = default;

gfx::SharedVertexLayout const& SceneImporter::GetDefaultVertexLayout() const
{
    return m_DefaultVertexLayout;
}

void SceneImporter::InitDefaultVertexLayout()
{
    m_DefaultVertexLayout = std::make_shared<gfx::VertexLayout>();
    m_DefaultVertexLayout->AddAtribute(BGFX_ATTRIB_POSITION, 3, BGFX_ATTRIB_TYPE_FLOAT, false);
    m_DefaultVertexLayout->AddAtribute(BGFX_ATTRIB_NORMAL, 3, BGFX_ATTRIB_TYPE_FLOAT, false);
    m_DefaultVertexLayout->AddAtribute(BGFX_ATTRIB_TANGENT, 3, BGFX_ATTRIB_TYPE_FLOAT, false);
    m_DefaultVertexLayout->AddAtribute(BGFX_ATTRIB_BITANGENT, 3, BGFX_ATTRIB_TYPE_FLOAT, false);
    m_DefaultVertexLayout->AddAtribute(BGFX_ATTRIB_TEXCOORD0, 2, BGFX_ATTRIB_TYPE_FLOAT, false);

    m_DefaultVertexLayout->Bake();
}

void SceneImporter::InitDefaultShader()
{
    m_DefaultShader = std::make_shared<gfx::ShaderProgram>("shaders\\vs_sponza.bin", "shaders\\fs_sponza.bin");
}

void SceneImporter::ImportScene(std::string file, Scene& scene)
{
    m_ImportedScenePath = file;
    m_NonameTemplateString = "noname_";

    m_RootMesh.m_SubMeshes.clear();
    m_ParsedMeshes.clear();
    m_ParsedMaterials.clear();

    Assimp::Importer importer;

    aiScene const* aiscene = importer.ReadFile(file, aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded);
    assert(aiscene != nullptr && "Failed to load scene");
    m_ImportedScene = aiscene;

    InitDefaultVertexLayout();
    InitDefaultShader();

    ParseNodeToMeshInternalRecursive(aiscene->mRootNode, m_RootMesh);
    ParseMeshInternalHierarchyToScene(scene);
}

std::uint32_t SceneImporter::ParseNodeToMeshInternalRecursive(aiNode* node, MeshInternal& nodeMesh)
{
    // TODO: later :D
    //auto nodeTransform = node->mTransformation;

    std::uint32_t const meshCount = node->mNumMeshes;
    for (std::uint32_t i = 0; i < meshCount; i++)
    {
        std::uint32_t const meshId = node->mMeshes[i];
        auto parsedMesh = m_ParsedMeshes.find(meshId);
        if (parsedMesh == m_ParsedMeshes.end())
        {
            aiMesh* mesh = m_ImportedScene->mMeshes[meshId];

            MeshInternal internalMesh;
            ParseMeshVertexData(mesh, internalMesh);
            ParseMeshMaterials(mesh, internalMesh);
            nodeMesh.m_SubMeshes.emplace_back(std::move(internalMesh));
        }
        else
        {
            nodeMesh.m_SubMeshes.emplace_back(parsedMesh->second);
        }
    }

    std::uint32_t const childNodeCount = node->mNumChildren;
    for (std::uint32_t i = 0; i < childNodeCount; i++)
    {
        MeshInternal childNodeMesh;
        if (0 < ParseNodeToMeshInternalRecursive(node->mChildren[i], childNodeMesh))
        {
            nodeMesh.m_SubMeshes.emplace_back(childNodeMesh);
        }
    }

    return meshCount;
}

void SceneImporter::ParseMeshVertexData(aiMesh* mesh, MeshInternal& result)
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
        auto& uv = mesh->mTextureCoords[0][i];
        importedVertices[i].m_UV[0] = uv.x;
        importedVertices[i].m_UV[1] = uv.y;
    }


    std::vector<std::uint32_t> importedIndicies;

    std::uint32_t const indiciesCount = mesh->mNumFaces * 3;
    importedIndicies.resize(indiciesCount);
    for (std::uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
        importedIndicies[i * 3 + 0] = static_cast<std::uint32_t>(mesh->mFaces[i].mIndices[0]);
        importedIndicies[i * 3 + 1] = static_cast<std::uint32_t>(mesh->mFaces[i].mIndices[1]);
        importedIndicies[i * 3 + 2] = static_cast<std::uint32_t>(mesh->mFaces[i].mIndices[2]);
    }


    result.m_Vertices = std::move(importedVertices);
    result.m_Indicies = std::move(importedIndicies);
}

void SceneImporter::ParseMeshMaterials(aiMesh* mesh, MeshInternal& result)
{
    assert(m_ImportedScene->mNumMaterials > 0 && "Imported scene has no materials.");

    aiMaterial* material = m_ImportedScene->mMaterials[mesh->mMaterialIndex];
    assert((0 < material->GetTextureCount(aiTextureType_DIFFUSE)) && "Material has no diffuse texture.");

    result.m_MaterialIndex = mesh->mMaterialIndex;

    auto materialInternalIt = m_ParsedMaterials.find(mesh->mMaterialIndex);
    if (materialInternalIt != m_ParsedMaterials.end())
    {
        return;
    }

    MaterialInternal& materialInternal = m_ParsedMaterials[mesh->mMaterialIndex];
    materialInternal.m_Name = material->GetName().C_Str();

    aiString aiDiffuseLocalPath;
    aiReturn resultCode = material->GetTexture(aiTextureType_DIFFUSE, 0, &aiDiffuseLocalPath);

    std::string const diffuseLocalPath = aiDiffuseLocalPath.C_Str();
    auto textureIt = m_ParsedTextures.find(diffuseLocalPath);
    if (textureIt != m_ParsedTextures.end())
    {
        materialInternal.m_DiffuseTexture = textureIt->second;
        return;
    }

    assert(resultCode == aiReturn_SUCCESS   && "Failed to retreive material texture.");
    assert(diffuseLocalPath[0] != '*'       && "Material references embedded texture which is not currently supported.");

    std::string sceneGlobalPath = m_ImportedScenePath;
    auto pathEnd = sceneGlobalPath.find_last_of('\\');
    assert(pathEnd != std::string::npos && "There are no path separators in scene path. We now support only global paths.");
    sceneGlobalPath.resize(pathEnd + 1);

    materialInternal.m_DiffuseTexture = std::make_shared<gfx::Texture>(sceneGlobalPath + diffuseLocalPath);
    m_ParsedTextures[diffuseLocalPath] = materialInternal.m_DiffuseTexture;
}

void SceneImporter::ParseMeshInternalHierarchyToScene(Scene& scene)
{
    Entity& rootEntity = scene.GetRootEntityRef();
    Entity* importedSceneEntity = rootEntity.AddChild("imported_scene");

    MeshInternalToEntitiesRecursive(m_RootMesh, *importedSceneEntity);
}

void SceneImporter::MeshInternalToEntitiesRecursive(MeshInternal& meshInternal, Entity& entity)
{
    // this is not just a hierarchical node
    if (!meshInternal.m_Vertices.empty())
    {
        gfx::SharedVertexBuffer vertexBuffer = std::make_shared<gfx::VertexBuffer>(m_DefaultVertexLayout, static_cast<std::uint32_t>(meshInternal.m_Vertices.size()), meshInternal.m_Vertices.data(), static_cast<std::uint32_t>(sizeof(gfx::Vertex)));
        entity.InitRenderableComponent(m_DefaultShader, vertexBuffer);

        if (!meshInternal.m_Indicies.empty())
        {
            entity.GetRenderableComponentRef().m_IndexBuffer = std::make_shared<gfx::IndexBuffer>(static_cast<std::uint32_t>(meshInternal.m_Indicies.size()), meshInternal.m_Indicies.data());
        }

        MaterialInternal& material = m_ParsedMaterials[meshInternal.m_MaterialIndex];
        entity.GetRenderableComponentRef().m_TextureData = gfx::UniformTextureData{ material.m_DiffuseTexture };
    }

    std::uint32_t const subMeshCount = static_cast<std::uint32_t>(meshInternal.m_SubMeshes.size());
    for (std::uint32_t i = 0; i < subMeshCount; i++)
    {
        MeshInternal& submesh = meshInternal.m_SubMeshes[i];

        std::string name;
        if (!submesh.m_Name.empty())
        {
            name = submesh.m_Name;
        }
        else
        {
            char strBuffer[64];
            std::strcpy(strBuffer, m_NonameTemplateString.c_str());
            std::sprintf(strBuffer + m_NonameTemplateString.size(), "%u", m_NonameTemplateCounter++);

            name = strBuffer;
        }

        Entity* childEntity = entity.AddChild(name);
        
        MeshInternalToEntitiesRecursive(submesh, *childEntity);
    }
}

} // namespace pg
