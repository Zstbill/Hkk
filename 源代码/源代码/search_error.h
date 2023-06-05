//
// Created by Louiiryl_luk on 2023/5/20.
//

#ifndef C___SEARCH_ERROR_H
#define C___SEARCH_ERROR_H
#include "Lexer.h"

string search_error(vector<Token> tokens)
{
    string error_box;
    for (auto &i : tokens)
    {
        if (i.getType() == TokenType::UNVALID)
        {
            error_box = "Invalid symbol ";
            error_box += "\"";
            error_box += i.getValue();
            error_box += "\".";
            return error_box;
        }
    }
    
    if (tokens[0].getType() == TokenType::SELECT)
    {
        if (tokens.size() == 1)
        {
            error_box = "Invalid Statement! Please complete your \"SELECT\" statement!";
            return error_box;
        }
        
        // SELECT * FROM test WHERE
        int i;
        bool if_all = false;
        bool if_list_name = false;
        bool if_from = false;
        // 有了*不能再有其他的列名
        // 必须只要有一个列名/*
        // 必须得有一个FROM
        for (i = 1; i < tokens.size(); i++)
        {
            Token token = tokens[i];
            if (token.getType() == TokenType::FROM)
            {
                if_from = true;
                break;
            }
            else if (token.getType() == TokenType::IDENTIFIER)
            {
                if (if_all == true)
                {
                    error_box = "Invalid Statement! Do not use \"*\" and other column names at the same time for lookups.";
                    return  error_box;
                }
                else
                {
                    if_list_name = true;
                }
            }
            else if (token.getType() == TokenType::ALL)
            {
                if (if_list_name == true)
                {
                    error_box = "Invalid Statement! Do not use \"*\" and other column names at the same time for lookups.";
                    return  error_box;
                }
                else
                {
                    if_all = true;
                }
            }
            else if (token.getType() == TokenType::COMMA)
            {
                if (tokens[i + 1].getType() == TokenType::IDENTIFIER)
                {
                    continue;
                }
                else
                {
                    error_box = "Invalid statement! Please don not use \",\" with no data followed!";
                    return error_box;
                }
            }
        }
        if (if_all == false && if_list_name == false)
        {
            error_box = "Invalid Statement! Enter at least one column name to query.";
            return error_box;
        }
        if (if_from == false)
        {
            error_box = "Invalid Statement! Please use \"FROM\" to enter the table you want to query";
            return error_box;
        }
        
        
        // FROM 后面要有一个表名，且不能是多个
        bool if_table_name = false;
        bool if_where = false;
        for (i = i + 1; i < tokens.size(); i++)
        {
            Token token = tokens[i];
            if (token.getType() == TokenType::WHERE)
            {
                if_where = true;
                break;
            }
            if (token.getType() == TokenType::IDENTIFIER)
            {
                if (if_table_name == true)
                {
                    error_box = "Invalid Statement! The current database engine does not support multi-table queries, so do not enter multiple tables.";
                    return  error_box;
                }
                else
                {
                    if_table_name = true;
                }
            }
        }
        if (if_table_name == false)
        {
            error_box = "Invalid Statement! Please enter at least one table name after \"FROM\" to search for it.";
            return error_box;
        }
        
        // WHERE后面不能是空的
        if (if_where == true)
        {
            // i为WHERE的索引
            if (i + 1 == tokens.size())
            {
                error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. The search condition after \"WHERE\" is empty.";
                return  error_box;
            }
            for (i = i + 1; i < tokens.size(); i++)
            {
                Token token = tokens[i];
                if (token.getType() == TokenType::IDENTIFIER)
                {
                    // 有一个分号
                    if (i + 3 < tokens.size())
                    {
                        if (tokens[i + 1].getType() == TokenType::in || tokens[i + 1].getType() == TokenType::BETWEEN || tokens[i + 1].getType() == TokenType::BINARY_OPERATOR)
                        {
                            if (tokens[i + 1].getType() == TokenType::BINARY_OPERATOR)
                            {
                                if (tokens[i + 2].getType() == TokenType::LITERAL)
                                {
                                    break;
                                }
                                else
                                {
                                    error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. ";
                                    error_box += "\"";
                                    error_box += tokens[i + 1].getValue();
                                    error_box += "\"";
                                    error_box += "is not a valid condition.";
                                }
                            }
                            else if (tokens[i + 1].getType() == TokenType::BETWEEN)
                            {
                                if (i + 5 < tokens.size())
                                {
                                    if (tokens[i + 2].getType() != TokenType::LITERAL)
                                    {
                                        error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. ";
                                        error_box += "\"";
                                        error_box += tokens[i + 2].getValue();
                                        error_box += "\"";
                                        error_box += " is not a valid literal.";
                                        return error_box;
                                    }
                                    else if (tokens[i + 3].getType() != TokenType::AND)
                                    {
                                        error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. Please use \"AND\" instead of ";
                                        error_box += "\"";
                                        error_box += tokens[i + 3].getValue();
                                        error_box += "\".";
                                        return error_box;
                                    }
                                    else if (tokens[i + 4].getType() != TokenType::LITERAL)
                                    {
                                        error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. ";
                                        error_box += "\"";
                                        error_box += tokens[i + 2].getValue();
                                        error_box += "\"";
                                        error_box += " is not a valid literal.";
                                        return error_box;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                                else
                                {
                                    error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. The \"BETWEEN\" statement in \"WHERE\" statement is incomplete.";
                                    return error_box;
                                }
                            }
                            else if (tokens[i + 1].getType() == TokenType::in)
                            {
                                bool if_left_paren = false;
                                bool if_right_paren = false;
                                bool if_literal = false;
                                for (i = i + 2; i < tokens.size(); i++)
                                {
                                    Token token = tokens[i];
                                    if (token.getType() == TokenType::LEFT_PAREN)
                                    {
                                        if_left_paren = true;
                                    }
                                    else if (if_left_paren == true)
                                    {
                                        if (token.getType() == TokenType::LITERAL)
                                        {
                                            if_literal = true;
                                        }
                                        else if (if_literal == true)
                                        {
                                            if (token.getType() == TokenType::RIGHT_PAREN)
                                            {
                                                if_right_paren = true;
                                                break;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. Please input a \"(\" after \"IN\".";
                                        return error_box;
                                    }
                                }
                                
                                if (if_literal == false)
                                {
                                    error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. The \"WHERE\" statement is incomplete. "
                                                "The specific query literal value is missing from parentheses to the right of \"IN\".";
                                    return error_box;
                                }
                                if (if_right_paren == false)
                                {
                                    error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. The \"WHERE\" statement is incomplete. Please input a \")\" after the \"(\"";
                                    return error_box;
                                }
                            }
                        }
                        else
                        {
                            error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. ";
                            error_box += "\"";
                            error_box += tokens[i + 1].getValue();
                            error_box += "\"";
                            error_box += " is not a valid operator.";
                            return  error_box;
                        }
                    }
                    else
                    {
                        error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. The \"WHERE\" statement is incomplete.";
                        return  error_box;
                    }
                }
                
                else
                {
                    error_box = "Invalid Statement! Unresolved \"WHERE\" instruction.";
                    return  error_box;
                }
            }
        }
    }
    else if (tokens[0].getType() == TokenType::INSERT)
    {
        if (tokens.size() >= 2)
        {
            if (tokens[1].getType() == TokenType::INTO)
            {
                bool if_table_name = false;
                bool if_left_paren = false;
                int i;
                for (i = 2; i < tokens.size(); i++)
                {
                    Token token = tokens[i];
                    if (token.getType() == TokenType::LEFT_PAREN)
                    {
                        if_left_paren = true;
                        break;
                    }
                    if (token.getType() == TokenType::IDENTIFIER)
                    {
                        if (if_table_name == false)
                        {
                            if_table_name = true;
                        }
                        else
                        {
                            error_box = "Invalid Statement! Please input one table's name!";
                            return error_box;
                        }
                    }
                }
    
                if (if_table_name == false)
                {
                    error_box = "Invalid statement! Please input a datasheet name after the \"INTO\"!";
                    return error_box;
                }
                if (if_left_paren == false)
                {
                    error_box = "Invalid statement! Please input an \"(\" after the table's name!";
                    return error_box;
                }
  
        
                bool if_right_paren = false;
                bool if_literal = false;
                for (i = i + 1; i < tokens.size(); i++)
                {
                    Token token = tokens[i];
                    if (token.getType() == TokenType::RIGHT_PAREN)
                    {
                        if_right_paren = true;
                        break;
                    }
                    else if (token.getType() == TokenType::LITERAL)
                    {
                        if_literal = true;
                    }
                }
                if (if_literal == false)
                {
                    error_box = "Invalid statement! Please input specific data to insert!";
                    return error_box;
                }
                if (if_right_paren == false)
                {
                    error_box = "Invalid statement! Please input an \")\" after the data!";
                    return error_box;
                }
            }
            else
            {
                error_box = "Invalid statement! Please input an \"INTO\" after \"INSERT\"!";
                return error_box;
            }
        }
       else
        {
           error_box = "Invalid statement! Please complete your \"INSERT\" statement!";
           return error_box;
        }
    }
    else if (tokens[0].getType() == TokenType::UPDATE)
    {
        if (tokens.size() >= 2)
        {
            if (tokens[1].getType() == TokenType::IDENTIFIER)
            {
                int i;
                bool if_set = false;
                bool if_where = false;
                bool if_list_name = false;
                for (i = 1; i < tokens.size(); i++)
                {
                    Token token = tokens[i];
                    if (token.getType() == TokenType::WHERE)
                    {
                        if_where = true;
                        break;
                    }
                    else
                    {
                        if (token.getType() == TokenType::SET)
                        {
                            if_set = true;
                            continue;
                        }
                        else if (if_set == true)
                        {
                            if (token.getType() == TokenType::IDENTIFIER)
                            {
                                if (i + 3 < tokens.size())
                                {
                                    if (tokens[i + 1].getValue() == "=")
                                    {
                                        if (tokens[i + 2].getType() == TokenType::LITERAL)
                                        {
                                            if_list_name = true;
                                            i = i + 2;
                                            continue;
                                        }
                                        else
                                        {
                                            error_box = "Invalid statement! Please enter the updated data after \"=\"!";
                                            return error_box;
                                        }
                                    }
                                    else
                                    {
                                        error_box = "Invalid statement! Please use \"=\" to input the action of updating data!";
                                        return error_box;
                                    }
                                }
                                else
                                {
                                    error_box = "Invalid statement! Check the integrity of the data to be updated and the \"WHERE\" statement!";
                                    return error_box;
                                }
                            }
                            else if (token.getType() == TokenType::COMMA)
                            {
                                if (tokens[i + 1].getType() == TokenType::IDENTIFIER)
                                {
                                    continue;
                                }
                                else
                                {
                                    error_box = "Invalid statement! Please don not use \",\" with no data followed!";
                                    return error_box;
                                }
                            }
                            else
                            {
                                error_box = "Invalid statement! Please enter exactly the data that needs to be updated!";
                                return error_box;
                            }
                        }
                    }
                }
    
                if (if_set == false)
                {
                    error_box = "Invalid statement! Please input a \"SET\" after the datasheet name!";
                    return error_box;
                }
                if (if_list_name == false)
                {
                    error_box = "Invalid statement! Please enter the data you need to update!";
                    return error_box;
                }
                if (if_where == false)
                {
                    error_box = "Invalid statement! Please enter the complete \"WHERE\" statement!";
                    return error_box;
                }
                
    
                // WHERE后面不能是空的
                if (if_where == true)
                {
                    // i为WHERE的索引
                    if (i + 1 == tokens.size())
                    {
                        error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. The search condition after \"WHERE\" is empty.";
                        return  error_box;
                    }
                    for (i = i + 1; i < tokens.size(); i++)
                    {
                        Token token = tokens[i];
                        if (token.getType() == TokenType::IDENTIFIER)
                        {
                            // 有一个分号
                            if (i + 3 < tokens.size())
                            {
                                if (tokens[i + 1].getType() == TokenType::in || tokens[i + 1].getType() == TokenType::BETWEEN || tokens[i + 1].getType() == TokenType::BINARY_OPERATOR)
                                {
                                    if (tokens[i + 1].getType() == TokenType::BINARY_OPERATOR)
                                    {
                                        if (tokens[i + 2].getType() == TokenType::LITERAL)
                                        {
                                            break;
                                        }
                                        else
                                        {
                                            error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. ";
                                            error_box += "\"";
                                            error_box += tokens[i + 1].getValue();
                                            error_box += "\"";
                                            error_box += "is not a valid condition.";
                                        }
                                    }
                                    else if (tokens[i + 1].getType() == TokenType::BETWEEN)
                                    {
                                        if (i + 5 < tokens.size())
                                        {
                                            if (tokens[i + 2].getType() != TokenType::LITERAL)
                                            {
                                                error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. ";
                                                error_box += "\"";
                                                error_box += tokens[i + 2].getValue();
                                                error_box += "\"";
                                                error_box += " is not a valid literal.";
                                                return error_box;
                                            }
                                            else if (tokens[i + 3].getType() != TokenType::AND)
                                            {
                                                error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. Please use \"AND\" instead of ";
                                                error_box += "\"";
                                                error_box += tokens[i + 3].getValue();
                                                error_box += "\".";
                                                return error_box;
                                            }
                                            else if (tokens[i + 4].getType() != TokenType::LITERAL)
                                            {
                                                error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. ";
                                                error_box += "\"";
                                                error_box += tokens[i + 2].getValue();
                                                error_box += "\"";
                                                error_box += " is not a valid literal.";
                                                return error_box;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. The \"BETWEEN\" statement in \"WHERE\" statement is incomplete.";
                                            return error_box;
                                        }
                                    }
                                    else if (tokens[i + 1].getType() == TokenType::in)
                                    {
                                        bool if_left_paren = false;
                                        bool if_right_paren = false;
                                        bool if_literal = false;
                                        for (i = i + 2; i < tokens.size(); i++)
                                        {
                                            Token token = tokens[i];
                                            if (token.getType() == TokenType::LEFT_PAREN)
                                            {
                                                if_left_paren = true;
                                            }
                                            else if (if_left_paren == true)
                                            {
                                                if (token.getType() == TokenType::LITERAL)
                                                {
                                                    if_literal = true;
                                                }
                                                else if (if_literal == true)
                                                {
                                                    if (token.getType() == TokenType::RIGHT_PAREN)
                                                    {
                                                        if_right_paren = true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. Please input a \"(\" after \"IN\".";
                                                return error_box;
                                            }
                                        }
                            
                                        if (if_literal == false)
                                        {
                                            error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. The \"WHERE\" statement is incomplete. "
                                                        "The specific query literal value is missing from parentheses to the right of \"IN\".";
                                            return error_box;
                                        }
                                        if (if_right_paren == false)
                                        {
                                            error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. The \"WHERE\" statement is incomplete. Please input a \")\" after the \"(\"";
                                            return error_box;
                                        }
                                    }
                                }
                                else
                                {
                                    error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. ";
                                    error_box += "\"";
                                    error_box += tokens[i + 1].getValue();
                                    error_box += "\"";
                                    error_box += " is not a valid operator.";
                                    return  error_box;
                                }
                            }
                            else
                            {
                                error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. The \"WHERE\" statement is incomplete.";
                                return  error_box;
                            }
                        }
            
                        else
                        {
                            error_box = "Invalid Statement! Unresolved \"WHERE\" instruction.";
                            return  error_box;
                        }
                    }
                }
            }
            else
            {
                error_box = "Invalid statement! Please input a datasheet name after the \"UPDATE\"!";
                return error_box;
            }
        }
        else
        {
            error_box = "Invalid statement! Please complete your \"UPDATE\" statement!";
            return error_box;
        }
    }
    else if (tokens[0].getType() == TokenType::Delete)
    {
        int i;
        bool if_from = false;
        for (i = 1; i < tokens.size(); i++)
        {
            Token token = tokens[i];
            if (token.getType() == TokenType::FROM)
            {
                if_from = true;
                break;
            }
        }
        if (if_from == false)
        {
            error_box = "Invalid statement! Please input a \"FROM\" after the\"DELETE\"";
            return error_box;
        }
        
        bool if_table_name = false;
        bool if_where = false;
        for (i = i + 1; i < tokens.size(); i++)
        {
            Token token = tokens[i];
            if (token.getType() == TokenType::WHERE)
            {
                if_where = true;
                break;
            }
            else
            {
                if (token.getType() == TokenType::IDENTIFIER)
                {
                    if (if_table_name == false)
                    {
                        if_table_name = true;
                    }
                    else
                    {
                        error_box = "Invalid Statement! Please input one datasheet's name!";
                        return error_box;
                    }
                }
                else if (token.getType() == TokenType::SEMICOLON)
                {
                    break;
                }
                else
                {
                    error_box = "Invalid Statement! Please check your datasheet's name!";
                    return error_box;
                }
            }
        }
    
        if (if_table_name == false)
        {
            error_box = "Invalid Statement! Please input a valid datasheet name after \"FROM\"!";
            return error_box;
        }
        
        // WHERE后面不能是空的
        if (if_where == true)
        {
            // i为WHERE的索引
            if (i + 1 == tokens.size())
            {
                error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. The search condition after \"WHERE\" is empty.";
                return  error_box;
            }
            for (i = i + 1; i < tokens.size(); i++)
            {
                Token token = tokens[i];
                if (token.getType() == TokenType::IDENTIFIER)
                {
                    // 有一个分号
                    if (i + 3 < tokens.size())
                    {
                        if (tokens[i + 1].getType() == TokenType::in || tokens[i + 1].getType() == TokenType::BETWEEN || tokens[i + 1].getType() == TokenType::BINARY_OPERATOR)
                        {
                            if (tokens[i + 1].getType() == TokenType::BINARY_OPERATOR)
                            {
                                if (tokens[i + 2].getType() == TokenType::LITERAL)
                                {
                                    break;
                                }
                                else
                                {
                                    error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. ";
                                    error_box += "\"";
                                    error_box += tokens[i + 1].getValue();
                                    error_box += "\"";
                                    error_box += "is not a valid condition.";
                                }
                            }
                            else if (tokens[i + 1].getType() == TokenType::BETWEEN)
                            {
                                if (i + 5 < tokens.size())
                                {
                                    if (tokens[i + 2].getType() != TokenType::LITERAL)
                                    {
                                        error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. ";
                                        error_box += "\"";
                                        error_box += tokens[i + 2].getValue();
                                        error_box += "\"";
                                        error_box += " is not a valid literal.";
                                        return error_box;
                                    }
                                    else if (tokens[i + 3].getType() != TokenType::AND)
                                    {
                                        error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. Please use \"AND\" instead of ";
                                        error_box += "\"";
                                        error_box += tokens[i + 3].getValue();
                                        error_box += "\".";
                                        return error_box;
                                    }
                                    else if (tokens[i + 4].getType() != TokenType::LITERAL)
                                    {
                                        error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. ";
                                        error_box += "\"";
                                        error_box += tokens[i + 2].getValue();
                                        error_box += "\"";
                                        error_box += " is not a valid literal.";
                                        return error_box;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                                else
                                {
                                    error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. The \"BETWEEN\" statement in \"WHERE\" statement is incomplete.";
                                    return error_box;
                                }
                            }
                            else if (tokens[i + 1].getType() == TokenType::in)
                            {
                                bool if_left_paren = false;
                                bool if_right_paren = false;
                                bool if_literal = false;
                                for (i = i + 2; i < tokens.size(); i++)
                                {
                                    Token token = tokens[i];
                                    if (token.getType() == TokenType::LEFT_PAREN)
                                    {
                                        if_left_paren = true;
                                    }
                                    else if (if_left_paren == true)
                                    {
                                        if (token.getType() == TokenType::LITERAL)
                                        {
                                            if_literal = true;
                                        }
                                        else if (if_literal == true)
                                        {
                                            if (token.getType() == TokenType::RIGHT_PAREN)
                                            {
                                                if_right_paren = true;
                                                break;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. Please input a \"(\" after \"IN\".";
                                        return error_box;
                                    }
                                }
                            
                                if (if_literal == false)
                                {
                                    error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. The \"WHERE\" statement is incomplete. "
                                                "The specific query literal value is missing from parentheses to the right of \"IN\".";
                                    return error_box;
                                }
                                if (if_right_paren == false)
                                {
                                    error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. The \"WHERE\" statement is incomplete. Please input a \")\" after the \"(\"";
                                    return error_box;
                                }
                            }
                        }
                        else
                        {
                            error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. ";
                            error_box += "\"";
                            error_box += tokens[i + 1].getValue();
                            error_box += "\"";
                            error_box += " is not a valid operator.";
                            return  error_box;
                        }
                    }
                    else
                    {
                        error_box = "Invalid Statement! Unresolved \"WHERE\" instruction. The \"WHERE\" statement is incomplete.";
                        return  error_box;
                    }
                }
            
                else
                {
                    error_box = "Invalid Statement! Unresolved \"WHERE\" instruction.";
                    return  error_box;
                }
            }
        }
        else
        {
            error_box = "Invalid Statement! Please input a valid \"WHERE\" statement!";
            return error_box;
        }
    }
    else if (tokens[0].getType() == TokenType::DROP)
    {
        if (tokens.size() == 4)
        {
            if (tokens[1].getType() != TokenType::DATABASE && tokens[1].getType() != TokenType::TABLE)
            {
                error_box = "Invalid Statement! Please input \"DATABASE\" or \"TABLE\" after the \"DROP\"!";
                return error_box;
            }
            if (tokens[2].getType() != TokenType::IDENTIFIER)
            {
                error_box = "Invalid Statement! Please input a valid name to be dropped!";
                return error_box;
            }
        }
        else
        {
            error_box = "Invalid Statement! Please check if the \"DROP\" statement is too long or too short!";
            return error_box;
        }
    }
    else if (tokens[0].getType() == TokenType::SHOW)
    {
        if (tokens.size() == 3)
        {
            if (tokens[1].getType() != TokenType::DATABASES && tokens[1].getType() != TokenType::TABLES)
            {
                error_box = "Invalid Statement! Please enter \"DATABASES\" or \"TABLES\" after \"SHOW\" instead of any other input!";
                return error_box;
            }
        }
        else
        {
            error_box = "Invalid Statement! Please check if the \"SHOW\" statement is too long or too short!";
            return error_box;
        }
    }
    else if (tokens[0].getType() == TokenType::DESCRIBE)
    {
        if (tokens.size() == 4)
        {
            if (tokens[1].getType() != TokenType::TABLE)
            {
                error_box = "Invalid Statement! Please input a \"TABLE\" after \"DESCRIBE\"!";
                return error_box;
            }
            if (tokens[2].getType() != TokenType::IDENTIFIER)
            {
                error_box = "Invalid Statement! Please input a valid data table name after \"TABLE\"!";
                return error_box;
            }
        }
        else
        {
            error_box = "Invalid Statement! Please check if the \"DESCRIBE\" statement is too long or too short!";
            return error_box;
        }
    }
    else if (tokens[0].getType() == TokenType::USE)
    {
        if (tokens.size() == 4)
        {
            if (tokens[1].getType() != TokenType::DATABASE)
            {
                error_box = "Invalid Statement! Please input \"DATABASE\" after \"USE\"!";
                return error_box;
            }
            if (tokens[2].getType() != TokenType::IDENTIFIER)
            {
                error_box = "Invalid Statement! Please input a valid database name after \"DATABASE\"!";
                return error_box;
            }
        }
        else
        {
            error_box = "Invalid Statement! Please check if the \"USE\" statement is too long or too short!";
            return error_box;
        }
    }
    else if (tokens[0].getType() == TokenType::CREATE)
    {
        if (tokens.size() >= 4)
        {
            if (tokens[1].getType() == TokenType::DATABASE)
            {
                if (tokens.size() == 4)
                {
                    if (tokens[2].getType() != TokenType::IDENTIFIER)
                    {
                        error_box = "Invalid Statement! Please input a valid database name after \"DATABASE\"!";
                        return error_box;
                    }
                }
                else
                {
                    error_box = "Invalid Statement! Please check if the \"CREATE DATABASE\" statement is too long!";
                    return error_box;
                }
            }
            else if (tokens[1].getType() == TokenType::TABLE)
            {
                if (tokens[2].getType() == TokenType::IDENTIFIER)
                {
                    if (tokens[3].getType() == TokenType::LEFT_PAREN)
                    {
                        int i = 4;
                        bool if_name = false;
                        bool if_right_paren = false;
                        while(if_right_paren == false)
                        {
                            if (i >= tokens.size())
                            {
                                break;
                            }
                            Token token = tokens[i];
                            if (token.getType() == TokenType::RIGHT_PAREN)
                            {
                                if_right_paren = true;
                                break;
                            }
                            
                            if (token.getType() == TokenType::IDENTIFIER)
                            {
                                if (i + 3 < tokens.size())
                                {
                                    if (tokens[i + 1].getType() != TokenType::INT && tokens[i + 1].getType() != TokenType::STRING && tokens[i + 1].getType() != TokenType::DOUBLE)
                                    {
                                        error_box = "Invalid statement! Please input a valid data type!";
                                        return error_box;
                                    }
                                    else
                                    {
                                        if (tokens[i + 2].getType() == TokenType::PRIMARYKEY)
                                        {
                                            i = i + 3;
                                            if_name = true;
                                            continue;
                                        }
                                        else
                                        {
                                            i = i + 2;
                                            if_name=1;
                                            continue;
                                        }
                                    }
                                    continue;
                                }
                                else
                                {
                                    error_box = "Invalid statement! Please check the integrity of the statement!";
                                    return error_box;
                                }
                            }
                            else if (token.getType() == TokenType::COMMA)
                            {
                                if (tokens[i + 1].getType() != TokenType::IDENTIFIER)
                                {
                                    error_box = "Invalid statement! Please do not use \",\" with no data followed!";
                                    return error_box;
                                }
                            }
                            else
                            {
                                error_box = "Invalid statement! Please input a valid name after the \"(\"!";
                                return error_box;
                            }
                            
                            i++;
                            continue;
                        }
                        if (if_name == false)
                        {
                            error_box = "Invalid statement! Please input the data's name and its type!";
                            return error_box;
                        }
                        if (if_right_paren == false)
                        {
                            error_box = "Invalid statement! Please input a valid \")\" after the data!";
                            return error_box;
                        }
                    }
                    else
                    {
                        error_box = "Invalid statement! Please input a valid \"(\" after the data table name to indicate the data name and its type!";
                        return error_box;
                    }
                }
                else
                {
                    error_box = "Invalid Statement! Please input a valid datasheet name after \"TABLE\"!";
                    return error_box;
                }
            }
            else
            {
                error_box = "Invalid Statement! Please enter \"TABLE\" or \"DATABASE\" after \"CREATE\"!";
                return error_box;
            }
        }
        else
        {
            error_box = "Invalid Statement! Please check if the \"CREATE\" statement is too short!";
            return error_box;
        }
    }
    else
    {
        error_box = "Unrecognized statement, please re-enter. By the way, remember to enter keywords in English uppercase.";
    }
    return error_box;
}

#endif //C___SEARCH_ERROR_H
