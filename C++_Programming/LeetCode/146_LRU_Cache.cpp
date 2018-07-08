class LRUCache {
private:
    int sz;
    ListNode* head;
    ListNode* tail;
    unordered_map<int,ListNode*> _hash;
    unordered_map<int,int> k2v;
public:
    LRUCache(int capacity) {
       sz = capacity; 
       head = new ListNode(0);
       tail = head;
    }
    
    int get(int key) {
        if(_hash.count(key))
        {
            ListNode* ptr = _hash[key];
            int val = k2v[key];
            
            if(ptr->next!=tail)
            {
                ListNode* tmp = ptr->next;
                ptr->next = ptr->next->next;
                _hash[tmp->next->val] = ptr;
                tail->next = tmp;
                tmp->next = NULL;
                _hash[key] = tail;
                tail = tmp;
            }
            
            return val;
        }
        else
        {
            return -1;
        }
    }
    
    void put(int key, int value) {
        if(_hash.count(key))
        {
            ListNode* ptr = _hash[key];
            k2v[key] = value;
            if(ptr->next != tail)
            {
                ListNode* tmp = ptr->next;
                ptr->next = ptr->next->next;
                _hash[tmp->next->val] = ptr;
                tail->next = tmp;
                tmp->next = NULL;
                _hash[key] = tail;
                tail = tmp;                
            }               
        }
        else
        {
           _hash[key] = tail;
           k2v[key]   = value;
           ListNode* ptr = new ListNode(key);
           tail->next = ptr;
           tail = ptr;           
           
           if(_hash.size() > sz)
           {
              ListNode* tmp = head->next;
              head->next    = tmp->next;
              _hash[tmp->next->val] = head;
              _hash.erase(tmp->val);
               k2v.erase(tmp->val);
              delete tmp; 
           }

        }
        return;
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache obj = new LRUCache(capacity);
 * int param_1 = obj.get(key);
 * obj.put(key,value);
 */