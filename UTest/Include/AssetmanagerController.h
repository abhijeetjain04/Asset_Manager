#pragma once
#include"AssetmanagerView.h"
#include"AssetmanagerModel.h"

namespace Controller
{
	//User Input type 
	enum UserInputType
	{
		CreateUser = 1, SendMessage, ReadMessage, QuitApplication
	};

	class AssetmanagerController
	{
	public:
		AssetmanagerController(View::AssetmanagerView* ui, Model::AssetmanagerModel* model) :m_pUI(ui),m_pModel(model) {}
		~AssetmanagerController();

		bool StartApplication();
		void QuitApplication(bool& ret);

		void Compress();
		//void SendMessage();
		//void ReadMessage();

	private:
		void PrintStartMessage();
		void PrintFinalMessage();
		bool isSupportedExtension(const std::string& filePath);
		void GetValidPath(std::string& filepath);
		bool HasSpecialCharacters(const char* str);
		void TakeInputPaths(std::string& directoryPath, std::set<std::string>& files);
		bool fileExistsInDirectory(const std::string& directoryPath);

		View::AssetmanagerView* m_pUI;
		Model::AssetmanagerModel* m_pModel;
	};

};
