#include <algorithm>
#include <memory>
#include <vector>

#include "parser.h"

namespace pascal2c {
namespace parser {
using TokenTYpe = lexer::TokenType;
void Parser::Eat(const lexer::TokenType &token_type) {
    if (current_token_.getType() == token_type) {
        current_token_ = lexer_->GetNextToken();
    } else {
        Error("Token type mismatch.");
    }
}

/**
 * Read a Program(name, block)
 */
std::shared_ptr<semantic::Program> Parser::Program() {
    Eat(lexer::PROGRAM);
    auto name = current_token_.GetValue();
    Eat(lexer::IDENTIFIER);
    Eat(lexer::SEMICOLON);
    auto block = Block();
    Eat(lexer::DOT); // End of Program

    return std::make_shared<semantic::Program>(name, block);
}

/**
 * Read a block(declarations, compound_statement)
 */
std::shared_ptr<semantic::Block> Parser::Block() {
    // Implement the logic for parsing a block
    // For now, we will create an empty block with an empty compound statement
    auto declarations = Declarations();
    auto compound_statement = CompoundStatement();

    return std::make_shared<semantic::Block>(declarations, compound_statement);
}

std::vector<std::shared_ptr<semantic::VarDecl>> Parser::Declarations() {
    auto declarations = std::vector<std::shared_ptr<semantic::VarDecl>>();
    while (current_token_.getType() == lexer::VAR) {
        Eat(lexer::VAR);
        while (current_token_.getType() == lexer::IDENTIFIER) {
            const auto var_node = Variable();
            Eat(lexer::COLON);
            const auto type_node = TypeSpec();
            declarations.push_back(std::make_shared<semantic::VarDecl>(
                semantic::VarDecl(var_node, type_node)));
        }
    }

    return declarations;
}

std::shared_ptr<semantic::Type> Parser::TypeSpec() {
    auto token = current_token_;
    Eat(lexer::TYPE);

    return std::make_shared<semantic::Type>(token);
}

std::shared_ptr<semantic::Compound> Parser::CompoundStatement() {
    Eat(lexer::BEGIN);
    auto compound = semantic::Compound();
    while (current_token_.getType() != lexer::END) {
        compound.AddChild(AssignStatement());
        Eat(lexer::SEMICOLON);
    }
    Eat(lexer::END);

    return std::make_shared<semantic::Compound>(compound);
}

std::shared_ptr<semantic::ASTNode> Parser::Factor() {
    auto token = current_token_;
    Eat(lexer::INTEGER);
    return std::make_shared<semantic::Num>(token);
}

std::shared_ptr<semantic::ASTNode> Parser::Term() {
    auto node = Factor();
    while (current_token_.getType() == lexer::MUL ||
           current_token_.getType() == lexer::DIV) {
        auto token = current_token_;
        if (token.getType() == lexer::MUL) {
            Eat(lexer::MUL);
        } else if (token.getType() == lexer::DIV) {
            Eat(lexer::DIV);
        }
        node = std::make_shared<semantic::BinOp>(node, token, Factor());
    }

    return node;
}

std::shared_ptr<semantic::Var> Parser::Variable() {
    auto node = semantic::Var(std::make_shared<lexer::Token>(current_token_));
    Eat(lexer::IDENTIFIER);

    return std::make_shared<semantic::Var>(node);
}

std::shared_ptr<semantic::ASTNode> Parser::Expression() {
    auto node = Term();

    std::vector<lexer::TokenType> expr_oper_types({lexer::PLUS, lexer::MINUS});
    while (std::find(expr_oper_types.begin(), expr_oper_types.end(),
                     current_token_.getType()) != expr_oper_types.end()) {
        auto token = current_token_;
        if (token.getType() == lexer::PLUS) {
            Eat(lexer::PLUS);
        } else if (token.getType() == lexer::MINUS) {
            Eat(lexer::MINUS);
        }

        node = std::make_shared<semantic::BinOp>(node, token, Term());
    }

    return node;
}

std::shared_ptr<semantic::Assign> Parser::AssignStatement() {
    auto left = Variable();
    auto token = current_token_;
    Eat(lexer::ASSIGN);
    auto right = Expression();
    return std::make_shared<semantic::Assign>(left, token, right);
}

} // namespace parser
} // namespace pascal2c
