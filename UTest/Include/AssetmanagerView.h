#pragma once

#include <string>
#include <iostream>

namespace View
{
	class IAssetmanagerView
	{
	public:
		IAssetmanagerView() = default;
		virtual ~IAssetmanagerView() = default;

		virtual void PrintOnScreen(const std::string& str, bool WantNewLine) = 0;
		virtual void GetInputString(std::string& str) = 0;
	};

	//UI implementation of console application    
	class AssetmanagerView : public IAssetmanagerView
	{
	public:
		AssetmanagerView() = default;
		~AssetmanagerView() = default;

		void PrintOnScreen(const std::string& str, bool WantNewLine) override;
		void WaitForUserInput();
		void ClearScreen();
		void GetInputString(std::string& str)override;
		void GetInputInteger(int& inputInt);
	};
};
