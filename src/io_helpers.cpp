#include <io_helpers.h>

#include <utility>
#include <fstream>
#include <cassert>
#include <iterator>
#include <algorithm>

namespace pg
{

FileData::FileData() = default;

FileData::FileData(std::string const& fileName)
    : m_FileName{ fileName }
    , m_FileData{}
{
}

FileData::FileData(FileData const& rhs) = default;
FileData::FileData(FileData&& rhs) = default;

FileData& FileData::operator=(FileData const& rhs) = default;
FileData& FileData::operator=(FileData&& rhs) = default;

FileData::~FileData() = default;

std::uint64_t FileData::Size() const
{
    return static_cast<std::uint64_t>(m_FileData.size());
}

std::byte* FileData::Read()
{
    if (m_FileData.size() != 0)
    {
        return m_FileData.data();
    }
    else
    {
        std::ifstream fileStream{ m_FileName };
        if (!fileStream)
        {
            assert(false && "Failed to open file via FileData::Read()");
            return nullptr;
        }

        std::copy(
            std::istream_iterator<std::byte>{ fileStream }, 
            std::istream_iterator<std::byte>{}, 
            std::back_inserter(m_FileData)
        );

        fileStream.close();

        return m_FileData.data();
    }
}

} // namespace pg


