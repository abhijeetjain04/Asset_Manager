#pragma once


#include "AssetmanagerView.h"
#include "AssetmanagerModel.h"

namespace Controller {

    enum UserInputType {
        CompressFiles = 1, UncompressFiles, AddorRemoveAsset, ListAllAssets, PrintMetadata, QuitApplication
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
        bool GetValidPath(std::string& filepath);
        void GetUserInput(std::string& directoryPath, std::set<std::string>& files);
        bool HasSpecialCharacters(const std::string& str);
        void Compress();
        void Uncompress();

        View::AssetmanagerView* m_pUI;
        Model::AssetmanagerModel* m_pModel;
    };
}
