#include "AssetmanagerView.h"

void View::AssetmanagerView::PrintOnScreen(const std::string& str, bool WantNewLine)
{
	if (WantNewLine)
		std::cout << str << std::endl;
	else
		std::cout << str;
}

void View::AssetmanagerView::WaitForUserInput()
{
	std::string temp;
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::getline(std::cin, temp);
}

void View::AssetmanagerView::ClearScreen()
{
	for (int i = 0; i < 80; ++i) std::cout << std::endl;
}

void View::AssetmanagerView::GetInputString(std::string& str)
{
	std::cin >> str;
}

void View::AssetmanagerView::GetInputInteger(int& inputInt)
{
	std::cin >> inputInt;
}
