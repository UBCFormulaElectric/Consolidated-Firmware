// ReSharper disable CppTooWideScope
#include "matplotlibcpp.h"
#include "torque_vectoring/estimation/tire_model.hpp"
#include "torque_vectoring/controllers/torque_allocator.hpp"

#include <vector>
#include <string>
#include <iomanip>

namespace plt = matplotlibcpp;

static void plot_combined_fx()
{
    constexpr int   n_points = 301; // includes endpoints
    constexpr float fz_N     = 700.0f;

    std::vector<double> kappas;
    kappas.reserve(n_points);
    for (int i = 0; i < n_points; ++i)
    {
        constexpr double kappa_max = 0.6f;
        constexpr double kappa_min = -0.6f;
        const double     t         = static_cast<float>(i) / static_cast<float>(n_points - 1);
        const double     kappa     = kappa_min + t * (kappa_max - kappa_min);
        kappas.push_back(kappa);
    }

    for (const float alpha_rad : { 0.15f, 0.125f, 0.1f, 0.075f, 0.05f, 0.025f, 0.0f })
    {
        std::vector<double> fxs;
        fxs.reserve(kappas.size());
        for (const double kappa : kappas)
        {
            fxs.push_back(app::tv::estimation::tire_model.computeCombinedFx_N<double>(fz_N, alpha_rad, kappa));
        }
        std::stringstream ss;
        ss << "\\alpha = " << std::setprecision(2) << alpha_rad;
        plt::named_plot(ss.str(), kappas, fxs);
    }

    plt::title("F_x as a function of \\kappa and \\alpha");
    plt::xlabel("\\kappa");
    plt::ylabel("F_x (N)");
    plt::grid(true);
    plt::legend();
    plt::show();
}

static void plot_combined_fy()
{
    constexpr int   n_points = 301; // includes endpoints
    constexpr float fz_N     = 700.0f;

    std::vector<float> alphas;
    alphas.reserve(n_points);
    for (int i = 0; i < n_points; ++i)
    {
        constexpr float alpha_max = 0.6f;
        constexpr float alpha_min = -0.6f;
        const float     t         = static_cast<float>(i) / static_cast<float>(n_points - 1);
        const float     alpha     = alpha_min + t * (alpha_max - alpha_min);
        alphas.push_back(alpha);
    }

    for (const double kappa : { 0.3, 0.25, 0.2, 0.15, 0.1, 0.05, 0.0 })
    {
        std::vector<double> fys;
        fys.reserve(alphas.size());
        for (const float alpha : alphas)
        {
            fys.push_back(app::tv::estimation::tire_model.computeCombinedFy_N<double>(fz_N, alpha, kappa));
        }
        std::stringstream ss;
        ss << "\\kappa = " << std::setprecision(2) << kappa;
        plt::named_plot(ss.str(), alphas, fys);
    }

    plt::title("F_y as a function of \\alpha and \\kappa");
    plt::xlabel("\\alpha");
    plt::ylabel("F_y (N)");
    plt::grid(true);
    plt::legend();
    plt::show();
}

int main()
{
    // constexpr app::tv::shared_datatypes::VehicleState<double> state{
    //     .v_x_mps        = 10,
    //     .v_y_mps        = 0,
    //     .yaw_rate_radps = 0,
    //     .a_x_mps2       = 1,
    //     .a_y_mps2       = 0,
    //     .apps           = 0.2,
    //     .delta          = 0,
    // };
    // const auto [fl, fr, rl, rr] = app::tv::controllers::allocator::optimize(state, 10.0, 5.0);
    // std::cout << "Optimal slip found: " << fl << " " << fr << " " << rl << " " << rr << std::endl;

    // plot_combined_fx();
    plot_combined_fy();
}