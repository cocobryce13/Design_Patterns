#include "sql_query_builder.h"
#include <iostream>

int main()
{
    SqlSelectQueryBuilder query_builder;
    query_builder.AddWhere("id", "42");
    query_builder.AddColumn("name").AddColumn("phone");
    query_builder.AddFrom("students");
    query_builder.AddWhere("name", "John");
    std::cout << query_builder.BuildQuery() << std::endl;

    SqlSelectQueryBuilder qb2;
    qb2.AddFrom("t");
    std::cout << qb2.BuildQuery() << std::endl;

    SqlSelectQueryBuilder qb3;
    qb3.AddFrom("old").AddFrom("new");
    std::cout << qb3.BuildQuery() << std::endl;
}
