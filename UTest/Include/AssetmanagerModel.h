#pragma once

#include <bit7z/bit7z.hpp>
#include <set>
#include <vector>
#include <string>
#include <optional>


namespace Model
{
	class AssetmanagerModel
	{
	public:
		AssetmanagerModel();
		~AssetmanagerModel();
		//void compressFolder();
		std::optional<std::string> compressFile(std::set<std::string>& files,const std::string& output);
		std::optional<std::string> UncompressFile(const std::string& zipfile, const std::string& outputdir);
		std::optional<std::string> AddFileToArchive(const std::string& zipfile, std::set<std::string>& filesToAdd);
		std::optional<std::string> RemoveFileFromArchive(const std::string& zipfile, const std::string& fileToDelete);
		std::optional<std::string> ListAllAssetsWithMetadata(const std::string& zipfile, std::vector<std::unordered_map<std::string, std::string>>& arc_items);
	private:
		bit7z::Bit7zLibrary m_lib;
	};

};
