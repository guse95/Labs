#include <iostream>
#include <string>
#include <regex>
#include <stack>
#include <fstream>

bool isVariable(const std::string& variable) {
    std::regex mask("^[a-zA-Z_][a-zA-Z0-9_]*$");
    return std::regex_match(variable, mask);
}

std::string cdecl_translate(const std::string& declaration) {
    try {
        std::smatch match;
        std::regex trash(R"(\s*;\s*)");
        std::regex base_mask(
                R"(^\s*(int|char|float|double|void)\s+((\**\s*)?([a-zA-Z_][a-zA-Z0-9_]*))(\[.*\])*(\s*;\s*)?$)"
        );
        std::regex func_mask(
                R"(^\s*(int|char|float|double|void)\s+\(((\**\s*)?([a-zA-Z_][a-zA-Z0-9_]*))(\[.*\])*\)(\(.*\))?(\s*;\s*)?$)"
        );

        if (!std::regex_match(declaration, match, base_mask) && !std::regex_match(declaration, match, func_mask)) {
            return "Syntax error in declaration: " + declaration;
        }

        std::string base_type = match[1].str();
        std::string pointer = match[3].str();
        std::string identifier = match[4].str();
        std::string array_part = match[5].str();
        std::string func_part = match[6].str();

        if (!isVariable(identifier)) {
            return "Syntax error at position " + std::to_string(declaration.find(identifier));
        }

        std::string description = "declare " + identifier + " as ";

        size_t pointerCount = std::count(pointer.begin(), pointer.end(), '*');



        if (!array_part.empty()) {
            std::regex arrayRegex(R"(\[([0-9]*)\])");
            std::sregex_iterator begin(array_part.begin(), array_part.end(), arrayRegex);
            std::sregex_iterator end;

            for (auto i = begin; i != end; ++i) {
                std::string size = (*i)[1].str();
                if (size.empty()) {
                    description += "array of ";
                } else {
                    description += "array of " + size + " elements of ";
                }
            }
        }

        for (size_t i = 0; i < pointerCount; ++i) {
            description += "pointer to ";
        }


        if (!func_part.empty() && !(std::regex_match(func_part, trash))) {
            description += "function returning ";
            description += base_type;
            description += " getting as args: " + func_part;
        } else {
            description += base_type;
        }

        return description;

    } catch (const std::exception& e) {
        return std::string("Error: ") + e.what();
    }
}

void interpretFile(const std::string& path_to_file) {
    std::ifstream file(path_to_file);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path_to_file << '\n';
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << cdecl_translate(line) << std::endl;
    }

    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::string path_to_file = argv[1];
    interpretFile(path_to_file);

    return 0;
}