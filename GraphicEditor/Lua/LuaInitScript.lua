#pragma once //"
const char* luaInitScript = R"( -- LONG STRING BEGIN

-- NOTE: Line for printing to console with Lua5.1 interpreter on Windows
io.stdout:setvbuf 'no'

---------------------------------------------------------
-- Includes
lpeg = require 'lpeg'
lfs = require 'lfs'

------------------------------------------------------------------------------------------------------------------ FILES HANDLING

---------------------------------------------------------
-- Opened files table, indexed with full file paths
-- @param grammar
-- @param extension file suffix
-- @param tree actual AST
Files = {
    extension = nil,
    grammar = nil,
    tree = nil,
}

---------------------------------------------------------
-- Opens file in location
-- @param path (string) path to file
-- @return[1] fullpath
-- @return[2] opened file reference
function openFile(path)
    -- TODO: pridat warning ked je subor read-only
    local file = assert(io.open(path, "r"))
    local text = file:read("*all")
    file:close()

    local workDir = lfs.currentdir()
    local filepath = lfs.fullpath(path)
    local fileExtension = filepath:match('.*[%.\\/](.*)$') -- get extension, relies that '\' sign cannot be in extension
    local fileGrammar = assert(Extensions[fileExtension], 'No grammar for extension'.. fileExtension .. 'defined').grammar

    Files[filepath] = {
        extension = fileExtension,
        grammar = fileGrammar,
        tree = fileGrammar:match(text)
    }

    return filepath, Files[filepath]
end

---------------------------------------------------------
-- Save file with its filename to its location
-- @param path (string) path to file
function saveFile(path)
    assert(Files[path], path .. ': File is not open!')
    local file = assert(io.open(Files[path].filepath, "w"))
    -- TODO: vyrob text podla stromu
    file:write(File[path].text)
    file:close()
end

---------------------------------------------------------
-- Save file with different filename and location
-- @param sourcePath (string) path to file, which we want to save
-- @param outputPath (string) location where we want to save that file
function saveFileAs(sourcePath, outputPath)
    assert(Files[sourcePath], sourcePath .. ': File is not open!')
    local file = assert(io.open(outputPath, "w"))
    -- TODO: vyrob text podla stromu
    file:write(File[path].text)
    file:close()
    Files[outputPath] = Files[sourcePath]
    Files[sourcePath] = nil
end

---------------------------------------------------------
-- Saves all files to disk
function saveAll()
    -- TODO
end

---------------------------------------------------------
-- Closes file
-- @param path (string) path to file
function closeFile(path)
    assert(Files[path], path .. ': File is not open!')
    Files[path] = nil
end

------------------------------------------------------------------------------------------------------------------ GRAMMARS HANDLING

---------------------------------------------------------
-- Table with all grammars. Indexed with grammar name.
-- @param grammar stored Lpeg grammar
Grammars = {
    grammar = nil,
}

---------------------------------------------------------
-- Table with all grammars extensions. Indexed with extensions
-- @param grammar entitiy from table Grammars
Extensions = {
    grammar = nil
}

---------------------------------------------------------
-- Loads grammars from directory to Grammars table
function loadGrammars()
    local count = 0
    
    local attr = lfs.attributes ('Grammars')
    assert (type(attr) == "table") -- No Grammars folder 
    assert (attr.mode == 'directory') -- No Grammars is not directory

    for file in lfs.dir'Grammars' do
        file = 'Grammars/' .. file
        
        if lfs.attributes(file, 'mode') == 'file' then
            loadGrammar(file)
            count = count + 1
        end
    end

    if count == 0 then
        dump "Warning: No grammars loaded!"
    end
end

---------------------------------------------------------
-- Load grammar and save it to Grammars table
function loadGrammar(filepath) 
    local grammarName = filepath:match(".*/(.*)%..*$")
    local grammar = assert(loadfile(filepath), 'Grammar ' .. grammarName .. 'failed to parse')

    grammar();
    
    -- Save grammar to global grammar table
    Grammars[#Grammars + 1] = {
        name = grammarName,
        grammar = G,
        chunk = grammar,
    }

    assert(G, 'Grammar must be defined in global variable G')
    assert(E, 'Extensions must be defined in global variable E')

    -- Save extensions to hash table
    for i, v in ipairs(E) do
        Extensions[v] = Grammars[#Grammars]
    end

    G = nil
    E = nil
end

------------------------------------------------------------------------------------------------------------------ ABSTRACT SYNTAX TREE HANDLING

local globalCallbackCounter = 0

local astMetatable = {
    __call = function(table, arg) 
        return table.capturePattern
    end
}
    
---------------------------------------------------------
-- Produces AST leaf capture from defined lpeg pattern
-- NO CHILDREN
-- @param var table with fields name(string) and pattern(lpeg.pattern)
-- @return leaf table
function leaf(var)
    dump(var.name, '\n')

    assert (type(var) == 'table',                "Table expected!")
    assert (type(var.name) == 'string',          "Must have 'name' field")
    assert (lpeg.type(var.pattern) == 'pattern', "Must have 'pattern' field")
    assert (var.capturePattern == nil,           "Index 'capturePattern' is system reserved!")
    assert (var.value == nil,                    "Index 'value' is system reserved!")

    var.capturePattern = lpeg.Ct( lpeg.Cg(lpeg.Cc(var), 'table') * lpeg.Cg(var.pattern, 'value') )
    
    setmetatable(var, astMetatable)
    return var
end

---------------------------------------------------------
-- Produces AST node capture from defined lpeg pattern
-- HAS CHILDREN
-- @param var table with fields name(string) and pattern(lpeg.pattern)
-- @return node table
function node(var)
    dump(var.name, '\n')

    assert (type(var) == 'table',                "Table expected!")
    assert (type(var.name) == 'string',          "Must have 'name' field")
    assert (lpeg.type(var.pattern) == 'pattern', "Must have 'pattern' field")
    assert (var.capturePattern == nil,           "Index 'capturePattern' is system reserved!")
    assert (var.value == nil,                    "Index 'value' is system reserved!")
    
    var.capturePattern = lpeg.Ct( lpeg.Cg(lpeg.Cc(var), 'table') * lpeg.Cg(lpeg.Ct(var.pattern), 'value') )

    setmetatable(var, astMetatable)
    return var
end

---------------------------------------------------------
-- Prints AST tree
-- @param ast grouped tables that are ast tree
function dumpAST(ast)
    function tprint (tbl, indent)
        for key, node in ipairs(tbl) do
            dump(string.rep(" ", indent) .. '[' .. key .. '] s=' .. tostring(node.startIndex) ..' e=' .. tostring(node.endIndex))
            dump(' n=' .. node.table.name)

            if type(node.value) == 'table' then
                dump('\n')
                tprint(node.value, indent + 3)
            else
                dump('  v=' .. node.value .. '\n')
            end
        end
    end
    tprint(ast, 0)
    dump'\n'
end

---------------------------------------------------------
-- Prints AST tree
-- @param ast grouped tables that are ast tree
function crawlAST(ast)
    
    globalCallbackCounter = 0

    local currentTextIndex = 0;
    local indexPath = {}
    local reverseIndexPath = {}

    ---------------------------------------------------------
    -- 
    -- @param node
    -- @param indexPath
    -- @param reverseIndexPath
    -- @param startTextIndex
    -- @param endTextIndex
    local function ASTnodeCallback(node, indexPath, reverseIndexPath, startTextIndex, endTextIndex)
        globalCallbackCounter = globalCallbackCounter + 1
        -- TODO: link with Qt
        dump(tostring(globalCallbackCounter) .. ':', startTextIndex, endTextIndex, indexPath, reverseIndexPath, '\n')
    end

    ---------------------------------------------------------
    -- Recursive AST search function with callback ASTnodeCallback
    -- @param tbl AST element
    local function search(tbl)
        local size = #tbl
        local startTextIndex = currentTextIndex
        local currentParent = tbl

        for key, node in ipairs(tbl) do
            node.parent = currentParent
            
            -- Insert current node to path
            indexPath[#indexPath + 1] = tostring(key)
            reverseIndexPath[#reverseIndexPath + 1] = tostring(size - key + 1)
                
            if type(node.value) == 'table' then
                -- We must go deeper!
                search(node.value, indexPath)
            else
                currentTextIndex = currentTextIndex + node.value:len()
            end

            ASTnodeCallback(node.table, 
                table.concat(indexPath, ':'), table.concat(reverseIndexPath, ':'), 
                startTextIndex, currentTextIndex)

            startTextIndex = currentTextIndex

            -- Pop current node from path
            indexPath[#indexPath] = nil
            reverseIndexPath[#reverseIndexPath] = nil
        end
    end

    search(ast)
end


---------------------------------------------------------
local function deleteItemCallback(node)
end

---------------------------------------------------------
local function addItemCallback(node)
end

-- Indicates number of reparseFile
local parseCycle = 0

---------------------------------------------------------
-- Recursive AST search function with callback ASTnodeCallback
-- @param filepath
-- @param text
function reparseFile(filepath, text)

    local function compareNodes(node1, node2)
        if node1 == nil or node2 == nil then 
            return false 
        end
        return node1.table.name == node2.table.name
    end

    local function findMaximum(tbl1, tbl2)
        if tbl1 == nil or type(tbl1.value) ~= 'table' then return #tbl2.value end
        if tbl2 == nil or type(tbl2.value) ~= 'table' then return #tbl1.value end
        return #tbl1.value > #tbl2.value and #tbl1.value or #tbl2.value
    end

    local function checkForEnd(tbl1, tbl2)
        -- Check if it is visited node
        if  tbl1 == nil and tbl2 == nil or
            tbl1 and tbl1.visit == parseCycle and 
            tbl2 and tbl2.visit == parseCycle
        then
            dump 'END OF TREE!\n'
            return true
        end
        return false
    end

    local function compareFront(old, new, parent)

        -- Check if it is visited node
        if  checkForEnd(old, new) then 
            finishFront = true
            return
        end
        if old and old.visit == parseCycle then old = nil end
        if new and new.visit == parseCycle then new = nil end

        -- Compare nodes
        if compareNodes(old, new) then

            if type(old.value) == 'string' and type(new.value) == 'string' and old.value ~= new.value then
                dump('UPDATE: ' .. old.table.name .. ' ' .. old.value .. ' to ' .. new.value .. '\n')
            end

            -- Copy values from old
            -- TODO: move references to new

        else
            
            -- If we havent search back then do it
            if finishFront == false then
                finishFront = true
                dump 'START SEARCH BACKWARD!\n'
                -- Let compareBack() work his job
                coroutine.yield()
                -- Check if back search didnt marked our node
                if  checkForEnd(old, new) then return end
                if old and old.visit == parseCycle then old = nil end
                if new and new.visit == parseCycle then new = nil end
            end

            -- Remove old nodes
            if old ~= nil then
                dump('REMOVE: ' .. old.table.name .. ' ' .. tostring(old.value) .. '\n')
                deleteNodesTable[#deleteNodesTable + 1] = old
            end
            -- Add new nodes
            if new ~= nil then
                dump('ADD: ' .. new.table.name .. ' ' .. tostring(new.value) .. '\n')
            end

        end

        -- Mark start index
        if new then 
            new.startIndex = currentTextIndex 
        end

        -- Check if it is node with table
        if  (old and type(old.value) == 'table') or 
            (new and type(new.value) == 'table') then
            
            -- Find maximum
            local max = findMaximum(old, new)
            
            -- Recursive search
            for index = 1, max, 1 do
                compareFront(old and old.value[index],new and new.value[index], old, index)
                if finishFront then return end
            end

        end

        -- Calculate index and mark end index and visit flag
        if old then 
            old.visit = parseCycle 
        end
        if new then
            
            if type(new.value) ~= 'table' then
                currentTextIndex = currentTextIndex + new.value:len()
            end
            new.endIndex = currentTextIndex
            new.visit = parseCycle

        end
    end

    local function compareBack(old, new, parent)

        -- Check if it is visited node
        if  checkForEnd(old, new) then 
            finishBack = true
            return 
        end

        if old and old.visit == parseCycle then old = nil end
        if new and new.visit == parseCycle then new = nil end

        -- Compare nodes
        if compareNodes(old, new) then

            if type(old.value) == 'string' and type(new.value) == 'string' and old.value ~= new.value then
                dump('UPDATE: ' .. old.table.name .. ' ' .. old.value .. ' to ' .. new.value .. '\n')
            end
            -- Copy values from old
            -- TODO: move references to new

        else

            -- Remove old nodes
            if old ~= nil then
                dump('REMOVE: ' .. old.table.name .. ' ' .. tostring(old.value) .. '\n')
                deleteNodesTable[#deleteNodesTable + 1] = old
            end
            -- Add new nodes
            if new ~= nil then
                dump('ADD: ' .. new.table.name .. ' ' .. tostring(new.value) .. '\n')
            end

        end

        -- Mark that we were here and end index
        if new then 
            new.endIndex = currentTextBackIndex 
        end

        -- Check if it is node or leaf
        if old and type(old.value) == 'table' or 
           new and type(new.value) == 'table' then
            
            -- Find maximum
            local max = findMaximum(old, new)
            local oldSize = old and #old.value
            local newSize = new and #new.value

            -- Recursive search
            for index = 1, max, 1 do
                compareBack(old and old.value[oldSize - index + 1], new and new.value[newSize - index + 1], old)
                if finishBack then return end
            end

        end

        -- Calculate index and mark start index
        if new then
            
            if type(new.value) ~= 'table' then
                currentTextIndex = currentTextIndex + new.value:len()
            end
            new.startIndex = currentTextBackIndex

        end


        -- Calculate index and mark end index and visit flag
        if old then 
            old.visit = parseCycle 
        end
        if new then
            
            if type(new.value) ~= 'table' then
                currentTextBackIndex = currentTextBackIndex - new.value:len()
            end
            new.startIndex = currentTextBackIndex 
            new.visit =parseCycle

        end

    end

    -- local combineAST = coroutine.create( function(old, new)
    --     -- cycle trees from front and back

    --     -- deleteItemCallback()
    --     -- addItemCallback()
    -- end)

    parseCycle = parseCycle + 1

    local file = assert(Files[filepath], 'No opened file with ' .. filepath)
    local oldTree = file.tree
    local newTree = file.grammar:match(text)

    finishFront = false
    finishBack = false

    currentTextIndex = 0
    currentTextBackIndex = #text

    dump('\n\nText lenght is ' .. tostring(currentTextBackIndex) .. '\n')

    deleteNodesTable = {}
    
    assert(oldTree and newTree)

    dump'\n\nOLD TREE\n'
    dumpAST(oldTree)

    dump '\n\nNEW TREE\n'
    dumpAST(newTree)
    
    local compareFrontCoroutine = coroutine.create(compareFront)
    local compareBackCoroutine = coroutine.create(compareBack)

    local rootOld = { table = { name='root' }, value = oldTree }
    local rootNew = { table = { name='root' }, value = newTree }

    coroutine.resume(compareFrontCoroutine, rootOld, rootNew)
    
    compareBack(rootOld, rootNew)
    dump'SEARCH BACKWARD ENDED!\n\n'

    coroutine.resume(compareFrontCoroutine)

    return newTree
end

------------------------------------------------------------------------------------------------------------------ TESTING COMMANDS

loadGrammars()

--local fullpath, openedfile
--fullpath, openedFile = openFile'../test input/input.arithmetic'

-- crawlAST(openedFile.tree)


-- openedFile.tree = reparseFile(fullpath, '1 + 2 + 3 + 4') --> add nothing
-- openedFile.tree = reparseFile(fullpath, 'aaa + (1 + (2 + 3) + 4)') --> add to the beginning
--openedFile.tree = reparseFile(fullpath, [[((1) + 2) + (a + (100 + 10 + 8)) + ((1) + 2) + (3 + (4 + ((1) + 2)+ x + (3 + (4  + ((1) + 2) + (3 + (4))))))
--]]) --> add in the middle

--openedFile.tree = reparseFile(fullpath, [[a + ((1) + 2) + (a + (100 + 10 + 8)) + ((1) + 2) + (3 + (4 + ((1) + 2)+ x + (3 + (4  + ((1) + 2) + (3 + (4))))))
--]]) --> add in the middle

-- openedFile.tree = reparseFile(fullpath, '1 + 2 + 3 + 4') --> add to the beginning with nodes
-- openedFile.tree = reparseFile(fullpath, '1 + 2 + 3 + 4') --> add to the end

--dump'\n\n'
--dumpAST(openedFile.tree)

---------------------------------------------------------
-- DONT DELETE! LONG STRING TERMINATOR )";
