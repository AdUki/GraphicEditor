#pragma once //"
const char* luaInitScript = R"( -- DO NOT DELETE! LONG STRING BEGIN

io.stdout:setvbuf 'no' -- NOTE: Line for printing to console with Lua5.1 interpreter on Windows
dump '\nInitialization script started\n'
dump '---------------------------------------------------------\n'

---------------------------------------------------------
-- Includes
lpeg = require 'lpeg'
lfs = require 'lfs'

------------------------------------------------------------------------------------------------------------------ FILES HANDLING

---------------------------------------------------------
-- Opened files table, indexed with their Qt pointers
-- @param grammar
-- @param extension file suffix
-- @param tree actual AST
-- @param absolutePath fullpath with filename
Files = {
    extension = nil,
    grammar = nil,
    tree = nil,
    absolutePath = nil,
}
LastFile = nil
setmetatable(Files, {
    __index = function(table, key) 
        LastFile = table[key] or LastFile
        return table[key]
    end,

    __newindex = function(table, key, value)
        rawset(table, key, value) -- we must use rawset to avoid recursive assingments
        LastFile = Files[key]
        return value
    end,
})

---------------------------------------------------------
-- Directly register file
-- @param name file indentifier
-- @param fileExtension
-- @param text
-- @param grammarName
-- @return file reference
function registerFile(name, text, grammarName)
    
    grammarName = grammarName or "default"
    local fileGrammar = assert(Grammars[grammarName] or DefaultGrammar)

    Files[name] = {
        grammar = fileGrammar,
        tree = {}
    }

    dump ('File "' .. tostring(name) .. '" registered with "' .. grammarName .. '" grammar')

    if text then
        reparseFile(name, text)
    end
    return Files[name]
end

---------------------------------------------------------
-- Closes / unregisters file
-- @param path (string) path to file
function unregisterFile(name)
    assert(Files[name], tostring(name) .. ': File is not registered!')
    Files[name] = nil

    dump ('File "' .. tostring(name) .. '" unregistered')
end

---------------------------------------------------------
function setFileAbsolutePath(name, path)
    local file = assert(Files[name], path .. ': File is not registered!')
    file.absolutePath = path

    dump ('File "' .. tostring(name) .. '" absolute path set to: ' .. path .. '')
end

---------------------------------------------------------
function setFileGrammar(name, grammar)
    local file = assert(Files[name], tostring(name) .. ': File is not registered!')
    local newGrammar = assert(Grammars[grammar], 'No grammar "'.. grammar .. '" defined')

    -- TODO: zmaz AST s osobitnou funkciou
    -- reparseFile(name, "")

    file.grammar = newGrammar;
    -- reparseFile(name, file.text)

    dump ('File "' .. tostring(name) .. '" switched to  "' .. grammar .. '" grammar')
end

------------------------------------------------------------------------------------------------------------------ GRAMMARS HANDLING

---------------------------------------------------------
-- Table with all grammars. Indexed with grammar name.
-- @param grammar stored Lpeg grammar
Grammars = {}

---------------------------------------------------------
-- Default grammar for text files
DefaultGrammar = nil

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

    assert(G, 'Grammar must be defined in global variable G in file: ' .. filepath .. '\n')
    assert(E, 'Extensions must be defined in global variable E in file: ' .. filepath .. '\n')

    -- Save grammar to global grammar table
    Grammars[grammarName] = G

    -- Save extensions to hash table
    for i, v in ipairs(E) do
        Extensions[v] = Grammars[grammarName]
    end

    dump('Grammar "'.. grammarName .. '" loaded with extensions: ' .. table.concat(E, ', ') .. '\n')

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
    --dump(var.name, '\n')

    assert (type(var) == 'table',                "Table expected!")
    assert (type(var.name) == 'string',          "Must have 'name' field")
    assert (lpeg.type(var.pattern) == 'pattern', "Must have 'pattern' field")
    assert (var.capturePattern == nil,           "Index 'capturePattern' is system reserved!")
    assert (var.value == nil,                    "Index 'value' is system reserved!")

    var.capturePattern = lpeg.Ct( 
        lpeg.Cg(lpeg.Cp(), 'startIndex') *
        lpeg.Cg(lpeg.Cc(var), 'table') * 
        lpeg.Cg(var.pattern, 'value') *
        lpeg.Cg(lpeg.Cc('leaf'), 'type') * 
        lpeg.Cg(lpeg.Cp(), 'endIndex')
    )
    
    setmetatable(var, astMetatable)
    return var
end

---------------------------------------------------------
-- Produces AST node capture from defined lpeg pattern
-- HAS CHILDREN
-- @param var table with fields name(string) and pattern(lpeg.pattern)
-- @return node table
function node(var)
    --dump(var.name, '\n')

    assert (type(var) == 'table',                "Table expected!")
    assert (type(var.name) == 'string',          "Must have 'name' field")
    assert (lpeg.type(var.pattern) == 'pattern', "Must have 'pattern' field")
    assert (var.capturePattern == nil,           "Index 'capturePattern' is system reserved!")
    assert (var.value == nil,                    "Index 'value' is system reserved!")
    
    var.capturePattern = lpeg.Ct(
        lpeg.Cg(lpeg.Cp(), 'startIndex') *
        lpeg.Cg(lpeg.Cc(var), 'table') * 
        lpeg.Cg(lpeg.Ct(var.pattern), 'value') *
        lpeg.Cg(lpeg.Cc('node'), 'type') * 
        lpeg.Cg(lpeg.Cp(), 'endIndex')
    )

    setmetatable(var, astMetatable)
    return var
end

---------------------------------------------------------
-- Prints AST tree
-- 
-- @param ast grouped tables that are ast tree
function dumpAST(ast)

    if ast == nil then
        print "Given tree is nil (error during parse)!"
        return
    end

    function tprint (tbl, indent)
        for key, node in ipairs(tbl) do
            dump(string.rep(" ", indent) .. '[' .. key .. '] s=' .. tostring(node.startIndex) ..' e=' .. tostring(node.endIndex))
            dump(' n=' .. node.table.name)

            if type(node.value) == 'table' then
                dump('\n')
                tprint(node.value, indent + 3)
            else
                dump('  v="' .. node.value .. '"\n')
            end
        end
    end
    tprint(ast, 0)
end

---------------------------------------------------------
-- Prints AST tree
-- 
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
    -- @param AST table
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
-- @param
-- @param
function reparseFile(name, text)

    print "\nReparsing file...\n===============================================\n"

    ---------------------------------------------------------
    local function compareNodes(node1, node2)

        -- NOTE: print only for debug
        -- print("Comparing [" .. tostring(node1 and node1.table and node1.table.name) .. "," .. tostring(node1 and node1.value) 
        --     .. "] with [" .. tostring(node2 and node2.table and node2.table.name) .. "," .. tostring(node2 and node2.value) .. "]")
        
        if node1 == nil or node2 == nil then
            return false
        end

        return node1.table.name == node2.table.name
    end

    ---------------------------------------------------------
    -- Compares two tables from ASTrees
    -- 
    -- @param table of old tree
    -- @param table of new tree
    -- @param parent to which we will put new nodes
    function compareTables(oldTable, newTable, parent)

        assert(oldTable ~= nil or newTable ~= nil, "Trying to compare nil with nil!")

        -- We will set empty table instead of nil because when we index empty table we will get nil
        oldTable = oldTable or {}
        newTable = newTable or {}

        local frontIndex = 1

        -- Start to compare from front until first error
        while compareNodes(oldTable[frontIndex], newTable[frontIndex]) do  
            compareTrees(oldTable[frontIndex], newTable[frontIndex], frontIndex, parent)
            frontIndex = frontIndex + 1
        end
        frontIndex = frontIndex - 1

        -- Set back indexes
        local sizeOfOld = #oldTable
        local sizeOfNew = #newTable
        local backOldIndex = 0
        local backNewIndex = 0

        -- Start compare from back until all nodes are compared
        while sizeOfOld ~= 0 or sizeOfNew ~= 0 do

            if sizeOfOld - backOldIndex < frontIndex then
                oldTable = {}
                sizeOfOld = 0
            end

            if sizeOfNew - backNewIndex < frontIndex then
                newTable = {}
                sizeOfNew = 0
            end

            compareTrees(
                oldTable[sizeOfOld - backOldIndex + 1], 
                newTable[sizeOfNew - backNewIndex + 1], 
                sizeOfNew - backNewIndex + 1, parent)

            backNewIndex = backNewIndex + 1
            backOldIndex = backOldIndex + 1
        end
    end

    ---------------------------------------------------------
    -- Functions compare trees represented by their root nodes
    -- 
    -- @param root node of old tree
    -- @param root node of new tree
    -- @param index of parent node where we will put new nodes
    function compareTrees(old, new, index, parent)

        if old == nil and new == nil then
            -- print ("Rejecting", old, new)
            return
        end
        -- print "Accepting"

        -- Compare nodes
        local nodesAreSame = compareNodes(old, new)
        if nodesAreSame then

            -- Copy values from old
            new.instance = old.instance

            -- Compare values and update nodes if necessary
            if  type(old.value) == 'string' and 
                type(new.value) == 'string' and 
                old.value ~= new.value then

                updateElement(new, old)
            end

        -- We add nodes BEFORE processing children
        elseif new ~= nil then
            addElement(new, parent, index)
        end

        -- Check for tables
        local tableNodeFromNew = nil
        local tableNodeFromOld = nil
        if old and type(old.value) == 'table' and new and type(new.value) == 'table' then
            -- Compare tables
            tableNodeFromNew = new.value
            tableNodeFromOld = old.value

        elseif old and type(old.value) == 'table' then
            -- Delete old nodes
            tableNodeFromOld = old.value

        elseif new and type(new.value) == 'table' then
            -- Add new nodes
            tableNodeFromNew = new.value
        end

        -- Process childrens of tables
        if tableNodeFromNew ~= nil or tableNodeFromOld ~= nil then
            compareTables(tableNodeFromOld, tableNodeFromNew, new)
        end

        -- We delete old nodes AFTER processing children
        if not nodesAreSame and old ~= nil then
            removeElement(old)
        end
    end

    ---------------------------------------------------------
    -- initialization of variables
    local file = assert(Files[name], 'No opened file: ' .. tostring(name))
    local oldTree = file.tree
    local newTree = file.grammar:match(text)

    if newTree == nil then

        -- if grammar failed to parse whole tree, return error
        print 'Reparsing done with error!\n'
        return

    else

        -- when successful compare trees and set new tree
        local rootOld = { table = { name='root' }, value = oldTree }
        local rootNew = { table = { name='root' }, value = newTree }
        compareTrees(rootOld, rootNew, 1, nil)

        file.tree = newTree

        print "Reparsing file done!\n"
        return newTree
    end
end

------------------------------------------------------------------------------------------------------------------ ELEMENT HANDLING

---------------------------------------------------------
-- Adds element from NEW TREE
-- @param item from new tree
-- @param parent from new tree
-- @param index from parent from new tree
function addElement(element, parent, index)

    -- Lua is indexing from 1, C++ from 0
    index = index - 1

    -- TODO formatovanie objektu
    if element.type == 'node' then
        element.instance = QT_addGrid(parent.instance, index )
    elseif element.type == 'leaf' then
        element.instance = QT_addItem(parent.instance, index , element.value)
    end

    print('ADD: {' .. tostring(element.instance) .. ' ' .. element.table.name .. ' "' .. tostring(element.value) 
        .. '"} to parent {' .. tostring(parent.instance) .. ' ' .. tostring(parent and parent.table.name) .. '} at index ' .. tostring(index + 1))
end

---------------------------------------------------------
-- Removes element from OLD TREE
-- @param item from old tree
-- @param index from parent from old tree
function removeElement(element)
    print('REMOVE: ' .. element.table.name .. ' "' .. tostring(element.value) .. '"')
    QT_removeElement(element.instance)
end

---------------------------------------------------------
function updateElement(newElement, oldElement)
    print('UPDATE: ' .. tostring(newElement.instance) .. ' ' .. oldElement.table.name .. ' "' .. oldElement.value .. '" to "' .. newElement.value .. '"')
    QT_updateItem(newElement.instance, newElement.value)
end

------------------------------------------------------------------------------------------------------------------ DEFAULT GRAMMAR
DefaultGrammar = lpeg.Ct({ 
    "Start",
    Start = lpeg.V'Line' * (lpeg.V'NewLine' * lpeg.V'Line')^0,
    Line = (node{ name = 'line', pattern = lpeg.V'Word' * (lpeg.V'Space' * lpeg.V'Word')^0 })(),
    Word = (leaf{ name = 'word', pattern = lpeg.P(1 - lpeg.S' \t\n')^1 })(),
    Space = (leaf{ name = 'space', pattern = lpeg.S' \t'^1 })(),
    NewLine = (leaf{ name = 'newline', pattern = lpeg.S'\n'^1 })()
})

------------------------------------------------------------------------------------------------------------------ ENDING COMMANDS

loadGrammars()

dump 'Initialization script ended successfuly\n\n'

---------------------------------------------------------
-- DON NOT DELETE! LONG STRING TERMINATOR )";
