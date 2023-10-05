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
	private:
		bit7z::Bit7zLibrary m_lib;
	};

};
