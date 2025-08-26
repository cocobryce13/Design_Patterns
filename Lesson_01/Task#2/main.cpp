#include "sql_query_builder.h"
#include <iostream>
#include <map>
#include <vector>

int main()
{
    SqlSelectQueryBuilder qb;
    qb.AddColumns({"name", "phone", "group"})
      .AddFrom("students")
      .AddWhere({{"id", "42"}, {"name", "John"}});

    std::cout << qb.BuildQuery() << std::endl;

    SqlSelectQueryBuilder qb2;
    qb2.AddFrom("t");
    std::cout << qb2.BuildQuery() << std::endl;

}
