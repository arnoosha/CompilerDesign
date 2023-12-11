#include "Parser.h"

// main point is that the whole input has been consumed
AST *Parser::parse()
{
    AST *Res = parseGSM();
    return Res;
}

AST *Parser::parseGSM()
{
    llvm::SmallVector<Expr *> exprs;
    while (!Tok.is(Token::eoi))
    {
        switch (Tok.getKind())
        {
        case Token::KW_int:
            Expr *d1;
            d1 = parseDec();
            if (d1)
                exprs.push_back(d1);
            else
                goto _error2;
            break;

            if (!Tok.is(Token::semicolon))
            {
                error();
                goto _error2;
            }

        case Token::id:
            Expr *d2;
            d2 = parseEquation();
            if (d2)
                exprs.push_back(d2);
            else
                goto _error2;
            break;

            if (!Tok.is(Token::semicolon))
            {
                error();
                goto _error2;
            }

        case Token::KW_loopc:
            Expr *d3;
            d3 = parseLoop();
            if (d3)
                exprs.push_back(d3);
            else
                goto _error2;
            break;

            /*if (!Tok.is(Token::semicolon))
            {
                error();
                goto _error2;
            }*/
        case Token::KW_if:
            Expr *d4;
            d4 = parseIf();
            if (d4)
                exprs.push_back(d4);
            else
                goto _error2;
            break;

            // if (!Tok.is(Token::semicolon))
            // {
            //     error();
            //     goto _error2;
            // }
        default:
            goto _error2;
            break;
        }
        advance(); // TODO: watch this part
    }
    return new NAN(exprs);
_error2:
    while (Tok.getKind() != Token::eoi)
        advance();
    return nullptr;
}

Expr *Parser::parseDec()
{
    Expr *E;
    llvm::SmallVector<llvm::StringRef, 8> Vars;
    if (!Tok.is(Token::KW_int) && !(Tok.is(Token::id)))
        goto _error;

    advance();
    if (expect(Token::KW_int))
        goto _error;

    advance();

    if (expect(Token::id))
        goto _error;
    Vars.push_back(Tok.getText());
    advance();

    while (Tok.is(Token::comma))
    {
        advance();
        if (expect(Token::id))
            goto _error;
        Vars.push_back(Tok.getText());
        advance();
    }
    

    if (Tok.is(Token::equal))
    {
        advance();
        E = parseExpr();
    }

    if (expect(Token::semicolon))
        goto _error;

    return new Declaration(Vars, E);
_error: // TODO: Check this later in case of error :)
    while (Tok.getKind() != Token::eoi)
        advance();
    return nullptr;
}

Expr *Parser::parseEquation()
{
    Expr *E;
    BinaryOp::Operator Op;

    if (!Tok.is(Token::id) /*&& !Tok.is(Token::num) && !Tok.is(Token::l_paren)*/)
    {
        // error();
        // return nullptr;
        goto _error;
    }

    advance();           // id += 2;
    
    //Fi = parseFinal();


    while (Tok.isOneOf(Token::plus, Token::minus , Token::equal, Token::power, Token::star, Token::slash , Token::KW_mod, Token::KW_plusEqual, Token::KW_minusEqual ,Token::KW_starEqual ,Token::KW_slashEqual , Token::KW_modEq)){
        
            //Tok.is(Token::star) ? BinaryOp::star : BinaryOp::slash;
            if (Tok.is(Token::plus)) 
                Op = BinaryOp::Operator::Plus;
            else if (Tok.is(Token::minus))  
                Op =  BinaryOp::Operator::Minus;
            else if (Tok.is(Token::equal))  
                Op =  BinaryOp::Operator::equal;
            else if (Tok.is(Token::power))  
                Op =  BinaryOp::Operator::power;
            else if (Tok.is(Token::star))  
                Op =  BinaryOp::Operator::star;
            else if (Tok.is(Token::slash))  
                Op =  BinaryOp::Operator::slash;
            else if (Tok.is(Token::KW_mod))  
                Op =  BinaryOp::Operator::KW_mod; 
            else if (Tok.is(Token::KW_plusEqual))  
                Op =  BinaryOp::Operator::KW_plusEqual;   
            else if (Tok.is(Token::KW_minusEqual))  
                Op =  BinaryOp::Operator::KW_minusEqual;   
            else if (Tok.is(Token::KW_starEqual))  
                Op =  BinaryOp::Operator::KW_starEqual;   
            else if (Tok.is(Token:: KW_slashEqual))  
                Op =  BinaryOp::Operator::KW_slashEqual; 
            else if (Tok.is(Token:: KW_modEq))  
                Op =  BinaryOp::Operator::KW_modEq;   

    }

    E = parseExpr();
    
    if (expect(Token::semicolon))
        goto _error;

    return new Equation(Op,E); //MUST EDIT AST
}

Expr *Parser::parseExpr()
{
    Expr *Left = parseTerm();
    while (Tok.isOneOf(Token::plus, Token::minus))
    {
        BinaryOp::Operator Op =
            Tok.is(Token::plus) ? BinaryOp::Plus : BinaryOp::Minus;
        advance();
        Expr *Right = parseTerm();
        Left = new BinaryOp(Op, Left, Right);
    }
    return Left;
}

Expr *Parser::parseTerm()
{
    Expr *Left = parseFactor();
    while (Tok.isOneOf(Token::star, Token::slash , Token::KW_mod))
    {
        BinaryOp::Operator Op;
            //Tok.is(Token::star) ? BinaryOp::star : BinaryOp::slash;
            if (Tok.is(Token::star)) 
                Op = BinaryOp::Operator::star;
            else if (Tok.is(Token::slash))  
                Op =  BinaryOp::Operator::slash;
            else if (Tok.is(Token::KW_mod))  
                Op =  BinaryOp::Operator::KW_mod;  


        advance();
        Expr *Right = parseFactor();
        Left = new BinaryOp(Op, Left, Right);
    }
    return Left;
}

Expr *Parser::parseFactor()
{
    Expr *Left = parseFinal();
    while (Tok.isOneOf(Token::power))
    {
        BinaryOp::Operator Op = BinaryOp::power;
        
        advance();
        Expr *Right = parseFinal();
        Left = new BinaryOp(Op, Left, Right);
    }
    return Left;
}

Expr *Parser::parseFinal()
{
    Expr *Res = nullptr;
    switch (Tok.getKind())
    {
    case Token::num:
        Res = new Factor(Final::num, Tok.getText());
        advance();
        break;
    case Token::id:
        Res = new Factor(Final::id, Tok.getText());
        advance();
        break;
    case Token::l_paren:
        advance();
        Res = parseExpr();
        if (!consume(Token::r_paren))
            break;
    default: // error handling
        if (!Res)
            error();
        while (!Tok.isOneOf(Token::r_paren, Token::star, Token::plus, Token::minus, Token::slash, Token::eoi))
            advance();
        break;
    }
    return Res;
}





//------------------------------------------------------------------------------------



If *Parser::parseIf() {

    Conditions condits = parseConditions();
    llvm::SmallVector<Equation *> Equations;
    llvm::SmallVector<Elif *> Elifs;
    ElseParse *Else;
    if(expect(Token::KW_if))
        goto _error;

    advance();
    
    if(expect(Token::KW_colon))
        goto _error;
    
    advance();

     if(expect(Token::KW_begin))
        goto _error;
    
    advance();

    while(Tok.is(Token::id)){
        Equation Eq = parseEquation();
        Equations.push_back(E);


    }


    if(expect(Token::KW_end))
        goto _error;
    
    advance();

    while(Tok.is(Token::KW_elif)){
        ElifParse Ef;
        Ef = parseElif();
        Elifs.push_back(Ef);

    }

    if(Tok.is(Token::KW_else))
        Else = parseElif();





    return new If(condits , Equations , Elifs , Else);

    
}



Elif *Parser::parseElif()
{
    llvm::SmallVector<Equation *> Equations;
    Conditions condits = parseConditions();

    if(expect(Token::KW_elif))
        goto _error;
    
    advance();

    if(expect(Token::KW_colon))
        goto _error;
    
    advance();

    if(expect(Token::KW_begin))
        goto _error;
    
    advance();

    while(Tok.is(Token::id)){
        Equation Eq = parseEquation();
        Equations.push_back(Eq);
        }

    if(expect(Token::KW_end))
        goto _error;
    
    advance();

    
    return new ElifParse(condits , Equations);


}

Else *Parser::parseElse(){
    llvm::SmallVector<Equation *> Equations;
    if(expect(Token::KW_else))
        goto _error;
    
    advance();

    if(expect(Token::KW_colon))
        goto _error();
    
    advance();


    while(Tok.is(Token::id)){
        Equation Eq = parseEquation();
        Equations.push_back(Eq);
        }


    if(expect(Token::KW_end))
        goto _error();
    
    advance();



    return new ElifParse(Equations);


}

//------------------------------------------------------------------------------


Conditions *Parser::parseConditions(){


    Conditions *Left = parseCondition();
    while (Tok.isOneOf(Token::KW_and , Token::KW_or)){
        Conditions :: andOr AO = 
            Tok.is(Token::KW_and) ? Conditions::KW_and  : Conditions::KW_or;
        advance();
        Conditions *Right = parseCondition();
        Left = new Conditions(AO , Left , Right);  
    }

    return Left;

}


Condition *Parser::parseCondition () {

    Expr *Left = parseExpr();
    while (Tok.isOneOf(Token::KW_EqEq, Token::KW_eqNot , Token::KW_greaterEqual , Token::KW_lessEqual , Token::KW_lessThan , Token::KW_greaterThan)) {
        
        Condition::OperatorCondition Op;
            if (Tok.is(Token::KW_EqEq)) 
                Op = Condition::OperatorCondition::KW_EqEq;
            else if (Tok.is(Token::KW_eqNot))  
                Op= Condition::OperatorCondition::KW_eqNot;
            else if (Tok.is(Token::KW_greaterEqual))  
                Op= Condition::OperatorCondition::KW_greaterEqual;
            else if (Tok.is(Token::KW_lessEqual))  
                Op= Condition::OperatorCondition::KW_lessEqual;
            else if (Tok.is(Token::KW_lessThan))  
                Op= Condition::OperatorCondition::KW_lessThan;
            else if (Tok.is(Token::KW_greaterThan))  
                Op= Condition::OperatorCondition::KW_greaterThan;
            else 
                goto _error();



            advance();
            Expr *Right = parseExpr();
            Left = new Condition(Op, Left, Right);


    }
    return Left;

}

//----------------
Loop *Parser:: parseLoop(){
    Loop *Res = nullptr;
    if(!Tok.is(Token::KW_loopc)){
        goto _error();

    }
    advance();

    Conditions *Conditions = parseConditions;
    advance();

    if(!Tok.is(Token::KW_colon)){
        goto _error();
    }

    advance();

    if (!Tok.is(Token::KW_begin))
    {
        goto _error();
    }
    advance();
    llvm::SmallVector<Equation*> Equ;

    while(!Tok.is(Token::id)){
        if(Tok.is(Token::id)){
            Equation *e;
            e = parseEquation();

            if (!Tok.is(Token::semicolon))
            {
                goto _error;
            }
            if(e)
            {
                Equation.push_back(e);
            }
            
        }
    }

    Res = new Loop(Conditions,Equation);
    return Res;
}



_error():
   { while (Tok.getKind() != Token::eoi)
    {
        advance();
    }
    return nullptr;
    }





//-----------------------

