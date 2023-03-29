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

#include "./QueryProcessor.h"

#include <iostream>
#include <algorithm>
#include <list>
#include <string>
#include <vector>

extern "C" {
  #include "./libhw1/CSE333.h"
}

using std::list;
using std::sort;
using std::string;
using std::vector;

namespace hw3 {

QueryProcessor::QueryProcessor(const list<string>& index_list, bool validate) {
  // Stash away a copy of the index list.
  index_list_ = index_list;
  array_len_ = index_list_.size();
  Verify333(array_len_ > 0);

  // Create the arrays of DocTableReader*'s. and IndexTableReader*'s.
  dtr_array_ = new DocTableReader* [array_len_];
  itr_array_ = new IndexTableReader* [array_len_];

  // Populate the arrays with heap-allocated DocTableReader and
  // IndexTableReader object instances.
  list<string>::const_iterator idx_iterator = index_list_.begin();
  for (int i = 0; i < array_len_; i++) {
    FileIndexReader fir(*idx_iterator, validate);
    dtr_array_[i] = fir.NewDocTableReader();
    itr_array_[i] = fir.NewIndexTableReader();
    idx_iterator++;
  }
}

QueryProcessor::~QueryProcessor() {
  // Delete the heap-allocated DocTableReader and IndexTableReader
  // object instances.
  Verify333(dtr_array_ != nullptr);
  Verify333(itr_array_ != nullptr);
  for (int i = 0; i < array_len_; i++) {
    delete dtr_array_[i];
    delete itr_array_[i];
  }

  // Delete the arrays of DocTableReader*'s and IndexTableReader*'s.
  delete[] dtr_array_;
  delete[] itr_array_;
  dtr_array_ = nullptr;
  itr_array_ = nullptr;
}

// This structure is used to store a index-file-specific query result.
typedef struct {
  DocID_t doc_id;  // The document ID within the index file.
  int     rank;    // The rank of the result so far.
} IdxQueryResult;

vector<QueryProcessor::QueryResult>
QueryProcessor::ProcessQuery(const vector<string>& query) const {
  Verify333(query.size() > 0);

  // STEP 1.
  // (the only step in this file)
  vector<QueryProcessor::QueryResult> final_result;

  for (int i = 0; i < array_len_; i++) {
    IndexTableReader* indexTableReader = itr_array_[i];
    DocTableReader* docTableReader = dtr_array_[i];
    DocIDTableReader* docIDTableReader = indexTableReader->LookupWord(query[0]);

    if (docIDTableReader == nullptr) {
      delete docIDTableReader;
      continue;
    }

    list<DocIDElementHeader> res = docIDTableReader->GetDocIDList();

    for (uint j = 1; j < query.size(); j++) {
      docIDTableReader = indexTableReader->LookupWord(query[j]);
      if (docIDTableReader == nullptr) {
        res.clear();
        delete docIDTableReader;
        break;
      }

      list<DocIDElementHeader> temp = docIDTableReader->GetDocIDList();
      list<DocIDElementHeader> valid_list;

      for (auto const& list1 : res) {
        int32_t positions = list1.num_positions;
        for (auto const& list2 : temp) {
          if (list2.doc_id == list1.doc_id) {
            positions += list2.num_positions;
            valid_list.push_back(DocIDElementHeader(list1.doc_id, positions));
            break;
          }
        }
      }
      res = valid_list;
      delete docIDTableReader;
    }

    if (res.size() == 0) {
      continue;
    }

    list<DocIDElementHeader>:: iterator it;
    for (auto const result : res) {
      QueryResult curr_result;
      Verify333(docTableReader->LookupDocID(result.doc_id,
        &curr_result.document_name));
      curr_result.rank = result.num_positions;
      final_result.push_back(curr_result);
    }
  }

  // Sort the final results.
  sort(final_result.begin(), final_result.end());
  return final_result;
}

}  // namespace hw3
