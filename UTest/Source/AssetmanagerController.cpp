#include <set>
#include <string>
#include <filesystem>
#include <optional>
#include "AssetmanagerController.h"

using namespace Controller;

void AssetmanagerController::PrintStartMessage()
{
    m_pUI->ClearScreen();
    m_pUI->PrintOnScreen("Please select an option:", true);
    m_pUI->PrintOnScreen("1. CompressFiles", true);
    m_pUI->PrintOnScreen("2. UncompressFiles", true);
    m_pUI->PrintOnScreen("3. AddOrRemoveAsset", true);
    m_pUI->PrintOnScreen("4. ListAllAssets", true);
    m_pUI->PrintOnScreen("5. PrintMetadata", true);
    m_pUI->PrintOnScreen("6. Quit", true);
}

void AssetmanagerController::PrintFinalMessage() 
{
    m_pUI->PrintOnScreen("Press ENTER to continue.....", true);
    m_pUI->WaitForUserInput();
}

bool AssetmanagerController::StartApplication()
{
    bool quit = false;
    PrintStartMessage();
    int UserInput;
    m_pUI->GetInputInteger(UserInput);

    //switch (static_cast<UserInputType>(UserInput))
    switch (UserInput)
    {
    case UserInputType::CompressFiles:
        Compress();
        break;
    case UserInputType::UncompressFiles:
        Uncompress();
        break;
    case UserInputType::AddorRemoveAsset:
        // Implement SendMessage and ReadMessage methods here
        break;
    case UserInputType::ListAllAssets:
        // Implement SendMessage and ReadMessage methods here
        break;
    case UserInputType::PrintMetadata:
        // Implement SendMessage and ReadMessage methods here
        break;
    case UserInputType::QuitApplication:
        QuitApplication();
        quit = true;
        break;

    default:
        m_pUI->PrintOnScreen("Invalid Option Selected", true);
        break;
    }

    PrintFinalMessage();
    return quit;
}

void AssetmanagerController::QuitApplication() {
    m_pUI->PrintOnScreen("Quitting!", true);
}

bool AssetmanagerController::IsSupportedExtension(const std::string& filePath)
{
    static std::set<std::string> supportedExtensions = { ".tga", ".wav", ".json", ".7z" };
    std::string fileExtension = std::filesystem::path(filePath).extension().string();
    return supportedExtensions.count(fileExtension) > 0;
}

bool AssetmanagerController::GetValidPath(std::string& filepath)
{
    m_pUI->PrintOnScreen("Please enter full path of the file", true);
    m_pUI->GetInputString(filepath);

    if (std::filesystem::is_regular_file(filepath) && IsSupportedExtension(filepath))
    {
        if (std::filesystem::exists(filepath))
        {
            std::replace(filepath.begin(), filepath.end(), '\\', '/');
            return true;
        }
        else 
        {
            m_pUI->PrintOnScreen("The provided path does not exist.Please entre file path again", true);
        }
    }
    else 
    {
        m_pUI->PrintOnScreen("Invalid file or unsupported extension. Please entre file path again", true);
    }

    filepath.clear();
    return false;
}

void AssetmanagerController::GetUserInput(std::string& directoryPath, std::set<std::string>& files) 
{
    std::string path;
    if (GetValidPath(path)) {
        auto result = files.insert(path);
        if (!result.second) {
            m_pUI->PrintOnScreen("File already exists, please check the path and enter a new one", true);
        }
    }

    do 
    {
        m_pUI->PrintOnScreen("Do you want to enter more files? Enter Y for Yes or N for No", true);
        m_pUI->GetInputString(path);

        while ((path != "Y") && (path != "N"))
        {
            m_pUI->PrintOnScreen("Invalid input. Please Enter Y to add more files or N to compress current selection", true);
            m_pUI->GetInputString(path);
        }
        if (path == "N") {
            break;
        }
        else {
           
            if (GetValidPath(path)) {
                auto result = files.insert(path);
                if (!result.second) {
                    m_pUI->PrintOnScreen("File already exists, please check the path and enter a new one", true);
                }
            }
        }
    } while (true);

}

bool AssetmanagerController::HasSpecialCharacters(const std::string& str)
{
    return std::any_of(str.begin(), str.end(), [](char c) {
        return !std::isalnum(c) && c != '_';
        });
}

void AssetmanagerController::Compress() 
{
    std::set<std::string> files;
    std::string directoryPath;
    GetUserInput(directoryPath, files);
    if (files.size() < 1)
    {
        m_pUI->PrintOnScreen("No files are selected for operation.", true);
        return;
    }

    directoryPath = std::filesystem::path(*(files.rbegin())).parent_path().string();

    std::string outputfilename;
    m_pUI->PrintOnScreen("Please enter the name of the output compressed file", true);

    do {
        m_pUI->GetInputString(outputfilename);
        if (HasSpecialCharacters(outputfilename)) {
            m_pUI->PrintOnScreen("File name contains special characters. Please enter a name without special characters", true);
            continue;
        }

        outputfilename = directoryPath + "/" + outputfilename + ".7z";

        if (std::filesystem::exists(outputfilename)) {
            m_pUI->PrintOnScreen("The file already exists at the specified path. Please enter a different name", true);
        }
        else {
            break;
        }
    } while (true);

    std::optional<std::string> error = m_pModel->compressFile(files, outputfilename);
    if (error.has_value())
    {
        m_pUI->PrintOnScreen(error.value(), true);
        return;
    }
    m_pUI->PrintOnScreen("Files has been successfuly compressed. Please check the following directory path: " + directoryPath, true);
}

void Controller::AssetmanagerController::Uncompress()
{
    std::string zipfile;
    std::string outputdir;

    if (GetValidPath(zipfile))
    {
       NEWFOLDER:
        m_pUI->PrintOnScreen("Please enter the path of folder where you want to uncompress", true);
        m_pUI->GetInputString(outputdir);
        while (!std::filesystem::is_directory(outputdir))
        {
            m_pUI->PrintOnScreen("Folder path is not valid. Please enter the valid folder path", true);
            m_pUI->GetInputString(outputdir);
        }
        if (!std::filesystem::is_empty(outputdir))
        {
            std::string wantToP;
            m_pUI->PrintOnScreen("Folder is not empty. Your files may be over ridden Do you want to proceed Y/N", true);
            m_pUI->GetInputString(wantToP);
            while ((wantToP != "Y") && (wantToP != "N"))
            {
                m_pUI->PrintOnScreen("Invalid input. Please Enter Y to proceed or N to enter new path", true);
                m_pUI->GetInputString(wantToP);
            }
            if ((wantToP == "N"))
            {
                goto NEWFOLDER;
            }
            else
            {
                return;
            }
        }

        std::optional<std::string> error = m_pModel->UncompressFile(zipfile, outputdir);
        if (error.has_value())
        {
            m_pUI->PrintOnScreen(error.value(), true);
            return;
        }
        m_pUI->PrintOnScreen("Files has been successfuly Uncompressed. Please check the following directory path: " + outputdir, true);

    }
}
