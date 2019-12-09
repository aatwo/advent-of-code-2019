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

int64_t calculate_fuel_required_for_mass(int64_t mass)
{
    return static_cast<int64_t>(floor(static_cast<double>(mass) / 3.0)) - 2;
}

int64_t calculate_fuel_required_for_module(int64_t module_mass)
{
    std::vector<int64_t> all_sub_fuel_values;

    int64_t total_fuel_required = 0;
    int64_t current_mass = module_mass;
    while(current_mass > 0)
    {
        // Get the fuel required for the current mass
        int64_t current_fuel_required = calculate_fuel_required_for_mass(current_mass);

        // Ignore negative fuel values
        if(current_fuel_required <= 0)
            break;

        // Add the fuel required to the running total
        all_sub_fuel_values.push_back(current_fuel_required);
        total_fuel_required += current_fuel_required;

        // The fuel required for the last mass becomes the next mass
        current_mass = current_fuel_required;
    }

    std::cout << "total fuel required (" << total_fuel_required << ") (";
    for(int i = 0; i < all_sub_fuel_values.size(); i++)
    {
        std::cout << all_sub_fuel_values[i];
        bool is_end = (i == all_sub_fuel_values.size() - 1);
        if(!is_end)
            std::cout << " + ";
        else
            std::cout << ")\n";
    }
    return total_fuel_required;
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

            std::cout << "    module (" << i << "), mass (" << current_module_mass << "), ";
            int64_t fuel_required = calculate_fuel_required_for_module(current_module_mass);
            total_fuel_required += fuel_required;
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