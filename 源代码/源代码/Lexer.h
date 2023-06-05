//
// Created by Louiiryl_luk on 2023/5/18.
//

#ifndef C___LEXER_H
#define C___LEXER_H

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>  // 引入标准异常库
#include <iomanip>
using namespace std;

// Token 类型枚举
enum class TokenType
{
    // 关键字
    SELECT, FROM, WHERE, AND, OR, INSERT, AS, INTO, UPDATE, SET, BETWEEN, in, Delete, DROP, TABLE, DATABASE,
    SHOW, DATABASES, TABLES, DESCRIBE, USE, CREATE, INT, DOUBLE, STRING, PRIMARYKEY,
    // 特殊符号
    COMMA, SEMICOLON, LEFT_PAREN, RIGHT_PAREN, ALL, UNVALID,
    // 字面量
    IDENTIFIER, LITERAL,
    // 运算符
    BINARY_OPERATOR
};

// Token 类
class Token
{
public:
    Token(const enum TokenType& _type, const string& _value) : type(_type), value(_value) {}
    const enum TokenType getType() const
    {
        return type;
    }
    const string getValue() const
    {
        return value;
    }

private:
    enum TokenType type;
    string value;
};

// 词法分析器类
class Lexer
{
private:
    const string input;
    int currentPos;
    char currentChar;

public:
    Lexer(const string& _input) : input(_input)
    {
        if ( input.empty() )
        {
            cout<<"Empty input!"<<endl;
        }
        currentPos = 0;
        currentChar = input[currentPos];
    }
    
    void advance()
    {
        if (currentPos == input.size() )
            cout<<"Invalid Pos Operation!"<<endl;
        
        ++currentPos;
        if (currentPos >= input.size())
        {
            currentChar = '\0';
        }
        else
        {
            currentChar = input[currentPos];
        }
    }
    
    void back()
    {
        if (currentPos == 0)
            cout<<"Invalid Pos Operation!"<<endl;
        else
        {
            --currentPos;
            currentChar = input[currentPos];
        }
    }
    
    void skip_Whitespaces()
    {
        while (isspace(currentChar) && currentChar != '\0')
        {
            advance();
        }
    }
    
    Token get_NextToken()
    {
        while (currentChar != '\0')
        {
            if ( isspace(currentChar) )
            {
                skip_Whitespaces();
                continue;
            }
            if ( isalpha(currentChar) )
            {
                return readIdentifierToken();
            }
            if ( isdigit(currentChar) )
            {
                return readLiteralToken();
            }
            switch ( currentChar )
            {
                case ',':
                    advance();
                    return Token(TokenType::COMMA, ",");
                case ';':
                    // 分号后面的字符不需要读取
                    return Token(TokenType::SEMICOLON, ";");
                
                
                case '(':
                    advance();
                    return Token(TokenType::LEFT_PAREN, "(");
                case ')':
                    advance();
                    return Token(TokenType::RIGHT_PAREN, ")");
                case '*':
                    advance();
                    return Token(TokenType::ALL, "*");
                
                
                case '\'':  // 读取字符串
                    return readStringLiteralToken();
                case '\"':  // 读取字符串
                    return readStringLiteralToken();
                
                
                case '=':   // = 一种符号
                    advance();
                    return Token(TokenType::BINARY_OPERATOR, "=");
                case '>':   // > >= 两种符号
                    return readOperator();
                case '<':   // < <= 两种符号
                    return readOperator();
                case '!':   // !> !< != 三种符号
                    return readOperator();
                    
                
                
                default:
                    string temp = string(1,currentChar);
                    advance();
                    return Token(TokenType::UNVALID, temp);
                    break;
            }
        }
        return Token(TokenType::SEMICOLON, "");
    }

private:
    Token readIdentifierToken()
    {
        string value;
        while (isalnum(currentChar) || currentChar == '_')
        {
            value += currentChar;
            advance();
        }
        
        if (value == "SELECT")
        {
            return Token(TokenType::SELECT, value);
        }
        else if (value == "FROM")
        {
            return Token(TokenType::FROM, value);
        }
        else if (value == "WHERE")
        {
            return Token(TokenType::WHERE, value);
        }
        else if (value == "AND")
        {
            return Token(TokenType::AND, value);
        }
        else if (value == "OR")
        {
            return Token(TokenType::OR, value);
        }
        else if (value == "INSERT")
        {
            return Token(TokenType::INSERT, value);
        }
        else if (value == "INTO")
        {
            return Token(TokenType::INTO, value);
        }
        else if (value == "UPDATE")
        {
            return Token(TokenType::UPDATE, value);
        }
        else if (value == "SET")
        {
            return Token(TokenType::SET, value);
        }
        else if (value == "AS")
        {
            return Token(TokenType::AS, value);
        }
        else if (value == "BETWEEN")
        {
            return Token(TokenType::BETWEEN, value);
        }
        else if (value == "DELETE")
        {
            return Token(TokenType::Delete, value);
        }
        else if (value == "DROP")
        {
            return Token(TokenType::DROP, value);
        }
        else if (value == "TABLE")
        {
            return Token(TokenType::TABLE, value);
        }
        else if (value == "TABLES")
        {
            return Token(TokenType::TABLES, value);
        }
        else if (value == "DATABASE")
        {
            return Token(TokenType::DATABASE, value);
        }
        else if (value == "DATABASES")
        {
            return Token(TokenType::DATABASES, value);
        }
        else if (value == "SHOW")
        {
            return Token(TokenType::SHOW, value);
        }
        else if (value == "DESCRIBE")
        {
            return Token(TokenType::DESCRIBE, value);
        }
        else if (value == "USE")
        {
            return Token(TokenType::USE, value);
        }
        else if (value == "CREATE")
        {
            return Token(TokenType::CREATE, value);
        }
        else if (value == "INT")
        {
            return Token(TokenType::INT, value);
        }
        else if (value == "DOUBLE")
        {
            return Token(TokenType::DOUBLE, value);
        }
        else if (value == "STRING")
        {
            return Token(TokenType::STRING, value);
        }
        else if (value == "PRIMARYKEY")
        {
            return Token(TokenType::PRIMARYKEY, "primarykey");
        }
        else if (value == "IN")
        {
            return Token(TokenType::in, value);
        }
        
            //// 在这里继续补充关键字
            ////
            ////
            
            
            // 如果不是关键字，则为标识符（列名，别名，文件名等）
        else
        {
            return Token(TokenType::IDENTIFIER, value);
        }
    }
    
    Token readLiteralToken()
    {
        string value;
        while (isdigit(currentChar) || currentChar == '.')
        {
            value += currentChar;
            advance();
        }
        return Token(TokenType::LITERAL, value);
    }
    
    Token readStringLiteralToken()
    {
        advance();  // 跳过左引号
        std::string value;
        while (currentChar != '\0' && currentChar != '\'' && currentChar != '\"')
        {
            value += currentChar;
            advance();
        }
        if (currentChar == '\'' || currentChar == '\"')
        {
            advance();  // 跳过右引号
        }
        else
        {
            cout<<"String literal missing right quote"<<endl;
        }
        return Token(TokenType::LITERAL, value);
    }
    
    Token readOperator()
    {
        string value;
        if (currentChar == '>' || currentChar == '<')
        {
            value += currentChar;
            advance();
            if (currentChar == '=')
            {
                value += currentChar;
                advance();
                return {TokenType::BINARY_OPERATOR, value};
            }
            else
            {
                return {TokenType::BINARY_OPERATOR, value};
            }
        }
        else if (currentChar == '!')
        {
            value += currentChar;
            advance();
            if ( currentChar == '=')
            {
                value += currentChar;
                advance();
                return {TokenType::BINARY_OPERATOR, value};
            }
            if ( currentChar == '>')
            {
                value += currentChar;
                advance();
                return {TokenType::BINARY_OPERATOR, value};
            }
            if ( currentChar == '<')
            {
                value += currentChar;
                advance();
                return {TokenType::BINARY_OPERATOR, value};
            }
        }
    }
};

vector<Token> get_statement_tokens(string SQL_statement)
{
    Lexer lexer(SQL_statement);
    vector<Token> tokens;
    Token token = lexer.get_NextToken();
    while (token.getType() != TokenType::SEMICOLON)
    {
        tokens.push_back(token);
        token = lexer.get_NextToken();
    }
    tokens.push_back(Token(TokenType::SEMICOLON, ";"));
    
    return tokens;
}

void print_tokens(vector<Token> tokens)
{
    for (const auto& t : tokens)
    {
        cout << "[";
        cout << internal << setw(4) << static_cast<int>(t.getType());
        cout << ", ";
        cout << internal << setw(8) << t.getValue();
        cout << "]" << endl;
    }
}
#endif //C___LEXER_H
