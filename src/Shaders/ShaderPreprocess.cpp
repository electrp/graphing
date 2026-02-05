//
// Created by willb on 9/6/2025.
//

#include "ShaderPreprocess.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <map>
#include <sstream>

std::string shaderPreprocess(
    std::string filename,
    std::set<std::string>* read_files,
    std::map<std::string, std::string>* defines
) {
    std::set<std::string> nrf;
    if (read_files == nullptr) {
        read_files = &nrf;
    }
    std::map<std::string, std::string> def;
    if (defines == nullptr) {
        defines = &def;
    }

    read_files->insert(filename);

    std::ifstream in {filename};
    std::string line;
    std::ostringstream out;


    // I HATE WINDOWS !!!
    // (bom detection)
    while (!in.eof()) {
        char c = in.peek();
        if (c == static_cast<char>(0xEF) || c == static_cast<char>(0xBB) || c == static_cast<char>(0xBF))
            in.seekg(1, std::ios_base::cur);
        else
            break;
    }

    while (std::getline(in, line)) {
        if (line.size() == 0) {
            out << '\n';
            continue;
        }

        if (line[0] == '#') {
            std::stringstream t1 {line};
            std::string intermediate;
            if (std::getline(t1, intermediate, ' ')) {
                if (intermediate == "#include") {
                    if (std::getline(t1, intermediate, '\n')) {
                        if (intermediate.back() == '\r')
                            intermediate.pop_back();;
                        // intermediate is now filename
                        if (read_files->contains(intermediate))
                            continue;

                        out << shaderPreprocess(intermediate, read_files, defines);
                        out << '\n';
                    }
                } else if (intermediate == "#ifdef") {
                    if (std::getline(t1, intermediate, '\n')) {
                        if (defines->contains(intermediate)) {
                            int depth = 1;
                            while (depth > 0 && std::getline(in, line)) {
                                t1 = std::stringstream {line};
                                std::getline(t1, intermediate, ' ');
                                if (intermediate == "#ifdef")
                                    depth++;
                                if (intermediate == "#endif")
                                    depth--;
                            }
                        }
                    }
                } else if (intermediate == "#define") {
                    if (std::getline(t1, intermediate, ' ')) {
                        std::string i2;
                        if (std::getline(t1, i2, '\n')) {
                            defines->insert({intermediate, i2});
                        }
                    }
                }
            }
        }
        else {
            std::regex regex {R"(\b\w+\b)"};
            std::smatch matches;
            if (std::regex_match(line, matches, regex)) {
                for (auto& match : matches) {
                    std::string match_str {match.str()};
                    if (auto def_it = defines->find(match_str); def_it != defines->end()) {
                        auto pos = match.first - line.begin();
                        line.erase(match.first, match.second);
                        line.insert(line.begin() + pos, match_str.begin(), match_str.end());
                    }
                }
            }
            out << line;
            out << '\n';
        }
    }

    // std::cout << out.str() << std::endl;;
    return out.str();
}
