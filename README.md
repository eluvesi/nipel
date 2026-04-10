<div align="center">
	<h1>Nipel Is (a) Polynomial Expression Language</h1>
</div>

### DESCRIPTION
Nipel is a small interpreted language for working with polynomial expressions,
and an interpreter for that language.

The name is recursive:
- Nipel Is (a) Polynomial Expression Language (as the language)
- Nipel Interprets Polynomial Expression Language (as the interpreter)

### BUILD
```shell
make
```

### USAGE
```shell
./nipel [FILE]...
```

### LANGUAGE
Language is designed to allow unambiguous tokenization
at the lexical analysis stage.
#### Variables
- polynomial variables: single lowercase letters (`a`, `x`, `y`)
- global identifiers: uppercase letters, digits, `_`
#### Syntax
- assignment: `identifier = expression`
- evaluation: a standalone expression is evaluated and printed
#### Semantics
- implicit multiplication is supported: `ab -> a * b`
- global variables must be accessed explicitly: `$A`

### AUTHOR
[eluvesi](https://github.com/eluvesi)

### LICENSE
[BSD 2-Clause  License](https://github.com/eluvesi/nipel?tab=BSD-2-Clause-1-ov-file)
