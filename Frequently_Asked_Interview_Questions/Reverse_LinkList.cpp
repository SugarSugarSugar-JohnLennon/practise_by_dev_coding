#include <iostream>
using namespace std;


struct ListNode {
    ListNode* next;
    int val;
    ListNode(int ival) : val(ival), next(nullptr){}
};

ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    ListNode* tmp = nullptr;
    
    while(curr != nullptr) {
        tmp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = tmp;
    }
    return prev;
}


// 打印链表
void printList(ListNode* head) {
    while (head) {
        cout << head->val;
        if (head->next) cout << " -> ";
        head = head->next;
    }
    cout << endl;
}

// 构建测试链表并测试反转
int main() {
    // 构建链表：1 -> 2 -> 3 -> 4 -> 5
    ListNode* head = new ListNode(1);
    head->next = new ListNode(2);
    head->next->next = new ListNode(3);
    head->next->next->next = new ListNode(4);
    head->next->next->next->next = new ListNode(5);

    cout << "Original list: ";
    printList(head);

    ListNode* reversed = reverseList(head);

    cout << "Reversed list: ";
    printList(reversed);

    return 0;
}
