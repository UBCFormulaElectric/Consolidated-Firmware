#include "Test_Shared.h"

extern "C"
{
#include "App_SharedErrorTable.h"
}

class SharedErrorTableTest : public testing::Test
{
  protected:
    void SetUp() override { error_table = App_SharedErrorTable_Create(); }
    void TearDown() override
    {
        TearDownObject(error_table, App_SharedErrorTable_Destroy);
    }

    struct ErrorTable *error_table;
};

TEST_F(SharedErrorTableTest, get_all_errors)
{
    // TODO: How do we prepare the "errors[]" argument for this function?
    // App_SharedErrorTable_GetAllErrors(error_table, )
}
