#include <scene/scene.h>

namespace pg
{

Scene::Scene()
    : m_RootEntity(std::make_unique<Entity>())
{
    m_RootEntity->SetName("ROOT");
    m_RootEntity->SetPosition(glm::vec3{ 0.0f, 0.0f, 0.0f });
    m_RootEntity->SetRotation(glm::identity<glm::quat>());
    m_RootEntity->SetScale(glm::vec3{ 1.0f, 1.0f, 1.0f });
}

Scene::Scene(Scene&&) = default;

Scene& Scene::operator=(Scene&&) = default;

Scene::~Scene() = default;

void Scene::Update(float dt)
{
    m_MainCamera.Update(dt);
}

void RecursiveEntityTraversal(Entity& current, Scene::EntityGenericDelegate entityDelegate)
{
    entityDelegate(current);
    std::uint32_t const childCount = current.GetChildCount();
    for (std::uint32_t i = 0; i < childCount; i++)
    {
        RecursiveEntityTraversal(*current.GetChild(i), entityDelegate);
    }
}

void Scene::ForEachEntity(Scene::EntityGenericDelegate entityDelegate)
{
    Entity& root = *m_RootEntity;
    std::uint32_t const childCount = root.GetChildCount();
    for (std::uint32_t i = 0; i < childCount; i++)
    {
        RecursiveEntityTraversal(*root.GetChild(i), entityDelegate);
    }
    
}

} // namespace pg

