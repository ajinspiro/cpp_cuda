#include <ranges>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

// sample input (1,2) (-2,3) (5,6)
int main(void)
{
    std::cout << "Linear regression: Enter data set in the format (x,y) (x,y) (x,y). The format is whitespace sensitive."
              << std::endl
              << "> ";
    std::string input = {};
    std::vector<std::tuple<double, double>> dataset = {};
    std::tuple<double, double> x_y_sum_pair = {};
    double sum_x = 0.0, sum_y = 0.0, sum_x_squred = 0.0, sum_xy = 0.0, beta_0 = 0.0, beta_1 = 0.0, y_mean = 0.0, x_mean = 0.0;
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
    sum_x = std::get<0>(x_y_sum_pair);
    sum_y = std::get<1>(x_y_sum_pair);
    sum_xy = std::transform_reduce(
        dataset.begin(),
        dataset.end(),
        0.0,                                       // reduce start value
        std::plus<>(),                             // reduce operation
        [](std::tuple<double, double> cordinate) { // transform operation
            return std::get<0>(cordinate) * std::get<1>(cordinate);
        });
    sum_x_squred = std::transform_reduce(
        dataset.begin(),
        dataset.end(),
        0.0,
        std::plus<>(),
        [](std::tuple<double, double> cordinate)
        {
            return std::pow(std::get<0>(cordinate), 2);
        });
    beta_1 = ((dataset.size() * sum_xy) - (sum_x * sum_y)) / ((dataset.size() * sum_x_squred) - std::pow(sum_x, 2));
    x_mean = sum_x / dataset.size();
    y_mean = sum_y / dataset.size();
    std::cout << "beta 1: " << beta_1 << std::endl;
    beta_0 = y_mean - (beta_1 * x_mean);
    std::cout << "beta 0: " << beta_0 << std::endl;
    return EXIT_SUCCESS;
}