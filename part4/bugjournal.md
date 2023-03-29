# Bug 1

## A) How is your program acting differently than you expect it to?
- Our program keeps running into a segmentation fault when running the tests for ServerSocket

## B) Brainstorm a few possible causes of the bug
- The issue is somewwhere in the first function, BindAndListen
- It seems to be a casting issue 
- Could also possibly be an issue with freeing 

## C) How you fixed the bug and why the fix was necessary
- I was trying to convert the port number in getaddrinfo() to a char* with char* port = reintepret_cast<char*>(port_), which doesn't work becaues I was casting the value of port rather than the address of port. I ended up fixing my issue by changing it to &port and I also used std::to_string and the .c_str() function. 


# Bug 2

## A) How is your program acting differently than you expect it to?
- Some more segmentation faults in ServerSocket

## B) Brainstorm a few possible causes of the bug
- Another casting issue?
- Possibly an issue with freeing?
- Null pointers?

## C) How you fixed the bug and why the fix was necessary
- I didn't properlty initialize caddr_len. Because of this, the caddr_len parameter being passed into accept() was null, causing the accept() function to not update the value, resulting in undefined behavior and segmentation faults when other functions attempt to access the fields of caddr. 


# Bug 3

## A) How is your program acting differently than you expect it to?
- Our program causes a segmentation fault whenever running the HttpConnection test

## B) Brainstorm a few possible causes of the bug
- The issue should be somewhere in the first function, GetNextRequest 
- Parsing incorrectly?
- Saving incorrectly to the output parameter request?

## C) How you fixed the bug and why the fix was necessary
- I realized I accidentally wrote "*request = ParseRequest(buffer_.substr(0, kHeaderEndLen));" rather than "*request = ParseRequest(buffer_.substr(0, pos + kHeaderEndLen))". The program was only parsing the first 4 characters of the buffer, resulting in the parsing of an incomplete HTTP request.
