namespace io::batteryCharging
{
void charger_enable(void);
void charger_disable(void);
bool check_status(void);
} // namespace io::batteryCharging

enum enable
{
    LOW,
    HIGH
};