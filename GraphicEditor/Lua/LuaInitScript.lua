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
-- @param extension extension of file whitout dot
-- @param tree actual AST
Files = {
    extension = nil,
    tree = nil,
}

---------------------------------------------------------
-- Opens file in location
-- @param path (string) path to file
-- @return opened file reference
function openFile(path)
    -- TODO: pridat warning ked je subor read-only
    local file = assert(io.open(path, "r"))
    local text = file:read("*all")
    file:close()

    local workDir = lfs.currentdir()
    local filepath = lfs.fullpath(path)
    local fileExtension = filepath:match('.*[%.\\/](.*)$') -- get extension, relies that '\' sign cannot be in extension

    Files[filepath] = {
        extension = fileExtension,
        tree = Extensions[assert(fileExtension, 'No grammar for extension'.. fileExtension .. 'defined')].grammar:match(text)
    }
    
    dump(filepath, 'Extension: ' .. fileExtension, '\n\n')
    dump(text, '\n\n')
    dumpAST(Files[filepath].tree)

    return Files[filepath]
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

    if count == 1 then
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
            dump(string.rep(" ", indent) .. '[' .. key .. '] t=')
            dump('n=' .. node.table.name)

            if type(node.value) == 'table' then
                dump('\n')
                tprint(node.value, indent + 3)
            else
                dump('\tv=' .. node.value .. '\n')
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

        for key, node in ipairs(tbl) do
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

------------------------------------------------------------------------------------------------------------------ TESTING COMMANDS
loadGrammars()
openedFile = openFile'../test input/input.arithmetic'
crawlAST(openedFile.tree)
---------------------------------------------------------
-- DONT DELETE! LONG STRING TERMINATOR )";
