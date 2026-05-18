#pragma once

namespace app
{
struct hsv
{
    double h; // angle in degrees
    double s; // a fraction between 0 and 1
    double v; // a fraction between 0 and 1
};

struct rgb
{
    double r; // a fraction between 0 and 1
    double g; // a fraction between 0 and 1
    double b; // a fraction between 0 and 1

    static rgb from(const hsv &in)
    {
        rgb out;
        if (in.s <= 0.0)
        { // < is bogus, just shuts up warnings
            out.r = in.v;
            out.g = in.v;
            out.b = in.v;
            return out;
        }
        double hh = in.h;
        if (hh >= 360.0)
            hh = 0.0;
        hh /= 60.0;
        const long   i  = static_cast<long>(hh);
        const double ff = hh - i;
        const double p  = in.v * (1.0 - in.s);
        const double q  = in.v * (1.0 - in.s * ff);
        const double t  = in.v * (1.0 - in.s * (1.0 - ff));

        switch (i)
        {
            case 0:
                out.r = in.v;
                out.g = t;
                out.b = p;
                break;
            case 1:
                out.r = q;
                out.g = in.v;
                out.b = p;
                break;
            case 2:
                out.r = p;
                out.g = in.v;
                out.b = t;
                break;

            case 3:
                out.r = p;
                out.g = q;
                out.b = in.v;
                break;
            case 4:
                out.r = t;
                out.g = p;
                out.b = in.v;
                break;
            case 5:
            default:
                out.r = in.v;
                out.g = p;
                out.b = q;
                break;
        }
        return out;
    }
};
} // namespace app