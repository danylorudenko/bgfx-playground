#include <gfx/gfx_pass.h>

namespace pg::gfx
{

Pass::Pass() = default;

Pass::Pass(Pass&& rhs) = default;

Pass& Pass::operator=(Pass&& rhs) = default;

Pass::~Pass() = default;

void Pass::Begin()
{

}

void Pass::End()
{

}

void Pass::AddScreenSpaceAttachment()
{
/**
 * Init attachment.
 *
 * @param[in] _handle Render target texture handle.
 * @param[in] _access Access. See `Access::Enum`.
 * @param[in] _layer Cubemap side or depth layer/slice.
 * @param[in] _mip Mip level.
 * @param[in] _resolve Resolve flags. See: `BGFX_RESOLVE_*`
 *
 */
//BGFX_C_API void bgfx_attachment_init(bgfx_attachment_t* _this, bgfx_texture_handle_t _handle, bgfx_access_t _access, uint16_t _layer, uint16_t _mip, uint8_t _resolve);

    Attachment attachment;

    //bgfx_attachment_init();
}

}
