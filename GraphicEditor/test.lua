print '\nLAUNCHING TEST SCRIPT'
print '---------------------------------------------------------'

local file = 'testfile.art'
local tree

---------------------------------------------------------
-- Prints AST tree
-- @param ast grouped tables that are ast tree
function countAST(ast)

    if ast == nil then
        print "Given tree is nil (error during parse)!"
        return
    end

    local count = 0

    function tcount (tbl)
        for key, node in ipairs(tbl) do
            count = count + 1

            if type(node.value) == 'table' then
                tcount(node.value)
            end

        end
    end
    tcount(ast)

    return count
end


function newText(text)

	local oldNodesCount = countAST(tree)
	oldNodesCount = oldNodesCount or 0

	if initialized == nil then
		tree = registerFile(file, text, 'arithmetic').tree
		initialized = true
	else
		tree = reparseFile(file, text)
	end

	local newNodesCount = countAST(tree)

	print ('Nodes in tree: ', newNodesCount)
	dumpAST(tree)

	print ('Comparison (must be zero!): ' .. tostring(oldNodesCount + totalAdds - totalRemoves - newNodesCount))
	-- if oldNodesCount + totalAdds - totalRemoves - newNodesCount ~= 0 then
	-- 	print '###### !!! NOT ZERO !!! ######'
	-- 	assert(false)
	-- end
	
	totalAdds = 0
	totalRemoves = 0
end

newText 'aaa+(bbb)+123'
newText '(aaa+bbb)+9999'

-- newText '1'
-- newText '(1'
-- newText '(1)'
-- newText '1)'

-- newText '1'
-- newText '1+2+3+4+5+6'
-- newText '(1+2)+3+4+(5+6)'
-- newText '(1+2)+3+4+(5)+6)'
-- newText '(1+(2)+3+4+(5)+6)'
-- newText '(1+(2)+3+4+(5+6)'
-- newText '(1+2)+3+4+(5+6)'

-- newText '(1+2)+3+4+(5+x+6)'

-- newText '(1+2)+3+4+(5+(200/5)*800+x+y+6)'

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
