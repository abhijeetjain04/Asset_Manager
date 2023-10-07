#pragma once

#include "AssetmanagerView.h"
#include "AssetmanagerModel.h"

namespace Controller {

    enum UserInputType {
        CompressFiles = 1, UncompressFiles, AddAsset, RemoveAsset, ListAllAssetsWithMetadata, QuitApplication
    };

    class AssetmanagerController {
    public:
        AssetmanagerController(View::AssetmanagerView* ui, Model::AssetmanagerModel* model)
            : m_pUI(ui), m_pModel(model) {}

        ~AssetmanagerController() {}

        bool StartApplication();
        void QuitApplication();

    private:
        void PrintStartMessage();
        void PrintFinalMessage();
        bool IsSupportedExtension(const std::string& filePath);
        bool GetValidPath(const std::string& firstMessage, std::string& filepath);
        void GetUserInput(const std::string& directoryPath, std::set<std::string>& files);
        bool HasSpecialCharacters(const std::string& str);
        void Compress();
        void Uncompress();
        void AddAsset();
        void RemoveAsset();
        void ListAllAssetsWithMetadata();

        View::AssetmanagerView* m_pUI;
        Model::AssetmanagerModel* m_pModel;
    };
}
