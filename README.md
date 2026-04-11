<p align="center">
	<img src="logo.png" width="400">
</p>

---

### NAME
nipel - a polynomial expression language and its interpreter

---

### SYNOPSIS
```shell
./nipel [FILE]
```
> With no FILE, or when FILE is -, read standard input.

---

### BUILD
```shell
make
```

---

### DESCRIPTION
Nipel is a small interpreted language for working with polynomial expressions and an interpreter for that language.

The name is recursive:
- Nipel Is (a) Polynomial Expression Language.
- Nipel Interprets Polynomial Expression Language.

The language is designed with a strict lexical structure to ensure unambiguous tokenization.

---

### LANGUAGE

#### Variables
- polynomial variables: single lowercase letters (`a`, `x`, `y`)
- global identifiers: uppercase letters, digits, `_`

#### Syntax
- assignment: `identifier = expression`
- evaluation: a standalone expression is evaluated and printed

#### Semantics
- implicit multiplication is supported: `ab -> a * b`
- global variables must be accessed explicitly: `$A`

---

### AUTHOR
[eluvesi](https://github.com/eluvesi)

---

### LICENSE
[BSD 2-Clause  License](https://github.com/eluvesi/nipel?tab=BSD-2-Clause-1-ov-file)
