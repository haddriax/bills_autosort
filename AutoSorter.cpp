//
// Created by gael- on 12/03/2023.
//

#include "AutoSorter.h"


#include <iostream>
#include <filesystem>
#include <windows.h>
#include <ctime>
#include <cerrno>
#include <cstring>

namespace FileSorter {

    void AutoSorter::ScanFolder(const char* folder)
    {

        WIN32_FIND_DATAA data;
        HANDLE hFindFile = FindFirstFileA(pathFromDirectory, &data);

        if (hFindFile != INVALID_HANDLE_VALUE)
        {
            do
            {
                ProcessFile(data);
            } while (FindNextFileA(hFindFile, &data));

            FindClose(hFindFile);
        }
    }

    int AutoSorter::ProcessFile(const WIN32_FIND_DATAA &data)
    {

        struct stat fileInfo;

        //@todo add checks for null path.
        if ( data.cFileName == "" )
        {
            std::cerr << "file == nullptr\n";
            return (EXIT_FAILURE);
        }

        // Concatenate path and filename to get absolute path to file.
        size_t len1 = strlen(pathFromDirectory);
        size_t len2 = strlen(data.cFileName);

        char *absPathToFile { new char[len1 + len2 + 2] };
        absPathToFile = strcpy(absPathToFile, pathFromDirectory);
        absPathToFile = strcat(absPathToFile, "\\");
        absPathToFile = strcat(absPathToFile, data.cFileName);

        if ( stat(absPathToFile, &fileInfo) != 0 )
        {  // Use stat() to get the info
            std::cerr << "Error: " << strerror(errno) << '\n';
            return(EXIT_FAILURE);
        }

        SortFile(fileInfo, data.cFileName);

        return 0;
    }

    void AutoSorter::SortFile(const struct stat &fileInfo, const char *fileName)
    {

        // Get the file's creation time.
        tm *t = gmtime(&fileInfo.st_ctime);

        // Keep the file if it matches the selected month and year.
        if ( (t->tm_mon == monthFilter) &&
                ((t->tm_year + 1900) == yearFilter) &&
                ((fileInfo.st_mode & S_IFMT) != S_IFDIR))
        {
            // Copy selected file name into the selected file collection.
            char *c = new char [strlen(fileName)];
            c = strcpy(c, fileName);

            sortedFiles.push_back( c);
        }
    }

    AutoSorter::AutoSorter()
    {
        pathFromDirectory = nullptr;
        pathToDirectory = nullptr;

    }

    AutoSorter::~AutoSorter()
    {
        for (auto *elem : sortedFiles)
            delete[] elem;
    }

    void AutoSorter::SetMonth(int month)
    {
        if (month > 0 && month <= 12)
        {
            monthFilter = (month - 1);
        }
        else
        {
            monthFilter = 1;
            std::cerr << "Invalid value for Month, filter will be set at January by default" << std::endl;
        }
    }

    void AutoSorter::SetYear(int year)
    {
        if (year > 2000 && year <= 2050)
        {
            yearFilter = year;
        }
        else
        {
            year = 2023;
            std::cerr << "Invalid value for Year, filter will be set at 2023 by default" << std::endl;
        }
    }

    void AutoSorter::SetScanLocation(char* scanLocation)
    {
        // @todo Check path validity.
        pathFromDirectory = scanLocation;
    }

    void AutoSorter::CopyFiles()
    {

        // Output directory creation.
        size_t len1 = strlen(pathFromDirectory);
        size_t len2 = strlen(outputDirectoryName);

        // Build absolute directory path.
        char *absPathToNewDirectory { new char[len1 + len2 + 2] };
        absPathToNewDirectory = strcpy(absPathToNewDirectory, pathFromDirectory);
        absPathToNewDirectory = strcat(absPathToNewDirectory, "\\");
        absPathToNewDirectory = strcat(absPathToNewDirectory, outputDirectoryName);

        // Create Output directory.
        std::filesystem::create_directory(pathToDirectory);

        // Iterate sorted files (paths) and copy them.
        // /!\ ORIGINAL FILES MUST BE KEPT SAFE
        for (const auto &f : sortedFiles)
        {
            try
            {
                std::filesystem::copy_file(f,absPathToNewDirectory);
            }
            catch (std::filesystem::filesystem_error& e)
            {
                std::cerr << "Error copying file " << f << " : " << e.what() << std::endl;
            }
        }

        // Free buffer.
        delete[] absPathToNewDirectory;
    }


} // FileSorter