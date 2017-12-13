# Algorithm-DataStructures

Implementations of common algorithms and data structures used for competitive programming.

## Vim plugin

The algorithms and data structures can easily be pasted into a document using the included Vim plugin.

### Installation

Use [vim-plug](https://github.com/junegunn/vim-plug) or any Vim plugin manager of your choice.
The plugin [fzf](https://github.com/junegunn/fzf) is a requirement.

With vim-plug:

```vim
Plug 'junegunn/fzf', { 'dir': '~/.fzf', 'do': './install --all' }
Plug 'jakobkogler/Algorithm-DataStructures'
```

### Command/Usage

The plugin defines a single command `AlgDS`.
It can be mapped like this:

```vim
nmap <leader>alg :AlgDS<CR>
```

## License

WTFPL
