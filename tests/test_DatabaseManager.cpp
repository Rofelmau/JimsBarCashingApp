#include <gtest/gtest.h>
#include "../src/DatabaseManager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

class DatabaseManagerTest : public ::testing::Test
{

};

TEST_F(DatabaseManagerTest, DatabaseConnection) {
    DatabaseManager uut;

    EXPECT_TRUE(uut.isOpen());
}

TEST_F(DatabaseManagerTest, ExecuteQuery) {
    DatabaseManager uut;

    QSqlDatabase db = uut.database();
    ASSERT_TRUE(db.isOpen());

    QSqlQuery query(db);
    ASSERT_TRUE(query.exec("SELECT 1"));
    ASSERT_TRUE(query.next());
    EXPECT_EQ(query.value(0).toInt(), 1);
}
