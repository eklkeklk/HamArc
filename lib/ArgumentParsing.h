#pragma once
#include <iostream>
#include <vector>

struct Arguments {
    bool create_;
    bool delete_;
    bool list_;
    bool append_;
    bool extract_;
    bool concatenate_;
    std::vector<char*> filenames;
    char* archive_name_to_create_;
    char* archive_name_to_delete_;
    char* archive_name_to_create_list_;
    char* filename_to_delete_;
    char* archive_name_to_extract_;
    char* filename_to_extract_;
    char* archive_name_to_append_;
    char* filename_to_append_;
    char* archive1_to_concatenate_;
    char* archive2_to_concatenate_;
    char* archive_name_to_concatenate_;

    Arguments() : create_(false), delete_(false), list_(false), append_(false), extract_(false), concatenate_(false) {
        archive_name_to_create_ = nullptr;
        archive_name_to_delete_ = nullptr;
        archive_name_to_create_list_ = nullptr;
        filename_to_delete_ = nullptr;
        archive_name_to_extract_ = nullptr;
        filename_to_extract_ = nullptr;
        archive_name_to_append_ = nullptr;
        filename_to_append_ = nullptr;
        archive1_to_concatenate_ = nullptr;
        archive2_to_concatenate_ = nullptr;
        archive_name_to_concatenate_ = nullptr;
    }

    bool IsCreatingArchive(int argc, char** argv);

    bool IsDeletingFile(int argc, char** argv);

    bool IsAppendingFile(int argc, char** argv);

    bool IsExtractingFile(int argc, char** argv);

    bool IsCreatingList(int argc, char** argv);

    bool IsConcatenatingArchives(int argc, char** argv);
};