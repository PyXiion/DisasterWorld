#ifndef PX_DISASTER_UTILS_RESOURCE_MANAGER_INTERFACE_HPP
#define PX_DISASTER_UTILS_RESOURCE_MANAGER_INTERFACE_HPP
#include <string>

#include <Disaster/Graphics/Texture.hpp>
#include <Disaster/Utils/MemoryStream.hpp>
#include <yaml-cpp/yaml.h>

namespace px::disaster {
  class ResourceManager {
  public:
    static graphics::Texture &GetTexture(const std::string &id);
    static std::string GetString(const std::string &id);
    static YAML::Node GetYAML(const std::string &id);
    static utils::MemoryStream GetRaw(const std::string &id);

    static bool IsTextureExists(const std::string &id);

    static void AddFolder(std::string folder);

  protected:
    static std::unordered_map<std::string, std::unique_ptr<graphics::Texture>> textures;
    static std::unordered_map<std::string, std::string> strings;
    static std::unordered_map<std::string, YAML::Node> yamls;
    static std::vector<std::string> folders;

    static std::string GetPath(std::string id);
  };
}

#endif // PX_DISASTER_UTILS_RESOURCE_MANAGER_INTERFACE_HPP