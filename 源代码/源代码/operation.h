//
// Created by Louiiryl_luk on 2023/5/18.
//

#ifndef C___OPERATION_H
#define C___OPERATION_H

#include "Lexer.h"
#include "file.h"

vector<Token> get_where_tokens(vector<Token> tokens)
{
    vector<Token> temp_tokens = tokens;
    bool b = false;
    for (int i = 0; i < temp_tokens.size(); i++)
    {
        if (temp_tokens.at(i).getType() == TokenType::WHERE)
        {
            temp_tokens.erase(temp_tokens.begin(), temp_tokens.begin() + i);
            b = true;
        }
    }
    if (b == false)
    {
        temp_tokens.clear();
    }
    return temp_tokens;
}
vector<vector<Token>> break_where_tokens_by_OR(vector<Token> tokens)
{
    vector<vector<Token>> final;
    vector<Token> temp_tokens;
    bool if_between = false;
    for (auto& token : tokens)
    {
        if (token.getType() == TokenType::WHERE)
        {
            continue;
        }
        if (token.getType() == TokenType::SEMICOLON)
        {
            final.push_back(temp_tokens);
            return final;
        }
        if (token.getType() == TokenType::BETWEEN)
        {
            if_between = true;
            auto temp_token = token;
            temp_tokens.push_back(temp_token);
            continue;
        }
        
        if (token.getType() != TokenType::AND && token.getType() != TokenType::OR)
        {
            auto temp_token = token;
            temp_tokens.push_back(temp_token);
        }
        else if (if_between == false)
        {
            final.push_back(temp_tokens);
            temp_tokens.clear();
        }
        else
        {
            auto temp_token = token;
            temp_tokens.push_back(temp_token);
            if_between = false;
        }
    }
    return final;
}
vector<vector<Token>> break_where_tokens_by_AND(vector<Token> tokens)
{
    vector<vector<Token>> final;
    vector<Token> temp_tokens;
    bool if_between = false;
    for (auto& token : tokens)
    {
        if (token.getType() == TokenType::WHERE)
        {
            continue;
        }
        if (token.getType() == TokenType::SEMICOLON)
        {
            final.push_back(temp_tokens);
            return final;
        }
        if (token.getType() == TokenType::BETWEEN)
        {
            if_between = true;
            auto temp_token = token;
            temp_tokens.push_back(temp_token);
            continue;
        }
        
        if (token.getType() != TokenType::AND && token.getType() != TokenType::OR)
        {
            auto temp_token = token;
            temp_tokens.push_back(temp_token);
        }
        else if (if_between == false)
        {
            final.push_back(temp_tokens);
            temp_tokens.clear();
        }
        else
        {
            auto temp_token = token;
            temp_tokens.push_back(temp_token);
            if_between = false;
        }
    }
    return final;
}

#endif //C___OPERATION_H
