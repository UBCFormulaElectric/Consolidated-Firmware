#pragma once
namespace io::loadCell
{

class LoadCell
{
    hw::Adc *src;

  public:
    bool  is_ocsc();
    float getMechanicalLoad();

    inline LoadCell(hw::Adc *src) { this->src = src; }

  private:
    float voltageToMechancialLoad(float voltage);

}

static extern const LoadCell sensor3;
static extern const LoadCell sensor4;
} // namespace io::loadCell
