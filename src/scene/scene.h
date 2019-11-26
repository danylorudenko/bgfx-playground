#pragma once

#include <scene/entity.h>
#include <scene/camera.h>

namespace pg
{

class Scene
{
public:
    Scene();

    Scene(Scene&& rhs);
    Scene& operator=(Scene&& rhs);

    Scene(Scene const&) = delete;
    Scene& operator=(Scene const&) = delete;

    ~Scene();


    Entity&         GetRootEntityRef() { return *m_RootEntity; }
    Entity const&   GetRootEntityRef() const { return *m_RootEntity; }

    Entity*         GetRootEntity() { return m_RootEntity.get(); }
    Entity const*   GetRootEntity() const { return m_RootEntity.get(); }

    Camera&         GetMainCamera() { return m_MainCamera; }
    Camera const&   GetMainCamera() const { return m_MainCamera; }


    using EntityGenericDelegate = void(*)(Entity& entity);
    using ConstEntityGenericDelegate = void(*)(Entity const& entity);

    void ForEachEntity(EntityGenericDelegate entityDelegate);
    void ForEachEntity(ConstEntityGenericDelegate entityDelete);


private:
    std::unique_ptr<Entity> m_RootEntity;

    Camera                  m_MainCamera;
};

} // namespace pg

