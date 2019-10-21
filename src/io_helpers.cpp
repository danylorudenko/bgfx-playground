#include <string>
#include <cstdint>

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

private:
    std::string     m_Name;

    void*           m_Data;
    std::uint64_t   m_Size;
};

} // namespace pg


