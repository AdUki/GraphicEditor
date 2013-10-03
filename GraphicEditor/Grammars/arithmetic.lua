local V = lpeg.V

---------------------------------------------------------
-- Operators
local plus = lpeg.P'+'
local minus = lpeg.P'-'
local multi = lpeg.P'*'
local fracal = lpeg.P'/'
local power = lpeg.P'^'
local indice = lpeg.P'_'
local root = lpeg.P'#'

---------------------------------------------------------
-- Logical
local lt = lpeg.P'<'
local gt = lpeg.P'>'
local eq = lpeg.P'='
local lte = lpeg.P'<='
local gte = lpeg.P'>='
local nlt = lpeg.P'!<'
local ngt = lpeg.P'!>'
local neq = lpeg.P'!='
local nlte = lpeg.P'!<='
local ngte = lpeg.P'!>='

local space = lpeg.S(" \t")^0

---------------------------------------------------------
-- Nodes

local newline = leaf{
	name = 'newline', 
	pattern = lpeg.S'\n\r',
}

local left = leaf{
	name = 'left', 
	pattern = space * lpeg.P'(' * space,
}
local right = leaf{
	name = 'right', 
	pattern = space * lpeg.P')' * space,
}

local number = leaf{
	name = 'number', 
	pattern = space * lpeg.R'09'^1 * (lpeg.P'.' * lpeg.R'09'^1)^-1,
}
local variable = leaf{
	name = 'variable', 
	pattern = space * (lpeg.R'az' + lpeg.R'AZ')^1,
}

local unaryOp = leaf{
	name = 'unaryOp', 
	pattern = space * (plus + minus) * space,
}
local binaryOp = leaf{
	name = 'binaryOp', 
	pattern = space * (plus + minus + multi + fracal + power + indice) * space,
}

local logical = leaf{
	name = 'logical',
	pattern = space * (lt + gt + eq + lte + gte + nlt + ngt + neq + nlte + ngte) * space,
}

---------------------------------------------------------
-- Node

local brackets = node{
	name = 'brackets',
	pattern = left() * V'UnaryExp' * right()
}

local equation = node{
	name = 'equation',
	pattern = V'UnaryExp' * (logical() * V'UnaryExp')^-1
}

---------------------------------------------------------
-- Errors

local invalidEquation = leaf{
	name = 'Invalid equation',
	pattern = (lpeg.P(1) - lpeg.P'\n')^0
}

local invalidTerm = leaf{
	name = 'Invalid term',
	pattern = (lpeg.P(1) - V'Term')^0
}

local invalidOperator = leaf{
	name = 'Invalid operator',
	pattern = (lpeg.P(1) - V'Operator')^0
}

local invalidLogical = leaf{
	name = 'Invalid logical',
	pattern = logical.pattern
}

---------------------------------------------------------
-- Grammar

G = lpeg.Ct({"Start",

	Start = newline()^0 * V'PEquation' * ( newline()^1 * V'PEquation' )^0,

	PEquation = V'Equation' * invalidEquation(), -- TODO: skusit zistit ako spravit aby nematchovalo empty string
	Equation = equation() * ( invalidLogical() * V'UnaryExp')^0,

	UnaryExp = unaryOp()^-1 * V'Exp',
	Exp = V"PTerm" * ( V'Operator' * V"PTerm" )^0,

	PTerm = V'Term' + ( invalidTerm() * V'Term'),
	Term = brackets() + number() + variable(),

	POperator = V'Operator' + ( invalidOperator() * V'Operator'), -- TODO: zistit ako parsnut chybu o zlom operatore
	Operator = binaryOp() ,
})

---------------------------------------------------------
-- Extensions

E = {'arithmetic', 'ath'}

-- -- good input
-- local input = [[5.1 * (8 + 5)
-- 5 + (1 +5)
-- 8.3*8.2
-- (1/2)
-- 1 + a
-- a+1+8+5=5
-- a=<8
-- 5 + (8)
-- 5 + ( 8 + 5 )
-- 8 + 9 / 10 + (8/5) + (5 + 8 )
-- (1 + 2) + 5
-- ]]

-- -- error input
-- local input = [[5.1 (8 + 5)
-- sd+'(f).;'d+;asf';.4'.sd;afas,d/.f
-- 5 + (1 +5)
-- 8.3*8.2
-- )(1//2)
-- 1 + +--/-*+.a
-- 1 5 + 5
-- 1++5
-- a==5
-- a=5=8
-- 5 + ) 8
-- 5 + ( 8 + 5 )
-- 8 +- 9 / 10 )+ (8//5) + (5 + 8 0)
-- ]]
-- local input2 = [[
-- ((1+1)/(1+1)+(1+1)/(1+1)+(1+1)/(1+1)+(1+1)/(1+1)+((1+1)/(1+1+(1+1)/(1+1+(1+1)/(1+1+(1+1)/(1+1+(1+1)/(1+1)+(1+1)/(1+1)+(1+1)/(1+1)+(1+1)/(1+1)+(1+1)/(1*(1+1)/(1+1)*(1+1)/(1+1)*(1+1)/(1+1)*(1+1)/(1+1*(1+1)/(1+1/(1+1)/(1+1*(1+1)/(1+1))))+1)))))+1+1)/(1+1)+(1+1)/(1+1)+1+1)/(1+1)
-- (100 * (1+1)/(1+ *(1+1)/(1+1)* 1) + 5) * 4 + (1+1)/((1+1)/(((1+1)/(1+1+(1+1)/(1+1+(1+1)/(1+1)+(1+1)/(1+1+(1+1)/(1+1)+(1+1)/(1+1)+(1+1)/(1+1)+(1+1)/(1+1))))+1+1)/(1+1)+1+1)+1+1)
-- ]]
-- local input = "(1)+1"
