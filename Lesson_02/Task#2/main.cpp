#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

class SqlSelectQueryBuilder
{
public:
    SqlSelectQueryBuilder& AddColumn(const std::string& column) noexcept
    {
        if (!column.empty())
        {
            columns_.push_back(column);
        }
        return *this;
    }

    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columns) noexcept
    {
        for (std::size_t i = 0; i < columns.size(); ++i)
        {
            if (!columns[i].empty())
            {
                columns_.push_back(columns[i]);
            }
        }
        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& table) noexcept
    {
        if (!table.empty())
        {
            table_ = table;
        }
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::string& key, const std::string& value) noexcept
    {
        if (!key.empty())
        {
            where_[key] = value;
        }
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept
    {
        for (std::map<std::string, std::string>::const_iterator it = kv.begin(); it != kv.end(); ++it)
        {
            if (!it->first.empty())
            {
                where_[it->first] = it->second;
            }
        }
        return *this;
    }

    std::string BuildQuery() const
    {
        std::ostringstream out;

        out << "SELECT ";
        if (columns_.empty())
        {
            out << "*";
        }
        else
        {
            for (std::size_t i = 0; i < columns_.size(); ++i)
            {
                out << columns_[i];
                if (i + 1 < columns_.size())
                {
                    out << ", ";
                }
            }
        }

        out << " FROM ";
        if (table_.empty())
        {
            out << "<table_not_set>";
        }
        else
        {
            out << table_;
        }

        if (!where_.empty())
        {
            out << " WHERE ";
            std::size_t i = 0;
            for (std::map<std::string, std::string>::const_iterator it = where_.begin(); it != where_.end(); ++it)
            {
                out << it->first << "='" << it->second << "'";
                if (++i < where_.size())
                {
                    out << " AND ";
                }
            }
        }

        out << ";";
        return out.str();
    }

private:
    std::vector<std::string> columns_;
    std::string table_;
    std::map<std::string, std::string> where_;
};

int main()
{
    SqlSelectQueryBuilder qb;

    std::vector<std::string> cols;
    cols.push_back("id");
    cols.push_back("name");
    cols.push_back("age");

    std::map<std::string, std::string> conds;
    conds["country"] = "RU";
    conds["active"] = "1";

    std::string sql = qb
        .AddColumns(cols)
        .AddFrom("users")
        .AddWhere(conds)
        .AddWhere("role", "engineer")
        .BuildQuery();

    std::cout << sql << std::endl;

    return 0;
}
