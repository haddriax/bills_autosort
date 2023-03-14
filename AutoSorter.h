//
// Created by gael- on 12/03/2023.
//

#ifndef BILLS_AUTOSORT_AUTOSORTER_H
#define BILLS_AUTOSORT_AUTOSORTER_H


#include <shlwapi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <list>

namespace FileSorter
{


    class AutoSorter
    {
    public:
        AutoSorter();
        ~AutoSorter();

    private:
        // Full path leading to the directory from where we want to copy from.
        char *pathFromDirectory;

        // Full path leading to the directory where files will be copied and sorted.
        char *pathToDirectory;

        // Directory that will be created at pathToDirectory.
        char outputDirectoryName[9] = "Factures";

        int monthFilter;
        int yearFilter;

        // Filtered files that must be copied.
        std::list<char*> sortedFiles;

        // Scan each file inside the folder.
        void ScanFolder(const char *folder);

        // Get more data about the file.
        int ProcessFile(const WIN32_FIND_DATAA& data);

        // Add file to sorted list if it meets the research criteria
        void SortFile(const struct stat &fileInfo, const char *fileName);

        void CopyFiles();

    public:
        void SetMonth(int month);
        void SetYear(int year);
        void SetScanLocation(char* scanLocation);
    };

} // FileSorter

#endif //BILLS_AUTOSORT_AUTOSORTER_H
