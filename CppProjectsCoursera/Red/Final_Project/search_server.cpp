#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

#include "profile.h"

/*
vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}
*/

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
  InvertedIndex new_index;

  {
  LOG_DURATION("UpdateDocumentBase");

  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(move(current_document));
  }

  }
  index = move(new_index);
}

void SearchServer::AddQueriesStream(
  istream& query_input, ostream& search_results_output
) {
  LOG_DURATION("AddQueriesStream");
  const size_t N = 5;
  vector<size_t> docid_count(index.DocsCount());
  for (string current_query; getline(query_input, current_query); ) {
    const auto words = SplitIntoWords(current_query);

    for (auto& el : docid_count) {
        el = 0;
    }
    for (auto word : words) {
      for (const auto& docid : index.Lookup(word)) {
        ++docid_count[docid];
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
//  LOG_DURATION("Add");
  docs.push_back(move(document));

  const size_t docid = docs.size() - 1;
  for (auto word : SplitIntoWords(docs.back())) {
    index[word].push_back(docid);
  }
}

const vector<size_t>& InvertedIndex::Lookup(string_view word) const {
//  LOG_DURATION("Lookup");
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return empty_lookup;
  }
}



/*
void SearchServer::AddQueriesStream(
  istream& query_input, ostream& search_results_output
) {
  LOG_DURATION("AddQueriesStream");
  const size_t N = 5;
  vector<size_t> docid_count(index.DocsCount());
  for (string current_query; getline(query_input, current_query); ) {
    const auto words = SplitIntoWords(current_query);

    for (auto& el : docid_count) {
        el = 0;
    }
    for (auto word : words) {
      for (const auto& docid : index.Lookup(word)) {
        ++docid_count[docid];
      }
    }

    array<pair<size_t, size_t>, N> search_results = GetRelevantElements<N>(docid_count);

    search_results.fill({0,0});

    size_t curr_index;
    pair<size_t, size_t> value;
    for (size_t i = 0; i < docid_count.size(); ++i)
    {
        if (docid_count[i] <= search_results[N-1].second) continue;
        curr_index = i > N-1 ? N-1 : i;
        value = {i, docid_count[i]};
        while (curr_index > 0 && value.second > search_results[curr_index - 1].second) {
            search_results[curr_index] = search_results[curr_index - 1];
            --curr_index;
        }
        search_results[curr_index] = value;
    }


    for (size_t i = 0; i < docid_count.size(); ++i)
        if (docid_count[i] > 0) search_results.push_back({i, docid_count[i]});

    partial_sort(
      begin(search_results),
      next(begin(search_results), min<size_t>(5, search_results.size())),
      end(search_results),
      [](const pair<size_t, size_t>& lhs, const pair<size_t, size_t>& rhs) {
        if (lhs.second != rhs.second) return lhs.second > rhs.second;
        else return lhs.first < rhs.first;
      }
    );

    search_results_output << current_query << ':';
    for (const auto& [docid, hitcount] : Head(search_results, 5)) {
      if (hitcount <= 0) break;
      search_results_output << " {"
        << "docid: " << docid << ", "
        << "hitcount: " << hitcount << '}';
    }

    for (const auto& [docid, hitcount] : search_results) {
        if (hitcount <= 0) break;
        search_results_output << " {"
          << "docid: " << docid << ", "
          << "hitcount: " << hitcount << '}';
    }

    search_results_output << '\n';
  }
}
*/
