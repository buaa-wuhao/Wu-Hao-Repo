/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int m, int n) {
        if(head==NULL || m==n) return head;
        ListNode* Dummy = new ListNode(0);
        Dummy->next = head;
        ListNode* ptr = Dummy;
        int cnt = 0;
        ListNode *ptr_m_prv,*ptr_n_next;
        ListNode *done,*ptr_m;        
        
        while(ptr)
        {
            cnt++;
            if(cnt==m) 
            {
                ptr_m_prv = ptr;
                ptr_m     = ptr_m_prv->next;
                done      = NULL;
                ptr = ptr->next;
             }
            else if(cnt>m && cnt <=n+1)
            {
                ListNode* tmp = ptr->next;
                ptr->next     = done;
                done          = ptr;
                ptr           = tmp;
            }
            else if(cnt>n+1)
            {
                ptr_n_next = ptr;
                break;
            }
            else if(cnt<m)
            {
                ptr=ptr->next;
            }
            //ptr = ptr->next;
        }
        ptr_m->next = ptr_n_next;
        ptr_m_prv->next = done;
        head = Dummy->next;
        delete Dummy;        
        return head;
              
    }
};