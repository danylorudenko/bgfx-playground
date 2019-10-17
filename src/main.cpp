#include <assimp/Importer.hpp>
#include <bgfx/c99/bgfx.h>

int main()
{
    //bgfx::Init init;
    //bgfx::init(init);
    bgfx_init(nullptr);
    
    Assimp::Importer importer;
	return 0;
}