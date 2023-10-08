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

		//std::optional<std::string> compressFile(const std::set<std::string>& files,const std::string& output);
		std::optional<std::string> compressFile(const std::set<std::string>& files,std::string_view output);
		std::optional<std::string> UncompressFile(const std::string& zipfile, const std::string& outputdir);
		std::optional<std::string> AddFileToArchive(const std::string& zipfile, std::set<std::string>& filesToAdd);
		std::optional<std::string> RemoveFileFromArchive(const std::string& zipfile, const std::string& fileToDelete);
		std::optional<std::string> ArchiveDetailsWithMetadata(const std::string& zipfile, std::vector<std::unordered_map<std::string, std::string>>& arc_items);
		std::pair<std::string, bool> ArchiveContainsFile(const std::string& zipfile,std::string& Files);
	private:
		bit7z::Bit7zLibrary m_lib;
	};

};
