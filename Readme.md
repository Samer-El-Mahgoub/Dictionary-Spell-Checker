# AVL Tree Spell Checker

A simple spell checker implemented in C using an AVL Tree for fast word lookup.

## Features
- Loads words from `Dictionary.txt`
- Stores words in a self-balancing AVL Tree
- Case-insensitive word checking
- Removes punctuation automatically
- Suggests nearby words for misspellings

## Suggestions based on:
- A. The word in the last node you reached before declaring that the word does not exist.
- B. The word in the inorder predecessor of that node in Part A.
- C. The word in the inorder successor of that node in Part A.

## Files
- `main.c` - Source code
- `Dictionary.txt` - Dictionary words

## Complexity
- Search: **O(log n)**
- Insert: **O(log n)**

Developed using AVL Trees to demonstrate efficient spell checking and word searching.
