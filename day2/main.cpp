#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <cmath>

void print_help()
{
    std::cout << "--------------------------------------------------\n";
    std::cout << "Usage:\n";
    std::cout << "    arg1: path to op code file\n";
    std::cout << "--------------------------------------------------\n";
}

std::vector<int64_t> read_op_codes_from_file(const std::filesystem::path& path)
{
    std::ifstream f(path);
    if(!f.is_open())
        throw std::runtime_error("unable to open file");

    std::string input;
    f >> input;

    std::vector<int64_t> return_op_codes;
    std::string current_number;
    auto process_current_op_code = [&current_number, &return_op_codes]()
    {
        if(!current_number.empty())
        {
            int64_t n = stoll(current_number);
            return_op_codes.push_back(n);
            current_number.clear();
        }
    };

    for(int i = 0; i < input.size(); i++)
    {
        char c = input[i];
        if(isdigit(c))
            current_number += c;
        else
            process_current_op_code();
    }
    process_current_op_code();
    return return_op_codes;
}

void perform_op_1(size_t i, std::vector<int64_t>& op_codes)
{
    size_t a_index_index = i + 1;
    size_t b_index_index = i + 2;
    size_t result_index_index = i + 3;
    
    if(result_index_index >= op_codes.size())
        throw std::runtime_error("unexpected end of op code 1 data");

    size_t a_index = op_codes[a_index_index];
    size_t b_index = op_codes[b_index_index];
    size_t result_index = op_codes[result_index_index];

    op_codes[result_index] = op_codes[a_index] + op_codes[b_index];
}

void perform_op_2(size_t i, std::vector<int64_t>& op_codes)
{
    size_t a_index_index = i + 1;
    size_t b_index_index = i + 2;
    size_t result_index_index = i + 3;
    
    if(result_index_index >= op_codes.size())
        throw std::runtime_error("unexpected end of op code 2 data");

    size_t a_index = op_codes[a_index_index];
    size_t b_index = op_codes[b_index_index];
    size_t result_index = op_codes[result_index_index];

    op_codes[result_index] = op_codes[a_index] * op_codes[b_index];
}

int main( int argc, char** argv )
{
    try
    {
        if(argc != 2)
        {
            print_help();
            throw std::runtime_error("invalid arg count");
        }

        std::filesystem::path path(argv[1]);
        
        std::cout << "Reading op codes from " << path << "\n";
        auto op_codes = read_op_codes_from_file(path);
        auto original_op_codes = op_codes;

        op_codes[1] = 12;
        op_codes[2] = 2;

        for(size_t i = 0; i < op_codes.size(); i+=4)
        {
            int64_t op = op_codes[i];

            switch(op)
            {
                case 99:
                {
                    std::cout << "performing op: exit\n";
                    i = op_codes.size();
                    break;
                }

                case 1:
                {
                    std::cout << "performing op: 1\n";
                    perform_op_1(i, op_codes);
                    break;
                }

                case 2:
                {
                    std::cout << "performing op: 2\n";
                    perform_op_2(i, op_codes);
                    break;
                }

                default:
                {
                    std::stringstream ss;
                    ss << "unknown op code " << op;
                    throw std::runtime_error(ss.str());
                }
            }
        }

        std::cout << "Difference:\n";
        for(size_t i = 0; i < op_codes.size() && i < original_op_codes.size(); i++)
        {
            if(op_codes[i] == original_op_codes[i])
                continue;
            std::cout << "    index (" << i << "), before (" << op_codes[i] << "), after (" << original_op_codes[i] << ")\n";
        }

        std::cout << "Final op_codes:\n    ";
        for(auto n : op_codes)
            std::cout << n << ", ";
        std::cout << "\n";

        std::cout << "Position 0: " << op_codes[0] << "\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }    

    return EXIT_SUCCESS;
}