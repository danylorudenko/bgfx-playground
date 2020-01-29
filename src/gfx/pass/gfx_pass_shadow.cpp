#include <gfx/pass/gfx_pass_shadow.h>

namespace pg::gfx
{

PassShadow::PassShadow(PassId passId)
    : PassBase{ passId }
{}

PassShadow::PassShadow(PassShadow&& rhs) = default;

PassShadow& PassShadow::operator=(PassShadow&& rhs) = default;

PassShadow::~PassShadow() = default;

void PassShadow::Render(Scene* scene)
{

}

}

