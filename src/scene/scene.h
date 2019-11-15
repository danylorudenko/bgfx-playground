#pragma once

#include <scene/entity.h>

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


    Entity& RootEntityRef() { return *m_RootEntity; }
    Entity const& RootEntityRef() const { return *m_RootEntity; }

    Entity* RootEntity() { return m_RootEntity.get(); }
    Entity const* RootEntity() const { return m_RootEntity.get(); }


    using EntityGenericDelegate = void(*)(Entity& entity);
    using ConstEntityGenericDelegate = void(*)(Entity const& entity);

    void ForEachEntity(EntityGenericDelegate entityDelegate);
    void ForEachEntity(ConstEntityGenericDelegate entityDelete);


private:
    std::unique_ptr<Entity> m_RootEntity;
};

} // namespace pg

