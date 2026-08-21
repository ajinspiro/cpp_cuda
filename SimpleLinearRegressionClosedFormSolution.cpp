#include <ranges>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <numeric>
#include <algorithm>

// sample input (1,2) (-2,3) (5,6)
int main(void)
{
    std::cout << "Linear regression: Enter data set in the format (x,y) (x,y) (x,y). The format is whitespace sensitive."
              << std::endl
              << "> ";
    std::string input = {};
    std::vector<std::tuple<double, double>> dataset = {};
    std::tuple<double, double> x_y_sum_pair = {};
    double x_mean = 0.0, y_mean = 0.0;
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
    x_y_sum_pair = std::reduce(
        dataset.begin(),
        dataset.end(),
        std::tuple<double, double>(0, 0),
        [](std::tuple<double, double> cordinate_1, std::tuple<double, double> cordinate_2)
        {
            return std::tuple<double, double>(std::get<0>(cordinate_1) + std::get<0>(cordinate_2), std::get<1>(cordinate_1) + std::get<1>(cordinate_2));
        });
    x_mean = std::get<0>(x_y_sum_pair) / dataset.size();
    y_mean = std::get<1>(x_y_sum_pair) / dataset.size();
    std::cout << "x mean: " << x_mean << std::endl
              << "y mean: " << y_mean << std::endl;
    double sum_xy = std::transform_reduce(
        dataset.begin(),
        dataset.end(),
        0.0,                                       // reduce start value
        std::plus<>(),                             // reduce operation
        [](std::tuple<double, double> cordinate) { // transform operation
            return std::get<0>(cordinate) * std::get<1>(cordinate);
        });
    std::cout << "sum XiYi: " << sum_xy << std::endl;
    return EXIT_SUCCESS;
}