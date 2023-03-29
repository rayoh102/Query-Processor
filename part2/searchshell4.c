#define _XOPEN_SOURCE 600

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
static void ProcessQueries(DocTable *dt, MemIndex *mi);
static void PrintResult(LinkedList* result, DocTable* doctable);

int main(int argc, char **argv) {
  if (argc != 2) {
     Usage();
  }

  DocTable *doctable;
  MemIndex *index;

  printf("Indexing \' %s\' \n",  argv[1]);
  if (!CrawlFileTree(argv[1], &doctable, &index)) {
    Usage();
  }

  ProcessQueries(doctable, index);
  DocTable_Free(doctable);
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

static void ProcessQueries(DocTable *dt, MemIndex *mi) {
  char *saveptr;
  char buffer[1024];

  while (1) {
    printf("enter query:\n");

    if (fgets(buffer, 1024, stdin) != NULL) {
      char **query = (char **)malloc(1024 * sizeof(char *));
      char *str = buffer;
      char* token;
      int qlen = 0;

      while (1) {
        token = strtok_r(str, " ", &saveptr);
	if (token == NULL) {
          break;
	}
	query[qlen] = token;
	qlen++;
	str = NULL;
      }
      char *p = strchr(query[qlen - 1], '\n');
      *p = '\0';

      LinkedList* result = MemIndex_Search(mi, query, qlen);
      if (result == NULL) {
        free(query);
	continue;
      }
      PrintResult(result, dt);
      free(query);
      LinkedList_Free(result, &free);
    }
  }
}

static void PrintResult(LinkedList* result, DocTable* doctable) {
  LLIterator* llitr = LLIterator_Allocate(result);
  Verify333(llitr != NULL);

  while (LLIterator_Next(llitr)) {
    SearchResult* res;
    LLIterator_Get(llitr, (LLPayload_t *)&res);
    DocID_t docid = res->doc_id;

    char *dir = DocTable_GetDocName(doctable, docid);
    printf("  %s (%d)\n", dir, res->rank);
  }

  LLIterator_Free(llitr);
}

