# Project 1 - COMP3659

## Introduction
Welcome to Project 1 for COMP3659. This project aims to blow bash out of the water with its outstanding 
and revolutionary completness.

## Features
- correctly implements basic functionality requirements.
- correctly implements basic background jobs.
- correctly implements basic I/O redirection.
- correctly implements basic (2-stage) command pipelines.
- correctly implements n-stage command pipelines.
- partially implements command search paths range.
- implements some additional enhancements, up to and including some
  basic signal handling

## Known Bugs and Limitations
If the user enters I/O redirection before a pipe, the shell will ignore the rest of the pipeline.
This is due to the parsing algorithm, `mysh` reads the string differently. All I/O redirection still 
has the desired effect, but commands must be formatted differently. For example:

#### bash command 
```
cat < input.txt | grep "pattern" | sort > output.txt
``` 
#### equivalent command in mysh 
```
cat | grep "pattern" | sort < input.txt > output.txt
``` 
## Developers
- Amtoj Punia
- Henry Nguyen
- Evan Wushke