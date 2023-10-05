#include <vector>
#include <iostream>
#include <string>
#include <bit7z/bit7z.hpp>
#include <bit7z/bitarchivewriter.hpp>
#include <filesystem>
#include "AssetmanagerController.h"

Controller::AssetmanagerController::~AssetmanagerController()
{
}

void Controller::AssetmanagerController::PrintStartMessage()
{
	//clear screen and show options
	m_pUI->ClearScreen();

	m_pUI->PrintOnScreen(("Please select an option:"), true);
	m_pUI->PrintOnScreen(("1. Compress file"), true);
	m_pUI->PrintOnScreen(("2. Send Message"), true);
	m_pUI->PrintOnScreen(("3. Receive All Messages For User"), true);
	m_pUI->PrintOnScreen(("4. Quit"), true);
}

void Controller::AssetmanagerController::PrintFinalMessage()
{
	m_pUI->PrintOnScreen((" "), true);
	m_pUI->PrintOnScreen(("Enter any key and press return to continue....."), true);
	m_pUI->WaitForUserInput();
}

bool Controller::AssetmanagerController::StartApplication()
{
	//Take input from user and proceed accordingly 
	bool quit = false;
	PrintStartMessage();
	int UserInput;
	m_pUI->GetInputInteger(UserInput);

	switch (UserInput)
	{
	case UserInputType::CreateUser:
		Compress();
		break;

	case UserInputType::SendMessage:
		//SendMessage();
		break;

	case UserInputType::ReadMessage:
		//ReadMessage();
		break;

	case UserInputType::QuitApplication:
		QuitApplication(quit);
		break;

	default:
		m_pUI->PrintOnScreen(("Invalid Option Selected"), false);
		break;
	}

	PrintFinalMessage();
	return quit;
}

void Controller::AssetmanagerController::QuitApplication(bool& ret)
{
	m_pUI->PrintOnScreen(("Quitting!"), true);
	ret = true;
}

#if 0
void Controller::AssetmanagerController::Compress()
{
	OPENFILENAMEW ofn;
	wchar_t szFileNames[MAX_PATH * 100] = L""; // Increased buffer size to hold multiple file names

	ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
	ofn.lStructSize = sizeof(OPENFILENAMEW);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L"All Supported Files (*.tga;*.ogg;*.wav;*.json)\0*.tga;*.ogg;*.wav;*.json\0"
		L"TGA Files (*.tga)\0*.tga\0"
		L"Audio Files (*.ogg, *.wav)\0*.ogg;*.wav\0"
		L"JSON Files (*.json)\0*.json\0";
	ofn.lpstrFile = szFileNames;
	ofn.nMaxFile = MAX_PATH * 100; // Increased buffer size to hold multiple file names
	ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	if (GetOpenFileNameW(&ofn))
	{
		// Extract and display multiple file paths
		wchar_t* pFile = szFileNames;

		m_pModel->compressFile(pFile);
	}

}
#endif // 0

bool Controller::AssetmanagerController::isSupportedExtension(const std::string& filePath) {
	std::set<std::string> supportedExtensions = { ".tga", ".wav", ".json" };
	std::string fileExtension = std::filesystem::path(filePath).extension().string();
	return supportedExtensions.count(fileExtension) > 0;
}

void Controller::AssetmanagerController::GetValidPath(std::string& filepath)// rewrite this in proper way
{
	m_pUI->PrintOnScreen(("Please enter full path of the file to compress"), true);
	//std::string filepath;
	 std::cin >> filepath;
	 // Check if the path exists
	 if (std::filesystem::exists(filepath)) {
		 if (std::filesystem::is_regular_file(filepath)) {
			 if (isSupportedExtension(filepath)) {
				 //later also check if the path size, name size, special character
				 std::cout << "File exists at the provided path with a supported extension." << std::endl;
				 std::replace(filepath.begin(), filepath.end(), '\\', '/');
			 }
			 else {
				 std::cout << "File exists at the provided path, but it has an unsupported extension." << std::endl;
				 filepath.clear();
			 }
		 }
		 else {
			 std::cout << "A non-file item (e.g., directory) exists at the provided path." << std::endl;
			 filepath.clear();
		 }
	 }
	 else {
		 std::cout << "The provided path does not exist." << std::endl;
		 filepath.clear();
	 }
}

void Controller::AssetmanagerController::TakeInputPaths(std::string& directoryPath, std::set<std::string>& files)
{
	std::string path;
	GetValidPath(path);
	if(!path.empty())// remeber to check if the files are in same folder
	{
		//files.push_back(path);
		auto result_1 = files.insert(path);
		if (!result_1.second)
			m_pUI->PrintOnScreen(("File already exist, please check the path and enter new path"), true);
	}
	do
	{
		int p;
		m_pUI->PrintOnScreen(("Do you want to enter more files. Enter 1 for Yes or 2 for No"), true);
		m_pUI->GetInputInteger(p);
		if (p == 2)
		{
			break;
		}
		else
		{
			m_pUI->PrintOnScreen(("Please enter full path of the file to compress"), true);
			//m_pUI->GetInputString(filepath);
			GetValidPath(path);
			if (!path.empty())// remeber to check if the files are in same folder
			{
				//files.push_back(path);
				auto result_1 = files.insert(path);
				if (!result_1.second)
					m_pUI->PrintOnScreen(("File already exist, please check the path and enter new path"), true);
			}
		}

	} while (true);

}

bool Controller::AssetmanagerController::fileExistsInDirectory(const std::string& directoryPath)
{
	for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
		if (std::filesystem::is_regular_file(entry.path()) && isSupportedExtension(entry.path().string())) {
			return true; // Found a file with a supported extension
		}
	}
	return false; // No supported file found in the directory
}

bool Controller::AssetmanagerController::HasSpecialCharacters(const char* str)
{
	return str[strspn(str, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_")] != 0;
}

void Controller::AssetmanagerController::Compress()
{
	bit7z::Bit7zLibrary lib("7z.dll");
	bit7z::BitArchiveWriter Writearchive(lib, bit7z::BitFormat::SevenZip);

	std::set<std::string> files;
	std::string directoryPath;
	TakeInputPaths(directoryPath, files);
	directoryPath = std::filesystem::path(*(files.rbegin())).parent_path().string();
	std::cout << "Output Directory path: " << directoryPath << std::endl;

	m_pUI->PrintOnScreen(("Please enter the name of output compress file"), true);
	std::string outputfilename;
	std::cin >> outputfilename; 
	while(HasSpecialCharacters(outputfilename.c_str()))
	{
		m_pUI->PrintOnScreen(("File name contain special character, please enter name without special character"), true);
		std::cin >> outputfilename;
	}

	outputfilename = directoryPath + "/" + outputfilename + ".7z";// check if the file already exist then tell user different 

	while (std::filesystem::exists(outputfilename))
	{
		m_pUI->PrintOnScreen(("The file exists at the specified path:"), false);
		std::cin >> outputfilename;
		outputfilename = directoryPath + "/" + outputfilename + ".7z";// check if the file already exist then tell user different 
	}

	m_pModel->compressFile(files, outputfilename);
}



