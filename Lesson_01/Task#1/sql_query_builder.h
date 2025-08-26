#pragma once
#include <string>
#include <vector>
#include <utility>
#include <sstream>

class SqlSelectQueryBuilder
{
public:
    SqlSelectQueryBuilder& AddColumn(const std::string& column)
    {
        columns_.push_back(column);
        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& table)
    {
        from_ = table;
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::string& field, const std::string& value)
    {
        where_.emplace_back(field, value);
        return *this;
    }

    std::string BuildQuery() const
    {
        std::ostringstream oss;
        oss << "SELECT ";
        if (columns_.empty())
        {
            oss << "*";
        }
        else
        {
            for (size_t i = 0; i < columns_.size(); ++i)
            {
                oss << columns_[i];
                if (i + 1 < columns_.size())
                {
                    oss << ", ";
                }
            }
        }
        if (!from_.empty())
        {
            oss << " FROM " << from_;
        }
        if (!from_.empty() && !where_.empty())
        {
            oss << " WHERE ";
            for (size_t i = 0; i < where_.size(); ++i)
            {
                oss << where_[i].first << "=" << where_[i].second;
                if (i + 1 < where_.size())
                {
                    oss << " AND ";
                }
            }
        }
        oss << ";";
        return oss.str();
    }

private:
    std::vector<std::string> columns_;
    std::string from_;
    std::vector<std::pair<std::string, std::string>> where_;
};
