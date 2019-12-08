#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <cmath>

void print_help()
{
    std::cout << "--------------------------------------------------\n";
    std::cout << "Usage:\n";
    std::cout << "    arg1: path to module mass list file\n\n";
    std::cout << "Explanation:\n";
    std::cout << "    The module mass list should be a file containing a newline separated list of numbers representing the mass of each module\n";
    std::cout << "--------------------------------------------------\n";
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
        std::ifstream f(path);
        if(!f.is_open())
            throw std::runtime_error("unable to open file");

        std::cout << "Reading module mass list from " << path << "\n";

        int64_t total_fuel_required = 0;
        int64_t i = 0;
        while(!f.eof())
        {
            int64_t current_module_mass = 0;
            f >> current_module_mass;
            if(current_module_mass <= 0)
                continue;

            int64_t fuel_required = 0;
            static_cast<int64_t>(floor(static_cast<double>(current_module_mass) / 3.0)) - 2;
            total_fuel_required += fuel_required;
            std::cout << "    line (" << i << "), mass (" << current_module_mass << "), fuel required (" << fuel_required << ")\n";
            i++;
        }
        
        std::cout << "Total fuel required: " << total_fuel_required << "\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }    

    return EXIT_SUCCESS;
}