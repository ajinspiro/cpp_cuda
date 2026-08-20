#include <ranges>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

// sample input (1,2) (-2,3)
int main(void)
{
    std::cout << "Linear regression: Enter data set in the format (x,y) (x,y) (x,y). The format is whitespace sensitive."
              << std::endl
              << "> ";
    std::string input = {};
    std::vector<std::tuple<double, double>> dataset = {};
    std::getline(std::cin, input);
    auto input_parts = input | std::views::split(' ');
    for (auto parenthesised_cordinate_pair : input_parts)
    {
        auto parenthesised_cordinate_pair_sv = std::string_view(parenthesised_cordinate_pair.begin(), parenthesised_cordinate_pair.end());
        auto parenthesis_stripped_cordinate_sv = parenthesised_cordinate_pair_sv.substr(1, parenthesised_cordinate_pair_sv.length() - 2);
        // split further by comma
        auto separated_cordinate_pair = parenthesis_stripped_cordinate_sv | std::views::split(',');
        for (auto [index, cordinate_component_string] : separated_cordinate_pair | std::views::enumerate)
        {
            auto temp = std::string(std::string_view(cordinate_component_string));
            double cordinate_component = std::stod(temp);
            if (index == 0)
            {
                dataset.push_back(std::tuple<double, double>(cordinate_component, 0));
            }
            else
            {
                std::get<1>(dataset.back()) = cordinate_component;
            }
        }
    }

    return EXIT_SUCCESS;
}