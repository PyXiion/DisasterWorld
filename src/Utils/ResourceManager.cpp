#include <Disaster/Utils/ResourceManager.hpp>
#include <filesystem>

namespace fs = std::filesystem;

std::string FileExists(const std::string& filename_without_ext) {
  // Get the current path and append the filename without extension
  std::filesystem::path filepath = std::filesystem::current_path() / filename_without_ext;

  // Check if the file exists without extension
  if (std::filesystem::exists(filepath)) {
    return filepath;
  }

  // Ff file doesn't exist, try adding common file extensions
  for (const std::string& ext : {".png", ".jpg", ".jpeg", ".data", ".yaml"}) {
      std::filesystem::path file_with_ext = filepath;
      file_with_ext.replace_extension(ext);
      if (std::filesystem::exists(file_with_ext)) {
        return file_with_ext;
      }
  }

  // file not found
  return std::string();
}

namespace px::disaster {
  std::unordered_map<std::string, std::unique_ptr<graphics::Texture>> ResourceManager::textures;
  std::unordered_map<std::string, std::string> ResourceManager::strings;
  std::unordered_map<std::string, YAML::Node> ResourceManager::yamls;
  std::vector<std::string> ResourceManager::folders = {"./"};

  std::string ResourceManager::GetPath(std::string id) {
    std::string pseudoPath(id);
    size_t pos;
    while ((pos = pseudoPath.find('.')) != std::string::npos) {
      pseudoPath[pos] = '/';
    }
    for (auto &folder : folders) {
      std::string path = FileExists(folder / fs::path(pseudoPath));
      if (!path.empty())
        return path;
    }
    throw std::runtime_error("Unkown id.");
  }

  graphics::Texture &ResourceManager::GetTexture(const std::string &id) {
    if (!textures.contains(id)) {
      auto texture = std::make_unique<graphics::Texture>();
      texture->LoadFromFile(GetPath(id));
      textures[id] = std::move(texture);
    }
    return *textures[id];
  }
  std::string ResourceManager::GetString(const std::string &id) {
    if (!strings.contains(id)) {
      std::ifstream ifs(GetPath(id));
      strings[id] = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
    }
    return strings[id];
  }
  YAML::Node ResourceManager::GetYAML(const std::string &id) {
    if (!yamls.contains(id)) {
      YAML::Node node = YAML::LoadFile(GetPath(id));
      yamls[id] = node;
      return node;
    }
    return yamls[id];
  }
  utils::MemoryStream ResourceManager::GetRaw(const std::string &id) {
    std::ifstream ifs(GetPath(id));
    utils::MemoryStream ms;
    ifs >> ms;
    return ms;
  }

  bool ResourceManager::IsTextureExists(const std::string &id) {
    return textures.contains(id);
  }

  void ResourceManager::AddFolder(std::string folder) {
    folders.push_back(folder);
  }
}