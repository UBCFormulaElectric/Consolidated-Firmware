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
    constexpr float fz_N     = 1000.0f;

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

    for (const float alpha_rad : { 0.3f, 0.25f, 0.2f, 0.15f, 0.1f, 0.05f, 0.0f })
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

int main()
{
    constexpr app::tv::shared_datatypes::VehicleState<double> state{
        .v_x_mps        = 0,
        .v_y_mps        = 0,
        .yaw_rate_radps = 0,
        .a_x_mps2       = 1,
        .a_y_mps2       = 0,
        .apps           = 0.2,
        .delta          = 0,
    };
    const auto [fl, fr, rl, rr] = app::tv::controllers::allocator::optimize(state, 10.0, 0.0);
    std::cout << fl << " " << fr << " " << rl << " " << rr << std::endl;
}