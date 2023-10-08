#pragma once
#include <string>
#include <optional>

#include <bit7z/bit7z.hpp>
namespace Model
{
	class AssetmanagerModel
	{
	public:
		AssetmanagerModel() = default;
		~AssetmanagerModel()= default;

		std::optional<std::string> compressFile(const std::set<std::string>& zipFile,std::string_view outputDirectory);
		std::optional<std::string> UncompressFile(const std::string_view zipFile, const std::string_view outputDirectory);
		std::optional<std::string> AddFileToArchive(const std::string& zipFile, std::set<std::string>& filesToAdd);
		std::optional<std::string> RemoveFileFromArchive(const std::string& zipFile, const std::string& fileToDelete);
		std::optional<std::string> ArchiveDetailsWithMetadata(const std::string& zipFile, std::vector<std::unordered_map<std::string, std::string>>& archiveItems);
		std::pair<std::string, bool> ArchiveContainsFile(const std::string& zipFile,std::string& Files);
	private:
		bit7z::Bit7zLibrary m_lib;
	};

};
