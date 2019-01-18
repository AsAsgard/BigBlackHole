#include "search_server.h"
//#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

//#include "profile.h"

vector<string_view> SplitIntoWords(string_view line) {
   vector<string_view> result;

   while (true) {
       size_t not_space = line.find_first_not_of(' ');

       if (not_space == line.npos) {
           break;
       } else {
           line.remove_prefix(not_space);
       }

       size_t space = line.find(' ');
       result.push_back(line.substr(0,space));

       if (space == line.npos) {
           break;
       } else {
           line.remove_prefix(space + 1);
       }
   }
   return result;
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
//  LOG_DURATION("UpdateDocumentBase");
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(move(current_document));
  }

  index = move(new_index);
}

void SearchServer::AddQueriesStream(
  istream& query_input, ostream& search_results_output
) {
//  LOG_DURATION("AddQueriesStream");
  const size_t N = 5;
  vector<size_t> docid_count(index.DocsCount());
  for (string current_query; getline(query_input, current_query); ) {
    const auto words = SplitIntoWords(current_query);

    for (auto& el : docid_count) {
        el = 0;
    }
    for (auto word : words) {
      for (const auto& [docid, count] : index.Lookup(word)) {
        docid_count[docid] += count;
      }
    }

    array<pair<size_t, size_t>, N> search_results = GetRelevantElements<N>(docid_count);

    search_results_output << current_query << ':';

    for (const auto& [docid, hitcount] : search_results) {
        if (hitcount <= 0) break;
        search_results_output << " {"
          << "docid: " << docid << ", "
          << "hitcount: " << hitcount << '}';
    }

    search_results_output << '\n';
  }
}

void InvertedIndex::Add(string&& document) {

  docs.push_back(move(document));

  const size_t docid = docs.size() - 1;
  unordered_map<string_view, size_t> words_count;

  for (auto word : SplitIntoWords(docs.back())) {
    ++words_count[word];
  }

  for (auto& [word, count] : words_count) {
    index[word].push_back({docid, count});
  }

}

const vector<pair<size_t,size_t>>& InvertedIndex::Lookup(string_view word) const {

  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return empty_lookup;
  }

}
