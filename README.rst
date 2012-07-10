.. vim: set filetype=rst :

=============
GP Calculator
=============

This is an simple command line calculator which I had to write for an application for a job opening.


Building and running
====================

There are no dependencies expect a C++ compiler. If ``g++`` and ``make`` are installed you just need to type::

    make
    ./gpc


About the code
==============


0. General
----------
I could choose from the following languages​​: Clojure, C++ and Python. I choosed C++ because that was the language I was most familiar with. I had no experience with Clojure or Python at this time.

I tooked a short look on Clojure because parsing can be much more straightforwarded with functional languages. But the syntax was kind of obscure to me.

I have a little background in C++ and after some initial syntax difficulties (I've worked with Java the last 3 years) it worked out well.

There are basicly four parts:

1. The tokenizer which splits the input into defined tokens.
2. The syntax tree which does the mathical processing.
3. The parser which creates a syntax tree out of a list of tokens.
4. And last but not least the main application which wires that all together.


1. Tokenizer
------------

Splits the string first at its possible operation symboles (like +, -, ...) and tokenizes the numeric data after that.
This is done this way because whitespaces do matter in combination with the english numerals.


2. Syntax tree
--------------
The definition and implementation of the calculation. The pasrer generates the synax tree. When the root node ``get eval()``'ed all children get ``eval()``ed as well. With a syntax tree the parenthesis can be easily guaranteed.


3. Parser
---------
The parser generates the syntax tree by analyzing the token list. It is based on a basic grammer for mathematical expressions::

    expression ::= term {′ +′ term | ′ −′ term }
    term ::= factor {′∗′factor | ′/′factor}
    factor ::= {digit_number | lexical_number}
    digit_number ::= digit {digit}
    digit ::= ′0′ |′1′ |′2′ |′3′ |′4′ |′5′ |′6′ |′7′ |′8′ |′9′
    lexical_number ::= 1_to_99 multiplicator {lexical_number}
    1_to_99 ::= 1_to_19 | 20_to_99
    1_to_19 ::= zero | one | two | tree | ...
    20_to_99 ::= tenner {onner}
    tenner ::= twenty | thirty | forty | ...
    onner ::= one | two | tree | ...  // (no zero)
    multiplicator ::= hundred | thousand | million

There is one problem. A number like ``21`` is spelled ``twenty minus one``. This is bad because a grammer cannot distinguish if ``twenty-one`` means ``21`` or ``20-1``. I decided to replace the ``-`` with a whitespace.


4. Main
-------
The main program just reads from ``stdin`` and outputs the result to the user.

