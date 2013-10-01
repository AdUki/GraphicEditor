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

---------------------------------------------------------
-- Produces AST leaf capture from defined lpeg pattern
-- NO CHILDREN
function leaf(var)
    dump('LEAF:', var, var.name, var.pattern, '\n')
    return lpeg.Ct( lpeg.Cg(lpeg.Cc(var.name), 't') * lpeg.Cg(var.pattern, 'v') )
end

---------------------------------------------------------
-- Produces AST node capture from defined lpeg pattern
-- HAS CHILDREN
function node(var)
    dump('LEAF:', var, var.name, var.pattern, '\n')
    return lpeg.Ct( lpeg.Cg(lpeg.Cc(var.name), 't') * lpeg.Cg(lpeg.Ct(var.pattern), 'v') )
end

---------------------------------------------------------
-- Produces error AST node
function error(var)
    dump('ERROR:', var, var.name, var.pattern, '\n')
    -- return lpeg.Ct( lpeg.Cg(lpeg.Cc('Error: ' .. var.name), 't') * lpeg.Cg(lpeg.C(var.pattern), 'v') )
    return lpeg.Ct( lpeg.Cg(lpeg.Cc('Error'), 't') * lpeg.Cg(lpeg.Cc(var.name), 'd') * lpeg.Cg(lpeg.C(var.pattern), 'v') )
end

---------------------------------------------------------
-- Prints AST tree
-- @param ast grouped tables that are ast tree
function dumpAST(ast)
    function tprint (tbl, indent)
      suffix = suffix or ''

      for k, v in pairs(tbl) do
        if type(v) == "table" then
            if k ~= 'v' then
                dump(string.rep(" ", indent) .. '[' .. k .. '] t=')
            else
                dump'\tv=table\n'
            end
            tprint(v, indent + 3)
        else
            if k == 't' then
                dump(v)
                if type(tbl[k].v) == 'table' then 
                    dump'\n' 
                end
            else
                dump('\t' .. k .. '=\'' .. v .. '\'\n')
            end
        end
      end
    end
    tprint(ast, 0)
    dump'\n'
end

loadGrammars()
openFile'input.arithmetic'

---------------------------------------------------------
-- DONT DELETE! LONG STRING TERMINATOR )";
