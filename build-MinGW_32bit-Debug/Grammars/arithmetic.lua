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

local newline = {
	name = 'newline', 
	pattern = lpeg.S'\n\r',
}

local left = {
	name = 'left', 
	pattern = space * lpeg.P'(' * space,
}
local right = {
	name = 'right', 
	pattern = space * lpeg.P')' * space,
}

local number = {
	name = 'number', 
	pattern = space * lpeg.R'09'^1 * (lpeg.P'.' * lpeg.R'09'^1)^-1,
}
local variable = {
	name = 'variable', 
	pattern = space * (lpeg.R'az' + lpeg.R'AZ')^1,
}

local unaryOp = {
	name = 'unaryOp', 
	pattern = space * (plus + minus) * space,
}
local binaryOp = {
	name = 'binaryOp', 
	pattern = space * (plus + minus + multi + fracal + power + indice) * space,
}

local logical = {
	name = 'logical',
	pattern = space * (lt + gt + eq + lte + gte + nlt + ngt + neq + nlte + ngte) * space,
}

local brackets = {
	name = 'brackets',
	pattern = node(left) * V'UnaryExp' * node(right)
}

local equation = {
	name = 'equation',
	pattern = V'UnaryExp' * (node(logical) * V'UnaryExp')^-1
}

---------------------------------------------------------
-- Errors

local invalidEquation = {
	name = 'invalid equation',
	pattern = (lpeg.P(1) - lpeg.P'\n')^0
}

local invalidTerm = {
	name = 'Invalid term',
	pattern = (lpeg.P(1) - V'Term')^0
}

local invalidOperator = {
	name = 'Invalid operator',
	pattern = (lpeg.P(1) - V'Operator')^0
}

local invalidLogical = {
	name = 'Invalid logical',
	pattern = logical.pattern
}

---------------------------------------------------------
-- Grammar

G = lpeg.Ct({"Start",

	Start = leaf(newline)^0 * V'PEquation' * ( leaf(newline)^1 * V'PEquation' )^0,

	PEquation = V'Equation' * error(invalidEquation), -- TODO: skusit zistit ako spravit aby nematchovalo empty string
	Equation = node(equation) * ( error(invalidLogical) * V'UnaryExp')^0,

	UnaryExp = leaf(unaryOp)^-1 * V'Exp',
	Exp = V"PTerm" * ( V'Operator' * V"PTerm" )^0,

	PTerm = V'Term' + ( error(invalidTerm) * V'Term'),
	Term = node(brackets) + leaf(number) + leaf(variable),

	POperator = V'Operator' + ( error(invalidOperator) * V'Operator'), -- TODO: zistit ako parsnut chybu o zlom operatore
	Operator = leaf(binaryOp) ,
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
