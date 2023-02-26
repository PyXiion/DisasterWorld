#ifndef PX_DISASTER_UTILS_MEMORY_STREAM_HPP
#define PX_DISASTER_UTILS_MEMORY_STREAM_HPP
#include <vector>
#include <string>
#include <fstream>
#include <bit>
#include <unordered_map>
#include <cstdint>

namespace px::disaster::utils {
  class MemoryStream;

  class ISerializable {
  public:
    virtual void Serialize(MemoryStream &stream) const = 0;
    virtual void Deserialize(MemoryStream &stream) = 0;
  };

  class MemoryStream {
  private:
    std::vector<uint8_t> m_buffer;
    size_t m_readPos;
    size_t m_writePos;

    template<class T>
    T SwapEndiannes(T data) {
      if constexpr (std::endian::native == std::endian::big) {// convert to little endian
        char* bytes = reinterpret_cast<char*>(&data);
        for (size_t i = 0; i < sizeof(T) / 2; i++) {
          std::swap(bytes[i], bytes[sizeof(T) - i - 1]);
        }
      }
      return data;
    }

    void Write(const void *data, size_t size);
    void Read(void *data, size_t size);

  public:
    template<class T> requires std::is_arithmetic_v<T>
    MemoryStream &operator<<(T value) {
      value = SwapEndiannes(value);
      Write(&value, sizeof(value));
      return *this;
    }
    template<class T> requires std::is_arithmetic_v<T>
    MemoryStream &operator>>(T &value) {
      Read(&value, sizeof(value));
      value = SwapEndiannes(value);
      return *this;
    }

    template<class A, class B>
    MemoryStream &operator<<(const std::unordered_map<A, B> &value) {
      *this << value.size();

      for (const auto &elem : value) {
        *this << elem.first << elem.second;
      }
      return *this;
    }
    template<class A, class B>
    MemoryStream &operator>>(std::unordered_map<A, B> &value) {
      value.clear();
      size_t size;
      *this >> size;
      value.reserve(size);

      for (size_t i = 0; i < size; i++) {
        A first; B second;
        *this >> first >> second;
        value[first] = second;
      }
      return *this;
    }

    template<class T>
    MemoryStream &operator<<(const std::vector<T> &value) {
      *this << value.size();
      for (const auto &elem : value) {
        *this << elem;
      }
      return *this;
    }
    template<class T>
    MemoryStream &operator>>(std::vector<T> &value) {
      value.clear();
      size_t size;
      *this >> size;
      value.resize(size);
      for (size_t i = 0; i < size; i++) {
        T elem;
        *this >> elem;
        value[i] = elem;
      }
      return *this;
    }

    MemoryStream &operator<<(const std::string &value);
    MemoryStream &operator>>(std::string &value);

    MemoryStream &operator<<(const ISerializable &serializable);
    MemoryStream &operator>>(ISerializable &serializable);

    std::vector<uint8_t> &GetBuffer();
    void Clear();

    void SetReadPos(size_t pos);
    size_t GetReadPos() const;

    void SetWritePos(size_t pos);
    size_t GetWritePos() const;

    size_t GetSize() const;
    void Reserve(size_t size);
  };
}

std::ifstream& operator>>(std::ifstream& ifs, px::disaster::utils::MemoryStream& ms);
std::ofstream& operator<<(std::ofstream& ofs, px::disaster::utils::MemoryStream& ms);

#endif // PX_DISASTER_UTILS_MEMORY_STREAM_HPP