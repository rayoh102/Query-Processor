/*
 * Copyright ©2023 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

// Feature test macro for strtok_r (c.f., Linux Programming Interface p. 63)
#define _XOPEN_SOURCE 600

// The size of the input buffer
#define BSIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "libhw1/CSE333.h"
#include "./CrawlFileTree.h"
#include "./DocTable.h"
#include "./MemIndex.h"

//////////////////////////////////////////////////////////////////////////////
// Helper function declarations, constants, etc
static void Usage(void);
static void ProcessQueries(DocTable* dt, MemIndex* mi);

//////////////////////////////////////////////////////////////////////////////
// Main
int main(int argc, char** argv) {
  if (argc != 2) {
    Usage();
  }

  // Implement searchshell!  We're giving you very few hints
  // on how to do it, so you'll need to figure out an appropriate
  // decomposition into functions as well as implementing the
  // functions.  There are several major tasks you need to build:
  //
  //  - Crawl from a directory provided by argv[1] to produce and index
  //  - Prompt the user for a query and read the query from stdin, in a loop
  //  - Split a query into words (check out strtok_r)
  //  - Process a query against the index and print out the results
  //
  // When searchshell detects end-of-file on stdin (cntrl-D from the
  // keyboard), searchshell should free all dynamically allocated
  // memory and any other allocated resources and then exit.
  //
  // Note that you should make sure the fomatting of your
  // searchshell output exactly matches our solution binaries
  // to get full points on this part.
  DocTable* docTable;
  MemIndex* memIndex;

  printf("Indexing '%s'\n", argv[1]);
  if (!CrawlFileTree(argv[1], &docTable, &memIndex)) {
    Usage();
  }

  ProcessQueries(docTable, memIndex);

  DocTable_Free(docTable);
  MemIndex_Free(memIndex);
  return EXIT_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////
// Helper function definitions

static void Usage(void) {
  fprintf(stderr, "Usage: ./searchshell <docroot>\n");
  fprintf(stderr,
          "where <docroot> is an absolute or relative " \
          "path to a directory to build an index under.\n");
  exit(EXIT_FAILURE);
}

static void ProcessQueries(DocTable* dt, MemIndex* mi) {
  char buffer[BSIZE];
  char* token;
  char* ptr;

  while (true) {
    printf("enter query: \n");
    if (fgets(buffer, BSIZE, stdin) == NULL) {
      break;
    }

    char** query = (char**) malloc(BSIZE * sizeof(char**));
    char* str = buffer;
    int length = 0;
    unsigned char *tptr = (unsigned char *)buffer;

    while (*tptr) {
      *tptr = tolower(*tptr);
       tptr++;
    }

    while (true) {
      token = strtok_r(str, " ", &ptr);
      if (token == NULL) {
        break;
      }
      query[length++] = token;
      str = NULL;
    }

    char *p = strchr(query[length - 1], '\n');
    *p = '\0';

    LinkedList* result = MemIndex_Search(mi, query, length);

    if (result != NULL) {
      LLIterator* itr = LLIterator_Allocate(result);

      while (LLIterator_IsValid(itr)) {
        SearchResult* search;
        LLIterator_Get(itr, (void **) &search);

        printf("  %s (%u)\n", DocTable_GetDocName(dt, search->doc_id),
          search->rank);

        LLIterator_Next(itr);
      }

      LinkedList_Free(result, (LLPayloadFreeFnPtr) free);
      LLIterator_Free(itr);
    }
    free(query);
    continue;
  }
}

