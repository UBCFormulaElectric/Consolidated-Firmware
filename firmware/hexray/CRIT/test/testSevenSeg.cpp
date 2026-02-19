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
    RENDER('0', io::seven_seg::zero);
    RENDER('1', io::seven_seg::one);
    RENDER('2', io::seven_seg::two);
    RENDER('3', io::seven_seg::three);
    RENDER('4', io::seven_seg::four);
    RENDER('5', io::seven_seg::five);
    RENDER('6', io::seven_seg::six);
    RENDER('7', io::seven_seg::seven);
    RENDER('8', io::seven_seg::eight);
    RENDER('9', io::seven_seg::nine);
    RENDER('a', io::seven_seg::a);
    RENDER('b', io::seven_seg::b);
    RENDER('c', io::seven_seg::c);
    RENDER('d', io::seven_seg::d);
    RENDER('e', io::seven_seg::e);
    RENDER('f', io::seven_seg::f);
    RENDER('g', io::seven_seg::g);
    RENDER('h', io::seven_seg::h);
    RENDER('i', io::seven_seg::i);
    RENDER('j', io::seven_seg::j);
    RENDER('k', io::seven_seg::k);
    RENDER('l', io::seven_seg::l);
    RENDER('m', io::seven_seg::m);
    RENDER('n', io::seven_seg::n);
    RENDER('o', io::seven_seg::o);
    RENDER('p', io::seven_seg::p);
    RENDER('q', io::seven_seg::q);
    RENDER('r', io::seven_seg::r);
    RENDER('s', io::seven_seg::s);
    RENDER('t', io::seven_seg::t);
    RENDER('u', io::seven_seg::u);
    RENDER('y', io::seven_seg::y);
    RENDER('.', io::seven_seg::dot);
}