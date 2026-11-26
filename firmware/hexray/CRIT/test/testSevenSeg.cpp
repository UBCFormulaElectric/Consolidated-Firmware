#include "critTestBase.hpp"
#include "io_sevenSeg.hpp"
#include <iostream>

class TestSevenSeg : public CRITBaseTest
{
};

static void RENDER(const char reference)
{
    // draws seven seg in console
    // Layout:
    //  aaa
    // f   b
    //  ggg
    // e   c
    //  ddd  (dp)

    const io::seven_seg::digit ddd = io::seven_seg::char_to_segment(reference);

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
    RENDER('0');
    RENDER('1');
    RENDER('2');
    RENDER('3');
    RENDER('4');
    RENDER('5');
    RENDER('6');
    RENDER('7');
    RENDER('8');
    RENDER('9');
    RENDER('a');
    RENDER('b');
    RENDER('c');
    RENDER('d');
    RENDER('e');
    RENDER('f');
    RENDER('g');
    RENDER('h');
    RENDER('i');
    RENDER('j');
    RENDER('k');
    RENDER('l');
    RENDER('m');
    RENDER('n');
    RENDER('o');
    RENDER('p');
    RENDER('q');
    RENDER('r');
    RENDER('s');
    RENDER('t');
    RENDER('u');
    RENDER('y');
    RENDER('.');
}