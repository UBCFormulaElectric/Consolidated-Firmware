#include "critTestBase.hpp"
#include "io_sevenSeg.hpp"
#include <iostream>

class TestSevenSeg : public CRITBaseTest
{
};

static void RENDER(const char reference, const io::seven_seg::digit &ddd)
{
    // draws seven seg in console
    // Layout:
    //  aaa
    // f   b
    //  ggg
    // e   c
    //  ddd  (dp)

    const char *top = ddd.a ? " ### " : "     ";
    const char *upl = ddd.f ? "#" : " ";
    const char *upr = ddd.b ? "#" : " ";
    const char *mid = ddd.g ? " ### " : "     ";
    const char *dnl = ddd.e ? "#" : " ";
    const char *dnr = ddd.c ? "#" : " ";
    const char *bot = ddd.d ? " ### " : "     ";
    const char *dp  = ddd.dot ? "." : " ";

    std::cout << "drawing " << reference << std::endl;
    std::cout << top << std::endl;
    std::cout << upl << "   " << upr << std::endl;
    std::cout << upl << "   " << upr << std::endl;
    std::cout << mid << std::endl;
    std::cout << dnl << "   " << dnr << std::endl;
    std::cout << dnl << "   " << dnr << std::endl;
    std::cout << bot << dp << std::endl;
    std::cout << std::endl;
}

TEST_F(TestSevenSeg, seven_seg_rendering)
{
    RENDER('0', zero);
    RENDER('1', one);
    RENDER('2', two);
    RENDER('3', three);
    RENDER('4', four);
    RENDER('5', five);
    RENDER('6', six);
    RENDER('7', seven);
    RENDER('8', eight);
    RENDER('9', nine);
    RENDER('a', a);
    RENDER('b', b);
    RENDER('c', c);
    RENDER('d', d);
    RENDER('e', e);
    RENDER('f', f);
    RENDER('g', g);
    RENDER('h', h);
    RENDER('i', i);
    RENDER('j', j);
    RENDER('l', l);
    RENDER('n', n);
    RENDER('o', o);
    RENDER('p', p);
    RENDER('q', q);
    RENDER('r', r);
    RENDER('s', s);
    RENDER('t', t);
    RENDER('u', u);
    RENDER('y', y);
}