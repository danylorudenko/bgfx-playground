#include <string>
#include <vector>
#include <cstdint>
#include <bgfx/c99/bgfx.h>

namespace pg::io_helpers
{

class FileReader
{
public:
    FileReader();
    FileReader(std::string const& fileName);

    FileReader(FileReader const& rhs);
    FileReader(FileReader&& rhs);

    FileReader& operator=(FileReader const& rhs);
    FileReader& operator=(FileReader&& rhs);

    ~FileReader();

    std::uint8_t const* Read();
    bgfx_memory_t const* ReadToBgfx();

    std::uint64_t Size() const;

private:
    std::string                 m_FileName;
    std::vector<std::uint8_t>   m_FileData;
};

} // namespace pg::io_helpers

