#include "ArgumentParsing.h"

bool Arguments::IsCreatingArchive(int argc, char** argv) {
    for (int i = 0; i < argc; ++i) {
        if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--create")) {
            create_ = true;
            if (i + 2 < argc && !strcmp(argv[i + 1], "-f")) {
                if (argv[i + 2][0] != '-') {
                    archive_name_to_create_ = argv[i + 2];
                    int j = i + 3;
                    while (j < argc && argv[j][0] != '-') {
                        filenames.push_back(argv[j]);
                        ++j;
                    }
                } else {
                    std::cerr << "Error: Programme can't create file without name";
                    std::exit(1);
                }
            } else if (i + 1 < argc && !strncmp(argv[i + 1], "--file=", 7) && strlen(argv[i + 1]) > 7) {
                archive_name_to_create_ = argv[i + 1] + 7;
                int j = i + 2;
                while (j < argc && argv[j][0] != '-') {
                    filenames.push_back(argv[j]);
                    ++j;
                }
            } else {
                std::cerr << "Error: Programme can't create file without name";
                std::exit(1);
            }
        }
    }

    return create_;
}

bool Arguments::IsDeletingFile(int argc, char** argv) {
    for (int i = 0; i < argc; ++i) {
        if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--delete")) {
            delete_ = true;
            if (i + 2 < argc && !strcmp(argv[i + 1], "-f")) {
                if (argv[i + 2][0] != '-') {
                    archive_name_to_delete_ = argv[i + 2];
                } else {
                    std::cerr << "Error: Programme can't delete file from unknown archive";
                    std::exit(1);
                }
                if (i + 3 < argc && argv[i + 3][0] != '-') {
                    filename_to_delete_ = argv[i + 3];
                } else {
                    std::cerr << "Error: Programme can't delete unknown file from archive";
                    std::exit(1);
                }
            } else if (i + 1 < argc && !strncmp(argv[i + 1], "--file=", 7) && strlen(argv[i + 1]) > 7) {
                archive_name_to_delete_ = argv[i + 1] + 7;
                if (i + 2 < argc && argv[i + 2][0] != '-') {
                    filename_to_delete_ = argv[i + 2];
                } else {
                    std::cerr << "Error: Programme can't delete unknown file from archive";
                    std::exit(1);
                }
            } else {
                std::cerr << "Error: Programme can't delete file from unknown archive";
                std::exit(1);
            }
        }
    }

    return delete_;
}

bool Arguments::IsAppendingFile(int argc, char** argv) {
    for (int i = 0; i < argc; ++i) {
        if (!strcmp(argv[i], "-a") || !strcmp(argv[i], "--append")) {
            append_ = true;
            if (i + 2 < argc && !strcmp(argv[i + 1], "-f")) {
                if (argv[i + 2][0] != '-') {
                    archive_name_to_append_ = argv[i + 2];
                } else {
                    std::cerr << "Error: Programme can't append file to unknown archive";
                    std::exit(1);
                }
                if (i + 3 < argc && argv[i + 3][0] != '-') {
                    filename_to_append_ = argv[i + 3];
                } else {
                    std::cerr << "Error: Programme can't append unknown file to archive";
                    std::exit(1);
                }
            } else if (i + 1 < argc && !strncmp(argv[i + 1], "--file=", 7) && strlen(argv[i + 1]) > 7) {
                archive_name_to_append_ = argv[i + 1] + 7;
                if (i + 2 < argc && argv[i + 2][0] != '-') {
                    filename_to_append_ = argv[i + 2];
                } else {
                    std::cerr << "Error: Programme can't append unknown file to archive";
                    std::exit(1);
                }
            } else {
                std::cerr << "Error: Programme can't append file to unknown archive";
                std::exit(1);
            }
        }
    }

    return append_;
}

bool Arguments::IsExtractingFile(int argc, char** argv) {
    for (int i = 0; i < argc; ++i) {
        if (!strcmp(argv[i], "-x") || !strcmp(argv[i], "--extract")) {
            extract_ = true;
            if (i + 2 < argc && !strcmp(argv[i + 1], "-f")) {
                if (argv[i + 2][0] != '-') {
                    archive_name_to_extract_ = argv[i + 2];
                } else {
                    std::cerr << "Error: Programme can't extract file from unknown archive";
                    std::exit(1);
                }
                if (i + 3 < argc && argv[i + 3][0] != '-') {
                    filename_to_extract_ = argv[i + 3];
                }
            } else if (i + 1 < argc && !strncmp(argv[i + 1], "--file=", 7) && strlen(argv[i + 1]) > 7) {
                archive_name_to_extract_ = argv[i + 1] + 7;
                if (i + 2 < argc && argv[i + 2][0] != '-') {
                    filename_to_extract_ = argv[i + 2];
                }
            } else {
                std::cerr << "Error: Programme can't extract file from unknown archive";
                std::exit(1);
            }
        }
    }

    return extract_;
}

bool Arguments::IsCreatingList(int argc, char** argv) {
    for (int i = 0; i < argc; ++i) {
        if (!strcmp(argv[i], "-l") || !strcmp(argv[i], "--list")) {
            list_ = true;
            if (i + 2 < argc && !strcmp(argv[i + 1], "-f")) {
                if (argv[i + 2][0] != '-') {
                    archive_name_to_create_list_ = argv[i + 2];
                } else {
                    std::cerr << "Error: Programme can't create unknown archive's list of files";
                    std::exit(1);
                }
            } else if (i + 1 < argc && !strncmp(argv[i + 1], "--file=", 7) && strlen(argv[i + 1]) > 7) {
                archive_name_to_create_list_ = argv[i + 1] + 7;
            } else {
                std::cerr << "Error: Programme can't create unknown archive's list of files";
                std::exit(1);
            }
        }
    }

    return list_;
}

bool Arguments::IsConcatenatingArchives(int argc, char** argv) {
    for (int i = 0; i < argc; ++i) {
        if (!strcmp(argv[i], "-A") || !strcmp(argv[i], "--concatenate")) {
            concatenate_ = true;
            if (i + 2 < argc && argv[i + 1][0] != '-') {
                archive1_to_concatenate_ = argv[i + 1];
                if (argv[i + 2][0] != '-') {
                    archive2_to_concatenate_ = argv[i + 2];
                } else {
                    std::cerr << "Error: Programme can't find second archive to merge";
                    std::exit(1);
                }
                if (i + 4 < argc && !strcmp(argv[i + 3], "-f")) {
                    if (argv[i + 4][0] != '-') {
                        archive_name_to_concatenate_ = argv[i + 4];
                    } else {
                        std::cerr << "Error: Programme can't merge archives into the unknown archive";
                        std::exit(1);
                    }
                } else if (i + 3 < argc && !strncmp(argv[i + 3], "--file=", 7) && strlen(argv[i + 3]) > 7) {
                    archive_name_to_concatenate_ = argv[i + 3] + 7;
                } else {
                    std::cerr << "Error: Programme can't merge archives into the unknown archive";
                    std::exit(1);
                }
            } else {
                std::cerr << "Error: Programme can't find archives to merge";
                std::exit(1);
            }
        }
    }

    return concatenate_;
}


