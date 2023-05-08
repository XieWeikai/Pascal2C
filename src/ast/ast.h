#ifndef AST_H
#define AST_H

#include <iostream>
#include <sstream>

#define GETTER(type, name) \
    const type &name() const { return name##_; }

namespace pascal2c::ast
{
    // Abstract Syntax Tree
    class Ast
    {
    public:
        // param:
        //     line is the line number of the first token of the node
        //     column is the column number of the first token of the node
        Ast(const int line, const int column)
            : line_(line), column_(column){};

        Ast() : line_(1), column_(1){};

        inline void SetLineAndColumn(const int line, const int column)
        {
            line_ = line;
            column_ = column;
        }

        inline virtual const int &line() const { return line_; }

        inline virtual const int &column() const { return column_; }

    protected:
        // for test use
        // param:
        //     str_s is the string stream to output to
        //     level is the indentation level
        inline static void
        IndentOutput(std::stringstream &str_s, const int level)
        {
            int temp = level;
            while (temp-- > 0)
                str_s << "    ";
        }

        // for test use
        // param:
        //     str_s is the string stream to output to
        inline void LineColumnOutput(std::stringstream &str_s) const
        {
            str_s << line_ << ":" << column_ << " ";
        }

    private:
        int line_;   // line number of the first token of the node
        int column_; // column number of the first token of the node
    };
}

#endif
