/* compiler: ee-gcc2.96-no-strict-aliasing */
struct Node {
    Node *next;
};

struct Queue {
    Node *head;
    Node **tail;
    Node *pendingHead;
    Node **pendingTail;
};

extern "C" void func_005B72A8(void);
extern "C" int func_005B72F8(void);

extern "C" void *func_00567718(Queue *queue) {
    func_005B72A8();
    Node *node = queue->head;
    if (node == 0) {
        func_005B72F8();
        return 0;
    }

    node->next = 0;
    *queue->pendingTail = node;
    queue->pendingTail = &node->next;
    if ((queue->head = node->next) == 0) {
        queue->tail = &queue->head;
    }
    func_005B72F8();
    return (char *)node + 4;
}
