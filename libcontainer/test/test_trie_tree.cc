#include <stdio.h>
#include "trie_tree.h"

int main(){
    printf("%d\n", TT_DICT_INDEX(' '));
    container::TrieTree<int> tt;
    tt.insert("abcdef7",100);
    tt.insert("abcef7",101);
    tt.insert("abce7",102);
    tt.insert("bce7",103);
    tt.insert("bce7ssss",103);
    printf("count [%d]\n", tt.getCount());
    printf("abcdef7 : [%d]\n", *(tt.find("abcdef7")));
    printf("abcef7 : [%d]\n", *(tt.find("abcef7")));
    tt.display();
    printf("---copy-----------\n");
    container::TrieTree<int> ttt(tt);
    printf("---copy-----------\n");
    printf("count [%d]\n", ttt.getCount());
    printf("abcdef7 : [%d]\n", *(ttt.find("abcdef7")));
    printf("abcef7 : [%d]\n", *(ttt.find("abcef7")));
    ttt.display();
    printf("--------------\n");
    tt.remove("bce7ssss");
    tt.display();
    tt.empty();
    tt.display();
    printf("count [%d]\n", tt.getCount());
    ttt.display();
    return 0;
}
