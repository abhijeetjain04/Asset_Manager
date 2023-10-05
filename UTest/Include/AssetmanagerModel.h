#pragma once
#include <set>
#include <vector>
#include <string>

namespace Model
{
	class AssetmanagerModel
	{
	public:
		AssetmanagerModel();
		~AssetmanagerModel();
		void compressFile(std::set<std::string>& files,const std::string& output);

		//void compressFolder();
	private:
	};

};
