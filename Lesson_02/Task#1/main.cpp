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
    std::string sql = qb
        .AddColumn("id")
        .AddFrom("users")
        .AddWhere("active", "1")
        .BuildQuery();

    std::cout << sql << std::endl;

    return 0;
}
