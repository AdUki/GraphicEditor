#pragma once //"
const char* luaInitScript = R"( -- DO NOT DELETE! LONG STRING BEGIN

-- NOTE: Line for printing to console with Lua5.1 interpreter on Windows
io.stdout:setvbuf 'no'
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

    dump ('File "' .. tostring(name) .. '" registered with "' .. grammarName .. '" grammar\n')

    if text then
        reparseFile(name, text)
    end
    return Files[name]
end

---------------------------------------------------------
-- Closes / unregisters file
-- @param path (string) path to file
function unregisterFile(name)
    assert(Files[name], name .. ': File is not registered!')
    Files[name] = nil

    dump ('File "' .. tostring(name) .. '" unregistered\n')
end

---------------------------------------------------------
function setFileAbsolutePath(name, path)
    local file = assert(Files[path], path .. ': File is not registered!')
    file.absolutePath = path

    dump ('File "' .. tostring(name) .. '" absolute path set to: ' .. path .. '\n')
end

---------------------------------------------------------
function setFileGrammar(name, grammar)
    local file = assert(Files[name], name .. ': File is not registered!')
    local newGrammar = assert(Grammars[grammar], 'No grammar "'.. grammar .. '" defined')

    -- TODO: zmaz AST s osobitnou funkciou
    -- reparseFile(name, "")

    file.grammar = newGrammar;
    -- reparseFile(name, file.text)

    dump ('File "' .. tostring(name) .. '" switched to  "' .. grammar .. '" grammar\n')
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
        dump "Warning: No grammars loaded!\n"
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
                dump('  v="' .. node.value .. '"\n')
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

    ---------------------------------------------------------
    local function compareNodes(node1, node2)
        if node1 == nil or node2 == nil then 
            return false 
        end

        return node1.table.name == node2.table.name
    end

    ---------------------------------------------------------
    local function findMaximum(tbl1, tbl2)
        if tbl1 == nil or type(tbl1.value) ~= 'table' then return #tbl2.value end
        if tbl2 == nil or type(tbl2.value) ~= 'table' then return #tbl1.value end
        return #tbl1.value > #tbl2.value and #tbl1.value or #tbl2.value
    end

    ---------------------------------------------------------
    local function checkForEnd(tbl1, tbl2)
        -- Check if it is done node
        if  tbl1 == nil and tbl2 == nil or
            tbl1 and tbl1.done == parseCycle and 
            tbl2 and tbl2.done == parseCycle
        then
            --dump 'END OF TREE!\n'
            return true
        end
        return false
    end

    ---------------------------------------------------------
    local function frontIterator(old, new, index)
        -- print ('front iterator '.. tostring(old) .. ' ' .. tostring(new) .. ' ' .. tostring(index))
        compareTrees(
            old and old.value[index], 
            new and new.value[index], 
            index,
            index,
            new)
    end

    ---------------------------------------------------------
    local function backIterator(old, new, index)
        -- print ('back iterator ' .. tostring(old) .. ' ' .. tostring(new) .. ' ' .. tostring(index))
        compareTrees(
            old and old.value[#old.value - index + 1], 
            new and new.value[#new.value - index + 1], 
            old and #old.value - index + 1,
            new and #new.value - index + 1,
            new)
    end

    ---------------------------------------------------------
    function compareTrees(old, new, oldIndex, newIndex, parent)
        -- print ('Compare oldIndex=' .. tostring(newIndex) .. ' newIndex' .. tostring(newIndex))

        -- Check if it is visited node
        if  checkForEnd(old, new) then return end
        if old and old.done == parseCycle then old = nil end
        if new and new.done == parseCycle then new = nil end

        -- Compare nodes
        if compareNodes(old, new) then

            -- Copy values from old
            new.instance = old.instance

            if type(old.value) == 'string' and type(new.value) == 'string' and old.value ~= new.value then
                updateElement(old, new, oldIndex, newIndex)
            end

        else
            -- Reverse direction of search
            coroutine.yield()

            -- Check if back search didnt marked our node
            if  checkForEnd(old, new) then return end

            -- Remove old nodes
            if old ~= nil and old.visit ~= parseCycle then
                removeElement(old, oldIndex);
                old.visit = parseCycle
            end
            -- Add new nodes
            if new ~= nil and new.visit ~= parseCycle then
                addElement(new, parent, newIndex)
                new.visit = parseCycle
            end

        end

        -- Check if it is node with table
        if  (old and type(old.value) == 'table') or 
            (new and type(new.value) == 'table') then
            
            -- Find maximum
            local max = findMaximum(old, new)
            
            -- Recursive search
            for index = 1, max, 1 do
                treeIterator(old, new, index)
                if checkForEnd(old, new) then return end
            end

        end

        -- Mark done flags
        if old then old.done = parseCycle end
        if new then new.done = parseCycle end

    end

    ---------------------------------------------------------
    parseCycle = parseCycle + 1

    local file = assert(Files[name], 'No opened file: ' .. tostring(name))
    local oldTree = file.tree
    local newTree = file.grammar:match(text)

    finishCompare = false
    
    assert(oldTree and newTree)
    
    local compareFrontCoroutine = coroutine.create(compareTrees)
    local compareBackCoroutine = coroutine.create(compareTrees)

    local rootOld = { table = { name='root' }, value = oldTree }
    local rootNew = { table = { name='root' }, value = newTree }

    treeIterator = frontIterator
    -- dump 'START: front search!\n'
    coroutine.resume(compareFrontCoroutine, rootOld, rootNew, 1, 1)

    treeIterator = backIterator
    -- dump 'START: back search!\n'
    coroutine.resume(compareBackCoroutine, rootOld, rootNew, 1, 1)

    -- Process tree until finished
    local frontCourotineFinished = false
    local backCourotineFinished = false
    while true do

        treeIterator = frontIterator
        -- dump 'START: front search!\n'
        if coroutine.resume(compareFrontCoroutine) == false then
            frontCourotineFinished = true
            if backCourotineFinished then break end
        end

        treeIterator = backIterator
        -- dump 'START: back search!\n'
        if coroutine.resume(compareBackCoroutine) == false then
            backCourotineFinished = true
            if frontCourotineFinished then break end
        end
    end

    file.tree = newTree
    treeIterator = nil

    dump "Reparsing file done!\n\n"

    return newTree
end

------------------------------------------------------------------------------------------------------------------ ELEMENT HANDLING

---------------------------------------------------------
-- Adds element from NEW TREE
-- @param item from new tree
-- @param parent from new tree
-- @param index from parent from new tree
function addElement(element, parent, index)
    if element.value == "" then return end

    dump('ADD: ' .. element.table.name .. ' "' .. tostring(element.value) .. '" to parent ' .. tostring(parent and parent.table.name) .. '\n')
    
    -- TODO formatovanie objektu
    if element.type == 'node' then
        element.instance = QT_addGrid(parent.instance, index)
    elseif element.type == 'leaf' then
        element.instance = QT_addItem(parent.instance, index)
    end
    -- dump('RETURNED: ' .. tostring(element.instance) .. '\n');
end

---------------------------------------------------------
-- Removes element from OLD TREE
-- @param item from old tree
-- @param index from parent from old tree
function removeElement(element, index)
    if element.value == "" then return end

    dump('REMOVE: ' .. element.table.name .. ' "' .. tostring(element.value) .. '"\n')

    if element.instance then
        dump('RETURNED: ' .. tostring(element.instance) .. '\n');
        QT_removeElement(element.instance)
    end
end

---------------------------------------------------------
function updateElement(oldElement, newElement, oldIndex, newIndex)
    dump('UPDATE: ' .. oldElement.table.name .. ' "' .. oldElement.value .. '" to "' .. newElement.value .. '"\n')
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
