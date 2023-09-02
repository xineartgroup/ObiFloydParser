// ObiFloydParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <Windows.h>
#include "Data\Grammer.h"

std::string getExecutablePath()
{
    char rawPathName[MAX_PATH];
    GetModuleFileNameA(nullptr, rawPathName, MAX_PATH);
    std::string executableFile(rawPathName);

    size_t count = executableFile.size();
    size_t lastIndexOf = count;
    for (size_t i = 0; i < count; i++)
    {
        if (executableFile[i] == '\\')
            lastIndexOf = i + 1;
    }

    return executableFile.substr(0, lastIndexOf);
}

std::string GetGrammerFileContents()
{
    std::string fileContent = "";
    std::string grammerFile = getExecutablePath() + "grammer_file.grm";
    std::fstream infile;
    infile.open(grammerFile);

    while (!infile.eof())
    {
        std::string data;
        infile >> data;
        fileContent = fileContent + data + "\n";
    }

    infile.close();

    return fileContent;
}

void WriteParseTree()
{
    std::string fileContent = Grammer::GetParseStructure();
    std::string parseFile = getExecutablePath() + "parse_tree.xml";
    std::ofstream myfile(parseFile);

    if (myfile.is_open())
    {
        myfile << fileContent;
        myfile.close();
    }
}

int main()
{
    std::string fileContent = GetGrammerFileContents();
    std::cout << fileContent;

    Grammer::ReadGrammer(fileContent);

    std::string parseText;

    while (true)
    {
        parseText = "";
        std::cout << "\n\nInput an expression to validate: ";
        std::cin >> parseText;
        if (parseText == "exit")
        {
            Grammer::Cleanup();
            return 0;
        }
        std::cout << "\nDisplay Parse tree: Y or N: ";
        std::string strVerbose = "";
        std::cin >> strVerbose;
        std::cout << Grammer::GetParseString(parseText, strVerbose == "Y" || strVerbose == "y") << std::endl;
        WriteParseTree();
    }

    return 0;
}