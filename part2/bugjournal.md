# Bug 1

## A) How is your program acting differently than you expect it to?
- When running a MemIndex search on multiple query words, it doesn't return the correct number of files that contains these words. 

## B) Brainstorm a few possible causes of the bug
- Wrong query index acesses 
- Failure of docTable implementation
- Not properly iterating through different files retrieved 
from the HashTable with iterator 

## C) How you fixed the bug and why the fix was necessary
- I fixed the bug by finding out that I forgot to switch 
the query to be query[i] instead of 0. 
This is why MemIndex search only worked for 1 query word searches.
Fix was necessary to pass MemIndexTest.

# Bug 2

## A) How is your program acting differently than you expect it to?
- When running FileParserTest, an assertion error occured when comparing if two strings are the same 

## B) Brainstorm a few possible causes of the bug
- Strncpy failed due to potential wrong input length
- Pointer allocation issues
- Passed in null string to be compared

## C) How you fixed the bug and why the fix was necessary
- The bug wasn't really a bug. I wanted to try to verify that the strncpy process was sucessful. 
Hence I wrote Verify333(*word_p == *word), which wouldn't be true.
This doesn't test if the words are the same.
Fix was necessary otherwise we would get an error that terminates when using FileParser in any situation.
# Bug 3

## A) How is your program acting differently than you expect it to?
- When running CrawlFileTreeTest, the number of entries in HandleDir is zero 
even when there should be entries. This appeared during the CheckDocTable test.

## B) Brainstorm a few possible causes of the bug
- Bad for loop implementation, doesn't enter loop
- Readdir fails on the first call
- Forget to increament numentries 

## C) How you fixed the bug and why the fix was necessary
- I switched the loop from the for loop to a while loop 
as I was more sure that one could be an infinite loop. 
As we still had to keep track of the index, I manually 
updated the i-th value so we could access the entries[i] struct.
Fix is necessary as CrawlFileTree won't check out the items in a directory otherwise.
