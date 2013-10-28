print '\nLAUNCHING TEST SCRIPT'
print '---------------------------------------------------------'

local file = 'testfile.art'
local tree

tree = registerFile(file, [[aa bb cc ]], 'default').tree
dumpAST(tree)

tree = reparseFile(file, [[cc aabb
sdfas
]])
dumpAST(tree)

tree = reparseFile(file, [[cc xx aabb ]])
dumpAST(tree)

-- tree = registerFile(file, '5+5', 'arithmetic').tree
-- dumpAST(tree)

-- tree = reparseFile(file, '5+4+4+4+4+4-5')
-- dumpAST(tree)


-- tree = reparseFile(file, '8-6')
-- dumpAST(tree)

-- tree = reparseFile(file, '8-b+6')
-- dumpAST(tree)

-- -- dumpAST(tree)

-- tree = reparseFile(file, 'aaa + (1 + (2 + 3) + 4)') --> add to the beginning

-- print '\n'

-- tree = reparseFile(file, [[((1) + 2) + (a + (100 + 10 + 8)) + ((1) + 2) + (3 + (4 + ((1) + 2)+ x + (3 + (4  + ((1) + 2) + (3 + (4))))))
-- ]]) --> add in the middle

-- print '\n'

-- tree = reparseFile(file, [[a + ((1) + 2) + (a + (100 + 10 + 8)) + ((1) + 2) + (3 + (4 + ((1) + 2)+ x + (3 + (4  + ((1) + 2) + (3 + (4))))))
-- ]]) --> add in the middle

-- print '\n'

-- tree = reparseFile(file, '1 + 2 + 3 + 4') --> add to the beginning with nodes

-- print '\n'

-- tree = reparseFile(file, '1 + 2 + 3 + 4') --> add to the end

-- print '\n'

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
