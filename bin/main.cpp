#include "../lib/Archive.h"

int main(int argc, char** argv) {
    Arguments args;
    if (args.IsCreatingArchive(argc, argv)) {
        CreateArchive(args);
    }
    if (args.IsDeletingFile(argc, argv)) {
        DeleteFile(args);
    }
    if (args.IsAppendingFile(argc, argv)) {
        AppendFile(args);
    }
    if (args.IsExtractingFile(argc, argv)) {
        if (!args.filename_to_extract_) {
            ExtractAllFiles(args);
        } else {
            ExtractFile(args);
        }
    }
    if (args.IsConcatenatingArchives(argc, argv)) {
        ConcatenateArchives(args);
    }
    if (args.IsCreatingList(argc, argv)) {
        MakeList(args);
    }
    if (args.append_ && args.delete_ && args.filename_to_append_ == args.filename_to_delete_) {
        std::cerr << "Do you really want to append new file and then delete it?\nIt's useless move.\nIf you made mistake, you can try again\n";
        std::cerr << "If you want to continue, write YES, else write NO\n";
        std::string answer;
        std::cin >> answer;
        if (answer == "NO") {
            std::cerr << "Fatal\n";
            exit(1);
        } else if (answer != "YES") {
            std::cerr << "There's a typo, but who cares\nFatal\n";
            exit(1);
        }
    } else if (args.append_ && args.extract_ && args.filename_to_append_ == args.filename_to_extract_) {
        std::cerr << "Do you really want to append new file and then extract it?\nIt's useless move.\nIf you made mistake, you can try again\n";
        std::cerr << "If you want to continue, write YES, else write NO\n";
        std::string answer;
        std::cin >> answer;
        if (answer == "NO") {
            std::cerr << "Fatal\n";
            exit(1);
        } else if (answer != "YES") {
            std::cerr << "There's a typo, but who cares\nFatal\n";
            exit(1);
        }
    }

    return 0;
}
