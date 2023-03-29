# Bug 1

## A) How is your program acting differently than you expect it to?
- When running the test for WriteIndex the output I get for fileIndexChecker.CheckHeader() is incorrect. It seems that the file size against the table offsets is incorrect.  

## B) Brainstorm a few possible causes of the bug
- Possible mistake when updating the offsets
- Mistake in advancing to the next element in chain 
- Updating by wrong amount? 

## C) How you fixed the bug and why the fix was necessary
- I forgot to update the element position. I was simply incrementing by 1 rather than the element size. 


# Bug 2

## A) How is your program acting differently than you expect it to?
- When running the test for HashTableReader, the chain length is 0 rather than 1 

## B) Brainstorm a few possible causes of the bug
- Most likely an error with the BucketRecord because the length of the chain is determined with chain num elements. 
- There may be a mistake in freading the number of elements
- 

## C) How you fixed the bug and why the fix was necessary
- I realized I didn't seek in LookupElementPositions before reading in BucketRecord. This caused an error because it allows for other seeks and reads to happen between calls to LookupElementPositions. 


# Bug 3

## A) How is your program acting differently than you expect it to?
- Verify333 Error in Query Processor where input = 0

## B) Brainstorm a few possible causes of the bug
- Didn't fill input vector correctly
- forgot to check for null input
- 

## C) How you fixed the bug and why the fix was necessary
- Fixed the bug by adding in a checker to check the user input into filesearchshell, if zero, terminate program
