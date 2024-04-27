void custom_assert(int condition)
{
    // Your custom implementation here
    if (!condition)
    {
        for (;;)
            ;
    }
}