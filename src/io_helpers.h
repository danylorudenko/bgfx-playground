#include <string>
#include <vector>
#include <cstdint>
#include <cstddef>

namespace pg
{

class FileData
{
public:
    FileData();
    FileData(std::string const& fileName);

    FileData(FileData const& rhs);
    FileData(FileData&& rhs);

    FileData& operator=(FileData const& rhs);
    FileData& operator=(FileData&& rhs);

    ~FileData();

    std::byte* Read(std::string const& fileName);
    std::byte* Read();

    std::uint64_t Size() const;

private:
    std::string             m_FileName;
    std::vector<std::byte>  m_FileData;
};

}

