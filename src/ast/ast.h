#ifndef AST_H
#define AST_H

#include <iostream>
#include <sstream>

namespace pascal2c::ast
{
    class Ast
    {
    public:
        // param:
        //     line is the line number of the first token of the node
        //     column is the column number of the first token of the node
        Ast(const int &line, const int &column)
            : line_(line), column_(column){};

        Ast() : line_(1), column_(1){};

        void SetLineAndColumn(const int &line, const int &column)
        {
            line_ = line;
            column_ = column;
        }

        virtual const int &line() const { return line_; }

        virtual const int &column() const { return column_; }

    protected:
        // param:
        //     str_s is the string stream to output to
        //     level is the indentation level
        static void
        IndentOutput(std::stringstream &str_s, const int &level)
        {
            int temp = level;
            while (temp-- > 0)
                str_s << "    ";
        }

    private:
        int line_;   // line number of the first token of the node
        int column_; // column number of the first token of the node
    };
}

#endif
