#include "Archive.h"

uint8_t kSymbolsInBlock = 8;
uint8_t kNumOfPartitionBlock = 8;
uint8_t kSizeOfEncodedBlock = 71;
uint8_t kDifferenceInASCII = 48;

void CreateArchive(const Arguments& args) {
    std::string arch_name(args.archive_name_to_create_);
    arch_name += ".haf";
    std::ofstream archive(arch_name, std::ios::out | std::ios::app);
    if (!archive.is_open()) {
        std::cerr << "Error: Programme can't open " << arch_name << " archive\nPlease, try again";
        exit(1);
    }
    if (args.filenames.empty()) {
        std::cout << "Empty archive was created\n";
        archive.close();
        return;
    }
    for (char* filename : args.filenames) {
        std::string f_name(filename);
        std::ifstream file(f_name, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error: Programme can't open " << f_name << " file\nPlease, try again";
            exit(1);
        }
        archive << '|' << f_name << '|';
        char symbol;
        while (file.good()) {
            std::vector<bool> block;
            for (int j = 0; j < kSymbolsInBlock; ++j) {
                std::bitset<8> part_of_block;
                file.get(symbol);
                if (file.eof()) {
                    break;
                }
                part_of_block = symbol;
                for (int k = 0; k < kNumOfPartitionBlock; ++k) {
                    block.push_back(part_of_block[k]);
                }
            }
            std::vector<bool> encoded_block = EncodeHamming(block);
            for (int j = 0; j < kSizeOfEncodedBlock; ++j) {
                if (j == encoded_block.size()) {
                    break;
                }
                archive.put(encoded_block[j] + kDifferenceInASCII);
            }
        }
        file.close();
    }
    std::cout << "Archive with " << args.filenames.size() << " files was created\n";
    archive.close();
}

void AppendFile(const Arguments& args) {
    std::string arch_name(args.archive_name_to_append_);
    arch_name += ".haf";
    std::ofstream archive(arch_name, std::ios::out | std::ios::app);
    if (!archive.is_open()) {
        std::cerr << "Error: Programme can't open " << arch_name << " archive\nPlease, try again";
        exit(1);
    }
    std::string f_name(args.filename_to_append_);
    std::ifstream file(f_name, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Programme can't open " << f_name << " file\nPlease, try again";
        exit(1);
    }
    archive << '|' << f_name << '|';
    char symbol;
    while (file.good()) {
        std::vector<bool> block;
        for (int i = 0; i < kSymbolsInBlock; ++i) {
            std::bitset<8> part_of_block;
            file.get(symbol);
            if (file.eof()) {
                break;
            }
            part_of_block = symbol;
            for (int j = 0; j < kNumOfPartitionBlock; ++j) {
                block.push_back(part_of_block[j]);
            }
        }
        std::vector<bool> encoded_block = EncodeHamming(block);
        for (int i = 0; i < kSizeOfEncodedBlock; ++i) {
            if (i == encoded_block.size()) {
                break;
            }
            archive.put(encoded_block[i] + kDifferenceInASCII);
        }
    }
    file.close();
    std::cout << f_name << " file was appended to " << arch_name << " archive\n";
    archive.close();
}

void ExtractFile(const Arguments& args) {
    std::string arch_name(args.archive_name_to_extract_);
    arch_name += ".haf";
    std::string f_name(args.filename_to_extract_);
    std::string extracted_f_name = f_name;
    std::ifstream archive(arch_name, std::ios::in);
    std::ofstream tmp("tmp.txt", std::ios::out | std::ios::trunc);
    std::ofstream extracted_file(extracted_f_name, std::ios::out | std::ios::trunc);
    if (!archive.is_open()) {
        std::cerr << "Error: Programme can't open " << arch_name << " file\nPlease, try again";
        exit(1);
    }
    if (!tmp.is_open()) {
        std::cerr << "Error: Something went wrong\n";
        exit(1);
    }
    if (!extracted_file.is_open()) {
        std::cerr << "Error: Programme can't open " << extracted_f_name << " file\nPlease, try again";
        exit(1);
    }
    char symbol;
    bool flag = false;
    while (archive.good()) {
        archive.get(symbol);
        if (symbol == '|' && !flag) {
            std::vector<char> filename;
            while (archive.peek() != '|') {
                archive.get(symbol);
                filename.push_back(symbol);
            }
            std::string compared_filename(filename.begin(), filename.end());
            if (f_name == compared_filename) {
                flag = true;
                archive.get(symbol); //здесь считывается вторая | у названия файла для корректной работы цикла while
                while (archive.peek() != '|' && !archive.eof()) {
                    std::vector<bool> block;
                    for (int i = 0; i < kSizeOfEncodedBlock; ++i) {
                        if (archive.eof() || archive.peek() == '|') { //если дошли до конца архива или конца файла
                            break;
                        }
                        archive.get(symbol);
                        block.push_back(symbol - kDifferenceInASCII);
                    }
                    std::vector<bool> decoded_block = DecodeHamming(block);
                    std::vector<bool> decoded_part_of_block;
                    for (int i = 0; i < decoded_block.size(); ++i) {
                        decoded_part_of_block.push_back(decoded_block[i]);
                        if ((i + 1) % 8 == 0) {
                            char converted_symbol = ConvertBoolVectorToChar(decoded_part_of_block);
                            extracted_file.put(converted_symbol);
                            decoded_part_of_block.clear();
                        }
                    }
                }
                if (!archive.eof()) {
                    archive.get(symbol); //считываем первую | у следующего файла
                }
            } else {
                tmp << '|' << compared_filename;
                archive.get(symbol); //считываем вторую | у названия файла, чтобы не входить вновь в этот if
            }
        }
        tmp.put(symbol);
    }
    archive.close();
    tmp.close();
    std::ofstream regenerated_archive(arch_name, std::ios::out | std::ios::trunc);
    std::ifstream read_tmp("tmp.txt", std::ios::in);
    if (!regenerated_archive.is_open()) {
        std::cerr << "Error: Programme can't open " << arch_name << " archive\nPlease, try again";
        exit(1);
    }
    if (!read_tmp.is_open()) {
        std::cerr << "Error: something went wrong\n";
        exit(1);
    }
    while (read_tmp.good()) {
        read_tmp.get(symbol);
        if (regenerated_archive.good()) {
            regenerated_archive.put(symbol);
        }
    }
    read_tmp.close();
    std::ofstream tmp_clear("tmp.txt", std::ios::out | std::ios::trunc);
    tmp_clear.close();
    regenerated_archive.close();
    std::cout << extracted_f_name << " was extracted from " << arch_name << " archive\n";
}

void ExtractAllFiles(const Arguments& args) {
    std::string arch_name(args.archive_name_to_extract_);
    arch_name += ".haf";
    std::ifstream archive(arch_name, std::ios::in);
    char symbol;
    while (archive.good()) {
        archive.get(symbol);
        if (symbol == '|') {
            std::vector<char> filename;
            while (archive.peek() != '|') {
                archive.get(symbol);
                filename.push_back(symbol);
            }
            std::string ex_f_name(filename.begin(), filename.end());
            std::string extracted_filename = ex_f_name;
            std::ofstream extracted_file(extracted_filename, std::ios::out | std::ios::trunc);
            archive.get(symbol); //здесь считывается вторая | у названия файла для корректной работы цикла while
            while (archive.peek() != '|' && !archive.eof()) {
                std::vector<bool> block;
                for (int i = 0; i < kSizeOfEncodedBlock; ++i) {
                    if (archive.eof() || archive.peek() == '|') { //если дошли до конца архива или конца файла
                        break;
                    }
                    archive.get(symbol);
                    block.push_back(symbol - kDifferenceInASCII);
                }
                std::vector<bool> decoded_block = DecodeHamming(block);
                std::vector<bool> decoded_part_of_block;
                for (int i = 0; i < decoded_block.size(); ++i) {
                    decoded_part_of_block.push_back(decoded_block[i]);
                    if ((i + 1) % 8 == 0) {
                        char converted_symbol = ConvertBoolVectorToChar(decoded_part_of_block);
                        extracted_file.put(converted_symbol);
                        decoded_part_of_block.clear();
                    }
                }
            }
            extracted_file.close();
        }
    }
    archive.close();
    std::ofstream clear_archive(arch_name, std::ios::out | std::ios::trunc);
    clear_archive.close();
    std::cout << "All files were extracted from " << arch_name << " archive\n";
}

void DeleteFile(const Arguments& args) {
    std::string arch_name(args.archive_name_to_delete_);
    arch_name += ".haf";
    std::ifstream archive(arch_name, std::ios::in);
    if (!archive.is_open()) {
        std::cerr << "Error: Programme can't open " << arch_name << " archive\nPlease, try again";
        exit(1);
    }
    std::string deleted_filename(args.filename_to_delete_);
    std::ofstream tmp("tmp.txt", std::ios::out | std::ios::trunc);
    if (!tmp.is_open()) {
        std::cerr << "Error: something went wrong\n";
        exit(1);
    }
    char symbol;
    bool flag = false; //чтобы удалить только один файл(он и так один удалится, но таким образом не будут происходить лишние проверки)
    while (archive.good()) {
        archive.get(symbol); //считываем символ
        if (symbol == '|' && !flag) {
            std::vector<char> filename; //сюда запишем имя файла, который мы встретили
            while (archive.peek() != '|') { //из этого следует, что после цикла будет считываться именно |
                archive.get(symbol);
                filename.push_back(symbol);
            }
            std::string compared_filename(filename.begin(), filename.end());
            if (deleted_filename == compared_filename) { //нужный файл найден
                flag = true;
                archive.get(symbol); //здесь считывается вторая | у названия файла для корректной работы цикла while
                while (archive.peek() != '|' && !archive.eof()) { //считываем, но никуда не записываем удаляемый файл
                    archive.get(symbol);
                }
                archive.get(symbol); //здесь считывается первая | у названия следующего файла, чтобы при выходе из if'а не записывать в tmp последний символ удаляемого файла
            } else { //если же не найден, то просто
                tmp << '|' << compared_filename;
                archive.get(symbol); //считываем вторую | у названия файла, чтобы не входить вновь в этот if
            }
        }
        tmp.put(symbol); //либо вставляем символ, который считали изначально, либо записываем первую | следующего файла, либо первый символ файла
    }
    archive.close();
    tmp.close();
    std::ofstream regenerated_archive(arch_name, std::ios::out | std::ios::trunc);
    std::ifstream read_tmp("tmp.txt", std::ios::in);
    if (!regenerated_archive.is_open()) {
        std::cerr << "Error: Programme can't open " << arch_name << " archive\nPlease, try again";
        exit(1);
    }
    if (!read_tmp.is_open()) {
        std::cerr << "Error: something went wrong\n";
        exit(1);
    }
    while (read_tmp.good()) {
        read_tmp.get(symbol);
        if (regenerated_archive.good()) {
            regenerated_archive.put(symbol);
        }
    }
    read_tmp.close();
    regenerated_archive.close();
    std::ofstream tmp_clear("tmp.txt", std::ios::out | std::ios::trunc);
    tmp_clear.close();
    std::cout << deleted_filename << " was deleted from " << arch_name << " archive\n";
}

void ConcatenateArchives(const Arguments& args) {
    std::string m_arch_name(args.archive_name_to_concatenate_);
    m_arch_name += ".haf";
    std::ofstream merged_archive(m_arch_name, std::ios::out | std::ios::app);
    if (!merged_archive.is_open()) {
        std::cerr << "Error: Programme can't open " << m_arch_name << " archive\nPlease, try again";
        exit(1);
    }
    std::string arch_name1(args.archive1_to_concatenate_);
    arch_name1 += ".haf";
    std::string arch_name2(args.archive2_to_concatenate_);
    arch_name2 += ".haf";
    std::ifstream archive1(arch_name1, std::ios::in);
    if (!archive1.is_open()) {
        std::cerr << "Error: Programme can't open " << arch_name1 << " archive\nPlease, try again";
        exit(1);
    }
    char symbol;
    while (archive1.good()) {
        archive1.get(symbol);
        if (merged_archive.good()) {
            merged_archive.put(symbol);
        }
    }
    archive1.close();
    std::ifstream archive2(arch_name2, std::ios::in);
    if (!archive2.is_open()) {
        std::cerr << "Error: Programme can't open " << arch_name2 << " archive\nWe copied first archive into the new one\nIf you need two similar archives, write YES, else write NO\n";
        std::string answer;
        std::cin >> answer;
        if (answer == "NO") {
            std::cerr << "Fatal\n";
            exit(1);
        } else if (answer != "YES") {
            std::cerr << "There's a typo, but who cares\nFatal\n";
            exit(1);
        }
        return;
    }
    while (archive2.good()) {
        archive2.get(symbol);
        if (merged_archive.good()) {
            merged_archive.put(symbol);
        }
    }
    archive2.close();
    std::cout << "Archive " << m_arch_name << " was created by concatenate " << arch_name1 << " and " << arch_name2 << '\n';
    merged_archive.close();
}

void MakeList(const Arguments& args) {
    std::string arch_name(args.archive_name_to_create_list_);
    arch_name += ".haf";
    std::ifstream archive(arch_name, std::ios::in);
    if (!archive.is_open()) {
        std::cerr << "Error: Programme can't open " << arch_name << " archive\nPlease, try again\n";
        exit(1);
    }
    std::cout << "List of " << arch_name << " archive:\n";
    char symbol;
    double counter = 0;
    double size_of_file;
    double size_of_archive = 0;
    while (archive.good()) {
        archive.get(symbol);
        if (symbol == '|') {
            if (counter != 0) {
                size_of_file = counter / 1024;
                size_of_archive += size_of_file;
                std::cout << "Size in archive: " << size_of_file << " KByte\n";
                counter = 0;
            }
            std::vector<char> filename;
            while (archive.peek() != '|') {
                archive.get(symbol);
                filename.push_back(symbol);
            }
            std::cout << "- ";
            std::string file(filename.begin(), filename.end());
            std::cout << file << '\n';
            archive.get(symbol); //считываем второе |
        }
        ++counter;
    }
    size_of_file = counter / 1024;
    size_of_archive += size_of_file;
    std::cout << "Size in archive: " << size_of_file << " KByte\n";
    std::cout << "Archive's list was created\nSize of all files in archive: " << size_of_archive << " KByte\n";
    archive.close();
}