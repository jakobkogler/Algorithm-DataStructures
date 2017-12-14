let s:plugin_path = expand("<sfile>:p:h:h")

function! alg_ds#AlgDS()
    let l:find_cmd = "find . -path '*\\.h' | cut -b3-" 
    let l:file = join(fzf#run({"source": l:find_cmd, "dir": s:plugin_path, "down": "30%", "options": "--reverse --preview='head -$LINES {}'"}))
    if len(l:file)
        let l:file_path = s:plugin_path . "/" . l:file
        let l:content = readfile(l:file_path)
        let l:filtered = alg_ds#Filter(l:content)
        put =l:filtered
    endif
endfunction

function! alg_ds#Filter(lines)
    " Remove leading includes and empty lines
    let l:filtered = []
    let l:started = 0
    for line in a:lines
        if and(match(line, "\#") != 0, or(l:started, strlen(line) > 0))
            let l:started = 1
            call add(l:filtered, line)
        endif
    endfor
    return join(l:filtered, "\n")
endfunction
