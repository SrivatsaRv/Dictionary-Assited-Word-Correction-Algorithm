#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cstdio>
#include <cctype>
#include <set>
#include <cstring>

using namespace std;
enum MissType {
  MISS = 1,
  ADD = 2,
  SWAP = 4,
  REPLACE = 8,
  ALL = 15,
};
class Node {
public:
  Node() {
    for (int i = 0; i < 26; i ++)
      index[i] = NULL;
    count = 0;
  }
  ~Node() {
    for (int i = 0; i < 26; i ++) {
      if (index[i] != NULL) delete index[i];
    }
  }
  Node *index[26];
  int count;
};

class Trie {
public:
  Trie() {
    string delimter = "!\"#$%&'()*+,-./0123456789:;<=>?@[]^_|~";
    // memset(delimter_, sizeof(delimter_)/sizeof(char), false);
    for (int i = 0; i < 256; i ++) delimter_[i] = false;
    for (size_t i = 0; i < delimter.size(); i ++) {
      delimter_[i] = true;
    }
    root_ = NULL;
    FILE *fp = fopen("corpus.txt", "r");
    char buffer[300];
    int len;
    while (fscanf(fp, "%s", buffer) != EOF) {
      len = strlen(buffer);
      for (len -- ; len >= 0; len --) {
        if (isalpha(buffer[len])) break;
        buffer[len] = '\0';
      }
      for (; len >= 0; len --) {
        buffer[len] = char(tolower(buffer[len]));
      }
      Insert(buffer);
    }
    fclose(fp);
  }
  ~Trie() {
    if (root_ != NULL) {
      delete root_;
    }
  }
  void Insert(char str[]) {
    if (root_ == NULL) root_ = new Node();
    Insert(root_, str, 0);
  }
  Node* Find(char buffer[]) {
    return Find(root_, buffer, 0);
  }
private:
  void Insert(Node *current, char str[], int pos) {
    if (!str[pos]) {
      current->count ++;
      return;
    }
    if (!isalpha(str[pos])) {
      current->count ++;
      Insert(root_, str, pos + 1);
    } else {
      int idx = str[pos] - 'a';
      if (current->index[idx] == NULL) {
        current->index[idx] = new Node();
      }
      Insert(current->index[idx], str, pos + 1);
    }
  }
  Node* Find(Node *current, char str[], int pos) {
    if (current == NULL)
      return NULL;
    if (!str[pos]) {
      return current->count > 0 ? current:NULL;
    }
    if (!isalpha(str[pos]))
      return NULL;
    int idx = tolower(str[pos]) - 'a';
    Node *result =  NULL;
    if (current->index[idx] != NULL) {
      return Find(current->index[idx], str, pos + 1);
    } else {
      return NULL;
    }
  }
  bool delimter_[256];
  Node *root_;
};
bool update_result(Node* result, char str[], Node* new_result, char new_str[]) {
  if (new_result == NULL) return false;
  if (result == NULL)
    return true;
  if (result->count < new_result->count)
    return true;
  else if (result->count > new_result->count)
    return false;
  if (strcmp(str, new_str) > 0)
    return true;
  return false;
}
int main(int argc, char const *argv[]) {
  Trie trie;
  char copy[300];
  char buffer[300];
  Node *result;
  Node *tmp;
  char ans[30];
  int query;
  scanf("%d", &query);
  gets(buffer);
  while (query -- ) {
    gets(buffer);
    int len = strlen(buffer);
    if (len > 20) goto end;
    result = trie.Find(buffer);
    if (result) {
      strcpy(ans, buffer);
      // cout << "find directly" << result->count << endl;
      goto end;
    }

    if (len > 1) {
      for (int i = 0; buffer[i]; i ++) {
        for (int j = 0; buffer[j]; j ++) {
          if (j == i) continue;
          copy[j - (j > i)] = buffer[j];
        }
        copy[len - 1] = 0;
        // printf("%s\n", copy);
        tmp = trie.Find(copy);
        if (update_result(result, ans, tmp, copy)) {
          result = tmp;
          strcpy(ans, copy);
        }
      }
    }

    // replace char
    strcpy(copy, buffer);
    for (int i = 0; buffer[i]; i ++) {
      for (char ch = 'a'; ch <= 'z'; ch ++) {
        if (ch == buffer[i]) continue;
        copy[i] = ch;
        tmp = trie.Find(copy);
        if (update_result(result, ans, tmp, copy)) {
          result = tmp;
          strcpy(ans, copy);
        }
      }
      copy[i] = buffer[i];
    }

    // add one char
    for (int i = 0; buffer[i]; i ++) {
      copy[i] = buffer[i];
      for (char ch = 'a'; ch <= 'z'; ch ++) {
        copy[i + 1] = ch;
        strcpy(copy + i + 2, buffer + i + 1);
        tmp = trie.Find(copy);
        if (update_result(result, ans, tmp, copy)) {
          result = tmp;
          strcpy(ans, copy);
        }
      }
    }

    // swap two char
    strcpy(copy, buffer);
    for (int i = 0; buffer[i + 1]; i ++) {
      swap(copy[i], copy[i + 1]);
      tmp = trie.Find(copy);
      if (update_result(result, ans, tmp, copy)) {
        result = tmp;
        strcpy(ans, copy);
      }
      swap(copy[i + 1], copy[i]);
    }
end:
    if (result) {
      printf("%s\n", ans);
      // cout << result->count << endl;
    } else {
      printf("%s\n", buffer);
    }
  }
  return 0;
