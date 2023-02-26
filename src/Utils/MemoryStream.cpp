#include <Disaster/Utils/MemoryStream.hpp>
#include <cstring>
#include <stdexcept>

namespace px::disaster::utils {
  void MemoryStream::Write(const void *data, size_t size) {
    if (m_writePos + size >= m_buffer.size())
      m_buffer.resize(m_writePos + size);
    std::memcpy(&m_buffer[m_writePos], data, size);
    m_writePos += size;
  }
  void MemoryStream::Read(void *data, size_t size) {
    if (m_readPos + size >= m_buffer.size()) {
      throw std::out_of_range("Not enought data in buffer to read.");
    }
    std::memcpy(data, &m_buffer[m_readPos], size);
    m_readPos += size;
  }

  MemoryStream &MemoryStream::operator<<(const std::string &value) {
    *this << static_cast<uint32_t>(value.size());
    m_buffer.insert(m_buffer.begin() + m_writePos, value.begin(), value.end());
    m_writePos += value.size();
    return *this;
  }
  MemoryStream &MemoryStream::operator>>(std::string &value) {
    uint32_t size;
    *this >> size;
    if (m_readPos + size > m_buffer.size()) {
      throw std::out_of_range("Attempt to read beyond end of stream.");
    }
    value.assign(m_buffer.begin() + m_readPos, m_buffer.begin() + m_readPos + size);
    m_readPos += size;
    return *this;
  }

  MemoryStream &MemoryStream::operator<<(const ISerializable &s) {
    s.Serialize(*this);
    return *this;
  }
  MemoryStream &MemoryStream::operator>>(ISerializable &s) {
    s.Deserialize(*this);
    return *this;
  }

  std::vector<uint8_t> &MemoryStream::GetBuffer() {
    return m_buffer;
  }

  void MemoryStream::Clear() {
    m_buffer.clear();
    m_readPos = 0;
    m_writePos = 0;
  }
  void MemoryStream::SetReadPos(size_t pos) {
    m_readPos = pos;
  }
  size_t MemoryStream::GetReadPos() const {
    return m_readPos;
  }
  void MemoryStream::SetWritePos(size_t pos) {
    m_writePos = pos;
  }
  size_t MemoryStream::GetWritePos() const {
    return m_writePos;
  }

  size_t MemoryStream::GetSize() const {
    return m_buffer.size();
  }
  void MemoryStream::Reserve(size_t size) {
    m_buffer.reserve(size);
  }
}

std::ifstream &operator>>(std::ifstream &ifs, px::disaster::utils::MemoryStream &ms) {
  // Get the size of the file
  ifs.seekg(0, std::ios::end);
  std::streampos fileSize = ifs.tellg();
  ifs.seekg(0, std::ios::beg);

  // Read the contents of the file into a MemoryStream
  ms.GetBuffer().resize(fileSize);
  ifs.read(reinterpret_cast<char*>(ms.GetBuffer().data()), fileSize);

  ms.SetWritePos(static_cast<size_t>(fileSize) - 1);

  return ifs;
}
std::ofstream &operator<<(std::ofstream &ofs, px::disaster::utils::MemoryStream &ms) {
  ofs.write(reinterpret_cast<char*>(ms.GetBuffer().data()), ms.GetSize());
  ms.SetReadPos(ms.GetSize() - 1);
  return ofs;
}