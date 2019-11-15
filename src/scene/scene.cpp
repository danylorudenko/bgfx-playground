#include <scene/scene.h>

namespace pg
{

Scene::Scene() = default;

Scene::Scene(Scene&&) = default;

Scene& Scene::operator=(Scene&&) = default;

Scene::~Scene() = default;

template<typename TEntity, typename TDelegate>
void RecursiveEntityTraversal(TEntity& current, TDelegate entityDelegate)
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
    RecursiveEntityTraversal<Entity&, Scene::EntityGenericDelegate>(*m_RootEntity, entityDelegate);
}

void Scene::ForEachEntity(Scene::ConstEntityGenericDelegate entityDelegate)
{
    RecursiveEntityTraversal<Entity const&, Scene::ConstEntityGenericDelegate>(*m_RootEntity, entityDelegate);
}

} // namespace pg

