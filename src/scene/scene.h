#pragma once

#include <functional>

#include <scene/entity.h>
#include <scene/camera.h>

#include <utils.hpp>

namespace pg
{

class Scene 
    : public utils::NonCopyable
    , public utils::StaticSingleton<Scene>
{
public:
    Scene();

    Scene(Scene&& rhs);
    Scene& operator=(Scene&& rhs);

    ~Scene();


    Entity&         GetRootEntityRef() { return *m_RootEntity; }
    Entity const&   GetRootEntityRef() const { return *m_RootEntity; }

    Entity*         GetRootEntity() { return m_RootEntity.get(); }
    Entity const*   GetRootEntity() const { return m_RootEntity.get(); }

    Camera&         GetMainCamera() { return m_MainCamera; }
    Camera const&   GetMainCamera() const { return m_MainCamera; }


    using EntityGenericDelegate = std::function<void(Entity& entity)>;
    void ForEachEntity(EntityGenericDelegate entityDelegate);


private:
    std::unique_ptr<Entity> m_RootEntity;

    Camera                  m_MainCamera;
};

} // namespace pg

