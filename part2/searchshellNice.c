#define _XOPEN_SOURCE 600

#define INPUT_BUFFER_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "libhw1/CSE333.h"
#include "./CrawlFileTree.h"
#include "./DocTable.h"
#include "./MemIndex.h"

static void Usage(void);

int main(int argc, char** argv) {
  if (argc != 2) {
    Usage();
  }
  
  DocTable* table;
  MemIndex* index;
  if (!CrawlFileTree(argv[1], &table, &index)) {
    fprintf(stderr, "Incorrect Crawl File Tree");
    Usage();
  }

  Verify333(table != NULL);
  Verify333(index != NULL);

  LinkedList* documents;
  LLIterator* iterator;
  SearchResult* searchResult;

  char input[INPUT_BUFFER_SIZE];
  char* token;
  char* ptr;

  printf("Indexing '%s'\n", argv[1]);
  
  while (true) {
    printf("enter query: \n");
    if (fgets(input, INPUT_BUFFER_SIZE, stdin) == NULL) {
      break;
    }

    char** query = (char**) malloc(INPUT_BUFFER_SIZE * sizeof(char**));
    Verify333(query != NULL);
    unsigned char *tptr = (unsigned char *)input;
    while (*tptr) {
      *tptr = tolower(*tptr);
      tptr++;
    }

    char* input_pointer = input;
    int length = 0;

    while (true) {
      token = strtok_r(input_pointer, " ", &ptr);
      if (token == NULL) {
        break;
      }
      query[length++] = token;
      input_pointer = NULL;
    }

    char *p = strchr(query[length - 1], '\n');
    if (p) *p = '\0';

    documents = MemIndex_Search(index, query, length);
    if (documents != NULL) {
      iterator = LLIterator_Allocate(documents);
      Verify333(iterator != NULL);

      do {
        LLIterator_Get(iterator, (void **) &searchResult);
	printf("  %s (%u)\n", DocTable_GetDocName(table, searchResult->doc_id), searchResult->rank);

	LLIterator_Next(iterator);
      } while (LLIterator_IsValid(iterator));

      LLIterator_Free(iterator);
      LinkedList_Free(documents, (LLPayloadFreeFnPtr) free);
    }
    free(query);
  }

  DocTable_Free(table);
  MemIndex_Free(index);

  return EXIT_SUCCESS;
}

static void Usage(void) {
  fprintf(stderr, "Usage: ./searchshell <docroot>\n");
    fprintf(stderr,
		   "where <docroot> is an absolute or relative " \
		   "path to a directory to build an index under.\n");
    exit(EXIT_FAILURE);
}
