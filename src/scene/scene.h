#pragma once

#include <functional>

#include <class_features/class_features.hpp>
#include <scene/entity.h>
#include <scene/camera.h>


namespace pg
{

class Scene 
    : public class_features::NonCopyable
    , public class_features::StaticSingleton<Scene>
{
public:
    Scene();

    Scene(Scene&& rhs);
    Scene& operator=(Scene&& rhs);

    ~Scene();

    Entity* LoadFromFile(std::string fileName);

    void            Update(float dt);

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

