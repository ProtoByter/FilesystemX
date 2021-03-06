//
// Created by bobro on 12/08/2020.
//

#ifndef FILESYSTEMX_FILESYSTEM_CPP17_HPP
#define FILESYSTEMX_FILESYSTEM_CPP17_HPP

#include "support.hpp"

#ifdef CPP17
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
#endif
#ifdef CPP14
#include <experimental/filesystem>
#include <fstream>
namespace fs = std::experimental::filesystem;
#endif

namespace ProtoFS {
    class FilesystemX : public FilesystemTemplate {
    public:
        FilesystemX(std::string _path) {
            path = _path;
        }

        std::vector<fileEntry> ls() override {
            std::vector<fileEntry> ret;
            for (const fs::directory_entry &entry : fs::directory_iterator(path)) {
                // Is it a file / directory?
                fileType type = File;
                if (fs::is_directory(entry)) {
                    type = Folder;
                }
                auto path = entry.path();
                std::string pathString = path.string();
                std::string filenameString = path.filename().string();
                ret.emplace_back(fileEntry(filenameString, pathString, type));
            }
            #ifdef FLX_IncludeSpecialDirs
            ret.insert(ret.begin(),fileEntry(".",path+".",Folder));
            ret.insert(ret.begin(),fileEntry("..",path+"..",Folder));
            #endif
            return ret;
        }

        bool mk(fileEntry entry) override {
            if (entry.type == File) {
                std::ofstream file(entry.filePath.c_str());
                return file.is_open();
            }
            else {
                return fs::create_directory(entry.filePath);
            }
        }

        bool rm(fileEntry entry) override {
            return fs::remove(entry.filePath);
        }
    };
}

#endif //FILESYSTEMX_FILESYSTEM_CPP17_HPP
