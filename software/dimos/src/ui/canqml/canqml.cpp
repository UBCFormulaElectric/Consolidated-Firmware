#include "canqml.h"

CanQML *CanQML::getInstance()
{
    static auto *instance = new CanQML();
    return instance;
}

CanQML *CanQML::create(const QQmlEngine *qmlEngine, const QJSEngine *jsEngine)
{
    Q_UNUSED(qmlEngine)
    Q_UNUSED(jsEngine)
    return getInstance();
}

void CanQML::fault_poll()
{
    Fault_Warning_Info faults_arr[10];
    Fault_Warning_Info warnings_arr[10];
    const uint8_t      warning_count = app_canAlerts_WarningInfo(warnings_arr);
    const uint8_t      fault_count   = app_canAlerts_FaultInfo(faults_arr);
    if (fault_count == 0)
    {
        has_fault = false;
        emit faultChanged();
    }
    else if (!has_fault)
    {
        has_fault = true;
        emit faultChanged();
    }

    if (const bool has_faults_changed = fault_consolidation(faults, faults_arr, fault_count))
        emit faults_changed();
    if (const bool has_warnings_changed = fault_consolidation(warnings, warnings_arr, warning_count))
        emit warnings_changed();
}

bool CanQML::fault_consolidation(QList<Q_Fault_Warning_Info *> &a, Fault_Warning_Info *b, int b_size)
{
    bool has_changed = false;
    int  ai = 0, bi = 0;
    while (ai < faults.size() && bi < b_size)
    {
        if (*faults[ai] == b[bi])
        {
            ai++;
            bi++;
        }
        else if (a[ai]->get_id() < b[bi].id)
        {
            a.erase(a.begin() + ai, a.begin() + ai + 1);
            has_changed = true;
        }
        else
        {
            a.insert(a.begin() + ai, new Q_Fault_Warning_Info(b[bi]));
            ai++;
            bi++;
            has_changed = true;
        }
    }
    if (bi < b_size)
    {
        for (int i = bi; i < b_size; i++)
            a.append(new Q_Fault_Warning_Info(b[i]));
        has_changed = true;
    }
    else if (ai < a.size())
    {
        a.erase(a.begin() + ai, a.end());
        has_changed = true;
    }

    return has_changed;
}

int CanQML::get_first_error_node()
{
    return -1; // TODO implement faults when they arrive
}
