#include <io/io_helpers.h>

#include <utility>
#include <fstream>
#include <cassert>
#include <iterator>
#include <algorithm>

namespace pg::io_helpers
{

FileReader::FileReader() = default;

FileReader::FileReader(std::string const& fileName)
    : m_FileName{ fileName }
    , m_FileData{}
{
}

FileReader::FileReader(FileReader const& rhs) = default;
FileReader::FileReader(FileReader&& rhs) = default;

FileReader& FileReader::operator=(FileReader const& rhs) = default;
FileReader& FileReader::operator=(FileReader&& rhs) = default;

FileReader::~FileReader() = default;

std::uint64_t FileReader::Size() const
{
    return static_cast<std::uint64_t>(m_FileData.size());
}

std::uint8_t const* FileReader::Read()
{
    if (m_FileData.size() != 0)
    {
        return m_FileData.data();
    }
    else
    {
        std::ifstream fileStream{ m_FileName, std::ios_base::binary | std::ios_base::ate };
        if (!fileStream)
        {
            assert(false && "Failed to open file via FileData::Read()");
            return nullptr;
        }

        std::streamsize fileSize = fileStream.tellg();

        fileStream.seekg(0, std::ios_base::beg);
        m_FileData.resize(fileSize);
        fileStream.read(reinterpret_cast<char*>(m_FileData.data()), fileSize);

        fileStream.close();

        return m_FileData.data();
    }
}

bgfx_memory_t const* FileReader::ReadToBgfx()
{
    std::uint8_t const* data = Read();
    return bgfx_copy(data, static_cast<std::uint32_t>(Size()));
}


} // namespace pg::io_helpers

